import numpy as np

'''
 @brief computes the Jacobian of the function h evaluated at x

 @param h the function for which the Jacobian is to be computed. Must accept x and args, must return type ublas::vector< ublas::vector<double> >
 @param x the value at which the Jacobain is to be computed.
 @param args additional arguments that the function h requires

 @return a m * n matrix_of_matrix where m is the dimension of the output of h, and n is the dimension of x. Notice, that since x may be a vector of vectors, each element of the returned value is not a scalar, it is a matrix.
'''
def jacobiaan(h, x, *args):
    y = h(x, *args)
    n = len(x)
    m = len(y)
    # H(i, j) should be equal to derivative of
    # y_j w.r.t x_i
    H = []
    for u in x:
        X = []
        for v in y:
            X.append(np.zeros((len(u), len(v))))
        H.append(X)
    delta = 1e-9
    for i in range(0, n):
        for k in range(0, len(x[i])):
            # compute and store in yDelta
            # the partial derivative of F(x) with respect
            # to x[i][k]
            x[i][k] += delta
            yDelta = h(x, *args) - y
            for j in range(0, m):
                yDelta[j] /= delta
            x[i][k] -= delta
            # now, since yDelta contains the derivative for
            # h(x)_{0...m - 1}, with respect to x[i][k]
            # we update for all j in 0...m - 1 H(i, j)_(k, l)
            # where l goes from 0 to yDelta[j].size() - 1
            for j in range(0, m):
                for l in range(0, len(yDelta[j])):
                    H[i][j][k][l] = yDelta[j][l]
    return H
