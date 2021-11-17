#include <cmath>
#include <iostream>
#include <vector>

#include "CGSolver.hpp"
#include "matvecops.hpp"

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol) {
    /* Function that implements the CG algorithm for linear system
       Ax = b, where A is a CSR-type sparse matrix. The starting
       guess for solution x* is provided in x, we run a maximum
       number of iterations equal to the size of the linear system,
       and let the algorithm converge if a certain tolerance tol is
       attained. The function returns the number of iterations until
       convergence or value -1 if the program did not converge. */

    int nIterMax = (int)x.size(); // Set maximum number of iterations

    // Initializations
    int nIter = 0;
    std::vector<double> Au0 = matVecProd(val, row_ptr, col_idx, x);
    std::vector<double> negAu0 = vecScalarProd(Au0, -1);
    std::vector<double> r = vecSum(b,negAu0);
    std::vector<double> p = r;
    double L2Normr0 = L2Norm(r);

    // Declare other variables used in CG algorithm
    double alpha, beta, L2Normr, rDot;
    std::vector<double> Apn;

    // Perform procedure as given in Figure 1 of project description
    while (nIter < nIterMax) {
        // Perform CG iterations while number of iterations < maximum
        nIter ++;
        Apn = matVecProd(val, row_ptr, col_idx, p);
        rDot = dotProduct(r,r); // Dot product gives r_n^T r_n
        alpha = rDot/dotProduct(p,Apn);
        x = vecSum(x, vecScalarProd(p, alpha)); // u_(n+1) update
        r = vecSum(r, vecScalarProd(Apn,-1*alpha)); // r_(n+1) update
        L2Normr = L2Norm(r);
        if (L2Normr/L2Normr0 < tol) {
            // Algorithm has converged
            return nIter;
        }
        beta = dotProduct(r,r)/rDot; // Dot product gives r_(n+1)^T r_(n+1)
        p = vecSum(r, vecScalarProd(p, beta));
    }
    // Algorithm did not converge
    return -1;
}
