#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <boost/multi_array.hpp>

#include "hw6.hpp"

class image {
    // Initialization
    boost::multi_array<unsigned char, 2> img;
    std::string originalFile;
    boost::multi_array<unsigned char, 2> output;

    // Constructor
    image(std::string inputFile);

    // Methods
    void Save(std::string writeFile="");
    void Convolution(boost::multi_array<unsigned char, 2>& input,
                     boost::multi_array<unsigned char, 2>& output,
                     boost::multi_array<float, 2>& kernel);
    void BoxBlur(unsigned int kernelSize);
    int Sharpness(void);
}

#endif /* IMAGE_HPP */
