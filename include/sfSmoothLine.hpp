/**
 * @file        sfSmoothLine.hpp
 * 
 * @brief       SFML line with thickness and color gradiens.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-23
 */
#pragma once

#include <sfLine.hpp>


class sfSmoothLine : public sfLine
{
protected:
    sf::Color m_col2;
    bool m_gammaCorr;

public:
    sfSmoothLine(const sf::Vector2f& point1, 
                 const sf::Vector2f& point2,
                 int thickness = 1.0f,
                 const sf::Color& inCol = sf::Color(255, 255, 255),
                 const sf::Color& outCol = sf::Color(0, 0, 0),
                 bool gammaCorr = true);

    sfSmoothLine(const sfSmoothLine& line);
    virtual sfSmoothLine& operator=(const sfSmoothLine& line);

    virtual void draw(sf::RenderTarget &target, 
                      const sf::RenderStates& states) const override;
};
