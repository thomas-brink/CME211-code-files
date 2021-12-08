#include <cmath>
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
int HeatEquation2D::Setup(std::string inputFile) {
    // Initializations
    unsigned int gridRows, gridCols, sizeA;
    double length, width, h, Tc, Th;
    std::ifstream inputData(inputFile);

    /* Make separate loadData function */
    if (inputData.is_open()) {
        // Read input data
        inputData >> length >> width >> h;
        inputData >> Tc >> Th;
        inputData.close();
    }
    else {
        // Cannot read input data
        //std::cerr << "Error while loading in the input data" << std::endl;
        return 1;
    }

    if (floor(length/h) == length/h && floor(width/h) == width/h) {
        // Set number of rows and columns in the grid
        gridRows = (int) (width/h) + 1;
        gridCols = (int) (length/h) + 1;
    }
    else {
        // Cannot divide the grid into equally spaced parts using h
        //std::cerr << "Length and/or width are no multiples of h" << std::endl;
        return 1;
    }

    // Calculate number of interior nodes in grid and resize A
    sizeA = gridRows*gridCols - 2*gridCols - (gridRows-2);
    A.Resize(sizeA, sizeA);
    int intRows = gridRows - 2;

    // Initialize x (vector of 1s) and b (vector of 0s)
    for (unsigned int i = 0; i < sizeA; i++) {
        x.push_back(1.0);
        b.push_back(0.0);
    }

    // Set up b (hot isothermal boundary)
    for (unsigned int k = 0; k < sizeA; k+=intRows) {
        //b[k] = Th/(h*h);
        b[k] = Th;
    }

    // Set up b (cold isothermal boundary)
    for (unsigned int l = intRows-1; l < sizeA; l+=intRows) {
        double bCheck = floor(l/intRows)*h - length/2;
        double bCheck2 = bCheck*bCheck*-10;
        double bCheck3 = exp(bCheck2) - 2;
        //b[l] = -Tc*bCheck3/(h*h);
        b[l] = -Tc*bCheck3;
    }

    // Fill up COO-type sparse matrix A
    for (unsigned int i = 0; i < sizeA; i++) {
        // u_ij coefficient
        //A.AddEntry(i, i, 4/(h*h));
        A.AddEntry(i, i, 4);
        if (i % intRows != 0) {
            // u_i-1,j coefficient
            //A.AddEntry(i, i-1, -1/(h*h));
            A.AddEntry(i, i-1, -1);
        }
        if ((i+1)%intRows != 0) {
            // u_i+1,j coefficient
            //A.AddEntry(i, i+1, -1/(h*h));
            A.AddEntry(i, i+1, -1);
        }
        // u_i,j-1 and u_i,j+1 coefficients
        //A.AddEntry(i, (i-intRows+sizeA)%sizeA, -1/(h*h));
        //A.AddEntry(i, (i+intRows)%sizeA, -1/(h*h));
        A.AddEntry(i, (i-intRows+sizeA)%sizeA, -1);
        A.AddEntry(i, (i+intRows)%sizeA, -1);
    }

    A.ConvertToCSR();
    return 0;
}

/* Function that solves the heat equation linear system using
   conjugate gradient and writes the progress of this solver
   to a file with a certain user-defined prefix every 10th
   iteration. */
int HeatEquation2D::Solve(std::string soln_prefix) {
    double tol = 1e-5;
    int iterCG = CGSolver(A, b, x, tol, soln_prefix);

    if (iterCG == -1) {
        //std::cerr << "Error: CG did not converge" << std::endl;
        return 1;
    }
    else {
        std::cout << "SUCCESS: CG solver converged in " << iterCG << " iterations." << std::endl;
        return 0;
    }
}
