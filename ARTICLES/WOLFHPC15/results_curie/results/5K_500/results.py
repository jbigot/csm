import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

cores = [8,16,32,64,128,256,512]
timesMSL = [818.69,404.02,197.106,98.95,49.07,29.44,19.88,]
timesSK = [1073.31,542.26,273.65,138.87,80.11,44.32,19.69]#,11.15,5.75,3.77]
#timesSK = [674.75,342.73,173.31,87.64,47.76,36.45,21.15]#,14.18,18.67,17,03]

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