/**
 * @file        main.cpp
 * 
 * @brief       River generation application.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <iostream>
#include <parser.hpp>
#include <geometry.hpp>
#include <noises.hpp>
#include <plane.hpp>
#include <stb_image_write.h>

int main(int argc, const char* const argv[])
{
    // // create the window
    // sf::RenderTexture renderTexture;
    // sf::Vector2u ImgSize(IMAGE_SIZE, IMAGE_SIZE);
    // if (!renderTexture.create(ImgSize))
    // {
    //     std::cerr << "Cannot open a render texture." << std::endl;
    //     return -1;
    // }

    // int NumPoints = 10;
    // int NumSamples = 30;
    // float Thickness = 100.0f;

    // if (argc > 1)
    //     NumPoints = std::atoi(argv[1]);
    // if (argc > 2)
    //     NumSamples = std::atoi(argv[2]);
    // if (argc > 3)
    //     Thickness = std::stof(argv[3]);

    // std::vector<sf::Vector2f> P;
    // P.reserve(NumPoints + 2);
    // for (int i = 0; i < NumPoints; ++i)
    //     P.emplace_back(Dist(Eng), Dist(Eng));
    // P.emplace_back(Dist(Eng), -0.05f);
    // P.emplace_back(Dist(Eng), 1.05f);


    // auto E = delaunay(P);
    // Graph G(P, E);
    // auto Path = G.ShortestPath(NumPoints, NumPoints + 1);
    // std::vector<sf::Vector2f> Nodes;
    // Nodes.reserve(Path.Nodes.size());
    // for (int i = 0; i < Path.Nodes.size(); ++i)
    //     Nodes.push_back(P[Path.Nodes[i]]);
    // Spline S(Nodes);


    // renderTexture.clear();
    // sf::Color c1, c2;
    // c1 = sf::Color(255, 255, 255, 255);
    // sf::RenderStates states;
    // states.blendMode = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha);
    // for (int i = 0; i < NumSamples - 1; ++i)
    // {
    //     float t0 = i / (float)NumSamples;
    //     float t1 = (i + 1) / (float)NumSamples;
    //     sf::Vector2f p0 = IMAGE_SIZE * S(t0);
    //     sf::Vector2f p1 = IMAGE_SIZE * S(t1);
    //     sf::CircleShape circle(Thickness / 2);
    //     circle.setPosition(p1 - sf::Vector2f(Thickness / 2, Thickness / 2));
    //     circle.setFillColor(c1);
    //     renderTexture.draw(circle);
    //     sfLine line(p0, p1, Thickness);
    //     renderTexture.draw(line, states);
    // }
    // renderTexture.display();

    // auto tex = renderTexture.getTexture();
    // auto img = tex.copyToImage();
    // gauss_blur(img, 60, 40, 10);
    // std::cout << "Blurred." << std::endl;
    // if (!img.saveToFile("my_first_line.png"))
    // {
    //     std::cerr << "Error while writing the image." << std::endl;
    // }


    // Parse arguments from file
    if (argc < 2)
    {
        std::cerr << "Missing input configuration file." << std::endl;
        return -1;
    }
    RiverParams Params;
    try
    {
        Params = parse_river_params(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }


    // Compute the river
    HeightMap HM = river(Params.Width, Params.Height, 
                         Params.RiverNodes, Params.RiverSamples, Params.RiverThickness,
                         Params.GaussKSX, Params.GaussKSY, Params.GaussSigma,
                         Params.RiverSeed);

    // Add noises
    add_voronoi(HM, Params.VoronoiWeight, Params.VoronoiScale);
    add_perlin(HM, Params.PerlinWeight, Params.PerlinScale, Params.PerlinOctaves);
    HM.Invert();

    // Add delta height
    for (int j = 0; j < HM.GetHeight(); ++j)
    {
        float t = j / (float)(HM.GetHeight() - 1);
        float d = Params.PlaneDelta * (1 - t);
        for (int i = 0; i < HM.GetWidth(); ++i)
            HM.Set(i, j, HM(i, j) + d);
    }


    // Save image
    HM.Quantize();
    stbi_write_png("my_first_river_scene.png", Params.Width, Params.Height, 1, HM.Quantized(), Params.Width);


    // Export plane
    float* verts;
    unsigned int* tris;
    int nverts = Params.PlaneWidth * Params.PlaneHeight;
    int ntris = 0;
    triangulate_plane(HM, Params.PlaneWidth, Params.PlaneHeight, ntris, &verts, &tris);
    export_plane_as_obj("my_first_river_scene.obj", nverts, ntris, verts, tris);
    

    return 0;
}