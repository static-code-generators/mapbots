#include "ekf.hpp"
#include <iomanip>
#include <cmath>
#include <vector>

typedef ublas::identity_matrix<double> identity_matrix;
typedef ublas::zero_matrix<double> zero_matrix;

vector_type filter(vector_type x, std::vector<vector_type> zActual, std::vector<vector_type> motion);
vector_type distanceEstimator(const vector_type &x);

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

vector_type filter(vector_type x, std::vector<vector_type> zActual, std::vector<vector_type> motion)
{
    //the first argument is the vector we are trying to predict
    //the second argument is the series of measurements of the ultrasonic sensors
    //the third argument is the series of motions the bot makes (from the wheel encoder)

    //here we implement the extended kalman filter.
    //(insert attribution to original paper here)

    int m = x.size();

    //covariance matrix:
    //possible optimisation -> use ublas::sparse_matrix instead, since
    //a lot of the entries here are 0.
    matrix_type P(m, m);
    //we are certain of the bot's original position (since it is the base reference)
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            P(i, j) = 0;
    //we are very uncertain of the feature's locations
    for (int i = 3; i < m; i += 2)
        for (int j = i; j < i + 2; ++j)
            P(i, j) = 42696942.53008;
    
    //covariance of white noise
    //in measurement of x
    matrix_type Q(m, m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            if (i == j)
                Q(i, j) = 1;
            else
                Q(i, j) = 0;
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
    matrix_type R(s, s);
    // R is no longer an asshole, see file history to know how it was.
    //TODO: find variance of sensors by experiment, and change R to have those.
    //R is actually just the variance of the error of the ultrasonic sensors.
    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            if (i == j)
                R(i, j) = 1;
            else
                R(i, j) = 0;

    //after all of this nonsense, we are finally ready to start
    //executing the filter.

    //psyche!
    //there is actually some more nonsense

    //these matrices are used in the predict step.
    matrix_type F(m, m);
    vector_type G(m, 0);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            if (i == j)
                F(i, j) = 1;
    //why do we have these specific values for G(i, j) and F(i, j)?
    //this is a difficult question we choose not to answer. fuck off.

    //the lies are over, this is the filter, I swear.
    int k = zActual.size();
    for (int i = 0; i < k; ++i) {
        vector_type botPos = ublas::subrange(x, 0, 3);
        matrix_type J1 = compositionJacobian1(botPos, motion[i]);
        matrix_type J2 = compositionJacobian2(botPos, motion[i]);
        botPos = composition(botPos, motion[i]);

        matrix_type Q00(3, 3);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                Q00(i, j) = Q(i, j);

        assert(J1.size1() == 3 && J1.size2() == 3);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                F(i, j) = J1(i, j);

        matrix_type FP = prod(F, P);
        P = prod(FP, ublas::trans(F));
        matrix_type J2Q = prod(J2, Q);
        matrix_type tempProduct = prod(J2Q, ublas::trans(J2));
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                P(i, j) += tempProduct(i, j);

        vector_type z = zActual[i];
        matrix_type H = jacobian(distanceEstimator, x); 
        matrix_type PHT = prod(P, ublas::trans(H)); //the product of P and H'
        matrix_type Z = prod(H, PHT) + R;
        matrix_type Zinv;
        invertMatrix(Z, Zinv);
        matrix_type K = prod(PHT, Zinv);
        
        x = x + prod(K, z - distanceEstimator(x));
        matrix_type KH = prod(K, H);
        P = prod(identity_matrix(m) - KH, P);
    }

    return x;
}

vector_type distanceEstimator(const vector_type &x)
{
    //Questions:
    //How are we differentiating between point and line features?
    //Two approaches:
    //  * add an additional parameter to each feature
    //  * make x so that the first k entries are points, the rest are lines, pass k as a parameter.

    //we assume that the position of the bot is a 3-vector
    //and is stored in x(0..2)
    vector_type botPos = ublas::subrange(x, 0, 3);

    //we assume that our bot has 24 sensors, placed 
    //evenly starting at 0 radian (wrt current bot orientation)
    //to 2*PI radians
    //TODO: determine the distance of the sensors from the center
    //of the bot, and hence add their correct x and y-coordinate.
    double angleIncr = 2 * M_PI / 8;

    //we store the distances we estimate for each sensor here.
    const int numSensors = 24;
    vector_type z(numSensors, 0);
    int numFeatures = (x.size() - 3) >> 1;

    double x1 = botPos(0), y1 = botPos(1), alpha = botPos(2);
    for (int i = 0; i < numSensors; ++i) {
        double readingEstimate = 0;
        //note that j iterates from 1 to x.size() - 1
        //since the position of the bot is stored in the x[0]
        //for now, we assume only line features
        for (int j = 0; j < numFeatures; ++j) {
            
            //enter parameters of feature
            //this is because the 0..2 are the botPosition
            //so each feature takes two elements like 3..4, 5..6 and so on
            //hence, we add 3 + 2(numFeatures before this one) to get the
            //starting index of this feature -> r
            //and next index -> beta
            double r = x(3 + 2*j), beta = x(3 + 2*j + 1);

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
        z(i) = readingEstimate;
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
