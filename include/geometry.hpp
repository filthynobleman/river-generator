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
#include <vector>
#include <set>


std::set<std::pair<int, int>> delaunay(const std::vector<sf::Vector2f>& P);
std::vector<sf::Vector2f> spline(const std::vector<sf::Vector2f>& P, int Resolution);