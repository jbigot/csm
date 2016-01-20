import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128]

timesMSL = [2.35,1.05,0.54,0.3]
timesMSLF = [1.42,0.54,0.28,0.17]

itsMSL = []
itsMSLF = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesMSLF:
    itsMSLF.append(500/i)

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

plt.hist(cores, itsMSL, normed=1, histtype='bar', stacked=True)
plt.hist(cores, itsMSLF, normed=1, histtype='bar', stacked=True)

#legend = plt.legend(loc='lower right', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('fusVSbase.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()