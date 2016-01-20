import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048]

# without fusion close
timesMSL = [6.08,2.69,1.38,0.74,0.45,0.34,0.35,0.56]
timesT2 = [6.02,3.1,1.38,0.87,0.49,0.31,0.26,0.31]
6.07,2.82,1.53,0.84,0.75,1
timesT4 = [8.28,3.92,2,1.07,0.61,0.36,0.28,0.43]

#without fusion spread
timesT2spread = [9.69,4.68,2.38,1.26,0.69,0.42,0.31,0.31]
timesT4spread = [11.8,5.85,2.97,1.56,0.86,0.5,0.3,0.36]

itsMSL = []
itsT2 = []
itsT4 = []
itsT2spread = []
itsT4spread = []

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

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

#plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL MPI', basex=2,basey=2)
plt.loglog(cores, itsT2, 'r-*', label='MSL MPI + 2 threads close', basex=2,basey=2)
plt.loglog(cores, itsT4, 'r-x', label='MSL MPI+ 4 threads close', basex=2,basey=2)
plt.loglog(cores, itsT2spread, 'g-*', label='MSL MPI + 2 threads spread', basex=2,basey=2)
plt.loglog(cores, itsT4spread, 'g-x', label='MSL MPI+ 4 threads spread', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('itpersecWF.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()