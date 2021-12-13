#ifndef SPARSE_HPP
#define SPARSE_HPP

#include <vector>

class SparseMatrix
{
  private:
    /* Class attributes */
    std::vector<int> i_idx;
    std::vector<int> j_idx;
    std::vector<double> a;
    int ncols;
    int nrows;

  public:
    /* Method to modify sparse matrix dimensions */
    void Resize(int nrows, int ncols);

    /* Method to add entry to matrix in COO format */
    void AddEntry(int i, int j, double val);

    /* Method to convert COO matrix to CSR format using provided function */
    void ConvertToCSR();

    /* Method to perform sparse matrix vector multiplication using CSR formatted matrix */
    std::vector<double> MulVec(std::vector<double> &vec);
};

#endif /* SPARSE_HPP */
