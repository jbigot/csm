import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [8,16,32,64,128,256,512,1024,2048]

timesMSL = [814.277,410.728,209.784,104.898,51.7,26.64,13.84,6.46,2.94]
timesSK = [808.604,408.767,214.941,105.926,64.958,34.97,15.6,7.57,2.99] #SK+MPI

spdMSL = []
spdSK = []

itsMSL = []
itsSK = []

for i in timesMSL:
    spdMSL.append(timesSK[0]/i)
    itsMSL.append(500/i)
for i in timesSK:
    spdSK.append(timesSK[0]/i)
    itsSK.append(500/i)
    
relatives = []
relatives_its = []
for i in cores:
    relatives.append(i/cores[0])
    relatives_its.append(500/(timesSK[0]/(i/cores[0])))

relatives2 = []
relatives_its2 = []
for i in cores:
    relatives2.append(i/cores[0])
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

# plt.plot(cores, relatives, 'g--', label='Ideal')
# plt.plot(cores, spdMSL, 'b-+', label='MSL NF')
# plt.plot(cores, spdSK, 'r-x', label='SkelGIS NF')
# legend = plt.legend(loc='upper left', shadow=True)
# plt.grid(True, which="both", linestyle="dotted")
# plt.xlabel('cores')
# plt.ylabel('speedup')
# pp = PdfPages('speedups_NF.pdf')
# plt.savefig(pp, format='pdf', bbox_inches='tight')
# pp.close()

# plt.cla()
# plt.clf()
# plt.close()

#plt.xkcd()

plt.plot(cores, relatives_its, 'r--', label='Ideal')
plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.plot(cores, itsMSL, 'b-+', label='MSL NF')
plt.plot(cores, itsSK, 'r-x', label='SkelGIS NF')
legend = plt.legend(loc='upper left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('itpersec_NF.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()

#plt.xkcd()

plt.loglog(cores, timesMSL,'b-+',label='MSL NF', basex=2,basey=2)
plt.loglog(cores, timesSK,'r-x',label='SkelGIS NF', basex=2,basey=2)
legend = plt.legend(shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('log cores')
plt.ylabel('log execution time')
pp2 = PdfPages('logtimes_NF.pdf')
plt.savefig(pp2, format='pdf', bbox_inches='tight')
pp2.close()

plt.cla()
plt.clf()
plt.close()