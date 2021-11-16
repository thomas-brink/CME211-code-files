#include "CGSolver.hpp"
#include "COO2CSR.hpp"
#include <fstream>
#include <iostream>
#include "matvecops.hpp"
#include <string>
#include <vector>

int main(int argc, char*argv[]) {
    /* Main function for solving the linear system of equations
       for sparse matrices using conjugate gradient. This function
       takes in an input file containing matrix data and writes
       the solution of the (sparse) linear system to a solution file
       with a given input name. */

    if (argc != 3) {
        // If appropriate arguments not passed, print usage message
        std::cout << "Usage:" << std::endl;
        std::cout << "  ./main <input matrix file name>";
        std::cout << " <output solution file name>" << std::endl;
        return 0;
    }

    // Set input and output files from user arguments
    std::ifstream inputFile(argv[1]);
    std::ofstream solutionFile(argv[2]);

    // Introduce the necessary variables
    std::vector<double> val;
    std::vector<int> row_idx, col_idx;
    unsigned int nrRows, nrCols;
    int rowEntry, colEntry;
    double valEntry;

    if (inputFile.is_open()) {
        // Read input data (first row contains nr. rows and cols)
        inputFile >> nrRows >> nrCols;
        while (inputFile >> rowEntry >> colEntry >> valEntry) {
            // Put input data in COO-format
            row_idx.push_back(rowEntry);
            col_idx.push_back(colEntry);
            val.push_back(valEntry);
        }
        inputFile.close();
    }
    else {
        // If input file cannot be opened, return error message
        std::cerr << "Error while loading input data" << std::endl;
        return 1;
    }

    // Convert COO-matrix to CSR-type
    COO2CSR(val, row_idx, col_idx); // Note: row_idx becomes row_ptr

    // Initialize CG variables
    std::vector<double> b(nrCols,0.0);
    std::vector<double> x(nrCols,1.0);
    double tol = 1.e-5;

    // Perform CG algorithm
    int nIter = CGSolver(val, row_idx, col_idx, b, x, tol);

    if (nIter != -1) {
        // If algorithm converges, return success message
        std::cout << "SUCCESS: CG solver converged in ";
        std::cout << nIter << " iterations." << std::endl;
        solutionFile.setf(std::ios::scientific, std::ios::floatfield);
        solutionFile.precision(4);

        for (unsigned int i=0; i < nrCols; i++) {
            // Write solution x* to solution file, line by line
            solutionFile << x[i] << std::endl;
        }
    }
    else {
        // If algorithm did not converge, report error message
        std::cerr << "FAILURE: solver did not converge." << std::endl;
        return 1;
    }
    return 0;
}
