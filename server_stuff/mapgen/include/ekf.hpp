#pragma once

#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#define BOOST_UBLAS_NDEBUG
//puts ublas out out debugging mode, probably a good idea to
//comment out when shit hits the fan
//but has dramatic (10x) slow down

namespace ublas = boost::numeric::ublas;
typedef ublas::matrix<double> matrix_type;
typedef ublas::matrix<matrix_type> matrix_of_matrix_type;
typedef ublas::vector<double> vector_type;
typedef ublas::vector<vector_type> vector_of_vector_type;

matrix_of_matrix_type prod_local(matrix_of_matrix_type A, matrix_of_matrix_type B);

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
matrix_of_matrix_type jacobian(F h, ublas::vector< ublas::vector<double> > x, Args... args);

template<typename T>
bool operator==(const ublas::matrix<T>& m, const ublas::matrix<T>& n);
