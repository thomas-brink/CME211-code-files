#ifndef MATVECOPS_HPP
#define MATVECOPS_HPP

#include <cmath>
#include <iostream>
#include <vector>

/* Function that performs vector addition given two input vectors. */
std::vector<double> vecSum(const std::vector<double> &a,
                           const std::vector<double> &b);

/* Function that performs scalar multiplication to a given vector. */
std::vector<double> vecScalarProd(const std::vector<double> &a,
                                  const double c);

/* Function that computes the dot product of two given vectors. */
double dotProduct(const std::vector<double> &a,
                  const std::vector<double> &b);

/* Function that computes the L2-norm of a given vector. */
double L2Norm(const std::vector<double> &a);

/* Function that performs matrix-vector multiplication given a sparse
   CSR-matrix and an input vector. */
std::vector<double> matVecProd(std::vector<double> &val,
                               std::vector<int> &row_ptr,
                               std::vector<int> &col_idx,
                               std::vector<double> &x);

#endif /* MATVECOPS_HPP */
