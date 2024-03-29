#define BOOST_DISABLE_ASSERTS

#include <boost/multi_array.hpp>
#include <cmath>
#include <iostream>
#include <string>

#include "image.hpp"
#include "hw6.hpp"

/* Constructor that reads input JPEG file and stores this file
   as a boost multi-array. */
image::image(std::string inputFile):originalFile(inputFile) {
    ReadGrayscaleJPEG(originalFile, inImg); // Read input file
    outImg.resize(boost::extents[inImg.shape()[0]][inImg.shape()[1]]);
    outImg = inImg; // Initialize output to input data
}

/* Function that takes a name of an output file and writes the
   data attribute to this file in JPEG format. Default is "",
   in which case we write the output to the original file name. */
void image::Save(std::string writeFile/*=""*/) {
    if (writeFile == "") {
        // Use original file and write to JPEG
        WriteGrayscaleJPEG(originalFile, outImg);
    }
    else {
        // Use provided name and write to JPEG
        WriteGrayscaleJPEG(writeFile, outImg);
    }
}

/* Method that takes in an input multi-array, original output
   multi-array, and a kernel, and performs convolution using
   the kernel to form a convolved output. */
void image::Convolution(boost::multi_array<unsigned char, 2>& input,
                        boost::multi_array<unsigned char, 2>& output,
                        boost::multi_array<float, 2>& kernel) {
    // Initializations
    unsigned int inputRows = (unsigned int) input.shape()[0];
    unsigned int inputCols = (unsigned int) input.shape()[1];
    unsigned int outputRows = (unsigned int) output.shape()[0];
    unsigned int outputCols = (unsigned int) output.shape()[1];
    unsigned int kernelRows = (unsigned int) kernel.shape()[0];
    unsigned int kernelCols = (unsigned int) kernel.shape()[1];

    // Check input and output are of same size
    if (inputRows != outputRows or inputCols != outputCols) {
        std::cerr << "Input and output do not have same size" << std::endl;
        exit(0);
    }

    // Check that kernel is square
    if (kernelRows != kernelCols) {
        std::cerr << "Kernel is not square" << std::endl;
        exit(0);
    }

    // Check that kernel has size >= 3
    if (kernelRows < 3) {
        std::cerr << "Kernel has too small size (< 3)" << std::endl;
        exit(0);
    }

    // Check kernel has odd size
    if (kernelRows % 2 == 0) {
        std::cerr << "Kernel does not have odd size" << std::endl;
        exit(0);
    }

    // Declare variables for convolution
    int startRow, endRow, startCol, endCol;
    unsigned int rowIndex, colIndex;
    float inputVal, outputVal, kernelVal;

    for (unsigned int i = 0; i < inputRows; i++) {
        for (unsigned int j = 0; j < inputCols; j++) {
            // Set indices of input elements used for kernel
            startRow = i - (kernelRows - 1)/2;
            endRow = i + (kernelRows - 1)/2;
            startCol = j - (kernelCols - 1)/2;
            endCol = j + (kernelCols - 1)/2;

            // Initialize output value for row i and column j
            outputVal = 0;

            for (int k = startRow; k <= endRow; k++) {
                // Iterate over row indices used for convolution
                if (k < 0) {
                    // Outside input row bounds; use first row
                    rowIndex = 0;
                }
                else if (k >= (int) inputRows) {
                    // Outside input row bounds; use last row
                    rowIndex = inputRows - 1;
                }
                else {
                    // Inside input row bounds
                    rowIndex = k;
                }
                for (int l = startCol; l <= endCol; l++) {
                    // Iterate over column indices used for convolution
                    if (l < 0) {
                        // Outside input column bounds; use first column
                        colIndex = 0;
                    }
                    else if (l >= (int) inputCols) {
                        // Outside input column bounds; use last column
                        colIndex = inputCols - 1;
                    }
                    else {
                        // Inside input column bounds
                        colIndex = l;
                    }

                    // Add product of input element and kernel element to output
                    kernelVal = kernel[k-startRow][l-startCol];
                    inputVal = (float) input[rowIndex][colIndex];
                    outputVal += inputVal*kernelVal;
                }
            }

            // Compute floor of resulting value as output
            outputVal = (float) floor(outputVal);

            // Correct for range {0,...,255} of unsigned char in JPEG
            if (outputVal > 255) {
                output[i][j] = (unsigned char) 255;
            }
            else if (outputVal < 0) {
                output[i][j] = (unsigned char) 0;
            }
            else {
                output[i][j] = (unsigned char) outputVal;
            }
        }
    }
}

/* Function that takes in the size of a kernel, creates a kernel with this
   size containing equal values summing up to 1, and performs convolution
   on the input and output multi-arrays to form a blurred image. */
void image::BoxBlur(unsigned int kernelSize) {
    // Initialization
    boost::multi_array<float,2> kernel(boost::extents[kernelSize][kernelSize]);
    float kernelNrElements = (float) kernel.num_elements();
    float scaleFactor = 1/kernelNrElements;

    for (unsigned int i = 0; i < kernelSize; i++) {
        for (unsigned int j = 0; j < kernelSize; j++) {
            // All elements have equal values, summing up to 1
            kernel[i][j] = scaleFactor;
        }
    }

    // Perform convolution on input, output, and kernel
    Convolution(inImg, outImg, kernel);
}

/* Function that computes the sharpness of an image by using a Laplacian
   kernel and convolution to find the maximum element of the resulting
   output matrix. */
unsigned int image::Sharpness() {
    // Initialize Laplacian kernel and sharpness
    boost::multi_array<float, 2> lapKernel(boost::extents[3][3]);
    unsigned int sharpness = 0;

    // Create Laplacian kernel
    lapKernel[0][0] = 0;
    lapKernel[0][1] = 1;
    lapKernel[0][2] = 0;
    lapKernel[1][0] = 1;
    lapKernel[1][1] = -4;
    lapKernel[1][2] = 1;
    lapKernel[2][0] = 0;
    lapKernel[2][1] = 1;
    lapKernel[2][2] = 0;

    // Make copy to distinguish input and output in convolution
    boost::multi_array<unsigned char, 2> outImgCopy = outImg;

    // Perform convolution on input, output, and Laplacian kernel
    Convolution(outImgCopy, outImg, lapKernel);

    // Find maximum element of output
    for (unsigned int i = 0; i < outImg.shape()[0]; i++) {
        for (unsigned int j = 0; j < outImg.shape()[1]; j++) {
            if ((unsigned int) outImg[i][j] > sharpness) {
                sharpness = (unsigned int) outImg[i][j];
            }
        }
    }

    // Return maximum element as sharpness
    return sharpness;
}
