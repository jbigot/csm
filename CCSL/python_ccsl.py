#################################################################
#Helene Coullon
#LIP INRIA
#Python version of the CCSL (component stencil language compiler)
#################################################################

from utils import *
import string
import re

##########################################
# Tools
##########################################
def addNewKernel(phgpk,compind):
    phase = phgpk[len(phgpk)-1]
    group = phase[len(phase)-1]
    newK = []
    newK.append(compind)
    group.append(newK)
def addNewGroup(phgpk,compind):
    phase = phgpk[len(phgpk)-1]
    newG = []
    newK = []
    newK.append(compind)
    newG.append(newK)
    phase.append(newG)
def addNewPhase(phgpk,compind):
    newP = []
    newG = []
    newK = []
    newK.append(compind)
    newG.append(newK)
    newP.append(newG)
    phgpk.append(newP)
def searchSameKernel(kernelCompNList,compNList):
    foundSame = -1
    count = 0
    for clist in kernelCompNList:
        same = True
        if len(clist)==len(compNList):
            for cN,cO in zip(compNList,clist):
                if cN!=cO:
                    same=False
                    break
        else:
            same=False
        if same:
            foundSame = count
            break
        count=count+1
    return foundSame
def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    return [ atoi(c) for c in re.split('(\d+)', text) ]
def getkey(item):
    return natural_keys(item[0])
#########################################

##########################################
# Parser
##########################################
def parse(dslFile,nprocs,toparse,outputfile):
    ########### mesh:cart,2,100,100
    firstL = toparse.readline()
    while firstL.find('mesh:')<0:
        firstL = toparse.readline()
    #split ':' and ','
    param = []
    param = firstL.split(':')[1].split(',')
    meshType = param[0]
    meshDim = int(param[1])
    meshWidth = int(param[2])
    meshHeight = int(param [3])
    #print meshType
    #print str(meshDim)
    print str(meshWidth)
    print str(meshHeight)

    ########### domains
    #secondL = toparse.readline()
    #while secondL.find('domains:')<0:
    #    secondL = toparse.readline()
    #split ':' and ','
    #domainsList = []
    #domainsList = secondL.split(':')[1].split(',')
    #print domainsList

    ########### data
    dataName = []
    dataOrder = []
    dataVal = []
    dataEi = []
    dataType = []

    dataL = toparse.readline()
    while dataL.find('data:')<0:
        dataL = toparse.readline()
    dataL = toparse.readline()
    while dataL.find('computations:')<0 and dataL[1]!='#':
        paramData = []
        paramData = dataL.split(',')
        dataName.append(paramData[0].strip('\t'))
        dataVal.append(paramData[1])
        dataOrder.append(paramData[2])
        dataEi.append(paramData[3])
        dataType.append(paramData[4].strip('\n'))
        dataL = toparse.readline()
    #print dataName
    #print dataOrder
    #print dataVal
    #print dataEi
    #print dataType

    ########### computations
    compName = [] #the order given by the user is kept
    compType = []
    compR = []
    compW = []
    compD = []
    nbiter = 0
    compL = dataL
    while compL.find('computations:')<0:
        compL = toparse.readline()
    nbiter = int(compL.split(':')[1].strip('\n'))
    print nbiter
    for line in toparse:
        if line.find(':')>=0 and line[1]!='#':
            compType.append(line.split(':')[0].strip('\t'))
            right = line.split(':')[1]
            right = right[0:right.rfind(')')+1]
            compName.append(right.split('(')[0])
            param = right.split('(')[1]
            param = param[0:len(param)-1]
            read = param.split('}')[0]
            compR.append(read[1:len(read)].split(','))
            compW.append(param.split('}')[1].split(',')[1])
            compD.append(param.split('}')[1].split(',')[2])
    #print compName
    #print compType
    #print compR
    #print compW
    #print compD

    ########### call phase 1
    phgpk = PGKdetection(compName,compType,compR,compW,compD)
    print phgpk
    ########### call phase 2
    createAssembly(meshWidth,meshHeight,nprocs,
        dataName,dataOrder,dataVal,dataEi,dataType,
        compName,compType,compR,compW,compD,phgpk,outputfile,nbiter)
##########################################

