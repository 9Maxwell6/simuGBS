import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors

# Create an empty 16x16 lattice
lattice = np.ones((15, 15))
lattice = np.zeros((15, 15))

# Fill some values at position (n,m) in the lattice

data = np.genfromtxt('./data/experiment_0-15.dat', delimiter=' ')
for i, j, v in data:
    lattice[int(j)-1, int(i)-1] = v
# Plot the lattice with heatmap
norm = colors.LogNorm(vmin=lattice.min()+1, vmax=lattice.max())
#norm= colors.SymLogNorm(linthresh=0.001,base=2)
plt.imshow(lattice, cmap='inferno', interpolation='nearest', norm=norm)
plt.gca().invert_yaxis() 
plt.xlim([-0.5,14.5])
plt.xticks(np.arange(0,15),np.arange(0,15)+1)

#plt.xticks(np.arange(-0.5,15.5))
plt.ylim([-0.5,14.5])
plt.yticks(np.arange(0,15),np.arange(0,15)+1)
cb = plt.colorbar()
cb.set_label('time [s]')

#plt.yticks(np.arange(-0.5,15.5))

plt.xlabel('# bits')
plt.ylabel('Highest order of correlation')
plt.show()