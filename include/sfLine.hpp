/**
 * @file        sfLine.hpp
 * 
 * @brief       SFML line with thickness.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-23
 */
#pragma once

#include <SFML/Graphics.hpp>



class sfLine : public sf::Drawable
{
protected:
    // sf::RectangleShape m_rect;
    sf::Vector2f m_pos;
    sf::Angle m_rot;
    float m_thick;
    float m_len;
    sf::Color m_col1;

public:
    sfLine(const sf::Vector2f& point1, 
           const sf::Vector2f& point2,
           int thickness = 1.0f,
           const sf::Color& color = sf::Color(255, 255, 255));

    sfLine(const sfLine& line);
    virtual sfLine& operator=(const sfLine& line);

    virtual void draw(sf::RenderTarget &target, 
                      const sf::RenderStates& states) const override;
};