##########################################
# Phase 1: Detection of Phases, Groups and Kernels
##########################################
def PGKdetection(compName,compType,compR,compW,compD):
    phgpk = []

    count = 0;
    for comp in compName:
        if len(phgpk) == 0:
            #create the first phase/group and kernel for computation 0
            addNewPhase(phgpk,0)
        else:
            if compType[count].find('local')>=0 or compType[count].find('bound')>=0:
                ###########
                # for each kernel in the current group check Rnw
                ###########
                newGroup=False
                addk = []
                phase = phgpk[len(phgpk)-1]
                group = phase[len(phase)-1]
                for k in group:
                    for c in k:
                        for r in compR[count]:
                            if compW[c]==r and compType[c].find('bound')<0:
                                newGroup=True       
                        if compD[c]==compD[count]:
                            addk=k
                ###########
                # a new group has to be created
                ###########
                if newGroup==True:
                    addNewGroup(phgpk,count)
                    #print 'new group for computation '+str(count)
                ###########
                # otherwize a new kernel or a new computation has to be added
                ###########
                else:
                    if len(addk)==0:
                        addNewKernel(phgpk,count)
                    else:
                        addk.append(count)
                ###########

            elif compType[count].find('stencil')>=0:
                ###########
                # for each kernel of each group in the current phase check Rnw
                # a stencil cannot create a new group
                ###########
                newPhase=False
                addk = []
                phase = phgpk[len(phgpk)-1]
                for g in phase:
                    for k in g:
                        for c in k:
                            for r in compR[count]:
                                if compW[c]==r:
                                    newPhase=True
                            if compD[c]==compD[count]:
                                addk=k
                ###########
                # a new phase has to be created
                ###########
                if newPhase==True:
                    addNewPhase(phgpk,count)
                    #print 'new phase for computation '+str(count)
                    print phgpk
                ###########
                # otherwize a new kernel or a new computation has to be added
                ###########
                else:
                    if len(addk)==0:
                        addNewKernel(phgpk,count)
                    else:
                        addk.append(count)
                ###########

        count = count +1
    return phgpk
##########################################

