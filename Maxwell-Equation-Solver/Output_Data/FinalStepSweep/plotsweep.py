import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import matplotlib

data = []

n_core = 1.45
n_clad = 1.222
a = 5.e-5

#def line(V, gamma):
	#return ((gamma*a)**2)/(V**2)

def plot_sweep(filename, char, color):
	
	data = np.loadtxt(filename, delimiter=',', unpack=True, skiprows=1)

	wavelength = data[0]
	neff = data[1]

	k = 2.*np.pi/wavelength
	V = k*a*((n_core**2 - n_clad**2)**0.5)
	b = (((neff/n_clad)**2)-1) / (((n_core/n_clad)**2) - 1)

	temp_b = []
	temp_V = []

	first_nonneg_x = 0
	for i in range(0, len(b)):
		if (b[i] > 0):
			
			temp_b.append(b[i])
			temp_V.append(V[i])
		else:
			if (first_nonneg_x == 0):
				first_nonneg_x = i
				print ("Next min wavelength: " + str(wavelength[i]))
	V = temp_V
	b = temp_b

	V = np.array(V)*1000.

	#plt.plot(data[0], y, 'r-')
	#plt.plot(data[0], y, 'rx')

	#popt, pcov = curve_fit(line, V, b)
	#plt.plot(V, line(V, *popt), 'k-', linewidth=2)
	plt.plot(V, b, color + char, markersize=10, markeredgewidth=3)
	plt.plot(V, b, color + '-')
	
	
filename = raw_input("Filename: ")
if (filename == ""):
	filename = "Sweep.dat"

num_modes = int(raw_input("Number of Modes: "))

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
#plt.rc('font', family='Times New Roman')
plt.figure(1, figsize = (10,8))
plt.rc('legend',fontsize=20)

point_types = ['x','o','^','s','v','P']
colors = ['r','g','b','k','c','m']

for i in range(0,num_modes):
	plot_sweep("Sweep" + str(i) + ".dat", point_types[i], colors[i])



plt.xlabel("Normalised frequency, $V$ ($10^{-3}$)", fontsize=30)
plt.ylabel("Normalised propagation constant, $b$", fontsize=30)
plt.xlim(0,12.4)
plt.ylim(0,1.0)
plt.xticks(np.linspace(0.0, 12,7), fontsize=24)
plt.yticks(fontsize=24)
plt.legend()
plt.tight_layout()
plt.savefig('Dispersion.eps', format='eps')
plt.savefig('Dispersion.png', format='png', dpi=200)
plt.show()