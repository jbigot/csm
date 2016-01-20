import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048]

# fusion close
timesMSL = [1.42,0.54,0.28,0.17,0.15,0.19,0.54,2.54]
timesT2 = [1.45,0.57,0.29,0.18,0.12,0.23,1.02,6.15]

# fusion spread
timesT2spread = [2.27,0.96,0.5,0.28,0.17,0.26,1.02,5.8]

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
for i in timesT2spread:
	if i!=0:
		itsT2spread.append(500/i)
	else :
		itsT2spread.append(0)

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(timesMSL[0]/(i/cores[0])))

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

#plt.plot(cores, relatives_its2, 'b--', label='Ideal')
plt.loglog(cores, itsMSL, 'b-+', label='MSL MPI', basex=2,basey=2)
plt.loglog(cores, itsT2, 'r-*', label='MSL MPI + 2 threads close', basex=2,basey=2)
plt.loglog(cores, itsT2spread, 'g-*', label='MSL MPI + 2 threads spread', basex=2,basey=2)

legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('itpersecF.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()