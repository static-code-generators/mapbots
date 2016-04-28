#include "ekf.h"

vector_of_vector_type testIdentity(vector_of_vector_type x)
{
    return x;
}

int main()
{
    vector_of_vector_type x(10, vector_type(3, 7));
    matrix_of_matrix_type J = jacobian(testIdentity, x);

    matrix_type I = ublas::identity_matrix(3);
    matrix_type O = ublas::identity_matrix(3, 3);

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j) {
            if (i == j) {
                //on diagonal entries should be equal to the identity matrix
                assert(J(i, j) == I);
            } else {
                //off-diagonal entries should be the zero matrix
                assert(J(i, j) == O);
            }
        }
}
