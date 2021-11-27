#include <iostream>
#include <string>
#include <vector>

#include "image.hpp"

int main() {
    std::string inputFile = "stanford.jpg";
    image img = image(inputFile);
    //img.Save("stanford2.jpg");
    unsigned int origSharpness = img.Sharpness();
    std::cout << "Original image: " << origSharpness << std::endl;

    for (unsigned int k = 3; k <= 27; k+=4) {
        image img = image(inputFile);
        img.BoxBlur(k);
        //std::string writeFile = "BoxBlurk.jpg";
        //img.Save(writeFile);
        unsigned int sharpnessk = img.Sharpness();
        std::cout << "Boxblur(" << k << "): " << sharpnessk << std::endl;
    }
}
