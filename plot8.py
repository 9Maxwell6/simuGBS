import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib
# Create an empty 16x16 lattice
lattice = np.zeros((16, 28))
#lattice2 = -0.0000001*np.ones((15, 15))
lattice2 = -np.ones((16, 28))
# Fill some values at position (n,m) in the lattice

data = np.genfromtxt('./data/experiment1-27.dat', delimiter=' ')
for i, j, v1, v2 in data:
    lattice[int(j)-1, int(i)-1] = v1
# Plot the lattice with heatmap
norm = colors.LogNorm(vmin=lattice.min()+1, vmax=lattice.max())

#norm= colors.SymLogNorm(linthresh=0.1,base=2)
plt.imshow(lattice, cmap='magma', interpolation='nearest', norm=norm)
plt.gca().invert_yaxis() 
plt.xlim([-0.5,27.5])
plt.xticks(np.arange(0,28),np.arange(0,28)+1)

#plt.xticks(np.arange(-0.5,15.5))
plt.ylim([-0.5,14.5])
plt.yticks(np.arange(0,16),np.arange(0,16)+1)
cb = plt.colorbar()
cb.set_label('time [s]')

#plt.yticks(np.arange(-0.5,15.5))

plt.xlabel('# bits')
plt.ylabel('Highest order of correlation')
plt.show()



