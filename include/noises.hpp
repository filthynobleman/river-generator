/**
 * @file        noises.hpp
 * 
 * @brief       Add noises to an heightmap.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#pragma once

#include <hmap.hpp>


void add_perlin(HeightMap& HM, float alpha, float scale, int octaves);
HeightMap perlin(int width, int height, float scale, int octaves);
void add_voronoi(HeightMap& HM, float alpha, float scale);
HeightMap voronoi(int width, int height, float scale);