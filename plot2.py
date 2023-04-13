import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import FuncFormatter

# read in the first CSV file
df1 = pd.read_csv('./data/simuGBS_12d_direct.csv').iloc[2:].astype(float)

# read in the second CSV file
df2 = pd.read_csv('./data/strawberry_12d.csv').iloc[2:].astype(float)
print(df1.shape)  # prints the number of rows in df1
print(df2.shape) 
print(df2.iloc[4,0])
# create a new figure
fig, ax = plt.subplots()
x=np.arange(df1.shape[0])

# plot the second column of the first CSV file on the positive y axis
ax.plot(x, df1.iloc[:, 0], color='blue',alpha=0.8,linewidth=0.5,label='simuGBS')


ax2 = ax.twiny()
ax2.invert_yaxis()
# plot the second column of the second CSV file on the negative y axis
ax2.plot(x, -df2.iloc[:, 0], color='red',alpha=0.8,linewidth=0.5,label='Strawberry Fields')

ax.fill_between(x, df1.iloc[:, 0], where=(df1.iloc[:, 0] >= 0), interpolate=True, color='blue', alpha=0.2)
ax2.fill_between(x, -df2.iloc[:, 0], where=(-df2.iloc[:, 0] <= 0), interpolate=True, color='red', alpha=0.2)

# plot a histogram of the second column of the first CSV file
#ax.hist(df1.iloc[:, 0], bins=50, alpha=0.5, color='blue')

# plot a histogram of the second column of the second CSV file
#ax.hist(-df2.iloc[:, 0], bins=50, alpha=0.5, color='red',bottom=-ax.get_ylim()[0])
ax.set_xlim((0, len(x)))
ax.set_ylim((-0.003, 0.003))
ax.set_yticks([-0.003,-0.002,-0.001,0,0.001,0.002,0.003])
ax.grid(True,linestyle='--')
ax.spines['left'].set_position(('data', 0))
ax.spines['right'].set_position(('data', len(x)))
ax2.set_xlim((0, len(x)))
ax2.set_ylim((-0.003, 0.003))

#ax2.set_yticks([-0.003,-0.002,-0.001,0,0.001,0.002,0.003])
#ax2.grid(True)
#ax2.spines['left'].set_position(('data', 0))
#ax2.spines['right'].set_position(('data', len(x)))
# set the x-axis label
ax.set_xlabel('All possible 12 digit bit-strings')

# set the y-axis label
ax.set_ylabel('Probability')


def neg_to_pos(value, tick_number):
    return abs(value)
ax.yaxis.set_major_formatter(FuncFormatter(neg_to_pos))

# Create the first legend and position it at the top right corner of the plot
legend1 = ax.legend(loc='upper right', bbox_to_anchor=(1.0, 1.0))
ax.add_artist(legend1)  # Add the first legend to the plot

# Create the second legend and position it at the bottom right corner of the plot
#legend2 = ax.legend(loc='lower right', bbox_to_anchor=(1.0, 0.0))
#ax.add_artist(legend2)  # Add the second legend to the plot

#ax2.plot(x, np.zeros_like(x), alpha=0)
legend2 = ax2.legend(['Strawberry Fields'], loc='lower right')
ax2.add_artist(legend2)  # Add the second legend to the plot


# show the plot
plt.show()



