import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048]

# median 10 runs
# without fusion and with OpenMP for
timesMSL = [2.35,1.05,0.54,0.3,0.2,0.19,0.24,0.49]
timesT2 = [1.83,0.88,0.47,0.27,0.17,0.11,0.088,0.077]
timesT4 = [1.57,0.77,0.42,0.25,0.16,0.12,0.1,0.086]
timesT8 = [1.49,0.72,0.47,0.28,0.21,0.15,0.13,0.1]
timesT16 = [2.77,1.64,0.96,0.67,0.45,0.35,0.29,0.3]

itsMSL = []
itsT2 = []
itsT4 = []
itsT8 = []
itsT16 = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesT2:
	if i!=0:
		itsT2.append(500/i)
	else :
		itsT2.append(0)
for i in timesT4:
	if i!=0:
		itsT4.append(500/i)
	else :
		itsT4.append(0)
for i in timesT8:
	if i!=0:
		itsT8.append(500/i)
	else :
		itsT8.append(0)
for i in timesT16:
	if i!=0:
		itsT16.append(500/i)
	else :
		itsT16.append(0)

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL data parallelization only', basex=2,basey=2)
plt.loglog(cores, itsT2, 'c-*', label='MSL using 2 cores for ompfor', basex=2,basey=2)
plt.loglog(cores, itsT4, 'g-x', label='MSL using 4 cores for ompfor', basex=2,basey=2)
plt.loglog(cores, itsT8, 'm-o', label='MSL using 8 cores for ompfor', basex=2,basey=2)
plt.loglog(cores, itsT16, 'r-+', label='MSL using 16 cores for ompfor', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('ompfor_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()
