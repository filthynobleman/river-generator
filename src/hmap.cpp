/**
 * @file        hmap.cpp
 * 
 * @brief       Implements class HeightMap.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <hmap.hpp>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>


HeightMap::HeightMap(int Size)
{
    m_Width = Size;
    m_Height = Size;
    m_QDirty = false;
    m_Min = 0.0f;
    m_Max = 0.0f;
    m_Data = (float*)std::calloc(m_Width * m_Height, sizeof(float));
    m_QData = (unsigned char*)std::calloc(m_Width * m_Height, sizeof(unsigned char));
}

HeightMap::HeightMap(int Width, int Height)
{
    m_Width = Width;
    m_Height = Height;
    m_QDirty = false;
    m_Min = 0.0f;
    m_Max = 0.0f;
    m_Data = (float*)std::calloc(m_Width * m_Height, sizeof(float));
    m_QData = (unsigned char*)std::calloc(m_Width * m_Height, sizeof(unsigned char));
}

HeightMap::HeightMap(const sf::Image& Img)
{
    m_Width = Img.getSize().x;
    m_Height = Img.getSize().y;
    m_QDirty = false;
    m_Min = 0.0f;
    m_Max = 0.0f;
    m_Data = (float*)std::calloc(m_Width * m_Height, sizeof(float));
    m_QData = (unsigned char*)std::calloc(m_Width * m_Height, sizeof(unsigned char));

    for (int j = 0; j < m_Height; ++j)
    {
        for (int i = 0; i < m_Width; ++i)
        {
            Set(i, j, Img.getPixel({ i, j }).r / 255.0f);
        }
    }

    Quantize();
}

HeightMap::HeightMap(const HeightMap& HM)
{
    m_Width = HM.m_Width;
    m_Height = HM.m_Height;
    m_QDirty = HM.m_QDirty;
    m_Min = HM.m_Min;
    m_Max = HM.m_Max;
    m_Data = (float*)std::malloc(m_Width * m_Height * sizeof(float));
    m_Data = (float*)std::memcpy(m_Data, HM.m_Data, m_Width * m_Height * sizeof(float));
    m_QData = (unsigned char*)std::malloc(m_Width * m_Height * sizeof(unsigned char));
    m_QData = (unsigned char*)std::memcpy(m_QData, HM.m_QData, m_Width * m_Height * sizeof(unsigned char));
}

HeightMap& HeightMap::operator=(const HeightMap& HM)
{
    m_Width = HM.m_Width;
    m_Height = HM.m_Height;
    m_QDirty = HM.m_QDirty;
    m_Min = HM.m_Min;
    m_Max = HM.m_Max;
    m_Data = (float*)std::malloc(m_Width * m_Height * sizeof(float));
    m_Data = (float*)std::memcpy(m_Data, HM.m_Data, m_Width * m_Height * sizeof(float));
    m_QData = (unsigned char*)std::malloc(m_Width * m_Height * sizeof(unsigned char));
    m_QData = (unsigned char*)std::memcpy(m_QData, HM.m_QData, m_Width * m_Height * sizeof(unsigned char));

    return *this;
}

HeightMap::HeightMap(HeightMap&& HM)
{
    m_Width = HM.m_Width;
    m_Height = HM.m_Height;
    m_QDirty = HM.m_QDirty;
    m_Min = HM.m_Min;
    m_Max = HM.m_Max;
    m_Data = HM.m_Data;
    HM.m_Data = nullptr;
    m_QData = HM.m_QData;
    HM.m_QData = nullptr;
}

HeightMap& HeightMap::operator=(HeightMap&& HM)
{
    m_Width = HM.m_Width;
    m_Height = HM.m_Height;
    m_QDirty = HM.m_QDirty;
    m_Min = HM.m_Min;
    m_Max = HM.m_Max;
    m_Data = HM.m_Data;
    HM.m_Data = nullptr;
    m_QData = HM.m_QData;
    HM.m_QData = nullptr;

    return *this;
}

HeightMap::~HeightMap()
{
    if (m_Data != nullptr)
        free(m_Data);
    if (m_QData != nullptr)
        free(m_QData);
}


int HeightMap::GetWidth() const { return m_Width; }
int HeightMap::GetHeight() const { return m_Height; }
float HeightMap::GetMin() const { return m_Min; }
float HeightMap::GetMax() const { return m_Max; }


float HeightMap::Get(int i, int j) const
{
    if (i < 0 || i >= m_Width)
        throw std::runtime_error("Index out of bound.");
    if (j < 0 || j >= m_Height)
        throw std::runtime_error("Index out of bound.");

    return m_Data[j * m_Width + i];
}
float& HeightMap::Get(int i, int j)
{
    if (i < 0 || i >= m_Width)
        throw std::runtime_error("Index out of bound.");
    if (j < 0 || j >= m_Height)
        throw std::runtime_error("Index out of bound.");

    return m_Data[j * m_Width + i];
}
float HeightMap::operator()(int i, int j) const { return Get(i, j); }
float& HeightMap::operator()(int i, int j) { return Get(i, j); }

void HeightMap::Set(int i, int j, float value)
{
    if (i < 0 || i >= m_Width)
        throw std::runtime_error("Index out of bound.");
    if (j < 0 || j >= m_Height)
        throw std::runtime_error("Index out of bound.");

    m_Data[j * m_Width + i] = value;
    m_Min = std::min(m_Min, value);
    m_Max = std::max(m_Max, value);
    m_QDirty = true;
}


void HeightMap::Clamp(float min, float max)
{
    m_Min = min;
    m_Max = max;
    size_t numel = m_Width * m_Height;
    for (size_t i = 0; i < numel; ++i)
        m_Data[i] = std::min(max, std::max(min, m_Data[i]));
    m_QDirty = true;
}

HeightMap HeightMap::Clamped(float min, float max) const
{
    HeightMap Other(*this);
    Other.Clamp(min, max);
    if (Other.IsDirty())
        Other.Quantize();
    return Other;
}


void HeightMap::Normalize(float min, float max)
{
    float m_diff = m_Max - m_Min;
    float diff = max - min;
    size_t numel = m_Width * m_Height;
    for (size_t i = 0; i < numel; ++i)
        m_Data[i] = ((m_Data[i] - m_Min) / m_diff) * diff + min;
    m_Min = min;
    m_Max = max;
    m_QDirty = true;
}


HeightMap HeightMap::Normalized(float min, float max) const
{
    HeightMap Other(*this);
    Other.Normalize(min, max);
    if (Other.IsDirty())
        Other.Quantize();
    return Other;
}

void HeightMap::Invert()
{
    size_t numel = m_Width * m_Height;
    for (size_t i = 0; i < numel; ++i)
        m_Data[i] = m_Max - m_Data[i] + m_Min;
}

HeightMap HeightMap::Inverted() const 
{
    HeightMap Other(*this);
    Other.Invert();
    if (Other.IsDirty())
        Other.Quantize();
    return Other;
}


void HeightMap::Quantize()
{
    float m_diff = m_Max - m_Min;
    size_t numel = m_Width * m_Height;
    for (size_t i = 0; i < numel; ++i)
        m_QData[i] = ((m_Data[i] - m_Min) / m_diff) * 255;
}
bool HeightMap::IsDirty() const { return m_QDirty; }
const unsigned char* const HeightMap::Quantized() const { return m_QData; }
const float* const HeightMap::RawData() const { return m_Data; }