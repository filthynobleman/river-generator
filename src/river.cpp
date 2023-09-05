/**
 * @file        river.cpp
 * 
 * @brief       Implements river generation pipeline.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <geometry.hpp>
#include <graph.hpp>
#include <spline.hpp>
#include <sfLine.hpp>
#include <sstream>
#include <random>


HeightMap river(int w, int h, int nodes, int samples, float thickness, int ksx, int ksy, float sigma, int seed)
{
    std::mt19937 Eng(seed);
    std::uniform_real_distribution<float> Dist(0.0f, 1.0f);

    // Randomly sample the plane
    std::vector<sf::Vector2f> P;
    P.reserve(nodes + 2);
    for (int i = 0; i < nodes; ++i)
        P.emplace_back(Dist(Eng), Dist(Eng));
    P.emplace_back(Dist(Eng), -0.1f);
    P.emplace_back(Dist(Eng), 1.1f);

    // Compute the river's spline
    auto E = delaunay(P);
    Graph G(P, E);
    auto Path = G.ShortestPath(nodes, nodes + 1);
    std::vector<sf::Vector2f> Nodes;
    Nodes.reserve(Path.Nodes.size());
    for (int i = 0; i < Path.Nodes.size(); ++i)
        Nodes.push_back(P[Path.Nodes[i]]);
    Spline S(Nodes);

    // Create the render target
    sf::RenderTexture renderTexture;
    sf::Vector2u ImgSize(w, h);
    if (!renderTexture.create(ImgSize))
    {
        std::stringstream ss;
        ss << "Cannot create a " << w << "-by-" << h << " render texture.";
        throw std::runtime_error(ss.str());
    }
    renderTexture.clear();
    sf::Color c1, c2;
    c1 = sf::Color(255, 255, 255, 255);
    sf::RenderStates states;
    states.blendMode = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha);
    for (int i = 0; i < nodes - 1; ++i)
    {
        float t0 = i / (float)nodes;
        float t1 = (i + 1) / (float)nodes;
        sf::Vector2f p0 = S(t0);
        sf::Vector2f p1 = S(t1);
        p0.x *= w;
        p0.y *= h;
        p1.x *= w;
        p1.y *= h;
        sf::CircleShape circle(thickness / 2);
        circle.setPosition(p1 - sf::Vector2f(thickness / 2, thickness / 2));
        circle.setFillColor(c1);
        renderTexture.draw(circle);
        sfLine line(p0, p1, thickness);
        renderTexture.draw(line, states);
    }
    renderTexture.display();

    // Transfer to heightmap and blur
    auto tex = renderTexture.getTexture();
    auto img = tex.copyToImage();
    HeightMap hmap(img);
    gauss_blur(hmap, ksx, ksy, sigma);


    return hmap;
}