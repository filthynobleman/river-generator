/**
 * @file        hmap.hpp
 * 
 * @brief       Declaration of HeightMap.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-23
 */
#pragma once


class HeightMap
{
private:
    int m_Width;                // Map x-size
    int m_Height;               // Map y-size
    float* m_Data;              // Terrain's height at each cell
    unsigned char* m_QData;     // Quantized height data (for exporting)

public:
    /**
     * @brief       Initialize a Size-by-Size heightmap.
     * 
     * @param Size 
     */
    HeightMap(int Size);

    /**
     * @brief       Initialize a Width-by-Height heightmap.
     * 
     * @param Width 
     * @param Height 
     */
    HeightMap(int Width, int Height);

    HeightMap(const HeightMap& HM);
    HeightMap(HeightMap&& HM);
    HeightMap& operator=(const HeightMap& HM);
    HeightMap& operator=(HeightMap&& HM);

    ~HeightMap();


    float Get(int i, int j) const;
    float& Get(int i, int j);
    float operator()(int i, int j) const;
    float& operator()(int i, int j);

    void Set(int i, int j, int value);

    void Clamp(float min = 0.0f, float max = 1.0f);
    HeightMap Clamped(float min = 0.0f, float max = 1.0f) const;

    void Normalize(float min = 0.0f, float max = 1.0f);
    HeightMap Normalized(float min = 0.0f, float max = 1.0f) const;
};