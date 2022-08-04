import matplotlib.pyplot as plt
import numpy as np

# Ionsophere Thickness
ym = [20, 50, 100, 200, 250]

# Max radius of Ionosphere
rm = 300

# Maximum Electron Density
Nm = 1e12

# Radial height vector
radius = np.arange(0,rm,1)

# Electron Density Vector
N = np.zeros((rm))


# Set up Plot
fig, ax = plt.subplots()
ax.set_xlabel('Electron Density ( m^-3)')
ax.set_ylabel('Height')


# Calculate EDP for each ionosphere thickness parameter
for y in ym:
    print("y is ",y)
    rb = rm - y
    for r in range(len(radius)):  
        if (rm - y) < r and (r < rm*rb/(rb-y)):
           N[r-1] = Nm*(1 - ((r-rm)*rb/(y*r))**2)
        else:
            N[r-1] = 0

    ax.plot(N,radius,label='%s' %y)
ax.legend()
plt.show()

    
    
