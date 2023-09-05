/**
 * @file        noises.cpp
 * 
 * @brief       Implement noises on heightmaps.
 * 
 * @details     Noise and hash functions are taken and/or inspired from the
 *              Blender's source code.
 *              https://github.com/blender/blender/blob/main/intern/cycles/util/hash.h
 *              https://github.com/blender/blender/blob/main/intern/cycles/kernel/osl/shaders/node_voronoi.h
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <noises.hpp>
#include <stb_perlin.h>
#include <iostream>
#include <cmath>
#include <limits>

#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))

#define mix(a, b, c) \
  { \
    a -= c; \
    a ^= rot(c, 4); \
    c += b; \
    b -= a; \
    b ^= rot(a, 6); \
    a += c; \
    c -= b; \
    c ^= rot(b, 8); \
    b += a; \
    a -= c; \
    a ^= rot(c, 16); \
    c += b; \
    b -= a; \
    b ^= rot(a, 19); \
    a += c; \
    c -= b; \
    c ^= rot(b, 4); \
    b += a; \
  } \
  ((void)0)

#define final(a, b, c) \
  { \
    c ^= b; \
    c -= rot(b, 14); \
    a ^= c; \
    a -= rot(c, 11); \
    b ^= a; \
    b -= rot(a, 25); \
    c ^= b; \
    c -= rot(b, 16); \
    a ^= c; \
    a -= rot(c, 4); \
    b ^= a; \
    b -= rot(a, 14); \
    c ^= b; \
    c -= rot(b, 24); \
  } \
  ((void)0)

unsigned int hash_uint(unsigned int kx)
{
  unsigned int a, b, c;
  a = b = c = 0xdeadbeef + (1 << 2) + 13;

  a += kx;
  final(a, b, c);

  return c;
}

unsigned int hash_uint2(unsigned int kx, unsigned int ky)
{
  unsigned int a, b, c;
  a = b = c = 0xdeadbeef + (2 << 2) + 13;

  b += ky;
  a += kx;
  final(a, b, c);

  return c;
}

unsigned int hash_uint3(unsigned int kx, unsigned int ky, unsigned int kz)
{
  unsigned int a, b, c;
  a = b = c = 0xdeadbeef + (3 << 2) + 13;

  c += kz;
  b += ky;
  a += kx;
  final(a, b, c);

  return c;
}

float uint_to_float_incl(unsigned int n)
{
  return (float)n * (1.0f / (float)0xFFFFFFFFu);
}

float hash_uint_to_float(unsigned int kx)
{
  return uint_to_float_incl(hash_uint(kx));
}

float hash_uint2_to_float(unsigned int kx, unsigned int ky)
{
  return uint_to_float_incl(hash_uint2(kx, ky));
}

float hash_uint3_to_float(unsigned int kx, unsigned int ky, unsigned int kz)
{
  return uint_to_float_incl(hash_uint3(kx, ky, kz));
}


void hash_uint2_to_float2(unsigned int i, unsigned int j, float& x, float& y)
{
    x = hash_uint2_to_float(i, j);
    y = hash_uint3_to_float(i, j, 1);
}


HeightMap perlin(int width, int height, float scale, int octaves)
{
    HeightMap HM(width, height);
    add_perlin(HM, 1.0f, scale, octaves);
    return HM;
}

HeightMap voronoi(int width, int height, float scale)
{
    HeightMap HM(width, height);
    add_voronoi(HM, 1.0f, scale);
    return HM;
}


void add_perlin(HeightMap& HM, float alpha, float scale, int octaves)
{
    float u, v;
    for (int j = 0; j < HM.GetHeight(); ++j)
    {
        v = j / (float)(HM.GetHeight() - 1);
        v *= scale;
        for (int i = 0; i < HM.GetWidth(); ++i)
        {
            u = i / (float)(HM.GetWidth() - 1);
            u *= scale;

            float value = stb_perlin_fbm_noise3(u, v, 0.0f, 2.0, 0.5, octaves);
            HM.Set(i, j, HM(i, j) + alpha * value);
        }
    }
}


void add_voronoi(HeightMap& HM, float alpha, float scale)
{
    float u, v;
    for (int j = 0; j < HM.GetHeight(); ++j)
    {
        v = j / (float)(HM.GetHeight() - 1);
        v *= scale;
        for (int i = 0; i < HM.GetWidth(); ++i)
        {
            u = i / (float)(HM.GetWidth() - 1);
            u *= scale;

            float value = std::numeric_limits<float>::infinity();
            int cell_x = std::floor(u);
            int cell_y = std::floor(v);
            float x = 0.0f;
            float y = 0.0f;
            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    hash_uint2_to_float2(cell_x + dx, cell_y + dy, x, y);
                    x += cell_x + dx;
                    y += cell_y + dy;
                    float d = (x - u) * (x - u) + (y - v) * (y - v);
                    value = std::min(value, d);
                }
            }

            HM.Set(i, j, HM(i, j) + alpha * std::sqrt(value));
        }
    }
}