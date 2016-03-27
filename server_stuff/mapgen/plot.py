#!/usr/bin/env python3

"""
Plot lines (Hesse normal form) and points (Cartesian coordinates)
"""
import matplotlib.pyplot as plt
import numpy as np
import argparse
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

def plotlines(f, plt, x_range=(0, 100), y_range=(0, 100)):
    """ plots lines, reads from f until eof in format:
    rho theta
    where rho is the distance of the line from the origin
    and theta is the angle of the shortest line from the origin
    and the x-axis, in radians
    plt: Matplotlib module
    x_range: Plot range of x-axis (min, max)
    y_range: Plot range of y-axis (min, max)
    """
    minX, maxX = x_range
    minY, maxY = y_range
    for line in f:
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

def plotpoints(f, plt, x_range=(0, 100), y_range=(0, 100)):
    """ plots points, reads from f until eof in format:
    x y
    where x and y are the cartesian coordinates of the point
    plt: matplotlib.pyplot module
    x_range: Plot range of x-axis (min, max)
    y_range: Plot range of y-axis (min, max)
    """
    minX, maxX = x_range
    minY, maxY = y_range
    for point in f:
        coord = tuple(map(float, point.split()))
        if (minX < coord[0] < maxX) and (minY < coord[1] < maxY):
            plt.plot(coord[0], coord[1], 'ro')

def main():
    program_desc ="""Plot lines (Hesse normal form) and points (Cartesian coordinates)"""
    parser = argparse.ArgumentParser(description=program_desc)
    parser.add_argument('-l', '--linefile', nargs='?',
            help='File from which to read lines',
            type=argparse.FileType('r'))
    parser.add_argument('-p', '--pointfile', nargs='?',
            help='File from which to read points',
            type=argparse.FileType('r'))
    parser.add_argument('outfile',
            help='Output image file',
            type=argparse.FileType('w'))
    parser.add_argument('--xmin',
            help='Minimum value to plot on X-axis (default = 0)',
            type=int, default=0)
    parser.add_argument('--xmax',
            help='Maximum value to plot on X-axis (default = 100)',
            type=int, default=100)
    parser.add_argument('--ymin',
            help='Minimum value to plot on Y-axis (default = 0)',
            type=int, default=0)
    parser.add_argument('--ymax',
            help='Maximum value to plot on Y-axis (default = 100)',
            type=int, default=100)
    args = parser.parse_args()
    if args.pointfile == None and args.linefile == None:
        parser.error('one of the following arguments are required: LINEFILE POINTFILE')
    x_range = (args.xmin, args.xmax)
    y_range = (args.ymin, args.ymax)
    if args.linefile != None:
        l = args.linefile
        plotlines(l, plt, x_range, y_range)
    if args.pointfile != None:
        p = args.pointfile
        plotpoints(p, plt, x_range, y_range)
    plt.savefig(args.outfile.name)
    print('Saved output to %s' % (args.outfile.name))

if __name__ == '__main__':
    main()
