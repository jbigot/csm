import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import glob, os, string, math

from matplotlib import rc
matplotlib.rc('pdf', fonttype=42)

cores = [16]#[2,4,8,16,32,64,128,256]

Tcrmean = []
Tsyncmedian = []

#---------------------------EXTRACT create thread
# print("**************************** EXTRACT create threads")
# os.chdir("./thread_create/task8")

# for c in cores:
#     filesearch = "task8MSL_500_200_"+str(c)+"_"

#     Tcomm = []

#     for file in glob.glob("*.o"):
#         if file.find(filesearch)>=0 :
#             print(file)
#             with open(file) as f:
#                 lines = f.readlines()
#                 tcommAll = [] #Cette liste contiendra ma map en 2D
#                 for i in range(c):
#                     tcommAll.append([])
#                 tcomm = []
#                 for line in lines :
#                     proc = []
#                     if line.find("Thread create")>=0 :
#                         proc = string.split(line," ")
#                     if len(proc) > 0 :
#                         for p in range(0,c):
#                             if int(proc[0])==p:
#                                 if line.find("Thread create")>=0 :
#                                     spl = string.split(line," : ")
#                                     tcommAll[p].append(float(string.strip(spl[1],"\n")))
#                                     break

#                  #because 2 sync times for nothing
#                 for e in tcommAll :
#                     e.remove(max(e))
#                     e.remove(max(e))
#                     tcomm.append(sum(e))
#                 #print(tcomm)
#                 Tcomm.append(max(tcomm))

#     TcommArray = np.array(Tcomm).astype(np.float)

#     print("****************************")
#     print("Create threads time median = "+str(np.median(TcommArray)))
#     print("****************************")
#     Tcrmean.append(np.median(TcommArray))
#---------------------------

#--------------------------- EXTRACT sync threads
print("**************************** EXTRACT sync threads")
os.chdir("./thread_sync/task8")

for c in cores:
    filesearch = "task8MSL_500_1_"+str(c)+"_"

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
                    if line.find("Thread sync")>=0 :
                        proc = string.split(line," ")
                    if len(proc) > 0 :
                        try:
                            for p in range(0,c):
                                if int(proc[0])==p:
                                    if line.find("Thread sync")>=0 :
                                        spl = string.split(line," : ")
                                        tcommAll[p].append(float(string.strip(spl[1],"\n")))
                                        break
                        except:
                            print "error next line"

                for e in tcommAll :
                    # e.remove(min(e))
                    # e.remove(min(e))
                    tcomm.append(sum(e))
                print(tcomm)
                #because 2 sync times for nothing
                Tcomm.append(min(tcomm)/2)

    TcommArray = np.array(Tcomm).astype(np.float)

    print("****************************")
    print("Sync threads time median = "+str(np.median(TcommArray)))
    print("****************************")
    Tsyncmedian.append(np.median(TcommArray))
#---------------------------