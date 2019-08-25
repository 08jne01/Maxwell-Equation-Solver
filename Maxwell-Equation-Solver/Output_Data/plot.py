import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

mode = raw_input("Mode: ")

filename = "Field_Components_Mode_" + str(mode) + ".dat"

if (filename == ""):
	filename = "Field_Components_Mode_0.dat"


arr = np.loadtxt(filename, delimiter=',', unpack=True, skiprows=1)

#size = np.sqrt(len(arr[0]))
hsize = 160
vsize = 80
line = 40

mpl.rcParams['mathtext.fontset'] = 'stix'
mpl.rcParams['font.family'] = 'STIXGeneral'


def get_field(field_big):

	x = np.linspace(0.0, 4.72031, hsize)
	field = []
	n = 0
	for i in x:
		field.append(field_big[n + hsize*(vsize*0.5)])
		n+=1
	return field,x
		




#print(get_field(arr[2]))


#fields = getfield(angles, arr)



#x, y = np.loadtxt
#arr = [y1, y2, y3, y4];

titles = ['$E_x$ field','$E_y$ field']
x_axis = ['$x$','$x$']
title_size=25
axis_size=20
step_size=1
tick_size = 15

mpl.rcParams['xtick.labelsize'] = tick_size
mpl.rcParams['ytick.labelsize'] = tick_size

fig, plots = plt.subplots(1, 2, figsize=(10,3))

for i in range(0,2):
	#print("nothing")
	val = len(get_field(arr[i+2]))/2
	if (i < 2):
		field,x = get_field(arr[i+2])
		min_val = np.min(arr[i+2])
		print min_val
		#field = field - min_val
		field_temp = arr[i+2]
		field_temp -= min_val
		max_val = np.max(np.abs(field_temp))
		print max_val
		field -= min_val
		field = field / max_val
		#field -= np.min(field)
		
		plots[i].plot(x, field, "k-")
		plots[i].set_title(titles[i], fontsize=title_size)

		plots[i].set_xlabel("$x$ $(\mu m)$", fontsize=axis_size)
		plots[i].set_ylabel("Normalised " + titles[i], fontsize=axis_size)

		start, end = plots[i].get_xlim()
		actual_end = end
		start = start + (step_size - (start % step_size))
		end = end + (end % step_size)
		ticks = np.arange(start, end, step_size)
		plots[i].xaxis.set_ticks(ticks)
		#plots[i].set_xlim(0.0, 4.72031)
		#plots[i].yaxis.set_ticks(np.arange(start, end, step_size))

#plots[0].plot(((arr[0]-80)**2+(arr[1]-80)**2)**0.5, (arr[5]**2 + arr[6]**2)**0.5, "kx")
#eigenVal[n-2]
#plt.plot(np.linspace(0, 10, len(eigenVal)), eigenVal, "r-");
#print(len(arr))
#plt.plot(x,y, "-");
#plt.xlabel("r (arb. units)")
#plt.ylabel("Et (arb. units)")
plt.tight_layout()
plt.savefig("Mode_Cross-section_" + str(mode) + ".png", dpi=200)
plt.savefig("Mode_Cross-section_" + str(mode) + ".eps")
plt.show()
