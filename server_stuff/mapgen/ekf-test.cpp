#include "ekf.hpp"
#include <iomanip>
#include <cmath>

vector_of_vector_type testSin(vector_of_vector_type x, vector_of_vector_type z)
{
    //this function has an additional useless argument,
    //this is merely for testing the template

    vector_of_vector_type ans = x;
    for (int i = 0; i < ans.size(); ++i)
        for (int j = 0; j < ans(i).size(); ++j)
            ans(i)(j) = std::sin(ans(i)(j));

    return ans;
}

vector_of_vector_type testIdentity(vector_of_vector_type x)
{
    return x;
}

vector_of_vector_type testConstant(vector_of_vector_type x, vector_of_vector_type z)
{
    //this function does not even depend on x

    return z;
}

int main()
{
    double val = M_PI / 4;
    vector_of_vector_type x(5, vector_type(3, val)), z = x;

    matrix_of_matrix_type J = jacobian(testSin, x, z);
    //the derivative should be cos(x) on the diagonal
    //elements of the diagonal elements
    //0 everywhere else
    std::cerr << "The test function is sin(x)" << std::endl;
    std::cerr << "The on diagonal values of the matrices on the diagonal should be 0.707" << std::endl;
    std::cerr << J << std::endl;

    J = jacobian(testIdentity, x);
    //the derivative should be the identity matrix on 
    //the diagonal elements
    //0 everywhere else
    std::cerr << "The test function simply returns x " << std::endl;
    std::cerr << "The on diagonal matrices should be identity" << std::endl;
    std::cerr << J << std::endl;

    J = jacobian(testConstant, x, z);
    //the derivative should be zero everywhere
    //this function is independent of x
    std::cerr << "The test function is independent of x" << std::endl;
    std::cerr << "Every entry should be 0" << std::endl;
    std::cerr << J << std::endl;
}
