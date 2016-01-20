import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

cores = [256,512,1024,2048,4096,8192,16384]
timesMSL = [979.18,565,]
timesSK = [1472.16,783.46,396.73,203.97,107.81,58.68,31.95]
timesSK3 = []

spdMSL = []
spdSK = []

for i in timesMSL:
    spdMSL.append(timesMSL[0]/i)
for i in timesSK:
    spdSK.append(timesMSL[0]/i)
    
relatives = []
for i in cores:
    relatives.append(i/cores[0])

plt.plot(cores, relatives, 'g--', label='Ideal')
plt.plot(cores, spdMSL, 'b-+', label='MSL')
plt.plot(cores, spdSK, 'r-x', label='SkelGIS')
legend = plt.legend(loc='upper left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('speedup')
pp = PdfPages('speedups.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()

plt.loglog(cores, timesMSL,'b-+',label='MSL', basex=2,basey=2)
plt.loglog(cores, timesSK,'r-x',label='SkelGIS', basex=2,basey=2)
legend = plt.legend(shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('log cores')
plt.ylabel('log execution time')
pp2 = PdfPages('logtimes.pdf')
plt.savefig(pp2, format='pdf', bbox_inches='tight')
pp2.close()

plt.cla()
plt.clf()
plt.close()