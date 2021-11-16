#include <"CGSolver.hpp">
#include <"COO2CSR.hpp">
#include <iostream>
#include <"matvecops.hpp">
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    /* Main function for solving the linear system of equations
       for sparse matrices using conjugate gradient. This function
       takes in an input file containing matrix data and writes
       the solution of the (sparse) linear system to a solution file
       with a given input name. */

    if (argc != 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  ./main <input matrix file name>";
        std::cout << " <output solution file name>" << std::endl;
    }

    int nrRows, nrCols;

    std::ifstream inputFile(argv[1]);
    std::ofstream solutionFile(argv[2]);

    std::vector<double> val;
    std::vector<int> row_idx, col_idx;

    int rowEntry, colEntry;
    double valEntry;

    if (inputFile.is_open()) {
        inputFile >> nrRows >> nrCols;
        while (inputFile >> rowEntry >> colEntry >> valEntry) {
            row_idx.push_back(rowEntry);
            col_idx.push_back(colEntry);
            val.push_back(valEntry);
        }
        inputFile.close();
    }
    else {
        std::cerr << "Error while loading input data" << std::endl;
        return 1;
    }

    COO2CSR(val, row_idx, col_idx); // Note: row_idx becomes row_ptr

    std::vector<double> b(nrCols,0.0);
    std::vector<double> x(nrCols,1.0);
    double tol = 1.e-5;

    int nIter = CGSolver(val, row_idx, col_idx, b, x, tol);

    if (nIter != -1) {
        std::cout << "SUCCESS: CG solver converged in ";
        std::cout << nIter << " iterations." << std::endl;
        std::cout.setf(std::ios::scientific, std::ios::floatfield);
        std::cout.precision(4);

        for (unsigned int i, i < nrCols, i++) {
            solutionFile << x[i] << std::endl;
        }
    }
    else {
        std::cerr << "FAILURE: solver did not converge." << std::endl;
        return 1;
    }

    return 0;
}
