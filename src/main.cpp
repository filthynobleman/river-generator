#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <sfSmoothLine.hpp>

int main(int argc, const char* const argv[])
{
    // create the window
    sf::RenderTexture renderTexture;
    if (!renderTexture.create({ 2048, 2048 }))
    {
        std::cerr << "Cannot open a render texture." << std::endl;
        return -1;
    }

    sf::Vector2f p0(1024.0f, 1024.0f);
    sf::Vector2f p1(0.0f, 0.0f);
    sf::Vector2f off(25.0f, 25.0f);

    if (argc > 1)
    {
        float pi = 3.14159265f;
        float angle = pi * (std::stof(argv[1]) / 180.0f);
        p1 = { std::cos(angle), std::sin(angle) };
        p1 = p0 + 512.0f * p1;
    }
    if (argc > 2)
        p1 = { std::stof(argv[1]), std::stof(argv[2]) };

    sfSmoothLine line(p0, p1, 30);

    sf::CircleShape circle(25.0f);
    circle.setFillColor(sf::Color::Red);


    renderTexture.clear();
    renderTexture.draw(line);
    circle.setPosition(p0 - off);
    renderTexture.draw(circle);
    circle.setPosition(p1 - off);
    renderTexture.draw(circle);
    renderTexture.display();

    auto tex = renderTexture.getTexture();
    auto img = tex.copyToImage();
    if (!img.saveToFile("my_first_line.jpg"))
    {
        std::cerr << "Error while writing the image." << std::endl;
    }

    return 0;
}