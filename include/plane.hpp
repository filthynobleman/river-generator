/**
 * @file        plane.hpp
 * 
 * @brief       Plane export functions.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#pragma once

#include <hmap.hpp>
#include <string>

void triangulate_plane(const HeightMap& HM, int w, int h, int& ntris, float** verts, unsigned int** tris);
void export_plane_as_obj(const std::string& filename, int nverts, int ntris, float* verts, unsigned int* tris);