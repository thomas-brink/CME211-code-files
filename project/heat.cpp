#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "CGSolver.hpp"
#include "heat.hpp"
#include "matvecops.hpp"
#include "sparse.hpp"

/* Function that takes an input file name for heat equation
   modeling and sets up its linear system Ax = b. */
void HeatEquation2D::Setup(std::string inputFile) {
    std::ifstream inputData(inputFile);
    int length, width, xNodes, yNodes, sizeA;
    float h, Tc, Th;

    if (inputData.is_open()) {
        inputData >> length >> width >> h;
        inputData >> Tc >> Th;
        inputData.close()
    }
    else {
        std::cerr << "Error while loading in the input data" << std::endl;
    }

    xNodes = (int) length/h + 1;
    yNodes = (int) width/h + 1;

    sizeA = xNodes*yNodes - 2*yNodes - (xNodes - 2);
    A.resize(sizeA, sizeA);

    // Initialize x (vector of 1s) and b (vector of 0s)
    for (unsigned int i = 0; i < sizeA; i++) {
        x.push_back(1.0);
        b.push_back(0.0);
    }

    // Set up b (hot isothermal boundary)
    for (unsigned int k = 0; k < sizeA*sizeA; k+=sizeA) {
        b[k] = Th;
    }

    // Set up b (cold isothermal boundary)
    for (unsigned int l = sizeA-1; l < sizeA*sizeA; l+=sizeA) {
        b[k] = -Tc*exp((-10*(h*(l%sizeA) - length/2)^2)-2);
    }

    // Fill up COO-type sparse matrix A
    for (unsigned int i = 0; i < sizeA; i++) {
        for (unsigned int j = 0; j < sizeA; j++) {
            // u_ij coefficient
            A.addEntry(i, j, 4/h^2);
            if (i > 0) {
                // u_i-1,j coefficient
                A.addEntry(i-1, j, -1/h^2);
            }
            if (i < sizeA-1) {
                // u_i+1,j coefficient
                A.addEntry(i+1, j, -1/h^2);
            }
            // u_i,j-1 and u_i,j+1 coefficients
            A.addEntry(i, (j-1+sizeA)%sizeA, -1/h^2);
            A.addEntry(i, (j+1+sizeA)%sizeA, -1/h^2);
        }
    }

    A.ConvertToCSR();
}

/* Function that solves the heat equation linear system using
   conjugate gradient and writes the progress of this solver
   to a file with a certain user-defined prefix every 10th
   iteration. */
int HeatEquation2D::Solve(std::string soln_prefix) {
    // Solve heat equations:
    //  - Call CGSolver function
    //     - where CGSolver is adjusted to handle SparseMatrix instance -> change input to SparseMatrix, ...
    //     - and where solution is saved every 10-th iteration
    //  - Write solutions to files
}
