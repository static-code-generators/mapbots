#include "ekf.hpp"

matrix_of_matrix_type prod_local(matrix_of_matrix_type A, matrix_of_matrix_type B)
{
    assert(A.size2() == B.size1());
    matrix_of_matrix_type C(A.size1(), B.size2());
    for (unsigned i = 0; i < A.size1(); ++i)
        for (unsigned j = 0; j < B.size2(); ++j) {
            C(i, j) = ublas::zero_matrix<double>(A(i, 0).size1(), B(0, j).size2());
            for (unsigned k = 0; k < A.size2(); ++k) 
                C(i, j) += prod(A(i, k), B(k, j));
        }
    return C;
}
