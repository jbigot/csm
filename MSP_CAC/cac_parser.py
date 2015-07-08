#===============================================================
# Helene Coullon
# LIP INRIA
# Parser of Python MSP_CAC
#===============================================================

#===============================================================
# Imports
#===============================================================
import sys
import os
sys.path.append(os.path.dirname("./utils/"))
from cac_types import *

#===============================================================
# format of the input :
# mesh:string
# data:
# id,domain
# ...
# time: int
# computations:
# type:id(R,w)
# ...
#===============================================================

#===============================================================
# Funtions
#===============================================================
#-----------------------
def parse(pfile,mesh,data,time,computations,duplicate):
#-----------------------
	#open the file to read
	toparse = open(pfile,'r')
	
	##### MESH
	#do not take empty lines at the beginning
	line = toparse.readline()
	while line.find('mesh:') < 0:
		line = toparse.readline()
	#mesh type
	mesh = line.split(':')[1].strip(' ')
	#####
	
	##### DATA
	#do not take empty lines between mesh and data
	line = toparse.readline()
	while line.find('data:')<0:
		line = toparse.readline()
	line = toparse.readline() #first line of data does not contain anything, next one
	#data extraction
	while line.find('time:')<0 and len(line)>0:
		tmp = line.split(',')
		ident = tmp[0].strip('\t').strip(' ')
		domain = tmp[1].strip('\n').strip(' ')
		data.append(Data(ident,domain))
		line = toparse.readline()
	#####
		
	##### TIME
	while line.find('time:')<0:
		line = toparse.readline()
	#time
	time = int(line.split(':')[1].strip(' '))
	print time
	######
	
	##### COMPUTATIONS
	#do not take empty lines between time and computations
	line = toparse.readline()
	while line.find('computations:')<0:
		line = toparse.readline()
	line = toparse.readline() #first line of computations does not contain anything, next one
	#computation extraction
	while len(line)>0:
		tmp = line.split(':')
		#type of computation on the left of ":"
		ctype = tmp[0].strip('\t').strip(' ')
		tmp2 = tmp[1].split('(')
		#name/id of the computation on the left of "("
		cid = tmp2[0].strip(' ')
		tmp3 = tmp2[1].split('}')
		if ctype!="stencil" :
			cw = tmp3[1].strip(',').strip('\n').strip(' ').strip(')')
			cn = ""
		else :
			tmp33 = tmp3[1].split(',')
			cw = tmp33[1].strip(' ')
			cn = tmp33[2].strip(',').strip('\n').strip(' ').strip(')')
		tmp4 = tmp3[0].strip('{').strip(' ').split(',')
		cr = list()
		for el in tmp4:
			cr.append(el)
		# create the computation
		computations.append(Computation(cid,ctype,cr,cw,cn))
		# incremente the number of instances needed for the name of the computation
		if cid in duplicate.keys():
			duplicate[cid] += 1
		else :
			duplicate[cid] = 1
			
		line = toparse.readline()
	#####
	toparse.close()
#-----------------------

#-----------------------
def parseMetaInf(pfile,dico):
#-----------------------
	#open the file to read
	toparse = open(pfile,'r')
	
	# fill the doctionary
	for line in toparse:
		if line.find('=') > -1:
			spl = line.strip('\n').strip('\t').split('=')
			dico[spl[0].strip(' ')] = spl[1].strip(' ')
			
	toparse.close()
#-----------------------

#-----------------------
def parseData(metafile,dico,dataid):
#-----------------------
	pfile = open(metafile,'r')

	# fill the doctionary
	# get to the data to parse
	line = pfile.readline()
	spl = line.strip('\n').strip('\t').split('=')
	while not (line.find('data') > -1 and spl[1].strip(' ') == dataid):
		line = pfile.readline()
		spl = line.strip('\n').strip('\t').split('=')
	
	#read the values until the next "data" or the end of file
	spl = line.strip('\n').strip('\t').split('=')
	dico[spl[0].strip(' ')] = spl[1].strip(' ')
	line = pfile.readline()
	while line.find('data') and (line!=""):
		if line.strip('\n').strip('\t').strip(" ")!= "":
			spl = line.strip('\n').strip('\t').split('=')
			dico[spl[0].strip(' ')] = spl[1].strip(' ')
		line = pfile.readline()
			
	pfile.close()
#-----------------------