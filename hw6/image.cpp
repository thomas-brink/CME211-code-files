#define BOOST_DISABLE_ASSERTS

#include <boost/multi_array.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "image.hpp"
#include "hw6.hpp"

/* Constructor that reads input JPEG file and stores this file
   as a boost multi-array. */
image::image(std::string inputFile):originalFile(inputFile) {
    ReadGrayscaleJPEG(inputFile, img);
    this->output = img;
    std::cout << img.shape()[0] << " " << img.shape()[1] << std::endl;
}

/* Function that takes a name of an output file and writes the
   data attribute to this file in JPEG format */
void image::Save(std::string writeFile /*=""*/) {
    if (writeFile == "") {
        // Use original file and write to JPEG
        WriteGrayscaleJPEG(originalFile, img);
    }
    else {
        // Use provided name and write to JPEG
        WriteGrayscaleJPEG(writeFile, img);
    }
}

/* Method that takes in an input multi-array, original output
   multi-array, and a kernel, and performs convolution using
   the kernel to form a convoluted output. */
void Convolution(boost::multi_array<unsigned_char, 2>& input,
                 boost::multi_array<unsigned_char, 2>& output,
                 boost::multi_array<float, 2>& kernel) {

    unsigned int inputRows = input.shape()[0];
    unsigned int inputCols = input.shape()[1];
    unsigned int outputRows = output.shape()[0];
    unsigned int outputCols = output.shape()[1];
    unsigned int kernelRows = kernel.shape()[0];
    unsigned int kernelCols = kernel.shape()[1];

    // Check input and output are of same size
    if (inputRows != outputRows or inputCols !=outputCols) {
        std::cerr << "Input and output do not have same size" << std::endl;
        return;
    }

    // Check that kernel is square
    if (kernelRows != kernelCols) {
        std::cerr << "Kernel is not square" << std::endl;
        return;
    }

    // Check that kernel has size >= 3
    if (kernelRows < 3) {
        std::cerr << "Kernel has too small size (< 3)" << std::endl;
        return;
    }

    // Check kernel has odd size
    if (kernelRows % 2 == 0) {
        std::cerr << "Kernel does not have odd size" << std::endl;
        return;
    }

    // Perform convolution
    unsigned int rowIndex;
    unsigned int colIndex;
    float kernelElement;

    for (unsigned int i = 0; i < inputRows; i++) {
        for (unsigned int j = 0; j < inputCols; j++) {
            // Set indices of input elements used for kernel
            startRow = i - (kernelRows - 1)/2;
            endRow = i + (kernelRows - 1)/2;
            startCol = j - (kernelCols - 1)/2;
            endCol = j + (kernelCols - 1)/2;

            // Initialize output value
            output[i][j] = 0;

            for (int k = startRow; k <= endRow; k++) {
                // Iterate over row indices used for convolution
                for (int l = startCol; l <= endCol; l++) {
                    // Iterate over column indices used for convolution
                    if (k < 0) {
                        // Outside row bounds; use element of first row
                        rowIndex = 0;
                    }
                    else if (k >= inputRows) {
                        // Outside row bounds; use element of last row
                        rowIndex = inputRows - 1;
                    }
                    else {
                        // Inside bounds
                        rowIndex = k;
                    }

                    if (l < 0) {
                        // Outside bounds; use element of first column
                        colIndex = 0;
                    }
                    else if (l >= inputCols) {
                        // Outside bounds; use element of last column
                        colIndex = inputCols - 1;
                    }
                    else {
                        // Inside bounds
                        colIndex = l;
                    }

                    // Add product of input element and kernel element to output
                    kernelElement = kernel[k-startRow][l-startCol];
                    output[i][j] += input[rowIndex][colIndex]*kernelElement;
                }
            }

            // Weigh output by number of elements in kernel
            output[i][j] *= floor(1/(kernelRows*kernelRows));

            if (output[i][j] > 255) {
                output[i][j] = 255;
            else if (output[i][j] < 0) {
                output[i][j] = 0;
            }
        }
    }
}

void BoxBlur(kernelSize) {
    boost::multi_array<unsigned char, 2> outputMatrix(boost::extents[][]);
    boost::multi_array<unsigned char, 2> kernelMatrix(boost::extents[][]);

    // Fill sizes in box blur
}

unsigned int Sharpness() {
    // Return sharpness of image
}
