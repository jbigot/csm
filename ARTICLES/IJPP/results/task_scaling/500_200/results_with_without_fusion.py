import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048]

# median 10 runs
# without fusion close
timesMSL = [2.35,1.05,0.54,0.3,0.2,0.19,0.24,0.49]
timesMSLfus = [1.38,0.54,0.28,0.17,0.148,0.21,0.57,2.64]

timesT2 = [2.32,1.05,0.54,0.3,0.18,0.14,0.16,0.22]
timesT8 = [3.06,1.48,0.77,0.41,0.23,0.14,0.12,0.078]

#with fusion close
timesT2fus = [1.44,0.57,0.29,0.17,0.115,0.236,1.05,5.98]
timesT8fus = [4.61,2.294,1.177,0.62,0.3,0.18,0.133,0.283]

itsMSL = []
itsMSLfus = []
itsT2 = []
itsT8 = []
itsT2fus = []
itsT8fus = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesMSLfus:
    itsMSLfus.append(500/i)
for i in timesT2:
	if i!=0:
		itsT2.append(500/i)
	else :
		itsT2.append(0)
for i in timesT8:
	if i!=0:
		itsT8.append(500/i)
	else :
		itsT8.append(0)

for i in timesT2fus:
	if i!=0:
		itsT2fus.append(500/i)
	else :
		itsT2fus.append(0)
for i in timesT8fus:
	if i!=0:
		itsT8fus.append(500/i)
	else :
		itsT8fus.append(0)

#relatives_its2 = []
#for i in cores:
#    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

#plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-o', label='Data parallelism without fusion', basex=2,basey=2)
plt.loglog(cores, itsMSLfus, 'b-x', label='Data parallelism with fusion', basex=2,basey=2)
#plt.loglog(cores, itsT2, 'r-*', label='Hybrid 2 cores for tasks without fusion', basex=2,basey=2)
plt.loglog(cores, itsT8, 'r-*', label='Hybrid 8 cores for tasks without fusion', basex=2,basey=2)
#plt.loglog(cores, itsT2fus, 'r-+', label='Hybrid 2 cores for tasks with fusion', basex=2,basey=2)
plt.loglog(cores, itsT8fus, 'r-+', label='Hybrid 8 cores for tasks with fusion', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('withwithout8_close_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()