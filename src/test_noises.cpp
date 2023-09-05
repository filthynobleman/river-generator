/**
 * @file        test_noises.cpp
 * 
 * @brief       Test noise functions.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-09-05
 */
#include <noises.hpp>
#include <stb_image_write.h>
#include <iostream>


int main(int argc, const char * const argv[])
{

    HeightMap V = perlin(1024, 1024, 5.0f, 6);
    add_voronoi(V, 0.5f, 5.0f);


    std::cout << V.GetMin() << ", " << V.GetMax() << std::endl;

    V.Quantize();
    stbi_write_png("my_first_voronoi.png", 1024, 1024, 1, V.Quantized(), 1024);


    
    return 0;
}
