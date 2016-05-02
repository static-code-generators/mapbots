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

template <typename F, typename... Args>
matrix_of_matrix_type jacobian(F h, vector_of_vector_type x, Args... args)
{
    vector_of_vector_type y = h(x, args...);
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

            x(i)(k) += delta;
            vector_of_vector_type yDelta = h(x, args...) - y;
            for (unsigned j = 0; j < m; ++j) {
                yDelta(j) /= delta;
            }
            x(i)(k) -= delta;


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
