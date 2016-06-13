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
timesT8Old = [3.06,1.48,0.77,0.41,0.23,0.14,0.12,0.078]
timesT8 = [2.95,1.43,0.74,0.39,0.22,0.13,0.2,0.11]

itsMSL = []
itsT8Old = []
itsT8 = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesT8:
	if i!=0:
		itsT8.append(500/i)
	else :
		itsT8.append(0)
for i in timesT8Old:
	if i!=0:
		itsT8Old.append(500/i)
	else :
		itsT8Old.append(0)

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL data parallelization only', basex=2,basey=2)
plt.loglog(cores, itsT8, 'r-+', label='MSL using 8 cores for tasks dynamic', basex=2,basey=2)
plt.loglog(cores, itsT8Old, 'g-+', label='MSL using 8 cores for tasks static', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('task8_close_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()