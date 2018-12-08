import numpy as np
import matplotlib.pyplot as plt

filename = raw_input("Filename: ")

if (filename == ""):
	filename = "Field_Components_Mode_1.dat"


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

fields = []

for i in range(2, 8):
	field = arr[i, :]
	fields.append(field.reshape(len(x), len(y)))

X, Y = np.meshgrid(x, y)

#print(get_field(arr[2]))


#fields = getfield(angles, arr)



#x, y = np.loadtxt
#arr = [y1, y2, y3, y4];

fig, plots = plt.subplots(1, 2, figsize=(10,5))

for i in plots:
	i.set_aspect(1.0)

print len(fields)

for i in range(0, 2):
	plots[i].pcolor(X,Y,fields[i+3])

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
plt.savefig("square100um.png")
plt.show()
