/**
 * @file        sfLine.cpp
 * 
 * @brief       Implements sfLine.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-23
 */
#include <sfLine.hpp>
#include <cmath>


sfLine::sfLine(const sf::Vector2f& point1, 
               const sf::Vector2f& point2,
               int thickness,
               const sf::Color& color)
{
    auto p = point2 - point1;
    m_thick = thickness / 2;
    m_len = p.length() / 2;
    float angle = std::atan2(p.y, p.x);
    // sf::Vector2f n(p.y, -p.x);
    // n = point1 + 0.5f * thickness * n.normalized();
    // n.y -= 0.5f * thickness;
    sf::Vector2f n = 0.5f * (point2 + point1);
    m_pos = n;
    m_rot = sf::radians(angle);
    m_col1 = color;
}

sfLine::sfLine(const sfLine& line)
{
    m_pos = line.m_pos;
    m_rot = line.m_rot;
    m_len = line.m_len;
    m_thick = line.m_thick;
    m_col1 = line.m_col1;
}

sfLine& sfLine::operator=(const sfLine& line)
{
    m_pos = line.m_pos;
    m_rot = line.m_rot;
    m_len = line.m_len;
    m_thick = line.m_thick;
    m_col1 = line.m_col1;

    return *this;
}

void sfLine::draw(sf::RenderTarget &target, 
                  const sf::RenderStates& states) const
{
    sf::RectangleShape r({ m_len, m_thick });
    r.setPosition(m_pos);
    r.setRotation(m_rot);
    r.setFillColor(m_col1);
    target.draw(r);
    r.scale({ -1.0f, 1.0f });
    target.draw(r);
    r.scale({ 1.0f, -1.0f });
    target.draw(r);
    r.scale({ -1.0f, 1.0f });
    target.draw(r);
}