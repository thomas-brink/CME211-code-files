#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "CGSolver.hpp"
#include "matvecops.hpp"
#include "sparse.hpp"

/* Function that implements CG for solving Ax=b, where A is a
   CSR-type sparse matrix. We input a starting guess for x and
   retrieve a solution x* in case the algorithm attains a certain
   tolerance level tol. We write our current solution vector x to
   a file every 10th iteration. The function returns the number of
   iterations until convergence (-1 if algorithm does not converge). */
int CGSolver(SparseMatrix              &A,
             std::vector<double>       &b,
             std::vector<double>       &x,
             double                    tol,
             std::string               soln_prefix) {

    int nIterMax = (int)x.size(); // Set maximum number of iterations

    // Initializations
    int nIter = 0;
    std::vector<double> Au0 = A.MulVec(x);
    std::vector<double> negAu0 = vecScalarProd(Au0, -1);
    std::vector<double> r = vecSum(b,negAu0);
    std::vector<double> p = r;
    double L2Normr0 = L2Norm(r);

    // Declare other variables used in CG algorithm
    double alpha, beta, L2Normr, rDot;
    std::vector<double> Apn;

    writeSolution(nIter, x, soln_prefix);

    // Perform procedure as given in Figure 1 of project description
    while (nIter < nIterMax) {
        // Perform CG iterations while number of iterations < maximum
        nIter ++;
        Apn = A.MulVec(p);
        rDot = dotProduct(r,r); // Dot product gives r_n^T r_n
        alpha = rDot/dotProduct(p,Apn);
        x = vecSum(x, vecScalarProd(p, alpha)); // u_(n+1) update
        r = vecSum(r, vecScalarProd(Apn,-1*alpha)); // r_(n+1) update
        L2Normr = L2Norm(r);
        if (L2Normr/L2Normr0 < tol) {
            writeSolution(nIter, x, soln_prefix);
            return nIter+1;
        }
        beta = dotProduct(r,r)/rDot; // Dot product gives r_(n+1)^T r_(n+1)
        p = vecSum(r, vecScalarProd(p, beta));

        if (nIter % 10 == 0) {
            writeSolution(nIter, x, soln_prefix);
        }
    }
    // Algorithm did not converge
    return -1;
}

/* Function that writes a current CG solution x to a file
   with the name defined by a soln_prefix and the number
   of iterations nIter. */
void writeSolution(const int nIter,
                   const std::vector<double> x,
                   const std::string soln_prefix) {

    // Format solution file name
    std::ostringstream solFileName;
    solFileName << soln_prefix << std::setw(4) << std::setfill('0') << nIter << ".txt";
    std::string solFileString(solFileName.str());

    // Write solution vector to file
    std::ofstream solFile(solFileString);
    for (unsigned int i = 0; i < x.size(); i++) {
        solFile << i << " " << x[i] << std::endl;
    }
}
