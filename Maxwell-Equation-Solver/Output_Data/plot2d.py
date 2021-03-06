import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from mpl_toolkits.axes_grid1 import make_axes_locatable

mode = raw_input("Mode: ")

filename = "Field_Components_Mode_" + str(mode) + ".dat"

if (filename == ""):
	filename = "Field_Components_Mode_0.dat"


arr = np.loadtxt(filename, delimiter=',', unpack=True, skiprows=1)

size = int(np.sqrt(len(arr[0])))



def get_field(field_big):

	x = np.linspace(0, size-1, size)
	field = []
	n = 0
	for i in x:
		field.append(field_big[i + size*size/2])

	return field
		
#x, y = np.mgrid[slice(0, int(size), 1), slice(0, int(size), 1)]

x = arr[0, :]
y = arr[1, :]
x = np.unique(x)
y = np.unique(y)

print len(x)
print len(y)

fields = []

for i in range(2, 8):
	field = arr[i, :]
	field = field.reshape(len(x), len(y))
	fields.append(field)

X, Y = np.meshgrid(y, x)

X /= 1e-2
Y /= 1e-2

#print(get_field(arr[2]))
title_size = 25
axis_size = 25
tick_size = 25

step_size = 1.0

#fields = getfield(angles, arr)

cmap = plt.get_cmap('jet')
shading = 'gouraud'
#norm = mpl.colors.Normalize(vmin=np.min(fields[2]),vmax=np.max(fields[0]))



#x, y = np.loadtxt
#arr = [y1, y2, y3, y4];
aspect = float(len(x))/float(len(y))
mpl.rcParams['mathtext.fontset'] = 'stix'
mpl.rcParams['font.family'] = 'STIXGeneral'
mpl.rcParams['xtick.labelsize'] = tick_size
mpl.rcParams['ytick.labelsize'] = tick_size


fig, plots = plt.subplots(1, 3, figsize=(15,5))

for i in plots:
	i.set_aspect(1.0)
	i.set_xlabel("$x$ $(\mu m)$", fontsize=axis_size)
	i.set_ylabel("$y$ $(\mu m)$", fontsize=axis_size)
	#i.tick_params(axis = 'both', which = 'major', labelsize=tick_size)

titles = ["$E_x$ field", "$E_y$ field", "$E_z$ field"]

for i in range(0, 3):
	abs_max = vmax=np.max(abs(fields[i]))
	norm = mpl.colors.Normalize(vmin=-abs_max,vmax=abs_max)
	
	im = plots[i].pcolormesh(X,Y,fields[i], cmap=cmap, shading=shading, norm=norm)
	plots[i].set_title(titles[i], fontsize=title_size)
	start, end = plots[i].get_xlim()
	plots[i].xaxis.set_ticks(np.arange(start, end, step_size))
	start, end = plots[i].get_ylim()
	plots[i].yaxis.set_ticks(np.arange(start, end, step_size))


	#plots[i].ytick_labels(fontsize=axis_size)
	
	divider = make_axes_locatable(plots[i])
	cax = divider.append_axes("right", size="5%", pad=0.05)
	cbar = fig.colorbar(im, ticks=[-abs_max,0,abs_max], cax=cax)
	cbar.ax.set_yticklabels(['-1', '0', '1'], fontsize=axis_size)

plt.tight_layout()
plt.savefig("E-Fields_Mode_" + str(mode) + ".png", dpi=200)
#plt.savefig("E-Fields_Mode_" + str(mode) + ".eps", format='eps')

fig2, plots2 = plt.subplots(1, 3, figsize=(15,5))

for i in plots2:
	i.set_aspect(1.0)
	i.set_xlabel("$x$ $(\mu m)$", fontsize=axis_size)
	i.set_ylabel("$y$ $(\mu m)$", fontsize=axis_size)
	#i.tick_params(axis = 'both', which = 'major', labelsize=tick_size)

titles = ["$H_x$ field", "$H_y$ field", "$H_z$ field"]

for i in range(0, 3):
	abs_max = vmax=np.max(abs(fields[i+3]))
	norm = mpl.colors.Normalize(vmin=-abs_max,vmax=abs_max)
	
	im = plots2[i].pcolormesh(X,Y,fields[i+3], cmap=cmap, shading=shading, norm=norm)
	plots2[i].set_title(titles[i], fontsize=title_size)
	start, end = plots2[i].get_xlim()
	plots2[i].xaxis.set_ticks(np.arange(start, end, step_size))
	start, end = plots2[i].get_ylim()
	plots2[i].yaxis.set_ticks(np.arange(start, end, step_size))

	divider = make_axes_locatable(plots2[i])
	cax = divider.append_axes("right", size="5%", pad=0.05)
	cbar = fig2.colorbar(im, ticks=[-abs_max,0,abs_max], cax=cax)
	cbar.ax.set_yticklabels(['-1', '0', '1'], fontsize=axis_size)


plt.xticks(fontsize=25)
plt.tight_layout()
plt.savefig("H-fields_Mode_" + str(mode) + ".png", dpi=200)
#plt.savefig("H-fields_Mode_" + str(mode) + ".eps", format='eps')
plt.show()

"""
n = 0
for i in plots:
	#print("nothing")
	val = len(get_field(arr[n+2]))/2
	if (n < 2):
		i.plot(np.linspace(-val, val, 2*val), get_field(arr[n+2]), "k-")
	n+=1
"""


#plots[0].plot(((arr[0]-80)**2+(arr[1]-80)**2)**0.5, (arr[5]**2 + arr[6]**2)**0.5, "kx")
#eigenVal[n-2]
#plt.plot(np.linspace(0, 10, len(eigenVal)), eigenVal, "r-");
#print(len(arr))
#plt.plot(x,y, "-");
#plt.xlabel("r (arb. units)")
#plt.ylabel("Et (arb. units)")

