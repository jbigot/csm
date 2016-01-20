import glob, os, string
os.chdir("./MSL")
filesearch = "NF_MSL_10k_1k_8192_"

# os.chdir("./SKMPI")
# filesearch = "strongSK_10k_1k_16384_"

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