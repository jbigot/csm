import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048]

# fusion close
timesMSL = [3.57,1.41,0.72,0.43,0.31,0.31,0.74,2.43]
timesT2 = [3.74,1.45,0.74,0.43,0.27,0.4,1.06,6.05]

# fusion spread
timesT2spread = [5.83,2.46,1.27,0.7,0.42,0.57,1.23,6.27]

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