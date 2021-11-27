#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <boost/multi_array.hpp>

#include "hw6.hpp"

class image {
    // Private

    /* Class attributes */
    boost::multi_array<unsigned char, 2> inImg;
    std::string originalFile;
    boost::multi_array<unsigned char, 2> outImg;

    /* Method that performs convolution on an output multi-array
       using an input and a kernel. */
    void Convolution(boost::multi_array<unsigned char, 2>& input,
                     boost::multi_array<unsigned char, 2>& output,
                     boost::multi_array<float, 2>& kernel);

  public:
    /* Constructor; takes in a JPEG image (.jpg) and
       stores its data in a multi-array. */
    image(std::string inputFile);

    /* Method that writes a multi-array image to a JPEG file
       under the name specified by the user. */
    void Save(std::string writeFile="");

    /* Method that creates a blurred image using a kernel of
       user-specified size. */
    void BoxBlur(unsigned int kernelSize);

    /* Method that computes the sharpness of a given multi-array
       representation of an image. */
    unsigned int Sharpness(void);
};

#endif /* IMAGE_HPP */
