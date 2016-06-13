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
timesT2 = [2.28,1.04,0.53,0.29,0.16,0.10,0.093,0.09]
timesT4 = [2.57,1.25,0.64,0.34,0.19,0.11,0.072,0.19]
#timesT8 = [2.95,1.43,0.74,0.39,0.22,0.13,0.2,0.11] #with all limits
timesT8 = [2.95,1.43,0.74,0.39,0.22,0.13,0.16,0.09] #with same limits than base
timesT16 = [6.1,3.01,1.45,0.78,0.44,0.27,0.19,0.18]

timesT2spread = [3.71,1.81,0.92,0.49,0.26,0.15,0.16,0.11]
timesT4spread = [4.33,2.19,1.12,0.59,0.31,0.18,0.1,0.21]
timesT8spread = [5.04,2.51,1.29,0.67,0.36,0.2,0.18,0.11]
timesT16spread = [9.52,4.83,2.39,1.25,0.67,0.38,0.23,0.19]

itsMSL = []
itsT2 = []
itsT4 = []
itsT8 = []
itsT16 = []
itsT2spread = []
itsT4spread = []
itsT8spread = []
itsT16spread = []

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


for i in timesT2spread:
	if i!=0:
		itsT2spread.append(500/i)
	else :
		itsT2spread.append(0)
for i in timesT4spread:
	if i!=0:
		itsT4spread.append(500/i)
	else :
		itsT4spread.append(0)
for i in timesT8spread:
	if i!=0:
		itsT8spread.append(500/i)
	else :
		itsT8spread.append(0)
for i in timesT16spread:
	if i!=0:
		itsT16spread.append(500/i)
	else :
		itsT16spread.append(0)

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL data parallelization only', basex=2,basey=2)
plt.loglog(cores, itsT2, 'c-*', label='MSL using 2 cores for tasks', basex=2,basey=2)
plt.loglog(cores, itsT4, 'g-x', label='MSL using 4 cores for tasks', basex=2,basey=2)
plt.loglog(cores, itsT8, 'm-o', label='MSL using 8 cores for tasks', basex=2,basey=2)
plt.loglog(cores, itsT16, 'r-+', label='MSL using 16 cores for tasks', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('base_close_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()

plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL data parallelization only', basex=2,basey=2)
plt.loglog(cores, itsT2spread, 'c-*', label='MSL using 2 cores for tasks', basex=2,basey=2)
plt.loglog(cores, itsT4spread, 'g-x', label='MSL using 4 cores for tasks', basex=2,basey=2)
plt.loglog(cores, itsT8spread, 'm-o', label='MSL using 8 cores for tasks', basex=2,basey=2)
plt.loglog(cores, itsT16spread, 'r-+', label='MSL using 16 cores for tasks', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('base_spread_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()