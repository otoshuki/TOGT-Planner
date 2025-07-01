import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

# Load data
df = pd.read_csv("togt_refined_traj.csv")
# df = pd.read_csv("togt_traj.csv")

# Extract relevant columns
x, y, z, t = df['p_x'], df['p_y'], df['p_z'], df['t']

# Create 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlim(x.min(), x.max())
ax.set_ylim(y.min(), y.max())
ax.set_zlim(z.min(), z.max())
ax.set_xlabel('p_x')
ax.set_ylabel('p_y')
ax.set_zlabel('p_z')
line, = ax.plot([], [], [], lw=2)
point, = ax.plot([], [], [], 'ro')

# Animation function
def update(frame):
    line.set_data(x[:frame], y[:frame])
    line.set_3d_properties(z[:frame])
    point.set_data(x[frame:frame+1], y[frame:frame+1])
    point.set_3d_properties(z[frame:frame+1])
    return line, point

ani = FuncAnimation(fig, update, frames=len(df), interval=10, blit=True)
plt.show()
