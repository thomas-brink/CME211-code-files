#include <iostream>
#include <vector>

#include "CGSolver.hpp"
#include "COO2CSR.hpp"
#include "matvecops.hpp"
#include "sparse.hpp"

/* Function that takes in arguments for the number of rows
   and columns of a sparse matrix and resizes this matrix. */
void SparseMatrix::Resize(int nrows, int ncols) {
    // Set number of rows and columns of class instance
    this->nrows = nrows;
    this->ncols = ncols;

    /* Use only upper left block from current matrix if
       input dimensions smaller than current dimensions. */
    std::vector<int> remove_indices;
    for (unsigned int i = 0; i < i_idx.size(); i--) {
        if (i_idx[i] > (nrows-1) or j_idx[i] > (ncols-1)) {
            remove_indices.push_back(i);
        }
    }

    for (int r = ((int) remove_indices.size()-1); r >= 0; r--) {
        i_idx.erase(i_idx.begin() + remove_indices[r]);
        j_idx.erase(j_idx.begin() + remove_indices[r]);
        a.erase(a.begin() + remove_indices[r]);
    }
}

/* Function that takes in a row index, column index, and
   corresponding value, and adds this triplet to the
   COO-type sparse matrix. */
void SparseMatrix::AddEntry(int i, int j, double val) {
    if (i >= 0 and i < nrows and j >= 0 and j < ncols) {
        i_idx.push_back(i);
        j_idx.push_back(j);
        a.push_back(val);
    }
    else {
        std::cerr << "Invalid row and/or column index" << std::endl;
    }
}

/* Function that converts the COO-type matrix to a CSR-type
   sparse matrix. This function uses the COO2CSR() function
   provided. */
void SparseMatrix::ConvertToCSR() {
    COO2CSR(a, i_idx, j_idx);
}

/* Function that takes in a vector and calculates the
   matrix-vector product of the sparse matrix and this
   vector. This function is designed for CSR-matrices. */
std::vector<double> SparseMatrix::MulVec(std::vector<double> &vec) {
    return matVecProd(a, i_idx, j_idx, vec);
}
