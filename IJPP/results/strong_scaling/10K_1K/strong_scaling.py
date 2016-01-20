import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import glob, os, string, math

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [32,64,128,256,512,1024,2048,4096,8192,16384] #1,2,4,8,

meanMSL = []
medianMSL = []
meanSK = []
medianSK = []

#---------------------------EXTRACT INFO MSL
print("**************************** EXTRACT MSL")
os.chdir("./MSL")
for c in cores:
    filesearch = "NF_MSL_10k_1k_"+str(c)+"_"

    vallist = []
    for file in glob.glob("*.o"):
        if file.find(filesearch)>=0 :
            print(file)
            with open(file) as f:
                lines = f.readlines()
                listVal = []
                for line in lines :
                    if line.find("Time TOTAL CALCULATION")>=0 :
                        spl = string.split(line," : ")
                        listVal.append(string.strip(spl[1],"\n"))
                if listVal:
                    print(max(listVal))
                    vallist.append(max(listVal))
    medianarray = np.array(vallist).astype(np.float)
    median = np.median(medianarray)
    mean = np.mean(medianarray)
    print("****************************")
    print("Execution time average = "+str(mean))
    print("****************************")
    meanMSL.append(median)

    print("****************************")
    print("Execution time median = "+str(median))
    print("****************************")
    medianMSL.append(median)
#---------------------------

#---------------------------EXTRACT INFO SKMPI
print("**************************** EXTRACT SKMPI")
os.chdir("../SKMPI")
for c in cores:
    filesearch = "strongSK_10k_1k_"+str(c)+"_"

    vallist = []
    for file in glob.glob("*.o"):
        if file.find(filesearch)>=0 :
            print(file)
            with open(file) as f:
                lines = f.readlines()
                listVal = []
                for line in lines :
                    if line.find("Time TOTAL CALCULATION")>=0 :
                        spl = string.split(line," : ")
                        listVal.append(string.strip(spl[2],"\n"))
                if listVal:
                    print(max(listVal))
                    vallist.append(max(listVal))
    medianarray = np.array(vallist).astype(np.float)
    median = np.median(medianarray)
    mean = np.mean(medianarray)
    print("****************************")
    print("Execution time average = "+str(mean))
    print("****************************")
    meanSK.append(mean)

    print("****************************")
    print("Execution time median = "+str(median))
    print("****************************")
    medianSK.append(median)
#---------------------------

print("Median MSL = "+str(medianMSL))
print("Mean MSL = "+str(meanMSL))

os.chdir("../")
matplotlib.rcParams.update({'font.size': 16})

#--------------------------- DRAW RESULTS MEAN
itsMSL = []
itsSK = []

for i in meanMSL:
    itsMSL.append(500/i)
for i in meanSK:
    itsSK.append(500/i)
    
relatives_its = []
for i in cores:
    relatives_its.append(500/(meanSK[0]/(i/cores[0])))

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(meanMSL[0]/(i/cores[0])))

#plt.xkcd()

#plt.loglog(cores, relatives_its, 'r--', label='Ideal', basex=2,basey=2)
plt.loglog(cores, relatives_its2, 'b--', label='Ideal', basex=2,basey=2)
plt.loglog(cores, itsMSL, 'b-+', label='SkelGIS + MSL', basex=2,basey=2)
plt.loglog(cores, itsSK, 'r-x', label='SkelGIS + sync', basex=2,basey=2)
legend = plt.legend(loc='upper left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('mean_strong.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()
#--------------------------- 

#--------------------------- DRAW RESULTS MEDIAN
itsMSL = []
itsSK = []

for i in medianMSL:
    itsMSL.append(500/i)
for i in medianSK:
    itsSK.append(500/i)
    
relatives_its = []
for i in cores:
    relatives_its.append(500/(medianSK[0]/(i/cores[0])))

relatives_its2 = []
for i in cores:
    relatives_its2.append(500/(medianMSL[0]/(i/cores[0])))

#plt.xkcd()

#plt.loglog(cores, relatives_its, 'r--', label='Ideal', basex=2,basey=2)
plt.loglog(cores, relatives_its2, 'b--', label='Ideal', basex=2,basey=2)
plt.loglog(cores, itsMSL, 'b-+', label='SkelGIS + MSL', basex=2,basey=2)
plt.loglog(cores, itsSK, 'r-x', label='SkelGIS + sync', basex=2,basey=2)
legend = plt.legend(loc='upper left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('iterations per second')
pp = PdfPages('median_strong.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()
#---------------------------
