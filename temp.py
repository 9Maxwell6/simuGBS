import numpy as np
import matplotlib.pyplot as plt
#'''
# Set the mean and standard deviation of the Gaussian distribution
mean = 0
std_dev = 1

# Generate the Gaussian distribution
x = np.linspace(-4, 4, 101)
y = np.linspace(-4, 4, 101)
X, Y = np.meshgrid(x, y)
Z = np.exp(-(10*X**2 + 10*Y**2)/(2*std_dev**2)) / (2*np.pi*std_dev**2)


theta=2
#x_r = np.cos(2)*x-np.sin(2)*y
#y_r = np.cos(2)*y+np.sin(2)*x
#X, Y = np.meshgrid(x_r, y_r)
#Z = np.exp(-(24*X**2 + 6*Y**2)/(2*std_dev**2)) / (2*np.pi*std_dev**2)

#Z = np.exp(-(10*(X-2)**2 + 10*(Y+3)**2)/(2*std_dev**2)) / (2*np.pi*std_dev**2)
Z = np.exp(-(20*(X)**2 + 20*(Y)**2)/(2*std_dev**2)) / (2*np.pi*std_dev**2)


# Plot the temperature map
fig, ax = plt.subplots()
im = ax.imshow(Z, cmap='hot')
#fig.colorbar(im)
#ax.set_title('Squeezed vacuum state')
ax.set_xlabel('x')
ax.set_ylabel('p')
#ax.tick_params(axis='x', colors='white')
#ax.tick_params(axis='y', colors='white')
# Move left y-axis and bottom x-axis to centre, passing through (0,0)
ax.spines['left'].set_position('center')
ax.spines['bottom'].set_position('center')
ax.spines['left'].set_color('white')
ax.spines['bottom'].set_color('white')
ax.set_xticks([])
ax.set_yticks([])

# Eliminate upper and right axes
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.yaxis.set_label_coords(0.49, 0.95)
ax.xaxis.set_label_coords(0.95, 0.49)
ax.xaxis.label.set_color('white')
ax.yaxis.label.set_color('white')

#ax.yaxis.set_label_coords(-.1, .9)
#ax.xaxis.set_label_coords(.9, -.1)
plt.show()
#'''


'''
import numpy as np
import matplotlib.pyplot as plt

# Set the mean of the Gaussian distribution
mean = [0, 0]
#mean = [0.5, 0.5]

# Set the covariance matrix of the Gaussian distribution to produce a narrow and rotated distribution
cov = [[0.07, 0],
       [0, 0.01]]
theta = np.radians(30)
rotation_matrix = np.array([[np.cos(theta), -np.sin(theta)],
                            [np.sin(theta), np.cos(theta)]])
cov = rotation_matrix @ cov @ rotation_matrix.T

# Generate the Gaussian distribution
x, y = np.meshgrid(np.linspace(-1, 1, 101), np.linspace(-1, 1, 101))
pos = np.empty(x.shape + (2,))
pos[:, :, 0] = x
pos[:, :, 1] = y
det_cov = np.linalg.det(cov)
inv_cov = np.linalg.inv(cov)
Z = np.empty(x.shape)
for i in range(x.shape[0]):
    for j in range(x.shape[1]):
        vec = np.array([5*(x[i, j] - mean[0]), 1*(y[i, j] - mean[1])])
        Z[i, j] = np.exp(-0.5 * vec.T @ inv_cov @ vec) / (2 * np.pi * np.sqrt(det_cov))

# Plot the temperature map
fig, ax = plt.subplots()
im = ax.imshow(Z, cmap='hot', extent=[-1, 1, -1, 1], origin='lower')
#fig.colorbar(im)
#ax.set_title('Squeezed vacuum state')
ax.set_xlabel('x')
ax.set_ylabel('p')
#ax.tick_params(axis='x', colors='white')
#ax.tick_params(axis='y', colors='white')
# Move left y-axis and bottom x-axis to centre, passing through (0,0)
ax.spines['left'].set_position('center')
ax.spines['bottom'].set_position('center')

ax.spines['left'].set_color('white')
ax.spines['bottom'].set_color('white')
ax.set_xticks([])
ax.set_yticks([])

# Eliminate upper and right axes
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.yaxis.set_label_coords(0.49, 0.95)
ax.xaxis.set_label_coords(0.95, 0.49)
ax.xaxis.label.set_color('white')
ax.yaxis.label.set_color('white')

#ax.yaxis.set_label_coords(-.1, .9)
#ax.xaxis.set_label_coords(.9, -.1)
plt.show()

'''