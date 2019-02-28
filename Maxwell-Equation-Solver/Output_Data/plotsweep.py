import numpy as np
import matplotlib.pyplot as plt

data = []

filename = raw_input("Filename: ")
if (filename == ""):
	filename = "Sweep.dat"
data = np.loadtxt(filename, delimiter=',', unpack=True, skiprows=1)

y = []
y =  data[1] - 0.999999

plt.plot(data[0], y, 'r-')
plt.plot(data[0], y, 'rx')
plt.xlabel("Wavelength (m)")
plt.ylabel("neff - 0.999999")

plt.show()