##########################################
#Phase 2: Generation of the assembly file
#if MPI the same assembly is generated for each process and linked through communicators
#Generation of an information output file of the phases/groups/kernels/computations construction 'out.info'
##########################################
def createAssembly(meshWidth,meshHeight,nprocs,
        dataName,dataOrder,dataVal,dataEi,dataType,
        compName,compType,compR,compW,compD,phgpk,
        outputfile,nbiter):

    out = ''
    peers = []

    (rootNode, rootChildNode) = makeRootNode()
    for i in range(nprocs):
        #create Driver, DDS, DriverApp and Data
        process = makeProcess(rootChildNode)
        #start property
        startProp = makeStartProperty(process,'dr_'+str(i),'inGo')
        #object driver
        driver = makeInstance(process,'dr_'+str(i),'Driver')
        makeConnection(driver,'structInit',[('services','st_'+str(i))])
        makeConnection(driver,'driverappGo',[('inGo','dra_'+str(i))])
        #object driverApp
        driverapp = makeInstance(process,'dra_'+str(i),'DriverApp')
        makeConnection(driverapp,'timeGo',[('inGo','ti_'+str(i))])
        #object Struct
        struct= makeInstance(process,'st_'+str(i),'Structure')
        makeInt64(struct,'globW', meshWidth)
        makeInt64(struct,'globH', meshHeight)
        makeInt32(struct,'nb_proc', nprocs)

        #data
        cd=0
        driverappdata = []
        for d in dataName:
            data = makeInstance(process, d+'_'+str(i), 'Data')
            makeConnection(data, 'str', [('services', 'st_'+str(i))])
            #initializer
            makeConnection(data, 'initializer', [('initdata', 'in_1_'+str(i))])
            makeInt16(data, 'border', dataOrder[cd])
            if dataEi[cd]=='cell':
                makeInt16(data, 'ei', 1)
            elif dataEi[cd]=='edgex':
                makeInt16(data, 'ei', 2)
            elif dataEi[cd]=='edgey':
                makeInt16(data, 'ei', 3)
            makeDouble(data, 'init_val', dataVal[cd])
            driverappdata.append(('inGo', d+'_'+str(i)))
            cd = cd+1
        makeConnection(driverapp, 'dataCompute', driverappdata)

        #initializer
        ini = makeInstance(process, 'in_1_'+str(i), 'Initializer')
        #object Time
        time= makeInstance(process, 'ti_'+str(i), 'Time')
        makeInt16(time, 'iter', nbiter)

        #tables to keep trace of creating kernels
        #some of them could be used more than once
        #in this case the name of properties are the same
        #in this case too the name of the kernel type is the same
        kernelCompNList = [] #list of computation names for each kernel
        kernelCompList = [] #list of computation indexes for each kernel
        kernelName = []

        globck = 0
        globcg = 0
        cph = 0
        timephase = []
        for phase in phgpk:
            #create phase component
            ph = makeInstance(process, 'ph_'+str(cph)+'_'+str(i), 'Phase')
            #add cppref to Time component
            timephase.append(('inGo', 'ph_'+str(cph)+'_'+str(i)))
            #write output file
            out = out +'Phase'+str(cph)+'\n'
            cg = 0
            phasegroup = []
            phasecomp = []
            for group in phase:
                #create group
                gp = makeInstance(process, 'group_'+str(cph)+'_'+str(cg)+'_'+str(i), 'Group')
                #add cppref to phases
                phasegroup.append(('inGo', 'group_'+str(cph)+'_'+str(cg)+'_'+str(i)))
                #write output file
                out = out +'\tGroup'+str(globcg)+'\n'
                ck=0
                groupkernel=[]

                for kernel in group:
                    name = 'K'+compName[kernel[0]]

                    #write output file
                    out = out +'\t\tKernel'+str(globck)+' of type called '+ name +'\n'

                    #create kernel
                    k = makeInstance(process, 'ker_'+str(cph)+'_'+str(cg)+'_'+str(ck)+'_'+str(i), name)
                    #write output file
                    out = out +'\t\t\t'

                    #list of computations to fill kernelCompNList and kernelCompList
                    compNList = []
                    compList = []
                    for c in kernel:
                        #add the computation for this current kernel
                        compList.append(c)
                        compNList.append(compName[c])
                    kernelName.append(name)

                    #to not connect more than once the same data
                    RWkernels = []
                    #search for a same kernel created before
                    findSameKernel = searchSameKernel(kernelCompNList,compNList)

                    if findSameKernel<0:
                        for c in kernel:
                            #write output file
                            out = out +compName[c]+','
                            for r in compR[c]:
                                if r+'_'+str(i) not in RWkernels:
                                    makeConnection(k, r, [('services', r+'_'+str(i))])
                                    RWkernels.append(r+'_'+str(i))
                                if compType[c]=='stencil':
                                    phasecomp.append(('services', r+'_'+str(i)))
                                    peers.append((r+'_'+str(i),'comm'))
                            if compW[c]+'_'+str(i) not in RWkernels:
                                makeConnection(k, compW[c], [('services', compW[c]+'_'+str(i))])
                                RWkernels.append(compW[c]+'_'+str(i))
                    else:
                        count = 0
                        for cN, cO in zip(kernel,kernelCompList[findSameKernel]):
                            #write output file
                            out = out +compName[cN]+','
                            for rN, rO in zip(compR[cN],compR[cO]):
                                if rN+'_'+str(i) not in RWkernels:
                                    makeConnection(k, rO, [('services', rN+'_'+str(i))])
                                    RWkernels.append(rN+'_'+str(i))
                                if compType[cN]=='stencil':
                                    phasecomp.append(('services', rN+'_'+str(i)))
                                    peers.append((rN+'_'+str(i),'comm'))
                            if compW[cN]+'_'+str(i) not in RWkernels:
                                makeConnection(k, compW[cO], [('services', compW[cN]+'_'+str(i))])
                                RWkernels.append(compW[cN]+'_'+str(i))
                            count = count +1
                    out = out +'\n'

                    #fill kernelList
                    kernelCompList.append(compList)
                    kernelCompNList.append(compNList)

                    #add cppref to groups
                    groupkernel.append(('inGo', 'ker_'+str(cph)+'_'+str(cg)+'_'+str(ck)+'_'+str(i)))
                    ck = ck+1
                    globck = globck+1

                groupkernel.sort()
                makeConnection(gp, 'kernelCompute', groupkernel)
                cg = cg+1
                globcg = globcg+1

            phasegroup.sort()
            makeConnection(ph, 'groupCompute', phasegroup)
            phasecomp = list(set(phasecomp))
            phasecomp.sort()
            makeConnection(ph, 'dataCompute', phasecomp)
            cph = cph+1

        timephase.sort()
        makeConnection(time, 'phaseCompute', timephase)

    peers = list(set(peers))
    peers.sort(key=getkey)
    peertemp = []
    ref = ''
    for pair in peers:
        print pair
        if pair[0].split('_')[0]!=ref:
            if len(peertemp)>0:
                print peertemp
                makeCommunicator(rootChildNode,peertemp)
            peertemp = []
            peertemp.append(pair)
            ref = pair[0].split('_')[0]
        else:
            peertemp.append(pair)
    if len(peertemp)>0:
        print peertemp
        makeCommunicator(rootChildNode,peertemp)

    f = open(outputfile, 'w')
    f.write("<?xml version='1.0'?>")
    f.write(xmlToString(rootNode))
    f.close()

    f = open('./out.info', 'w')
    f.write(out)
    f.close()

##########################################

#TODO--Phase 3: Generation of a HLCM composite

##########################################
# Main
##########################################
def main():
    dslFile = sys.argv[1]
    nprocs = int(sys.argv[2])
    outputfile = sys.argv[3]
    toparse = open(dslFile,'r')
    parse(dslFile,nprocs,toparse,outputfile)
##########################################

if __name__ == '__main__':
    main()
