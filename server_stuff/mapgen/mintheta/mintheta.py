from sympy.solvers import solve
from sympy import *

#symbols for point of intersection
x = Symbol('x')
y = Symbol('y')

#symbols for line 1
x1 = Symbol('x1')
y1 = Symbol('y1')
theta = Symbol('theta')
alpha = Symbol('alpha')

#equation for line 1
l1 = y - y1 - (x - x1) * tan(alpha + theta)
print("the equation for the first line is")
print(latex(l1))

#symbols for line 2
r = Symbol('r')
beta = Symbol('beta')

#equation for line 2
l2 = x * cos(beta) + y * sin(beta) - r
print("the equation for the second line is")
print(latex(l2))

#solve for point of intersection of the two lines
soln = solve([l1, l2], x, y)

#variable containing distance, we have to minimise this wrt theta
d = (soln[x] - x1)**2 + (soln[y] - y1)**2

#compute the derivative wrt theta
d_prime = diff(d, theta)

#solve for value of theta minimising d
theta_min = solve(d_prime, theta)

print("minimising theta and the corresponding values of x and y are")
for i in theta_min:
    print("theta")
    print(latex(i))
    print("x")
    print(latex(soln[x].subs({theta:i})))
    print("y")
    print(latex(soln[y].subs({theta:i})))
