import matplotlib.pyplot as plt
import numpy as np

plt.ion()  # interactive mode on

x = np.linspace(0, 4*np.pi, num=100)  # x data
y = np.sin(x)  # initial y data

# create line object
line, = plt.plot(x, y)

for i in range(1, 1000):
    # shifting y data
    y = np.sin(x + i / 50)

    # update line data
    line.set_ydata(y)

    plt.draw()  # redraw the canvas
    plt.pause(0.001)

plt.ioff()  # turn interactive mode off
plt.show()
