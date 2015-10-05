import sys
import os
sys.path.append(os.path.dirname("./utils/"))
import copy
from string import Template

nb_proc = 2048
size_x=5000
size_y=5000
iteration=10

tempProcF = open("template_process.xml",'r')
tempCommF = open("template_comm.xml",'r')

tempProc = Template(tempProcF.read())

ladFile = open("./outputs/FusNew_"+str(size_x)+"_"+str(iteration)+"_"+str(nb_proc)+".lad", "w")
ladTxt=""
ladTxt = "<?xml version='1.0'?>\n"
ladTxt += "<lad xmlns=\"http://www.inria.fr/avalon/lad\"> <!-- lad file of the first version of the heat equation -->\n"
ladTxt += "<mpi>\n"

for p in range(0,nb_proc):
    ladTxt += "<process>\n"
    ladTxt += tempProc.substitute(proc=p,sizex=size_x,sizey=size_y,iter=iteration,nbp=nb_proc)
    ladTxt += "</process>\n"
    
for line in tempCommF :
    ladTxt += "<communicator>"
    lineComm = Template(line)
    for p in range(0,nb_proc):
        ladTxt += lineComm.substitute(proc=p)
    ladTxt += "</communicator>"

ladTxt += "</mpi>\n"
ladTxt += "</lad>\n"

ladFile.write(ladTxt)
ladFile.close()
tempProcF.close()
tempCommF.close()