import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Read data from file
data = {'origin': None, 'direction': None, 'corners': [], 'intersection': None}
with open('visualization_data.txt') as f:
    for line in f:
        parts = line.split()
        if parts[0] == 'origin':
            data['origin'] = np.array([float(parts[1]), float(parts[2]), float(parts[3])])
        elif parts[0] == 'direction':
            data['direction'] = np.array([float(parts[1]), float(parts[2]), float(parts[3])])
        elif parts[0] == 'corner':
            data['corners'].append(np.array([float(parts[1]), float(parts[2]), float(parts[3])]))
        elif parts[0] == 'intersection':
            data['intersection'] = np.array([float(parts[1]), float(parts[2]), float(parts[3])])

# Prepare figure
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the origin
ax.scatter(*data['origin'], color='red', label='Origin')

# Plot the line (extend the direction vector for visibility)
line_end = data['origin'] + data['direction'] * 10
ax.plot([data['origin'][0], line_end[0]], [data['origin'][1], line_end[1]], [data['origin'][2], line_end[2]], 'r--', label='Line')

# Plot the rectangle
corners = np.array(data['corners'])
print(corners)
ax.plot_trisurf(corners[:, 0], corners[:, 1], corners[:, 2], color='cyan', alpha=0.5)

# Plot the intersection point if it exists
if data['intersection'] is not None:
    ax.scatter(*data['intersection'], color='blue', label='Intersection')

# Set plot labels and legend
ax.set_xlabel("X (m)")
ax.set_ylabel("Y (m)")
ax.set_zlabel("Z (m)")
ax.legend()
plt.show()
