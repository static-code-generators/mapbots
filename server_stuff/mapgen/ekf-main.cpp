#include "ekf.hpp"
#include <iomanip>
#include <cmath>

vector_of_vector_type distanceEstimator(vector_of_vector_type x);

int main()
{

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

    double currX = botPos[0], currY = botPos[1], currAngle = botPos[2];
    for (int i = 0; i < numSensors; ++i) {
        //note that j iterates from 1 to x.size() - 1
        //since the position of the bot is stored in the x[0]
        for (int j = 1; j < x.size(); ++j) {
            //fill in code to find the distance of feature j from
            //sensors i
        }
    }

    return z;
}
