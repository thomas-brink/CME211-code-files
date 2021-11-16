#include <cmath>
#include <iostream>
#include "matvecops.hpp"
#include <vector>

std::vector<double> vecSum(const std::vector<double> &a,
                           const std::vector<double> &b) {
    /* Function that performs vector addition to two given
       input vectors and returns a sum vector. */
    std::vector<double> vecAdd;
    for (unsigned int i = 0; i < a.size(); i++) {
        // Append elementwise sums
        vecAdd.push_back(a[i] + b[i]);
    }
    return vecAdd;
}

std::vector<double> vecScalarProd(const std::vector<double> &a,
                                  const double c) {
    /* Function that computes the scalar multiple of a vector. */
    std::vector<double> vecScaled;
    for (unsigned int i = 0; i < a.size(); i++) {
        // Append elementwise scalar multiples
        vecScaled.push_back(c*a[i]);
    }
    return vecScaled;
}

double dotProduct(const std::vector<double> &a,
                  const std::vector<double> &b) {
    /* Function that computes the dot product of two vectors. */
    double dotProd = 0;
    for (unsigned int i = 0; i < a.size(); i++) {
        // Add elementwise products
        dotProd += a[i]*b[i];
    }
    return dotProd;
}

double L2Norm(const std::vector<double> &a) {
    /* Function that computes the L2-norm of a vector.
       We calculate the L2-norm as the square root of
       a vector's dot product with itself. */
    double normSq = dotProduct(a,a);
    return sqrt(normSq);
}

std::vector<double> matVecProd(std::vector<double> &val,
                               std::vector<int> &row_ptr,
                               std::vector<int> &col_idx,
                               std::vector<double> &x) {
    /* Function that computes the product Ax between vector
       x and a sparse CSR-type matrix A given by values (val),
       row pointers (row_ptr), and column indices (col_idx)
       of nonzero entries. */
    std::vector<double> resultVec;
    for (unsigned int i = 0; i < row_ptr.size()-1; i++) {
        // Iterate over rows
        double vecSum = 0;
        unsigned int startIndex = row_ptr[i];
        unsigned int endIndex = row_ptr[i+1];
        for (unsigned int j = startIndex; j < endIndex; j++) {
            // Multiply values in row i by corresponding elements in x
            vecSum += val[j]*x[col_idx[j]];
        }
        resultVec.push_back(vecSum); // Append to Ax
    }
    return resultVec;
}
