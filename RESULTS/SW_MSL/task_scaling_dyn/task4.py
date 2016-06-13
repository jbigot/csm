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
timesT4Old = [2.8,1.32,0.68,0.37,0.23,0.14,0.13,0.22]
timesT4 = [2.57,1.25,0.64,0.34,0.19,0.11,0.072,0.19]

itsMSL = []
itsT4Old = []
itsT4 = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesT4:
	if i!=0:
		itsT4.append(500/i)
	else :
		itsT4.append(0)
for i in timesT4Old:
	if i!=0:
		itsT4Old.append(500/i)
	else :
		itsT4Old.append(0)

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL data parallelization only', basex=2,basey=2)
plt.loglog(cores, itsT4, 'r-+', label='MSL using 4 cores for tasks dynamic', basex=2,basey=2)
plt.loglog(cores, itsT4Old, 'g-+', label='MSL using 4 cores for tasks static', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('task4_close_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()