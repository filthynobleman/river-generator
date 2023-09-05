/**
 * @file        parser.cpp
 * 
 * @brief       Implement argument parsing.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <parser.hpp>
#include <filesystem>
#include <algorithm>
#include <cctype>

char my_tolower(char c)
{
    return std::tolower(c);
}

RiverParams parse_river_params(const std::string& filename)
{
    std::ifstream stream;
    stream.open(filename, std::ios::in);
    if (!stream.is_open())
    {
        std::stringstream ss;
        ss << "Cannot open file " << filename << " for reading.";
        throw std::runtime_error(ss.str());
    }

    nlohmann::json j = nlohmann::json::parse(stream);
    stream.close();


    RiverParams params;
    // Image size
    if (j.contains("size"))
    {
        if (j["size"].is_number_integer())
        {
            params.Width = j["size"];
            params.Height = params.Width;
        }
        else if (j["size"].is_array())
        {
            std::vector<int> size;
            try
            {
                size = j["size"].template get<std::vector<int>>();
            }
            catch(const std::exception& e)
            {
                std::stringstream ss;
                ss << "JSON parse error on file " << filename << std::endl;
                ss << "Array attribute \"size\" must contain integer numbers.";
                throw std::runtime_error(ss.str());
            }
            params.Width = size[0];
            params.Height = size[1];
        }
        else
        {
            std::stringstream ss;
            ss << "JSON parse error on file " << filename << std::endl;
            ss << "Attribute \"size\" must be either an integer or an array of integers.";
            throw std::runtime_error(ss.str());
        }
    }
    else if (j.contains("width") && j.contains("height"))
    {
        if (!j["width"].is_number_integer() || !j["height"].is_number_integer())
        {
            std::stringstream ss;
            ss << "JSON parse error on file " << filename << std::endl;
            ss << "Attributes \"width\" and \"height\" must be integers.";
            throw std::runtime_error(ss.str());
        }
        params.Width = j["width"];
        params.Height = j["height"];
    }
    else
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contain attribute \"size\" or both attributes \"width\" and \"height\".";
        throw std::runtime_error(ss.str());
    }


    // Perlin noise settings
    if (!j.contains("perlin"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contains attribute \"perlin\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"].is_object())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"perlin\" must be an object.";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"].contains("weight"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"perlin\" must contain sub-attribute \"weight\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"]["weight"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"weight\" inside \"perlin\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"].contains("scale"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"perlin\" must contain sub-attribute \"scale\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"]["scale"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"scale\" inside \"perlin\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"].contains("octaves"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"perlin\" must contain sub-attribute \"octaves\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["perlin"]["octaves"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"octaves\" inside \"perlin\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    params.PerlinWeight = j["perlin"]["weight"];
    params.PerlinScale = j["perlin"]["scale"];
    params.PerlinOctaves = j["perlin"]["octaves"];


    // Voronoi settings
    if (!j.contains("voronoi"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contains attribute \"voronoi\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["voronoi"].is_object())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"voronoi\" must be an object.";
        throw std::runtime_error(ss.str());
    }
    if (!j["voronoi"].contains("weight"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"voronoi\" must contain sub-attribute \"weight\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["voronoi"]["weight"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"weight\" inside \"voronoi\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    if (!j["voronoi"].contains("scale"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"voronoi\" must contain sub-attribute \"scale\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["voronoi"]["scale"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"scale\" inside \"voronoi\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    params.VoronoiWeight = j["voronoi"]["weight"];
    params.VoronoiScale = j["voronoi"]["scale"];


    // River settings
    if (!j.contains("river"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contains attribute \"river\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"].is_object())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"river\" must be an object.";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"].contains("thickness"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"river\" must contain sub-attribute \"thickness\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"]["thickness"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"thickness\" inside \"river\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"].contains("nodes"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"river\" must contain sub-attribute \"nodes\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"]["nodes"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"nodes\" inside \"river\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"].contains("samples"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"river\" must contain sub-attribute \"samples\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"]["samples"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"samples\" inside \"river\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"].contains("seed"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"river\" must contain sub-attribute \"seed\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["river"]["seed"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"seed\" inside \"river\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    params.RiverThickness = j["river"]["thickness"];
    params.RiverNodes = j["river"]["nodes"];
    params.RiverSamples = j["river"]["samples"];
    params.RiverSeed = j["river"]["seed"];


    // Blur settings
    if (!j.contains("gauss"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contains attribute \"gauss\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"].is_object())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"gauss\" must be an object.";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"].contains("sigma"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"gauss\" must contain sub-attribute \"sigma\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"]["sigma"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"sigma\" inside \"gauss\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"].contains("ksx"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"gauss\" must contain sub-attribute \"ksx\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"]["ksx"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"ksx\" inside \"gauss\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"].contains("ksy"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"gauss\" must contain sub-attribute \"ksy\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["gauss"]["ksy"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"ksy\" inside \"gauss\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    params.GaussSigma = j["gauss"]["sigma"];
    params.GaussKSX = j["gauss"]["ksx"];
    params.GaussKSY = j["gauss"]["ksy"];


    // Plane settings
    if (!j.contains("plane"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contains attribute \"plane\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"].is_object())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"plane\" must be an object.";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"].contains("delta"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"plane\" must contain sub-attribute \"delta\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"]["delta"].is_number_float())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"delta\" inside \"plane\" must be a float.";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"].contains("width"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"plane\" must contain sub-attribute \"width\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"]["width"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"width\" inside \"plane\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"].contains("height"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"plane\" must contain sub-attribute \"height\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["plane"]["height"].is_number_integer())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Sub-attribute \"height\" inside \"plane\" must be an integer.";
        throw std::runtime_error(ss.str());
    }
    params.PlaneDelta = j["plane"]["delta"];
    params.PlaneWidth = j["plane"]["width"];
    params.PlaneHeight = j["plane"]["height"];



    // Output filenames
    if (!j.contains("output_file"))
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "File must contains attribute \"output_file\".";
        throw std::runtime_error(ss.str());
    }
    if (!j["output_file"].is_string())
    {
        std::stringstream ss;
        ss << "JSON parse error on file " << filename << std::endl;
        ss << "Attribute \"output_file\" must be a string.";
        throw std::runtime_error(ss.str());
    }
    params.OutHMap = j["output_file"];
    std::transform(params.OutHMap.begin(), params.OutHMap.end(), params.OutHMap.begin(), my_tolower);
    params.OutMesh = std::filesystem::path(params.OutHMap).replace_extension(".obj").string();


    return params;
}