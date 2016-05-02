#include "ekf.hpp"
#include <iomanip>
#include <cmath>

vector_type testSin(vector_type x, vector_type z)
{
    //this function has an additional useless argument,
    //this is merely for testing the template

    vector_type ans = x;
    for (int i = 0; i < ans.size(); ++i)
        ans(i) = std::sin(ans(i));

    return ans;
}

vector_type testIdentity(vector_type x)
{
    return x;
}

vector_type testConstant(vector_type x, vector_type z)
{
    return z;
}

int main()
{
    double val = M_PI / 4;
    vector_type x(15, val), z = x;

    matrix_type J = jacobian(testSin, x, z);
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
