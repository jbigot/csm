import math
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [1,2,4,8,16,32,64,128]

#median
timesMSL = [30.5,15.86,8.53,4.83,2.35,1.04,0.54,0.3]
errminMSL = [30.1,15.85,8.23,4.81,2.34,1.04,0.53,0.297]
errmaxMSL = [30.9,15.87,8.83,4.84,2.37,1.06,0.56,0.307]

timesMSLF = [17.08,8.95,4.68,3.2,1.38,0.54,0.28,0.17]
errminMSLF = [17,8.9,4.58,3.201,1.38,0.53,0.28,0.17]
errmaxMSLF = [17.2,9.01,4.77,3.207,1.59,0.56,0.3,0.18]

itsMSL = []
itsMSLF = []

for i in timesMSL:
    itsMSL.append(500/i)
for i in timesMSLF:
    itsMSLF.append(500/i)

matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

# widths = []
# for c in cores :
# 	widths.append(math.log(c,2)*(0.5))

# print(widths)

plt1 = np.log2(cores)

p2 = plt.bar(plt1, itsMSLF, color='b', alpha=0.75,label='With fusion', align='center') #yerr=[errminMSLF,errmaxMSLF],
p1 = plt.bar(plt1, itsMSL, color='r', alpha=0.75,label='Without fusion', align='center') #yerr=[errminMSL,errmaxMSL], 

plt.xticks(plt1, ('1', '2', '4', '8', '16','32','64','128'))

legend = plt.legend(loc='upper left', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('fusVSbase.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()