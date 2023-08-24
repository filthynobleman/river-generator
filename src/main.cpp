#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <geometry.hpp>
#include <graph.hpp>
#include <spline.hpp>
#include <sfSmoothLine.hpp>

#define IMAGE_SIZE 2048.0f


std::mt19937 Eng(0);
std::uniform_real_distribution<float> Dist(0.0f, 1.0f);

int main(int argc, const char* const argv[])
{
    // create the window
    sf::RenderTexture renderTexture;
    sf::Vector2u ImgSize(IMAGE_SIZE, IMAGE_SIZE);
    if (!renderTexture.create(ImgSize))
    {
        std::cerr << "Cannot open a render texture." << std::endl;
        return -1;
    }

    int NumPoints = 10;
    int NumSamples = 30;
    float Thickness = 100.0f;

    if (argc > 1)
        NumPoints = std::atoi(argv[1]);
    if (argc > 2)
        NumSamples = std::atoi(argv[2]);
    if (argc > 3)
        Thickness = std::stof(argv[3]);

    std::vector<sf::Vector2f> P;
    P.reserve(NumPoints + 2);
    for (int i = 0; i < NumPoints; ++i)
        P.emplace_back(Dist(Eng), Dist(Eng));
    P.emplace_back(Dist(Eng), -0.05f);
    P.emplace_back(Dist(Eng), 1.05f);


    auto E = delaunay(P);
    Graph G(P, E);
    auto Path = G.ShortestPath(NumPoints, NumPoints + 1);
    std::vector<sf::Vector2f> Nodes;
    Nodes.reserve(Path.Nodes.size());
    for (int i = 0; i < Path.Nodes.size(); ++i)
        Nodes.push_back(P[Path.Nodes[i]]);
    Spline S(Nodes);


    renderTexture.clear();
    sf::Color c1, c2;
    c1 = sf::Color(255, 255, 255, 255);
    sf::RenderStates states;
    states.blendMode = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha);
    for (int i = 0; i < NumSamples - 1; ++i)
    {
        float t0 = i / (float)NumSamples;
        float t1 = (i + 1) / (float)NumSamples;
        sf::Vector2f p0 = IMAGE_SIZE * S(t0);
        sf::Vector2f p1 = IMAGE_SIZE * S(t1);
        sf::CircleShape circle(Thickness / 2);
        circle.setPosition(p1 - sf::Vector2f(Thickness / 2, Thickness / 2));
        circle.setFillColor(c1);
        renderTexture.draw(circle);
        sfLine line(p0, p1, Thickness);
        renderTexture.draw(line, states);
    }
    renderTexture.display();

    auto tex = renderTexture.getTexture();
    auto img = tex.copyToImage();
    if (!img.saveToFile("my_first_line.png"))
    {
        std::cerr << "Error while writing the image." << std::endl;
    }

    return 0;
}