/**
 * @file        geometry.hpp
 * 
 * @brief       Geometric algorithms.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <hmap.hpp>
#include <vector>
#include <set>


HeightMap river(int w, int h, int nodes, int samples, float thickness, int ksx, int ksy, float sigma, int seed = 0);
std::set<std::pair<int, int>> delaunay(const std::vector<sf::Vector2f>& P);
void gauss_blur(sf::Image& Img, int ksx, int ksy, float sigma);
void gauss_blur(HeightMap& HM, int ksx, int ksy, float sigma);