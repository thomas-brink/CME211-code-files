#include "CGSolver.hpp"
#include <cmath>
#include <iostream>
#include "matvecops.hpp"
#include <vector>

/* Function that implements the CG algorithm for a linear system
 *
 * Ax = b
 * where A is in CSR format. The starting guess for the solution
 * is provided in x, and the solver runs a maximum number of iterations
 * equal to the size of the linear system. Function returns the
 * number of iterations to converge the solution to the specified
 * tolerance, or -1 if the solver did not converge.
 */

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol) {

    int nIterMax = x.size();
    int nIter = 0

    std::vector<double> Au0 = matVecProd(val, row_ptr, col_idx, x);
    std::vector<double> negAu0 = vecScalarProd(Au0, -1);
    std::vector<double> r = vecSum(b,negAu0);
    std::vector<double> p = r;

    double L2Normr0 = L2Norm(r);

    double alpha, beta, L2Normr;
    std::vector<double> Apn;

    while (nIter < nIterMax) {
        nIter ++ 1;
        Apn = matVecProd(val, row_ptr, col_idx, p);
        rDot = dotProduct(r,r);
        alpha = rDot/dotProduct(p,Apn);
        x = vecSum(x, vecScalarProd(p, alpha));
        r = vecSum(r, vecScalarProd(Apn,-1*alpha);
        L2Normr = L2Norm(r);
        if L2Normr/L2Normr0 < tol {
            return nIter;
        }
        beta = rDot/dotProduct(r,r);
        p = vecSum(r, vecScalarProd(p, beta));
    }
    return -1;
}
