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
timesT16Old = [5.7,2.9,1.49,0.8,0.47,0.3,0.24,0.3]
timesT16 = [6.1,3.01,1.45,0.78,0.44,0.27,0.19,0.18]

itsMSL = []
itsT16Old = []
itsT16 = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesT16:
	if i!=0:
		itsT16.append(500/i)
	else :
		itsT16.append(0)
for i in timesT16Old:
	if i!=0:
		itsT16Old.append(500/i)
	else :
		itsT16Old.append(0)

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL data parallelization only', basex=2,basey=2)
plt.loglog(cores, itsT16, 'r-+', label='MSL using 16 cores for tasks dynamic', basex=2,basey=2)
plt.loglog(cores, itsT16Old, 'g-+', label='MSL using 16 cores for tasks static', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('task16_close_median.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()