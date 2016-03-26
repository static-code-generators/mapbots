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
    for line in sys.stdin:
        rho, theta = map(float, line.split())
        line_function = line_function_creator(rho, theta)
        X = np.arange(0, 10, 0.2);
        Y = [line_function(i) for i in X]
        plt.plot(X, Y)
    plt.savefig('plot_lines.png')
    print("saved output to plot_lines.png")

if __name__ == '__main__':
    main()
