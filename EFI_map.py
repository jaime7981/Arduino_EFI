import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors

rpms = np.array([4000,3500,3000,2500,2000,1500,1000,500])
throttle = np.array([0,0,10,20,40,60,80,100,120])
efi_map = np.array([[17.2, 16.8, 15.5, 14.8, 13.8, 13.0, 12.2],
                    [17.0, 16.5, 15.0, 14.0, 13.4, 13.0, 12.4],
                    [16.8, 16.0, 14.6, 14.2, 13.6, 13.2, 12.6],
                    [16.6, 15.8, 14.8, 14.4, 13.8, 13.4, 12.8],
                    [16.4, 15.5, 15.0, 14.6, 14.0, 13.6, 13.0],
                    [16.2, 15.6, 15.2, 14.8, 14.2, 13.8, 13.2],
                    [16.0, 15.8, 15.5, 15.1, 14.6, 14.0, 13.5]])

def ShowEFIMap():
    plt.figure(figsize = (6, 6))
    ax = plt.subplot(111)
    ax.set_ylabel("RPM")
    ax.set_xlabel("Throttle")

    plt.imshow(efi_map, cmap = "autumn")

    ax.set_xticklabels(throttle)
    ax.set_yticklabels(rpms)

    for a in range(len(efi_map)):
        for b in range(len(efi_map[a])):
            ax.text(a,b,efi_map[b,a], ha = "center", va = "center", color = "b")

    ax.set_title("EFI MAP")
    plt.colorbar()

    plt.show()

ShowEFIMap()