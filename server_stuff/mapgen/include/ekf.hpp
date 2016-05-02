#pragma once

#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#define BOOST_UBLAS_NDEBUG
//puts ublas out out debugging mode, probably a good idea to
//comment out when shit hits the fan
//but has dramatic (10x) slow down

namespace ublas = boost::numeric::ublas;
typedef ublas::matrix<double> matrix_type;
typedef ublas::matrix<matrix_type> matrix_of_matrix_type;
typedef ublas::vector<double> vector_type;
typedef ublas::vector<vector_type> vector_of_vector_type;

matrix_type flatten(matrix_of_matrix_type A);

template <typename F, typename... Args>
matrix_type jacobian(F h, vector_type x, Args... args);

template<typename T>
bool operator==(const ublas::matrix<T>& m, const ublas::matrix<T>& n);

#include "ekf.tpp"
