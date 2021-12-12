//--documentation_0
//--Well done
//--END

//--functionality_0
//--Good work
//--END

//--code-quality_0
//--you can consider finding co-ordinates
//--for the neighbors (ni, nj) by doing
//--i.e. ni := min(max(0, ni), m - 1)
//--i.e. nj := min(max(0, nj), n - 1)
//--END

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "image.hpp"

/* This function reads the (original) "stanford.jpg" file and
   computes its sharpness. Furthermore, for different kernels,
   the function blurs this image, saves the blurred image,
   and computes its sharpness. */
int main() {
    std::string inputFile = "stanford.jpg";
    image img = image(inputFile);

    // Compute and output sharpness of original image
    unsigned int origSharpness = img.Sharpness();
    std::cout << "Original image: " << origSharpness << std::endl;

    for (unsigned int k = 3; k <= 27; k+=4) {
        // Create class instance
        image img = image(inputFile);

        // Blur and save image
        img.BoxBlur(k);
        std::ostringstream imageK;
        imageK << "BoxBlur" << std::setw(2) << std::setfill('0') << k << ".jpg";
        std::string imageStringK(imageK.str());
        img.Save(imageStringK);

        // Compute and output sharpness
        unsigned int sharpnessK = img.Sharpness();
        std::cout << "Boxblur(" << std::setw(2) << k << "): "
                  << sharpnessK << std::endl;
    }
}
