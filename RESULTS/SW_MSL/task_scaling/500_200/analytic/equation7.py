import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import glob, os, string, math

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048]

#new
first = [0.0005,0.0018,0.0013,0.00075,0.00077,0.0029,0.018,0.0623]
second = [0.0133,0.0066,0.0037,0.0028,0.0028,0.0018,0.001,0.0009]

matplotlib.rcParams.update({'font.size': 12})
plt1 = np.log2(cores)

width = 0.3

p2 = plt.bar(plt1, first,width, color='b', alpha=0.75,label='Tcom1')
p1 = plt.bar(plt1+width, second, width, color='r', alpha=0.75,label='Tcom2 + Ttask') 

plt.xticks(plt1+width, ('16','32','64','128','256','512','1024','2048'))

legend = plt.legend(loc='upper center', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('time (s)')
plt.yscale("log", basey=2)
pp = PdfPages('tth.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()