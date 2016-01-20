import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import glob, os, string, math

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16,32,64,128,256,512,1024,2048,4096,8192,16384] #1,2,4,8,
sizes = [1600,1600,3200,3200,6400,6400,12800,12800,25600,25600,51200] #400,400,800,800,

meanMSL = []
medianMSL = []
timesMSLerrmin=[]
timesMSLerrmax=[]
meanSK = []
medianSK = []
timesSKerrmin=[]
timesSKerrmax=[]

#---------------------------EXTRACT INFO MSL
print("**************************** EXTRACT MSL")
os.chdir("./MSL")
for c,s in zip(cores,sizes):
    filesearch = "weakMSL_"+str(s)+"_500_"+str(c)+"_"

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

    print("****************************")
    print("Execution time min = "+str(min(vallist)))
    print("****************************")
    timesMSLerrmin.append(math.fabs(float(min(vallist))-median))

    print("****************************")
    print("Execution time max = "+str(max(vallist)))
    print("****************************")
    timesMSLerrmax.append(math.fabs(float(max(vallist))-median))
#---------------------------

#---------------------------EXTRACT INFO SKMPI
print("**************************** EXTRACT SKMPI")
os.chdir("../SKMPI")
for c,s in zip(cores,sizes):
    filesearch = "weakSK_"+str(s)+"_500_"+str(c)+"_"

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

    print("****************************")
    print("Execution time min = "+str(min(vallist)))
    print("****************************")
    timesSKerrmin.append(math.fabs(float(min(vallist))-median))

    print("****************************")
    print("Execution time max = "+str(max(vallist)))
    print("****************************")
    timesSKerrmax.append(math.fabs(float(max(vallist))-median))
#---------------------------

#--------------------------- DRAW RESULTS MEAN
os.chdir("../")
matplotlib.rcParams.update({'font.size': 16})

#plt.xkcd()

#plt.loglog(cores, timesMSL,'b-+',label='MSL', basex=2,basey=2)
#plt.loglog(cores, timesSK,'r-+',label='MPI', basex=2,basey=2,yerr=timesSKerr)

# print(timesSKerrmin)
# print(timesSKerrmax)
# print(timesSK)

# print(timesMSLerrmin)
# print(timesMSLerrmax)
print("Median MSL = "+str(medianMSL))
print("Mean MSL = "+str(meanMSL))

plt.xscale("log", basex=2)
#plt.yscale("log", basey=2)
#plt.xlim([float(min(cores)),float(max(cores))+5000])
plt.ylim([0,float(max(medianMSL))+float(max(timesMSLerrmax))+10])
plt.errorbar(cores, medianMSL, yerr=[np.array(timesMSLerrmin).astype(np.float),np.array(timesMSLerrmax).astype(np.float)], fmt='b-+',label='SkelGIS + MSL')
plt.errorbar(cores, medianSK, yerr=[np.array(timesSKerrmin).astype(np.float),np.array(timesSKerrmax).astype(np.float)], fmt='r-+',label='SkelGIS + sync')

legend = plt.legend(loc='lower left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('time (s)')

#plt.show()

pp = PdfPages('median_weak.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()
#--------------------------- 

#--------------------------- DRAW RESULTS MEDIAN
print("Median SK = "+str(medianSK))
print("Mean SK = "+str(meanSK))

plt.xscale("log", basex=2)
#plt.yscale("log", basey=2)
#plt.xlim([float(min(cores)),float(max(cores))+5000])
plt.ylim([0,float(max(meanMSL))+float(max(timesMSLerrmax))])
plt.errorbar(cores, meanMSL, yerr=[np.array(timesMSLerrmin).astype(np.float),np.array(timesMSLerrmax).astype(np.float)], fmt='b-+',label='SkelGIS + MSL')
plt.errorbar(cores, meanSK, yerr=[np.array(timesSKerrmin).astype(np.float),np.array(timesSKerrmax).astype(np.float)], fmt='r-+',label='SkelGIS + sync')

legend = plt.legend(loc='lower left', shadow=True)
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('time (s)')

#plt.show()

pp = PdfPages('mean_weak.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()
#---------------------------

