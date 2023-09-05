/**
 * @file        plane.cpp
 * 
 * @brief       
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <plane.hpp>
#include <fstream>
#include <sstream>
#include <cmath>

void triangulate_plane(const HeightMap& HM, int w, int h, int& ntris, float** verts, unsigned int** tris)
{
    int nverts = w * h;
    ntris = (w - 1) * (h - 1) * 2;

    *verts = (float*)std::malloc(3 * nverts * sizeof(float));
    *tris = (unsigned int*)std::malloc(3 * ntris * sizeof(unsigned int));

    for (int j = 0; j < h; ++j)
    {
        float v = j / (float)h;
        float jj = v * HM.GetHeight();
        int j0 = std::floor(jj);
        int j1 = std::ceil(jj);
        jj -= j0;
        for (int i = 0; i < w; ++i)
        {
            float u = i / (float)w;
            float ii = u * HM.GetWidth();
            int i0 = std::floor(ii);
            int i1 = std::ceil(ii);
            ii -= i0;

            float z00 = HM(i0, j0);
            float z01 = HM(i0, j1);
            float z10 = HM(i1, j0);
            float z11 = HM(i1, j1);
            float z0 = z01 * ii + z00 * (1 - ii);
            float z1 = z11 * ii + z10 * (1 - ii);
            float z = z1 * jj + z0 * (1 - jj);

            (*verts)[3 * (j * w + i)] = u;
            (*verts)[3 * (j * w + i) + 1] = v;
            (*verts)[3 * (j * w + i) + 2] = z;
        }
    }

    int idx = 0;
    for (int j = 0; j < h - 1; ++j)
    {
        for (int i = 0; i < w - 1; ++i)
        {
            int k00 = j * w + i;
            int k01 = j * w + i + 1;
            int k10 = (j + 1) * w + i;
            int k11 = (j + 1) * w + i + 1;

            (*tris)[idx++] = k00;
            (*tris)[idx++] = k10;
            (*tris)[idx++] = k11;

            (*tris)[idx++] = k00;
            (*tris)[idx++] = k11;
            (*tris)[idx++] = k01;
        }
    }
}

void export_plane_as_obj(const std::string& filename, int nverts, int ntris, float* verts, unsigned int* tris)
{
    std::ofstream of;
    of.open(filename, std::ios::out);
    if (!of.is_open())
    {
        std::stringstream ss;
        ss << "Cannot open file " << filename << " for writing.";
        throw std::runtime_error(ss.str());
    }

    of << "# Created with RiverGenerator\n";
    of << "o RiverLandscape\n";

    for (int i = 0; i < nverts; ++i)
        of << "v " << verts[3 * i] << ' ' << verts[3 * i + 1] << ' ' << verts[3 * i + 2] << '\n';
    for (int i = 0; i < ntris; ++i)
        of << "f " << tris[3 * i] + 1 << ' ' << tris[3 * i + 1] + 1 << ' ' << tris[3 * i + 2] + 1 << '\n';


    of.close();
}