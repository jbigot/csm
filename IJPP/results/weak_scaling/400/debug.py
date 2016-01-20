import numpy as np
import glob, os, string
os.chdir("./test")
filesearch = "weakMSL_25600_500_4096_"

vallist = []
for file in glob.glob("*.o"):
    if file.find(filesearch)>=0 :
        print(file)
        with open(file) as f:
            lines = f.readlines()
            listVal = []
            for line in lines :
                if line.find("Time SYNC")>=0 :
                    spl = string.split(line," : ")
                    if len(spl)==2:
                        point = string.split(spl[1],".")
                        if len(point)==2:
                            listVal.append(string.strip(spl[1],"\n"))
            if listVal:
                print("****************************")
                print("Execution sync min = "+str(min(listVal)))
                print("****************************")

                print("****************************")
                print("Execution sync max = "+str(max(listVal)))
                print("****************************")