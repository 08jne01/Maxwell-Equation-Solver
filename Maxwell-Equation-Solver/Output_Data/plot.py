import numpy as np
import matplotlib.pyplot as plt

filename = raw_input("Filename: ")

if (filename == ""):
	filename = "Field_Components_Mode_1.dat"


arr = np.loadtxt(filename, delimiter=',', unpack=True, skiprows=1)

size = np.sqrt(len(arr[0]))



def get_field(field_big):

	x = np.linspace(0, size-1, size)
	field = []
	n = 0
	for i in x:
		field.append(field_big[i + size*size/2])

	return field
		




#print(get_field(arr[2]))


#fields = getfield(angles, arr)



#x, y = np.loadtxt
#arr = [y1, y2, y3, y4];

fig, plots = plt.subplots(1, 2, figsize=(10,5))

n = 0
for i in plots:
	#print("nothing")
	val = len(get_field(arr[n+2]))/2
	if (n < 2):
		i.plot(np.linspace(-val, val, 2*val), get_field(arr[n+2]), "k-")
	n+=1

#plots[0].plot(((arr[0]-80)**2+(arr[1]-80)**2)**0.5, (arr[5]**2 + arr[6]**2)**0.5, "kx")
#eigenVal[n-2]
#plt.plot(np.linspace(0, 10, len(eigenVal)), eigenVal, "r-");
#print(len(arr))
#plt.plot(x,y, "-");
#plt.xlabel("r (arb. units)")
#plt.ylabel("Et (arb. units)")

plt.show()
