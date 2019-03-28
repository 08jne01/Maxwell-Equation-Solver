import numpy as np
import matplotlib.pyplot as plt

filename = raw_input("Filename: ")

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


#fields = getfield(angles, arr)



#x, y = np.loadtxt
#arr = [y1, y2, y3, y4];
aspect = float(len(x))/float(len(y))


fig, plots = plt.subplots(1, 3, figsize=(10,5))

for i in plots:
	i.set_aspect(1.0)
	i.set_xlabel("x (microns)")
	i.set_ylabel("y (microns)")

titles = ["Ex field", "Ey field", "Ez field"]

for i in range(0, 3):
	plots[i].pcolor(X,Y,fields[i])
	plots[i].set_title(titles[i])

plt.tight_layout()
plt.savefig("E-Fields.png")

fig2, plots2 = plt.subplots(1, 3, figsize=(10,5))

for i in plots2:
	i.set_aspect(1.0)
	i.set_xlabel("x (microns)")
	i.set_ylabel("y (microns)")

titles = ["Hx field", "Hy field", "Hz field"]

for i in range(0, 3):
	plots2[i].pcolor(X,Y,fields[i+3])
	plots2[i].set_title(titles[i])

plt.tight_layout()
plt.savefig("H-fields.png")
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

