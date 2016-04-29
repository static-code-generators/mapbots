#include "ekf.hpp"
#include <iomanip>

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

    std::cerr << J << std::endl;
}
