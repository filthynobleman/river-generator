/**
 * @file        parser.hpp
 * 
 * @brief       JSON parsing.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <sstream>


struct RiverParams
{
    int Width;
    int Height;
    float PerlinWeight;
    float PerlinScale;
    int PerlinOctaves;
    float VoronoiWeight;
    float VoronoiScale;
    int RiverNodes;
    int RiverSamples;
    float RiverThickness;
    int RiverSeed;
    int GaussKSX;
    int GaussKSY;
    float GaussSigma;
    float PlaneDelta;
    int PlaneWidth;
    int PlaneHeight;

    std::string OutHMap;
    std::string OutMesh;
};


RiverParams parse_river_params(const std::string& filename);