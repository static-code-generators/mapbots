"""
plots lines, reads from stdin until eof
in format
rho theta
where rho is the distance of the line from the origin
and theta is the angle of the shortest line from the origin and the x-axis, in radians
"""
import matplotlib.pyplot as plt
import numpy as np
import sys

from math import sin, cos, tan
from decimal import Decimal

def line_function_creator(rho, theta):
    """returns a function that takes x and gives y for line described by rho and theta"""
    m = -1 / tan(theta)
    x1 = rho * cos(theta)
    y1 = rho * sin(theta)
    c = y1 - m * x1
    def line_function(x):
        return (m * x + c)
    return line_function

def main():
    plt.clf()
    plt.cla()
    lines = []
    for line in sys.stdin:
        lines.append(tuple(map(float, line.split())))

    #put all the lines which have theta = 0 at the end
    negative_second = lambda tup : -abs(tup[1])
    lines.sort(key=negative_second)

    #to know the bounds over which we want to plot lines
    #which are parallel to the Y-axis
    maxY, minY = 0, 0 
    at_zeros = 0
    for line in lines:
        rho, theta = line
        try:
            line_function = line_function_creator(rho, theta)
            X = np.arange(0, 10, 0.2)
            Y = [line_function(i) for i in X]
            maxY = max(maxY, max(Y))
            minY = min(minY, min(Y))
            plt.plot(X, Y)
        except ZeroDivisionError:
            #tune bounds to fill empty space matplotlib leaves
            if at_zeros == 0:
                at_zeros = 1
                minY -= 5
                maxY += 5
            Y = np.arange(minY, maxY, 0.2)
            X = [rho for _ in Y]
            plt.plot(X, Y)

    plt.savefig('plot_lines.png')
    print("saved output to plot_lines.png")

if __name__ == '__main__':
    main()
