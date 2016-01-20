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
timesT2 = [2.32,1.05,0.54,0.3,0.18,0.14,0.16,0.22]
timesT4 = [2.8,1.32,0.68,0.37,0.23,0.14,0.13,0.22]
timesT8 = [3.06,1.48,0.77,0.41,0.23,0.14,0.12,0.078]
timesT16 = [5.7,2.9,1.49,0.8,0.47,0.3,0.24,0.3]

#without fusion spread
timesT2spread = [3.78,1.82,0.93,0.5,0.28,0.2,0.17,0.22]
timesT4spread = [4.5,2.26,1.16,0.61,0.35,0.21,0.14,0.19]
timesT8spread = [5.16,2.53,1.3,0.68,0.37,0.21,0.13,0.09]
timesT16spread = [9.3,4.8,2.37,1.24,0.69,0.39,0.26,0.25]

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
plt.loglog(cores, itsMSL, 'b-+', label='MSL MPI only', basex=2,basey=2)
plt.loglog(cores, itsT2, 'r-*', label='MSL MPI + 2 threads close', basex=2,basey=2)
plt.loglog(cores, itsT4, 'r-x', label='MSL MPI+ 4 threads close', basex=2,basey=2)
plt.loglog(cores, itsT8, 'r-o', label='MSL MPI + 8 threads close', basex=2,basey=2)
plt.loglog(cores, itsT16, 'r-+', label='MSL MPI+ 16 threads close', basex=2,basey=2)

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
plt.loglog(cores, itsMSL, 'b-+', label='MSL MPI only', basex=2,basey=2)
plt.loglog(cores, itsT2spread, 'g-*', label='MSL MPI + 2 threads spread', basex=2,basey=2)
plt.loglog(cores, itsT4spread, 'g-x', label='MSL MPI+ 4 threads spread', basex=2,basey=2)
plt.loglog(cores, itsT8spread, 'g-o', label='MSL MPI + 8 threads spread', basex=2,basey=2)
plt.loglog(cores, itsT16spread, 'g-+', label='MSL MPI+ 16 threads spread', basex=2,basey=2)

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