import math
from kalman import jacobiaan
import numpy as np

def testsin(x):
    ans = np.copy(x) 
    for i in range(0, len(ans)):
        for j in range(0, len(ans[i])):
            ans[i][j] = math.sin(ans[i][j])
    return ans

x = np.full([3, 5], math.pi/4)
print(x)
print(testsin(x))
H = jacobiaan(testsin, x)
for row in H:
    for matrix in row:
        print(matrix)
