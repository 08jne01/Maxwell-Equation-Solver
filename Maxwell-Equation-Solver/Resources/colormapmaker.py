from matplotlib import cm
import numpy as np

buf = cm.coolwarm(np.arange(256))

cmap = []
for i in buf:
	rgb = []
	for j in range(0,3):
		rgb.append(i[j])
	cmap.append(rgb)

data = cmap
data_name = "coolwarm"

print ("Length of Data: " + str(len(data)))

f = open(data_name + ".dat", "w+")

for i in data:
    n = 0
    n_max = len(i)
    for j in i:
        if (n < n_max - 1):
            f.write(str(j) + ",")
        else:
            f.write(str(j) + "\n")
        n += 1
            
f.close()