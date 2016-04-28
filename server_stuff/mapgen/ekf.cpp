#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

//#define BOOST_UBLAS_NDEBUG

namespace ublas = boost::numeric::ublas;
typedef ublas::matrix<double> matrix_type;
typedef ublas::matrix<matrix_type> matrix_of_matrix_type;

matrix_of_matrix_type prod_local(matrix_of_matrix_type A, matrix_of_matrix_type B);

template <typename F, typename... Args>
matrix_of_matrix_type jacobian(F h, ublas::vector< ublas::vector<double> > x, Args... args);

int main()
{
    matrix_of_matrix_type A(10, 10, ublas::identity_matrix<double>(3)), B = A;
    matrix_type C = ublas::identity_matrix<double>(3), D = C;
}

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

template <typename F, typename... Args>
/**
 * @brief computes the Jacobian of the function h evaluated at x
 *
 * @param h the function for which the Jacobian is to be computed. Must accept x and args, must return type ublas::vector< ublas::vector<double> >
 * @param x the value at which the Jacobain is to be computed.
 * @param args additional arguments that the function h requires
 *
 * @return a m * n matrix_of_matrix_type where m is the dimension of the output of h, and n is the dimension of x. Notice, that since x may be a vector of vectors, each element of the returned value is not a scalar, it is a matrix <double>. 
 */
matrix_of_matrix_type jacobian(F h, ublas::vector< ublas::vector<double> > x, Args... args)
{
    auto y = F(x, args...);
    unsigned n = x.size(), m = y.size();
    matrix_of_matrix_type H(n, m);

    //H(i, j) should be equal to the derivative of 
    //h(x)_j with respect to x_i
    //we set the sizes of the matrices with H now

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            H(i, j) = ublas::matrix<double>(x[i].size(), y[j].size());

    double delta = 1e-9;
    for (unsigned i = 0; i < n; ++i) {
        for (unsigned k = 0; k < x[i].size(); ++k) {
            //compute and store in yDelta
            //the partial derivative of F(x) with respect
            //to x[i][k]
            x[i][k] = x[i][k] + delta;
            auto yDelta = F(x, args...) - y;
            yDelta /= delta;
            x[i][k] = x[i][k] - delta;
            
            //now, since yDelta contains the derivative for
            //h(x)_{0...m - 1}, with respect to x[i][k]
            //we update for all j in 0...m - 1 H(i, j)_(k, l)
            //where l goes from 0 to yDelta[j].size() - 1

            for (unsigned j = 0; j < m; ++j)
                for (unsigned l = 0; l < yDelta[j].size(); ++l)
                    H(i, j)(k, l) = yDelta(j)(l);
        }
    }

    return H;
}
