#include "ekf.hpp"
#include <iomanip>
#include <cmath>
#include <vector>

typedef ublas::identity_matrix<double> identity_matrix;
typedef ublas::zero_matrix<double> zero_matrix;

vector_of_vector_type distanceEstimator(vector_of_vector_type x);
vector_of_vector_type filter(vector_of_vector_type x, std::vector<vector_of_vector_type> zActual, std::vector<vector_type> motion);

vector_type composition(vector_type xB, vector_type xC);
matrix_type compositionJacobian1(vector_type xB, vector_type xC);
matrix_type compositionJacobian2(vector_type xB, vector_type xC);

template<typename T>
T square(T x)
{
    return x * x;
}

int main()
{

}

vector_of_vector_type filter(vector_of_vector_type x, std::vector<vector_of_vector_type> zActual, std::vector<vector_type> motion)
{
    //the first argument is the vector we are trying to predict
    //the second argument is the series of measurements of the ultrasonic sensors
    //the third argument is the series of motions the bot makes (from the wheel encoder)

    //here we implement the extended kalman filter.
    //(insert attribution to original paper here)

    int m = x.size(); //number of features + 1(position of bot)

    //x[0] is bot position, hence a 3-vector.
    assert(x[0].size() == 3);
    //x[1 ... m - 1] are features, hence 2-vectors.
    for (int i = 1; i < m; ++i)
        assert(x[i].size() == 2);

    //covariance matrix:
    //possible optimisation -> use ublas::sparse_matrix instead, since
    //a lot of the entries here are 0.
    matrix_of_matrix_type P(m, m);
    //we are certain of the bot's original position (since it is the base reference)
    P(0, 0) = zero_matrix(3, 3);
    //we are very uncertain of the feature's locations
    for (int i = 1; i < m; ++i)
        P(i, i) = matrix_type(2, 2, 42696942.1);
    
    //covariance of white noise
    //in measurement of x
    matrix_of_matrix_type Q(m, m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            if (i == j)
                Q(i, j) = identity_matrix(x[i].size());
            else
                Q(i, j) = zero_matrix(x[i].size(), x[j].size());
    //on-diagonal entries become identity
    //off-diagonal entries become 0 matrix
    //TODO: tune using actual readings and adventures.
    //note that this is essentially the variance of the 
    //odometer's error.

    //covariance of white noise
    //in sensor measurements
    //why is all the noise white? because we are racist.
    //we know that number of sensors is 24.
    //(it's actually 8, we're simulating 24 by using a servo,
    //but shoo)
    int s = 24; //unhelpful variable name, sue me.
    matrix_of_matrix_type R(s, s);
    //R is a real MVP tbh, since it is the only matrix which 
    //is actually a matrix of scalars. 
    //but because the other matrices are assholes, R has to be an asshole
    //too. hence, R is a matrix of 1x1 matrices :'(
    //TODO: find variance of sensors by experiment, and change R to have those.
    //R is actually just the variance of the error of the ultrasonic sensors.
    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            if (i == j)
                R(i, j) = identity_matrix(1);
            else
                R(i, j) = zero_matrix(1, 1);

    //after all of this nonsense, we are finally ready to start
    //executing the filter.

    //psyche!
    //there is actually some more nonsense

    //the people in the paper have added the wrong dimensions for G
    //maybe they were high.
    //unless I have the wrong dimensions for Q.
    //(which is possible) TODO: check if I messed up.
    //these matrices are used in the predict step.
    matrix_of_matrix_type F(m, m), G(m, m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j) {
            G(i, j) = F(i, j) = zero_matrix(x[i].size(), x[j].size());
            if (i == j)
                F(i, j) = identity_matrix(x[i].size(), x[i].size());
        }
    //why do we have these specific values for G(i, j) and F(i, j)?
    //this is a difficult question we choose not to answer.

    //the lies are over, this is the filter, I swear.
    int k = zActual.size();
    for (int i = 0; i < k; ++i) {
        F(0, 0) = compositionJacobian1(x[0], motion[i]);
        G(0, 0) = compositionJacobian2(x[0], motion[i]);
        x[0] = composition(x[0], motion[i]);

        //I hope this works. If anything breaks, check here first.
        //the function prod_local is in include/ekf.{h,c}pp
        P = prod_local(prod_local(F, P), ublas::trans(F));
        P += prod_local(prod_local(G, Q), ublas::trans(G));
    }
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

vector_type composition(vector_type xB, vector_type xC)
{
    //the composition transformation.
    //look at the appendix of the paper for more details
    //takes xB with respect to some xA, 
    //xC with respect to xB
    //return xC with respect to xA.

    //xB and xC must be the same size
    assert(xB.size() == xC.size());
    //xB and xC must each have three values:
    //(x, y, theta) [theta -> orientation (in radians)]
    assert(xB.size() == 3);

    double x1 = xB(0), y1 = xB(1), theta1 = xB(2);
    double x2 = xC(0), y2 = xC(1), theta2 = xC(2);

    //we do not derive these formulae here, but trust me,
    //they are correct. (refer to the back of the paper if you don't)
    double x3 = x1 + x2 * cos(theta1) - y2 * sin(theta1);
    double y3 = y1 + x2 * sin(theta1) + y2 * cos(theta1);
    double theta3 = theta1 + theta2;
    //TODO: decide a range for angles, and make sure this addition
    //gets theta3 in the correct range. 

    //denotes xC with respect to A
    vector_type xCA(3);
    
    xCA(0) = x3;
    xCA(1) = y3;
    xCA(2) = theta3;

    return xCA;
}

matrix_type compositionJacobian1(vector_type xB, vector_type xC)
{
   //returns the jacobian of the composition operator
   //with respect to xB

    //xB and xC must be the same size
    assert(xB.size() == xC.size());
    //xB and xC must each have three values:
    //(x, y, theta) [theta -> orientation (in radians)]
    assert(xB.size() == 3);

    double x1 = xB(0), y1 = xB(1), theta1 = xB(2);
    double x2 = xC(0), y2 = xC(1), theta2 = xC(2);

    matrix_type J(3, 3, 0);

    //refer to the appendix for these formulae.
    J(0, 0) = 1;
    J(1, 1) = 1;
    J(2, 2) = 1;
    J(0, 2) = -x2 * sin(theta1) - y2 * cos(theta1);
    J(1, 2) = x2 * cos(theta1) - y2 * sin(theta1);
    //the rest of the entries remain 0, woo.
    //wow, someone is actually reading this,
    //"Person, I am not your father" - Parth

    return J;
}

matrix_type compositionJacobian2(vector_type xB, vector_type xC)
{
   //returns the jacobian of the composition operator
   //with respect to xC

    //xB and xC must be the same size
    assert(xB.size() == xC.size());
    //xB and xC must each have three values:
    //(x, y, theta) [theta -> orientation (in radians)]
    assert(xB.size() == 3);

    double x1 = xB(0), y1 = xB(1), theta1 = xB(2);
    double x2 = xC(0), y2 = xC(1), theta2 = xC(2);

    matrix_type J(3, 3, 0);

    //refer to the appendix for these formulae.
    //note that they don't derive them either, but we did
    //before writing them here (yes we did)
    J(0, 0) = cos(theta1);
    J(0, 1) = -sin(theta1);
    J(1, 0) = sin(theta1);
    J(1, 1) = cos(theta1);
    J(2, 2) = 1;
    //the rest of the entries are 0.
    //and seriously, we did derive these.

    return J;
}
