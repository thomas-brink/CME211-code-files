#ifndef CGSOLVER_HPP
#define CGSOLVER_HPP

#include <string>
#include <vector>

#include "sparse.hpp"

/* Function that implements the CG algorithm for a linear system
 *
 * Ax = b
 *
 * where A is in CSR format.  The starting guess for the solution
 * is provided in x, and the solver runs a maximum number of iterations
 * equal to the size of the linear system.  Function returns the
 * number of iterations to converge the solution to the specified
 * tolerance, or -1 if the solver did not converge.
 */

int CGSolver(SparseMatrix        &A,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol,
             std::string         soln_prefix);

/* Function that writes a solution x at a given step of the
   CG process to a solution file.. */
void writeSolution(const int nIter,
                   const std::vector<double> x,
                   const std::string soln_prefix);

#endif /* CGSOLVER_HPP */
