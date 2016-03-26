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
    minX, maxX = 0, 300
    minY, maxY = 0, 300
    for line in sys.stdin:
        rho, theta = map(float, line.split())
        try:
            line_function = line_function_creator(rho, theta)
            X = np.arange(minX, maxX, 0.2)
            X = np.array([i for i in X if minY <= line_function(i) <= maxY])
            Y = np.array([line_function(i) for i in X])
            plt.plot(X, Y)
        except ZeroDivisionError:
            Y = np.arange(minY, maxY, 0.2)
            X = [rho for _ in Y]
            if minX <= rho <= maxX:
                plt.plot(X, Y)

    plt.savefig('plot_lines.png')
    print("saved output to plot_lines.png")

if __name__ == '__main__':
    main()
