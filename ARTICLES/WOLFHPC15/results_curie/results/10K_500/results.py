import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

cores = [16,32,64,128,256,512,1024]#,2048,4096]
timesMSL = [1624.07,788.81,396.5,199.55,97.92,56.55,30.4]
timesSK = [2166.63,1077.17,548.59,279.45,175.53,78.41,46.45]#,21.9,12.68]
#timesSK = [1356.31,687.42,344.25,177.30,90.74,54.27,41.81]

spdMSL = []
spdSK = []

for i in timesMSL:
    spdMSL.append(timesMSL[0]/i)
for i in timesSK:
    spdSK.append(timesSK[0]/i)
    
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