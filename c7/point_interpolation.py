# Short script which interpolates a set of known points and values to a structured grid
# Known locations are compared to the nearest 4 structured points in the grid and 
# weights are applied to each of the 4 nearest points based on the area of the 
# rectangle formed by the known location in one corner and each of the nearest
# structured points in the opposite corner. 
# The known value is then distributed among the 4 nearest corners proportionally based on 
# these weight values

from re import A
import matplotlib.pyplot as plt
import numpy as np


# Structured Grid Points
grid_lon = np.linspace(121,131,70)
grid_lat = np.linspace(10,16,50)
grid_val = np.zeros((70,50))

# Known Values and locations
lon = [121.39,126.19,130.27,127.42,126.14,125.96,123.15,130.5,129.08,122.74]
lat = [13.51,12.02,13.11,10.09,15.33,14,10.88,11.18,15.78,15.82]
val = [1.494,1.934,2.148,9.155,2.221,8.1,2.039,1.916,3.729,7.137]

for k in range(0,10):
    for i in range(0,69):                      
        for j in range(0,49):        
            if((grid_lon[i]<lon[k]) and (lon[k] < grid_lon[i+1])):    
                if((grid_lat[j]<lat[k]) and (lat[k] < grid_lat[j+1])):
                    
                    x1 = grid_lon[i]
                    x2 = grid_lon[i+1]
                    y1 = grid_lat[j]
                    y2 = grid_lat[j+1]

                    x0 = lon[k]
                    y0 = lat[k]

                    den = (x2 - x1)*(y2-y1)

                    w11 = (x0 - x1)*(y0-y1)/den
                    w12 = (x2 - x0)*(y0-y1)/den
                    w21 = (x0 - x1)*(y2-y0)/den
                    w22 = (x2 - x0)*(y2-y0)/den

                    grid_val[i,j] = w11*val[k]
                    grid_val[i+1,j] = w12*val[k]
                    grid_val[i,j+1] = w21*val[k]
                    grid_val[i+1,j+1] = w22*val[k]

        
fig, ax = plt.subplots()
ax.set_xlabel('longitude (deg)')
ax.set_ylabel('latitude (deg)')
ax.set_title('Known Values Interpolated to Structured Grid')


    
im = ax.imshow(np.transpose(grid_val),extent=[121,131,16,10],origin='upper')
ax.invert_yaxis()
fig.colorbar(im)
plt.show()