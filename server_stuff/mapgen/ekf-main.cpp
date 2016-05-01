#include "ekf.hpp"
#include <iomanip>
#include <cmath>
#include <vector>

vector_of_vector_type distanceEstimator(vector_of_vector_type x);
vector_of_vector_type filter(vector_of_vector_type x, std::vector<vector_of_vector_type> z_actual);

template<typename T>
T square(T x)
{
    return x * x;
}

int main()
{

}

vector_of_vector_type filter(vector_of_vector_type x, std::vector<vector_of_vector_type> z_actual)
{
    //here we implement the extended kalman filter.
    //(insert attribution to original paper here)

    int m = x.size(); //number of features + 1(position of bot)

    //covariance matrix:
    //possible optimisation -> use ublas::sparse_matrix instead, since
    //a lot of the entries here are 0.
    matrix_of_matrix_type P(m, m);
    //we are certain of the bot's original position (since it is the base reference)
    P(0, 0) = ublas::zero_matrix<double>(3, 3);
    //we are very uncertain of the feature's locations
    for (int i = 1; i < m; ++i)
        P(i, i) = matrix_type(2, 2, 42696942.1);
}

vector_of_vector_type distanceEstimator(vector_of_vector_type x)
{
    //Questions:
    //How are we differentiating between point and line features?
    //Two approaches:
    //  * add an additional parameter to each feature
    //  * make x so that the first k entries are points, the rest are lines, pass k as a parameter.

    //we assume that the position of the bot is a 3-vector
    //and is stored in x(0)
    vector_type botPos = x[0];

    //we assume that our bot has 24 sensors, placed 
    //evenly starting at 0 radian (wrt current bot orientation)
    //to 2*PI radians
    //TODO: determine the distance of the sensors from the center
    //of the bot, and hence add their correct x and y-coordinate.
    double angleIncr = 2 * M_PI / 8;

    //we store the distances we estimate for each sensor here.
    //notice that z is a vector of 1-dimensional vectors
    //this is because of the type requirements of the jacobian computer
    const int numSensors = 24;
    vector_of_vector_type z(numSensors, vector_type(1, 0));

    double x1 = botPos[0], y1 = botPos[1], alpha = botPos[2];
    for (int i = 0; i < numSensors; ++i) {
        double readingEstimate = 0;
        //note that j iterates from 1 to x.size() - 1
        //since the position of the bot is stored in the x[0]
        //for now, we assume only line features
        for (int j = 1; j < x.size(); ++j) {
            
            //enter parameters of feature
            double r = x[j][0], beta = x[j][1];

            //minima for distance
            double theta = -alpha + atan(tan(beta));
            //corresponding value of x
            //note that this is slightly different from the docs, you can get
            //here by multiplying and dividing the expression there by cos(beta)
            double x = r * cos(beta) + x1 * square(sin(beta)) - y1 * sin(beta) * cos(beta);
            //corresponding value of y
            //again, slightly different from the docs, you can get here
            //by same as above.
            double y = r * sin(beta) + y1 * square(cos(beta)) - x1 * sin(beta) * cos(beta);
            
            //TODO: figure out how to mark out line-segments using point features
            //TODO: add check on x and y, see if they lie on the feature
            //(ie. on the line segment rather than the line)

            if (std::abs(theta) <= M_PI / 12){
                double d = sqrt(square(x - x1) + square(y - y1));
                if (readingEstimate == 0 || d < readingEstimate)
                    readingEstimate = d;
            }
        }
        //this is because z is a vector of 1-vectors
        z[i][0] = readingEstimate;
    }

    return z;
}
