# This script reads in two data files, assigns the data to a structured grid, and calculates their 2D convolution

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LogNorm
from scipy import signal
from scipy import misc


# Load Data Files
data_0 = np.loadtxt("convolve_raw_0.dat",delimiter='\t')
data_1 = np.loadtxt("convolve_raw_1.dat",delimiter='\t')

# Count Length in each
line_count_0 = len(data_0)
line_count_1 = len(data_1)


# Import data to named arrays
x0 = data_0[:,0]
y0 = data_0[:,1]
z0 = data_0[:,2]

x1 = data_1[:,0]
y1 = data_1[:,1]
z1 = data_1[:,2]

# Empty variables to hold overlapping values
xs = []
ys = []
zs0 = []
zs1 = []

# Determine the x,y data that overlaps and store z data from each file
line_overlap = 0
for i in range(0,line_count_0):
    for j in range(0,line_count_1):
        if x0[i] == x1[j] and y0[i] == y1[j]:
            xs.append(x0[i])
            ys.append(y0[i])
            zs0.append(z0[i])
            zs1.append(z1[i])
            line_overlap += 1

# Extract the extrema from the overlapping data bounds
min_x = np.min(xs)
max_x = np.max(xs)
min_y = np.min(ys)
max_y = np.max(ys)

# Spatial resolution of x and y data
dx = .04
dy = 1/3000

# Determine the range of x and y data that covers the overlapping data
M = round((max_x-min_x)/dx)+1
N = round((max_y-min_y)/dy)+1


x_range = np.zeros(M)
y_range = np.zeros(N)


# Set up linear arrays which span the overlapping data spatially
x_range[0] = min_x
y_range[0] = min_y


for i in range(1,M):
    x_range[i] = x_range[i-1] + dx

for i in range(1,N):
    y_range[i] = y_range[i-1] + dy

x_range = np.round(x_range,6)
y_range = np.round(y_range,6)


# 2D arrays which z data will be inserted into if x,y positions match
f_pad = np.zeros((M,N))
g_pad = np.zeros((M,N))

for i in range(0,M):
    for j in range(0,N):
        for k in range(0,line_overlap):
            if x_range[i] == xs[k] and y_range[j] == ys[k]:
                f_pad[i,j] = (zs0[k])
                g_pad[i,j] = (zs1[k])

# Transpose second data set for convolution 
g_pad = np.transpose(g_pad)

# Determine minimum size needed for 2D correlation
O = M+N-1

# Padded arrays to account for boundary issues
f = np.zeros((O,O))
g = np.zeros((O,O))
f[0:M,0:N] = f_pad
g[0:N,0:M] = g_pad

# 2D convolution array
convolution = np.zeros((O,O))


# Compute 2D convolution
for i in range(0,O):
    for j in range(0,O):
        sum = 0
        for m in range(0,M-1):
            for n in range(0,N-1):
                sum = sum + f[m,n]*g[i-m,j-n]
        convolution[i,j] = sum

# Alternate second array for use in scipy convolution comparison
g_alt = np.zeros((O,O))
g_alt[0:M,0:N] = np.transpose(g_pad)

# Comparison 2D convolution
scipy_convolution = signal.convolve2d(f,g,boundary='fill',mode='full')


# Visualization for my 2D convolution
fig8, ax8 = plt.subplots()
ax8.set_xlabel('x')
ax8.set_ylabel('y')
ax8.set_title('My 2D convolution')
im8 = ax8.imshow(np.transpose(convolution[0:M,0:3]),extent=[-2.8,2.8,0.000666,0],aspect="auto",interpolation='none')
ax8.invert_yaxis()
fig8.colorbar(im8,ax=ax8)
fig8.savefig('my_2d_convolution.png')

# Visualization for scipy 2D convolution
fig9, ax9 = plt.subplots()
ax9.set_xlabel('x')
ax9.set_ylabel('y')
ax9.set_title('Scipy 2D Convolution')
im9 = ax9.imshow(np.transpose(scipy_convolution[0:M,58:83]),interpolation='none',aspect="auto")#,norm=LogNorm(),extent=[-2.8,2.8,0.000666,0])
ax9.invert_yaxis()
fig9.colorbar(im9,ax=ax9)
fig9.savefig('scipy_convolution.png')

###############
# Visualizations for individual data sets and their over lapping regions
###############

fig1, ax1 = plt.subplots()
ax1.set_xlabel('x')
ax1.set_ylabel('y')
ax1.set_title('data 0')
im1 = ax1.scatter(x0,y0,c=z0,cmap='viridis',norm=LogNorm())
fig1.colorbar(im1,label='data 0')
fig1.savefig('data_0_scatter.png')


fig2, ax2 = plt.subplots()
ax2.set_xlabel('x')
ax2.set_ylabel('y')
ax2.set_title('data 1')
im2 = ax2.scatter(x1,y1,c=z1,cmap='viridis',norm=LogNorm())
fig2.colorbar(im2,label='data 1')
fig2.savefig('data_1_scatter.png')

fig3, ax3 = plt.subplots()
ax3.set_xlabel('x')
ax3.set_ylabel('y')
ax3.set_title('Overlaid Data')
im3 = ax3.scatter(x1,y1,c=z1,cmap='viridis',norm=LogNorm())
ax3.scatter(x0,y0,c=z0,alpha=.1,cmap='plasma')
#fig3.colorbar(im3,label='overlap')
fig3.savefig('overlaid_data_scatter.png')


fig4, ax4 = plt.subplots()
ax4.set_xlabel('x')
ax4.set_ylabel('y')
ax4.set_title('Overlaid region of Data 0')
im4 = ax4.scatter(xs,ys,c=np.log10(zs0),cmap='viridis')
fig4.colorbar(im4,label='log10(data_0)')
fig4.savefig('overlaid_from_data_0_scatter.png')

fig5, ax5 = plt.subplots()
ax5.set_xlabel('x')
ax5.set_ylabel('y')
ax5.set_title('Overlaid region of Data 1')
im5 = ax5.scatter(xs,ys,c=np.log10(zs1),cmap='viridis')
fig5.colorbar(im5,label='log10(data_1)')
fig5.savefig('overlaid_from_data_1_scatter.png')

fig6, ax6 = plt.subplots()
ax6.set_xlabel('x')
ax6.set_ylabel('y')
ax6.set_title('Data 0 overlap in full 2D Grid')
im6 = ax6.imshow(np.transpose(f_pad),extent=[-2.8,2.8,0.000666,0],norm=LogNorm(),aspect="auto",interpolation='none')
ax6.invert_yaxis()
fig6.colorbar(im6,ax=ax6,label='data 0')
fig6.savefig('data_0_overlap.png')

fig7, ax7 = plt.subplots()
ax7.set_xlabel('x')
ax7.set_ylabel('y')
ax7.set_title('Data 1 overlap in full 2D Grid')
im7 = ax7.imshow((g_pad),extent=[-2.8,2.8,0.000666,0],norm=LogNorm(),aspect="auto",interpolation='none')
ax7.invert_yaxis()
fig7.colorbar(im7,ax=ax7,label='data 1')
fig7.savefig('data_1_overlap.png')

#plt.show()