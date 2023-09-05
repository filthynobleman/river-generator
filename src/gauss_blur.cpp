/**
 * @file        gauss_blur.cpp
 * 
 * @brief       Implements gauss_blur().
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#include <geometry.hpp>
#include <cmath>
#include <iostream>


void gauss_blur(sf::Image& Img, int ksx, int ksy, float sigma)
{
    int ksmax = std::max(ksx, ksy);
    int smax = std::max(Img.getSize().x, Img.getSize().y);
    float* tmp = (float*)malloc((2 * ksmax + 1 + smax) * sizeof(float));
    if (tmp == nullptr)
        throw std::runtime_error("Cannot allocate support memory for gaussian blur.");

    float* ker = tmp + smax;


    // Horizontal blur
    ker[ksx] = 1.0f;
    float sum = 1.0f;
    for (int i = 0; i < ksx; ++i)
    {
        float x = i;
        ker[ksx - i - 1] = std::exp(-x * x / (2 * sigma * sigma));
        ker[ksx + i + 1] = ker[ksx - i - 1];
        sum += ker[ksx + i + 1] * 2;
    }
    for (int i = 0; i < 2 * ksx + 1; ++i)
        ker[i] /= sum;
    
    // Blur each line
    for (int j = 0; j < Img.getSize().y; ++j)
    {
        // Copy the line into temporary array and convert to float
        for (int i = 0; i < Img.getSize().x; ++i)
        {
            // We assume to work with grayscale images
            tmp[i] = Img.getPixel({i, j}).r / 255.0f;
        }
        // Apply blur
        for (int i = 0; i < Img.getSize().x; ++i)
        {
            float pixel = ker[ksx] * tmp[i];
            for (int dx = 0; dx < ksx; ++dx)
            {
                int ii = i - dx - 1;
                if (ii >= 0)
                    pixel += ker[ksx - dx - 1] * tmp[ii];
                ii = i + dx + 1;
                if (ii < Img.getSize().x)
                    pixel += ker[ksx + dx + 1] * tmp[ii];
            }
            // Anyway, we want to save the image to all channels
            char qpix = pixel * 255.0f;
            // char pixel = 255;
            Img.setPixel({ i, j }, sf::Color(qpix, qpix, qpix, 255));
        }
    }


    // Vertical blur
    ker[ksy] = 1.0f;
    sum = 1.0f;
    for (int i = 0; i < ksy; ++i)
    {
        float y = i;
        ker[ksy - i - 1] = std::exp(-y * y / (2 * sigma * sigma));
        ker[ksy + i + 1] = ker[ksy - i - 1];
        sum += ker[ksy + i + 1] * 2;
    }
    for (int i = 0; i < 2 * ksy + 1; ++i)
        ker[i] /= sum;

    // Blur each column
    for (int i = 0; i < Img.getSize().x; ++i)
    {
        // Copy the column into temporary array and convert to float
        for (int j = 0; j < Img.getSize().y; ++j)
        {
            // We assume to work with grayscale images
            tmp[j] = Img.getPixel({i, j}).r / 255.0f;
        }
        // Apply blur
        for (int j = 0; j < Img.getSize().y; ++j)
        {
            float pixel = ker[ksy] * tmp[j];
            for (int dy = 0; dy < ksy; ++dy)
            {
                int jj = j - dy - 1;
                if (jj >= 0)
                    pixel += ker[ksy - dy - 1] * tmp[jj];
                jj = j + dy + 1;
                if (jj < Img.getSize().x)
                    pixel += ker[ksy + dy + 1] * tmp[jj];
            }
            // Anyway, we want to save the image to all channels
            char qpix = pixel * 255.0f;
            // char pixel = 255;
            Img.setPixel({ i, j }, sf::Color(qpix, qpix, qpix, 255));
        }
    }


    free(tmp);
}


void gauss_blur(HeightMap& HM, int ksx, int ksy, float sigma)
{
    int ksmax = std::max(ksx, ksy);
    int smax = std::max(HM.GetWidth(), HM.GetHeight());
    float* tmp = (float*)malloc((2 * ksmax + 1 + smax) * sizeof(float));
    if (tmp == nullptr)
        throw std::runtime_error("Cannot allocate support memory for gaussian blur.");

    float* ker = tmp + smax;


    // Horizontal blur
    ker[ksx] = 1.0f;
    float sum = 1.0f;
    for (int i = 0; i < ksx; ++i)
    {
        float x = i;
        ker[ksx - i - 1] = std::exp(-x * x / (2 * sigma * sigma));
        ker[ksx + i + 1] = ker[ksx - i - 1];
        sum += ker[ksx + i + 1] * 2;
    }
    for (int i = 0; i < 2 * ksx + 1; ++i)
        ker[i] /= sum;
    
    // Blur each line
    for (int j = 0; j < HM.GetHeight(); ++j)
    {
        // Copy the line into temporary array and convert to float
        for (int i = 0; i < HM.GetWidth(); ++i)
            tmp[i] = HM(i, j);
        // Apply blur
        for (int i = 0; i < HM.GetWidth(); ++i)
        {
            float pixel = ker[ksx] * tmp[i];
            for (int dx = 0; dx < ksx; ++dx)
            {
                int ii = i - dx - 1;
                if (ii >= 0)
                    pixel += ker[ksx - dx - 1] * tmp[ii];
                ii = i + dx + 1;
                if (ii < HM.GetWidth())
                    pixel += ker[ksx + dx + 1] * tmp[ii];
            }
            HM.Set(i, j, pixel);
        }
    }


    // Vertical blur
    ker[ksy] = 1.0f;
    sum = 1.0f;
    for (int i = 0; i < ksy; ++i)
    {
        float y = i;
        ker[ksy - i - 1] = std::exp(-y * y / (2 * sigma * sigma));
        ker[ksy + i + 1] = ker[ksy - i - 1];
        sum += ker[ksy + i + 1] * 2;
    }
    for (int i = 0; i < 2 * ksy + 1; ++i)
        ker[i] /= sum;

    // Blur each column
    for (int i = 0; i < HM.GetWidth(); ++i)
    {
        // Copy the column into temporary array and convert to float
        for (int j = 0; j < HM.GetHeight(); ++j)
            tmp[j] = HM(i, j);
        // Apply blur
        for (int j = 0; j < HM.GetHeight(); ++j)
        {
            float pixel = ker[ksy] * tmp[j];
            for (int dy = 0; dy < ksy; ++dy)
            {
                int jj = j - dy - 1;
                if (jj >= 0)
                    pixel += ker[ksy - dy - 1] * tmp[jj];
                jj = j + dy + 1;
                if (jj < HM.GetWidth())
                    pixel += ker[ksy + dy + 1] * tmp[jj];
            }
            HM.Set(i, j, pixel);
        }
    }


    free(tmp);
}