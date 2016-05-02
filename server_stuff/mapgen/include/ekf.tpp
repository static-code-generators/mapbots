template <typename F, typename... Args>
matrix_type jacobian(F h, vector_type x, Args... args)
{
    vector_type y = h(x, args...);
    unsigned n = x.size(), m = y.size();
    matrix_type H = ublas::zero_matrix<double>(n, m);

    //H(i, j) should be equal to the derivative of 
    //h(x)_j with respect to x_i
    //we set the sizes of the matrices with H now

    double delta = 1e-9;
    for (unsigned i = 0; i < n; ++i) {
        //compute and store in yDelta
        //the partial derivative of F(x) with respect
        //to x(i)

        x(i) += delta;
        vector_type yDelta = h(x, args...) - y;
        yDelta /= delta;
        x(i) -= delta;


        //now, since yDelta contains the derivative for
        //h(x)_{0...m - 1}, with respect to x(i)
        //we update for all j in 0...m - 1 H(i, j)

        for (unsigned j = 0; j < m; ++j)
            H(i, j) = yDelta(j);
    }

    return H;
}

template<typename T>
bool operator==(const ublas::matrix<T>& m, const ublas::matrix<T>& n)
{
    bool returnValue = 
        (m.size1() == n.size1()) &&
        (m.size2() == n.size2());

    if (returnValue)
    {
        for (unsigned int i = 0; returnValue && i < m.size1(); ++i)
        {
            for (unsigned int j = 0; returnValue && j < m.size2(); ++j)
            {
                returnValue &= m(i,j) == n(i,j);
            }
        }
    }
    return returnValue;
}

template<class T>
bool invertMatrix(const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    // create a working copy of the input
    matrix<T> A(input);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = lu_factorize(A,pm);
    if( res != 0 ) return false;

    // create identity matrix of "inverse"
    inverse.assign(ublas::identity_matrix<T>(A.size1()));

    // backsubstitute to get the inverse
    lu_substitute(A, pm, inverse);

    return true;
}

