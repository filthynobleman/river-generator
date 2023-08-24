/**
 * @file        sfSmoothLine.cpp
 * 
 * @brief       Implements sfSmoothLine.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-23
 */
#include <sfSmoothLine.hpp>
#include <cmath>


sfSmoothLine::sfSmoothLine(const sf::Vector2f& point1, 
                           const sf::Vector2f& point2,
                           int thickness,
                           const sf::Color& inCol,
                           const sf::Color& outCol,
                           bool gammaCorr)
    : sfLine(point1, point2, thickness, inCol)
{
    m_col2 = outCol;
    m_gammaCorr = gammaCorr;
}

sfSmoothLine::sfSmoothLine(const sfSmoothLine& line)
    : sfLine(line)
{
    m_col2 = line.m_col2;
    m_gammaCorr = line.m_gammaCorr;
}

sfSmoothLine& sfSmoothLine::operator=(const sfSmoothLine& line)
{
    sfLine::operator=(line);
    m_col2 = line.m_col2;
    m_gammaCorr = line.m_gammaCorr;

    return *this;
}


void sfSmoothLine::draw(sf::RenderTarget &target, 
                        const sf::RenderStates& states) const
{
    if (m_thick <= 1.0f)
    {
        sfLine::draw(target, states);
        return;
    }

    float thickness = m_thick;
    sf::RectangleShape rect({ 1.0f, 1.0f });
    rect.setPosition(m_pos);
    rect.setRotation(m_rot);
    sf::Color col = m_col2;

    for (; thickness >= 1.0f; thickness -= 1.0f)
    {
        float t = (thickness - 1) / (m_thick - 1);
        float r1 = m_col1.r / 255.0f;
        float g1 = m_col1.g / 255.0f;
        float b1 = m_col1.b / 255.0f;
        float a1 = m_col1.a / 255.0f;
        float r2 = m_col2.r / 255.0f;
        float g2 = m_col2.g / 255.0f;
        float b2 = m_col2.b / 255.0f;
        float a2 = m_col2.a / 255.0f;
        float r, g, b, a;
        if (m_gammaCorr)
        {
            r = std::sqrt((1 - t) * r1 * r1 + t * r2 * r2);
            g = std::sqrt((1 - t) * g1 * g1 + t * g2 * g2);
            b = std::sqrt((1 - t) * b1 * b1 + t * b2 * b2);
            a = std::sqrt((1 - t) * a1 * a1 + t * a2 * a2);
        }
        else
        {
            r = (1 - t) * r1 + t * r2;
            g = (1 - t) * g1 + t * g2;
            b = (1 - t) * b1 + t * b2;
            a = (1 - t) * a1 + t * a2;
        }
        col = sf::Color(255 * r, 255 * g, 255 * b, 255 * a);

        rect.setFillColor(col);
        rect.setScale({ m_len, thickness });
        target.draw(rect, states);
        rect.setScale({ -m_len, thickness });
        target.draw(rect, states);
        rect.setScale({ m_len, -thickness });
        target.draw(rect, states);
        rect.setScale({ -m_len, -thickness });
        target.draw(rect, states);

    }
}