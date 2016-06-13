import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import glob, os, string, math

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [1,2,4,8,16,32,64,128,256,512,1024,2048]

COMPmean = []
COMMmean = []

COMPmedian = []
COMMmedian = []

# #---------------------------EXTRACT INFO COMP
# print("**************************** EXTRACT COMP")
# os.chdir("./comp_1")

# for c in cores:
#     filesearch = "base_MSL_500_1_"+str(c)+"_"

#     Tcomp = []

#     for file in glob.glob("*.o"):
#         if file.find(filesearch)>=0 :
#             print(file)
#             with open(file) as f:
#                 lines = f.readlines()
#                 tcomp = [0] * c
#                 for line in lines :
#                     proc = []
#                     if line.find("compute")>=0 :
#                         proc = string.split(line," compute ")
#                     if len(proc) > 0 :
#                         for p in range(0,c):
#                             if int(proc[0])==p:
#                                 if line.find("compute")>=0 :
#                                     spl = string.split(line," : ")
#                                     tcomp[p] = tcomp[p] + float(string.strip(spl[1],"\n"))
#                                     break
#                 #print(tcomp)
#                 #because the computation is performed for nothing one time
#                 Tcomp.append(max(tcomp)/2)

#     TcompArray = np.array(Tcomp).astype(np.float)

#     print("****************************")
#     print("Computation time average = "+str(np.mean(TcompArray)))
#     print("****************************")
#     COMPmean.append(np.mean(TcompArray))

#     print("****************************")
#     print("Computation time median = "+str(np.median(TcompArray)))
#     print("****************************")
#     COMPmedian.append(np.median(TcompArray))
# #---------------------------

#---------------------------EXTRACT INFO COMM
print("**************************** EXTRACT COMM")
os.chdir("./comm_1")

for c in cores:
    filesearch = "base_MSL_500_1_"+str(c)+"_"

    Tcomm = []

    for file in glob.glob("*.o"):
        if file.find(filesearch)>=0 :
            print(file)
            with open(file) as f:
                lines = f.readlines()
                tcommAll = [] #Cette liste contiendra ma map en 2D
                for i in range(c):
                    tcommAll.append([])
                tcomm = []
                for line in lines :
                    proc = []
                    if line.find("Time SYNC")>=0 :
                        proc = string.split(line," - ")
                    if len(proc) > 0 :
                        for p in range(0,c):
                            if int(proc[0])==p:
                                if line.find("Time SYNC")>=0 :
                                    spl = string.split(line," : ")
                                    tcommAll[p].append(float(string.strip(spl[1],"\n"))) #= tcomm[p] + float(string.strip(spl[1],"\n"))
                                    break

                 #because 2 sync times for nothing
                for e in tcommAll :
                    # e.remove(max(e))
                    # e.remove(max(e))
                    e.remove(e[0])
                    e.remove(e[0])
                    #print(len(e))
                    tcomm.append(sum(e))
                #print(tcomm)
                #print(len(tcomm))
                Tcomm.append(max(tcomm))

    TcommArray = np.array(Tcomm).astype(np.float)

    print("****************************")
    print("Communication time average = "+str(np.mean(TcommArray)))
    print("****************************")
    COMMmean.append(np.mean(TcommArray))

    print("****************************")
    print("Communication time median = "+str(np.median(TcommArray)))
    print("****************************")
    COMMmedian.append(np.median(TcommArray))
#---------------------------

#--------------------------- DRAW
os.chdir("../")

#old without T/2
#COMPmean = [0.274861598,0.1457109,0.0853183,0.0493435984,0.0255794,0.0130133,0.0067347,0.0035327,0.0019599,0.0011134,0.0006435,0.0004252]
#COMPmedian = [0.274327,0.145451,0.085005,0.050599492,0.0257645,0.0129415,0.0067195,0.003495,0.001961,0.001082,0.000637,0.0004252]

#new
COMPmean = [0.137430799,0.07285545,0.04265915,0.0246717992,0.0127897,0.00650665,0.00336735,0.00176635,0.00097995,0.0005567,0.00032175,0.0002126]
COMPmedian = [0.1371635,0.0727255,0.0425025,0.025299746,0.01288225,0.00647075,0.00335975,0.0017475,0.0009805,0.000541,0.0003185,0.00020825]

matplotlib.rcParams.update({'font.size': 12})
plt1 = np.log2(cores)

width = 0.3

p2 = plt.bar(plt1, COMPmedian,width, color='b', alpha=0.75,label='Computations')
p1 = plt.bar(plt1+width, COMMmedian, width, color='r', alpha=0.75,label='Communications') 

plt.xticks(plt1+width, ('1', '2', '4', '8', '16','32','64','128','256','512','1024','2048'))
plt.ylim(ymin=0.0001)

legend = plt.legend(loc='upper center', shadow=True,prop={'size':12})
plt.grid(True, which="both", linestyle="dotted")
plt.xlabel('cores')
plt.ylabel('time (s)')
plt.yscale("log", basey=2)
pp = PdfPages('times.pdf')
plt.savefig(pp, format='pdf', bbox_inches='tight')
pp.close()

plt.cla()
plt.clf()
plt.close()
#---------------------------