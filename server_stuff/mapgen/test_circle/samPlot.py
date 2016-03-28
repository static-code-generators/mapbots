import matplotlib.pyplot as plt

# just change the filename below as required to visualize its data
f = open('data/batch03.dat','r')

lines = f.readlines()

xC = []
yC = []

for i in range(1, len(lines)):
    xy = lines[i].split(' ')
    
    if len(xy) != 1:
        xC.append(xy[0])
        yC.append(xy[1])

    else:
        plt.plot(xC, yC, 'ro')
        plt.axis([-1.5, 1.5, -1.5, 1.5])
        plt.show()
        xC = []
        yC = []
