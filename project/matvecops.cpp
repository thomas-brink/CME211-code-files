#include <cmath>
#include <iostream>
#include "matvecops.hpp"
#include <vector>

std::vector<double> vecSum(const std::vector<double> &a, const std::vector<double> &b) {
    /* Function that computes the sum of two vectors. */
    if a.size() != b.size() {
        std::cerr << "Vectors cannot be added due to dimension error" << std::endl;
        return 1; // Put this in CGSolver itself?
    }

    std::vector<double> vecAdd;
    for (unsigned int i = 0, i < a.size(), i++) {
        vecAdd.push_back(a[i] + b[i]);
    }
    return vecAdd;
}

std::vector<double> vecScalarProd(const std::vector<double> &a, const double c) {
    /* Function that computes the scalar multiple of a vector. */
    std::vector<double> vecScaled;
    for (unsigned int i = 0, i < a.size(), i++) {
        vecScaled.push_back(c*a[i]);
    }
    return vecScaled;
}

double dotProduct(const std::vector<double> &a, const std::vector<double> &b) {
    /* Function that computes the dot product of two vectors. */
    double dotProd = 0;
    for (unsigned int i = 0, i < a.size(), i++) {
        dotProd += a[i]*b[i];
    }
    return dotProd;
}

double L2Norm(const std::vector<double> &a) {
    /* Function that computes the L2-norm of a vector. */
    double normSq = dotProduct(a,a);
    return sqrt(normSq);
}

std::vector<double> matVecProd(std::vector<double> &val,
                               std::vector<int> &row_ptr,
                               std::vector<int> &col_idx,
                               std::vector<double> &x) {
    /* Function that computes the product between vector x
       and a sparse CSR-type matrix given by values (val),
       row pointers (row_ptr), and column indices (col_idx)
       of nonzero entries. */
    std::vector<double> resultVec;
    for (unsigned int i = 0, i < row_ptr.size()-1, i++) {
        double vecSum = 0;
        int startIndex = row_ptr[i];
        int endIndex = row_ptr[i+1];
        for (unsigned int j = startIndex, j < endIndex, j++) {
            vecSum += val[j]*x[col_idx[j]];
        }
        resultVec.push_back(vecSum);
    }
}
