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
#include <filesystem>

int main(int argc, const char* const argv[])
{
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
    std::filesystem::path OutImg(Params.OutHMap);
    if (OutImg.extension() == ".png")
        stbi_write_png(Params.OutHMap.c_str(), Params.Width, Params.Height, 1, HM.Quantized(), Params.Width);
    else if(OutImg.extension() == ".jpg" || OutImg.extension() == ".jpeg")
        stbi_write_jpg(Params.OutHMap.c_str(), Params.Width, Params.Height, 1, HM.Quantized(), 90);
    else if (OutImg.extension() == ".bmp")
        stbi_write_bmp(Params.OutHMap.c_str(), Params.Width, Params.Height, 1, HM.Quantized());
    else if (OutImg.extension() == ".hdr")
        stbi_write_hdr(Params.OutHMap.c_str(), Params.Width, Params.Height, 1, HM.RawData());
    else
    {
        std::cerr << "Image format " << OutImg.extension() << " is not yet supported. Exporting as PNG." << std::endl;
        Params.OutHMap = OutImg.replace_extension(".png").string();
        stbi_write_png(Params.OutHMap.c_str(), Params.Width, Params.Height, 1, HM.Quantized(), Params.Width);
    }


    // Export plane
    float* verts;
    unsigned int* tris;
    int nverts = Params.PlaneWidth * Params.PlaneHeight;
    int ntris = 0;
    HM.Normalize();
    triangulate_plane(HM, Params.PlaneWidth, Params.PlaneHeight, ntris, &verts, &tris);
    export_plane_as_obj("my_first_river_scene.obj", nverts, ntris, verts, tris);
    

    return 0;
}