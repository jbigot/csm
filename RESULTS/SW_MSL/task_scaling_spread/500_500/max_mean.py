import glob, os, string
# os.chdir("./base")
# filesearch = "base_MSL_500_500_4096_"

# os.chdir("./task2")
# filesearch = "task2MSL_500_500_1024_"

# os.chdir("./task4")
# filesearch = "task4MSL_500_500_512_"

# os.chdir("./fus")
# filesearch = "fus_MSL_500_500_2048_"

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

# os.chdir("./task2Fus")
# filesearch = "taskFus2MSL_500_500_1024_"

# mean = 0
# nb = 0
# for file in glob.glob("*.o"):
#     if file.find(filesearch)>=0 :
#         print(file)
#         with open(file) as f:
#             lines = f.readlines()
#             listVal = []
#             for line in lines :
#                 if line.find("Time TOTAL CALCULATION")>=0 :
#                     spl = string.split(line," : ")
#                     listVal.append(string.strip(spl[1],"\n"))
#             if listVal:
#                 #print(listVal)
#                 print(max(listVal))
#                 mean = mean + float(max(listVal))
#                 nb = nb + 1
# mean = mean / nb
# print("****************************")
# print("Execution time = "+str(mean))
# print("****************************")