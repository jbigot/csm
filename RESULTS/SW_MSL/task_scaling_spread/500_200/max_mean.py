import numpy as np
import glob, os, string

os.chdir("./task16")
filesearch = "task16MSL_500_200_128_"


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

print("****************************")
print("Execution time median = "+str(median))
print("****************************")

print("****************************")
print("Execution time min = "+str(min(vallist)))
print("****************************")

print("****************************")
print("Execution time max = "+str(max(vallist)))
print("****************************")