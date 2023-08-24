/**
 * @file        spline.hpp
 * 
 * @brief       Realizes a natural cubic spline passing through points.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>



class Spline
{
private:
    Eigen::VectorXf m_T;
    Eigen::VectorXf m_X;
    Eigen::VectorXf m_Y;
    Eigen::VectorXf m_Kx;
    Eigen::VectorXf m_Ky;


public:
    Spline(const std::vector<sf::Vector2f>& P);
    Spline(const Spline& S);
    Spline& operator=(const Spline& S);
    ~Spline();


    sf::Vector2f Evaluate(float t) const;
    sf::Vector2f operator()(float t) const;
};