#include "ekf.hpp"

vector_of_vector_type testIdentity(vector_of_vector_type x, vector_of_vector_type z)
{
    return x;
}

int main()
{
    vector_of_vector_type x(10, vector_type(3, 7)), z = x;

    matrix_of_matrix_type J = jacobian(testIdentity, x, z);

    matrix_type I = ublas::identity_matrix<double>(3);
    matrix_type O = ublas::zero_matrix<double>(3, 3);

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
    std::cout << "The test has passed." << std::endl;
}
