import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib
# Create an empty 16x16 lattice
lattice = np.zeros((15, 15))
#lattice2 = -0.0000001*np.ones((15, 15))
lattice2 = -np.ones((15, 15))
# Fill some values at position (n,m) in the lattice

data = np.genfromtxt('./data/experiment_0-15_1.dat', delimiter=' ')
for i, j, v1, v2 in data:
    lattice[int(j)-1, int(i)-1] = v1*10**(-9)
    lattice2[int(j)-1, int(i)-1] = v2*10**(-9)
# Plot the lattice with heatmap
norm = colors.LogNorm(vmin=lattice.min()+1, vmax=lattice.max())
#norm= colors.SymLogNorm(linthresh=0.1,base=2)
plt.imshow(lattice, cmap='magma', interpolation='nearest', norm=norm)
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



cmap = plt.cm.get_cmap('bone')
cmap = plt.cm.get_cmap('magma')
cmap.set_under(color='white')

norm = colors.Normalize(vmin=0, vmax=lattice2.max())

plt.imshow(lattice2, cmap=cmap, interpolation='nearest',norm =norm)
current_cmap = matplotlib.cm.get_cmap()
current_cmap.set_bad(color='white')
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

