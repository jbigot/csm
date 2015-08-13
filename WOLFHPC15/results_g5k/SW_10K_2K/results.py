import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

cores = [16,32,64,128,256]
timesMSL = [9035.94,4519.9,2390.61,1300.08,693.294]
timesSK = [13692.4,6946.07,3235.88,1861.24,892.882]

spdMSL = []
spdSK = []

itsMSL = []
itsSK = []

for i in timesMSL:
    spdMSL.append(timesMSL[0]/i)
    itsMSL.append(500/i)
for i in timesSK:
    spdSK.append(timesMSL[0]/i)
    itsSK.append(500/i)
    
relatives = []
relatives_its = []
for i in cores:
    relatives.append(i/cores[0])
    relatives_its.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

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

#plt.xkcd()

plt.plot(cores, relatives_its, 'g--', label='Ideal')
plt.plot(cores, itsMSL, 'b-+', label='MSL')
plt.plot(cores, itsSK, 'r-x', label='SkelGIS')
legend = plt.legend(loc='upper left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('itpersec.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()

#plt.xkcd()

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