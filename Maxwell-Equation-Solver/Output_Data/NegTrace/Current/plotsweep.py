import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from mpl_toolkits.axes_grid1.inset_locator import inset_axes
from matplotlib.cbook import get_sample_data
from matplotlib.offsetbox import (TextArea, DrawingArea, OffsetImage,AnnotationBbox)
from matplotlib.patches import Circle
import matplotlib.image as mpimg
import matplotlib

data = []

n_core = 3.55
#n_clad = 1.222
n_clad = 1.45
a = 4.e-5

#def line(V, gamma):
	#return ((gamma*a)**2)/(V**2)

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
#plt.rc('font', family='Times New Roman')
plt.rc('legend',fontsize=20)


fig, ax1 = plt.subplots(1, 1, figsize=(10,8))
#inset_axis = inset_axes(ax1, width="40%", height="40%", loc=1, borderpad=0)
#inset_axis.tick_params(labelleft=False,labelbottom=False,bottom=False,left=False)

#fn = get_sample_data("2ndpointmode0.png", asfileobj=False)


def plot_sweep(filename, char, color, label_str, prev_label, index):
	
	data = np.loadtxt(filename, delimiter=',', unpack=True, skiprows=1)

	wavelength = data[0]
	neff = data[1]

	arr_img1 = []
	arr_img2 = []
	arr_img3 = []
	arr_img4 = []
	images = []
	coords = []
	offsets = []
	if (index == 0):
		arr_img1 = plt.imread("2ndpointmode0.png", format='png')
		arr_img2 = plt.imread("mode0.png", format='png')
		arr_img3 = plt.imread("mode0other.png", format='png')
		arr_img4 = plt.imread("mode0end.png", format='png')
		images = [arr_img1, arr_img2, arr_img3, arr_img4]
		coords = [(1.107692308, 0.99983), (1.1, 0.9998345379), (1.292307692,0.9997613734), (1.292307692,0.9997613734)]
		offsets = [(50, -200), (280, -50), (-50, 200), (-220, 50)]
	if (False):
		for i in range(0,4):
			imagebox = OffsetImage(images[i], zoom=0.20)
			imagebox.image.axes = ax1
			s = ""
			if (i % 2 == 0):
				s = "angle,angleA=0,angleB=90,rad=3"
			else:
				s = "angle,angleA=90,angleB=0,rad=3"
			ab = AnnotationBbox(imagebox, coords[i],xybox=offsets[i],xycoords='data',boxcoords="offset points",pad=0.0,arrowprops=dict(arrowstyle="->", connectionstyle=s)) #))
			ax1.add_artist(ab)


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

	V = np.array(V)*1000
	#plt.plot(data[0], y, 'r-')
	#plt.plot(data[0], y, 'rx')

	#popt, pcov = curve_fit(line, V, b)
	#plt.plot(V, line(V, *popt), 'k-', linewidth=2)

	x_truncated = wavelength[5:11:1]
	y_truncated = neff[5:11:1]


	number_lines = 10
	if (index < number_lines):
		ax1.plot(wavelength*(1.e+2), neff, color + char, markersize=10, markeredgewidth=3)
		ax1.plot(wavelength*(1.e+2), neff, color + '-', linewidth=2.0, label=label_str)
		#inset_axis.plot(x_truncated, y_truncated, color + '-', markersize=7, markeredgewidth=2)
	else:
		ax1.plot(wavelength*(1.e+2), neff, color + char, markersize=10, markeredgewidth=3)
		
		if (label_str == ""):
			ax1.plot(wavelength*(1.e+2), neff, color + '-.', linewidth=1.0, label=(prev_label + " in-phase"))
			#inset_axis.plot(x_truncated, y_truncated, color + '-.', markersize=7, markeredgewidth=2)
		else:
			ax1.plot(wavelength*(1.e+2), neff, color + ':', linewidth=1.0, label=(label_str + " anti-phase"))
			#inset_axis.plot(x_truncated, y_truncated, color + ':', markersize=7, markeredgewidth=2)
	


	
	
#filename = raw_input("Filename: ")
#if (filename == ""):
	#filename = "Sweep.dat"

num = raw_input("Number of Traces: ")
if (num == ""):
	num = 1

label = raw_input("Label: ")

sep = '0'

num = int(num)



point_types = ['x','o','^','s','v','P']
colors = ['r','g','b','k','c','m']
#line_type = ['-', '-', '-', '-', '-', '-', '-', '-', '-', '-']
label_str = ['Mode 0', 'Mode 1', 'Mode 2','Mode 3','Mode 4','','0.20 $\mu m$ Separation','','0.50 $\mu m$ Separation','']
#label_str = ['Mode 0', 'Mode 1', 'Mode 2','Mode 3','Mode 4','','0.20 $\mu m$ Separation','','0.50 $\mu m$ Separation','']
#point_types = ['x','o','^','s','v','P']
#colors = ['r','g','b','k','c','m']

for i in range(0,num):
	prev_label = ''
	if (i > 0):
		prev_label = label_str[i-1]
	plot_sweep("Sweep" + str(i) + ".dat", point_types[i], colors[i], label_str[i], prev_label, i)



#plt.xlabel("Normalised frequency, $V$ ($10^{-3}$)", fontsize=30)
ax1.set_xlabel("Wavelength in free space, $\lambda$ ($10^{-6}$)", fontsize=30)
ax1.set_ylabel("Effective refractive index, $n_{eff}$", fontsize=30)
#plt.xlim(0,12.4)
#plt.ylim(0,1.0)
#plt.xticks(np.linspace(0.0, 12,7), fontsize=24)
ax1.tick_params(axis='both', which='major', labelsize=24)
ax1.legend()
plt.tight_layout()
plt.savefig('Dispersion' + label + '.eps', format='eps')
plt.savefig('Dispersion' + label + '.png', format='png', dpi=200)
plt.show()