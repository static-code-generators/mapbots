#include "ekf.hpp"

matrix_type flatten(matrix_of_matrix_type A)
{
    //calculate sizes of the new matrix
    int m = 0;
    for (int i = 0; i < A.size1(); ++i)
        m += A(i, 0).size1();
    int n = 0;
    for (int i = 0; i < A.size2(); ++i)
        n += A(0, i).size2();

    matrix_type flatA(m, n);
    //we need variables for starting
    //index of current sub-matrix of A
    int baseI = 0, baseJ = 0;
    for (int i = 0; i < A.size1(); ++i) {
        for (int j = 0; j < A.size2(); ++j) {
            for (int ii = 0; ii < A(i, j).size1(); ++ii)
                for (int jj = 0; jj < A(i, j).size2(); ++jj)
                    flatA(baseI + ii, baseJ + jj) = A(i, j)(ii, jj);
            baseJ += A(i, j).size2();
        }
        baseI += A(i, 0).size1();
        baseJ = 0;
    }

    return flatA;
}
