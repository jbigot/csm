#===============================================================
# Helene Coullon
# LIP INRIA
# Python MSP_CAC (Multi-Stencil Program Component Assembly Compiler)
#===============================================================

#===============================================================
# Imports
#===============================================================
import sys
import os
sys.path.append(os.path.dirname("./utils/"))
import copy
from xml import *
from cac_parser import *
from cac_drawer import *
from networkx.algorithms import bipartite

#===============================================================
# Class compiler
# This class represent the actual compiler from the DSL to 
# a component assembly.
# two parts : 1 - from msp to tsp, 2 - from tsp to component assembly, or dump
#===============================================================
class CAC_Compiler:
	#-----------------------
	def __init__(self,mspfile,metafile,tfile,dfile):
	#-----------------------
		self.MSPfile = mspfile
		self.metaFile = metafile
		self.templateFile = open(tfile,'r')
		self.dataTempFile = open(dfile,'r')
		self.nb_proc = 2
		#self.dump_type = "hybrid"
		self.dump_type = "data_base"
	#-----------------------
	
	#-----------------------
	def compile(self):
	#-----------------------
		# from msp file to series-parallel tree decomposition
		self.msp_to_tsp()
		# from TSP to l2C component assembly
		self.dump()
	#-----------------------

	#-----------------------
	def msp_to_tsp(self):
	#-----------------------
		# parse the input file
		print "Read msp file"
		self.read_dsl()
		# compute gamma data : separate updates
		#self.gamma_data() #it seems it does not work for SW entirely
		# compute gamma data : do not separate updates
		print "Compute Gamma data"
		self.gamma_data_seq()
		
		if self.dump_type != "data_base":
			# compute gamma hybrid
			print "Compute Gamma hybrid"
			self.gamma_hyb()
			# first transitive reduction
			print "Compute transitive reduction"
			self.transitive_reduction(self.gamma)
			##### DRAW
			saveGraph(self.gamma,"./outputs/trans.dot")
			# compute sources/roots of self.gamma
			self.sources()
			# remove N shapes
			print "Remove Nshapes"
			#self.nshape = self.gamma.copy()
			self.nshape = self.gamma
			self.level = 0
			self.remove_nshape(self.sources)
			##### DRAW
			saveGraph(self.nshape,"./outputs/nshape.dot")
			
			# series-parallel tree decomposition
			print "Compute series-parallel decomposition"
			self.tsp()
			
			# canonical form
			print "Compute canonical form"
			self.canonic = nx.DiGraph()
			labels = nx.get_node_attributes(self.toReduce[self.startEdge],'label')
			nodes = self.toReduce[self.startEdge].nodes()
			#find the root
			self.root = 0
			for node in nodes:
				if len(self.toReduce[self.startEdge].predecessors(node))==0:
					self.root = node
			
			### networks are not ordered, we have to keep order of computations by specific attributes during all steps of the compilation
			# table to store the order index for each node (representing the order index for its successors)
			# this is used to keep the good order of computations in the canonical form
			self.order_index = {}
			# self.orders from self.toReduce[self.startEdge]
			self.orders = nx.get_node_attributes(self.toReduce[self.startEdge],'order')
			
			# start the canonical reduction
			successors = self.toReduce[self.startEdge].successors(self.root)
			self.orderSuccessors(successors) #always work on an ordered list of successors !
			self.canonical(self.root,successors,labels)
			
			# self.orders from self.canonic
			self.orders = nx.get_node_attributes(self.canonic,'order')
			
			##### DRAW
			saveGraph(self.canonic,"./outputs/canonic.dot")
	#-----------------------

	#-----------------------
	def read_dsl(self):
	#-----------------------
		#final list of computations
		self.computations = []
		#final list of data
		self.data = []
		#mesh type
		self.mesh = ""
		#time
		self.time = 0
		self.duplicates = {}
		
		#see parser.py for more details
		parse(self.MSPfile,self.mesh,self.data,self.time,self.computations,self.duplicates)
		
		##### PRINT
		# print "MESH TYPE = " + self.mesh + "\n"
		# print "DATA = "
		# print self.data
		# print "\n"
		# print "TIME = " + str(self.time) + "\n"
		# print "COMPUTATIONS = "
		# print self.computations
		# print "\n"
		#####
	#-----------------------

	#-----------------------
	# to build Gamma_data the ordered list self.computations is modified
	# some update computations are added into it when needed
	# O(n^2)
	# in this version of the function, each needed update is itself a new computation
	#-----------------------
	# TODO !!! Same than in gamma_data_seq, if the data has already been updated don't do it again
	def gamma_data(self):
	#-----------------------
		current = 0
		while current<len(self.computations):
			toupdate = set()
			for j in range(current-1,-1,-1):
				# if the current ith computation is a stencil and if what is read has been written in the jth computation
				# add w of jth in toupdate
				if self.computations[current][1]=="stencil" and (self.computations[j][3] in self.computations[current][2]) :
					toupdate.add(self.computations[j][3])
			if len(toupdate)>0:
				toadd = set()
				counter = 0
				for up in toupdate:
					# fake data is added to the update (written) and the initial computation (read) to keep an order of computation
					name_wu = "wu_"+self.computations[current][0]+"_"+str(counter)
					self.computations.insert(current,Computation("upd_"+self.computations[current][0]+"_"+str(counter),"update",set(up),name_wu,""))
					toadd.add(name_wu)
					counter = counter+1
					current = current+1
				for el in toadd:
					self.computations[current][2].append(el)
				current = current+1
			else :
				current = current+1
				
		##### PRINT
		#for elem in self.computations:
			#print elem
	#-----------------------
	
	#-----------------------
	# alternative to gamma_data
	# to build Gamma_data the ordered list self.computations is modified
	# in this case updates for one computation are not splitted but represent a single computation
	# O(n^2)
	# in this version of the function, all updates needed for a computation represent a single update
	#-----------------------
	def gamma_data_seq(self):
	#-----------------------
		current = 0
		while current<len(self.computations):
			toupdate = set()
			tolink = set()
			for j in range(current-1,-1,-1):
				# if the current ith computation is a stencil and if what is read has been written in the jth computation
				# add w of jth in toupdate
				# however if an update is done between the current computation and the computation j
				# and the data is updated in this update, do not add it, only link it to the update
				if self.computations[current][1]=="stencil" and (self.computations[j][3] in self.computations[current][2]) :
					addit = True
					for i in range(j+1,current-1,1):
						if self.computations[i][1]=="update" and (self.computations[j][3] in self.computations[i][2]):
							addit = False
							tolink.add(self.computations[i][3])
							break
					if addit==True:
						#print "add "+self.computations[j][3]
						toupdate.add(self.computations[j][3])
			if len(toupdate)>0:
				name_wu = "wu_"+self.computations[current][0]
				self.computations.insert(current,Computation("upd_"+self.computations[current][0],"update",toupdate,name_wu,""))
				current = current+1
				self.computations[current][2].append(name_wu)
			if len(tolink)>0:
				for link in tolink:
					self.computations[current][2].append(link)
				
			current = current+1
				
		##### PRINT
		# for elem in self.computations:
		# 	print elem
	#-----------------------

	#-----------------------
	# to build Gamma_hyb the ordered list self.computations is transformed to a DAG called self.gamma
	# use of the type DiGraph of Networkx
	# this method also builds the initial adjacence matrix adjMat
	# this matrix is used in the next step for the transitive reduction of self.gamma
	# O(n^2)
	#-----------------------
	def gamma_hyb(self):
	#-----------------------
		#adjMat initialization
		# 1 at (i,j) if a directed edge from i to j
		# 0 otherwize
		#used for transitive reduction
		self.adjMat = [[-9999999999 for x in range(len(self.computations))] for x in range(len(self.computations))]
		
		self.gamma = nx.DiGraph()
		for i in range(0,len(self.computations)):
			self.gamma.add_node(i,label=self.computations[i][0])
		for i in range(0,len(self.computations)):
			for j in range(i-1,-1,-1):
				if (self.computations[i][1]!="bound" or self.computations[j][1]!="bound") and (self.computations[j][3] in self.computations[i][2]):
					self.gamma.add_edge(j,i)
					self.adjMat[j][i] = 1
		
		##### DRAW
		#saveGraph(self.gamma,"./outputs/dag.dot")
	#-----------------------

	#-----------------------
	# variant of Floyd Warshall's algorithm to keep max path
	# O(n^3)
	#-----------------------
	def transitive_reduction(self,graph):
	#-----------------------
		# new adjacancy matrix to compute
		new_adjMat = copy.deepcopy(self.adjMat)
		# variant of Floyd Warshall's algorithm
		# keep the maximum path from i to j
		for k in range(0,len(self.computations)):
			for i in range(0,len(self.computations)):
				for j in range(0,len(self.computations)):
					new_adjMat[i][j] = max(new_adjMat[i][j],new_adjMat[i][k]+new_adjMat[k][j])
		
		# if a direct path exists in self.adjMat between i and j
		# but also an undirect in new_adjMat
		# the direct path has to be removed from the graph
		for i in range(0,len(self.computations)):
			for j in range(0,len(self.computations)):
				if self.adjMat[i][j]==1 and new_adjMat[i][j]>1 :
					#print "Remove edge : (" + self.computations[i][0] + "," + self.computations[j][0] + ")\n"
					graph.remove_edge(i,j)
					#note the removal for future transitive reduction
					self.adjMat[i][j]=-9999999999
		
		##### DRAW
		#saveGraph(self.gamma,"./outputs/trans.dot")
	#-----------------------
	
	# O(n)
	#-----------------------
	def sources(self):
	#-----------------------
		self.sources = set()
		for no in self.gamma.nodes():
			if len(self.gamma.predecessors(no))==0:
				self.sources.add(no)
	#-----------------------
	
	#-----------------------
	# recursive BFS by successors
	# take a source set of nodes, and their successors
	# if sources have common successors and that the subgraph is not complete bipartite, make it complete + transitive reduction
	# longest part of the compiler
	#-----------------------
	def remove_nshape(self,sources):
	#-----------------------
		# compute successors for the given sources
		# compute a set with single values
		# and compute the list
		successors = set()
		succ_list = []
		for s in sources:
			successors |=  set(self.gamma.successors(s))
			succ_list += self.gamma.successors(s)
			
		# if successors and succ_list has the same size it means that the intersection of successors of sources is empty
		# if successors is shorter than succ_list, the subgraph is connected
		is_connected = (len(successors)<len(succ_list))
		
		# remove from sources the nodes also present in destination
		# in this case we have a loop
		# this is correct but has to be proved ideally
		for succ in successors:
			if succ in sources:
				sources.remove(succ)
			
		##### PRINT
		# sys.stdout.write('Sources = (')
		# for s in sources :
		# 	sys.stdout.write(self.computations[s][0]+',')
		# sys.stdout.write(")\n successors set = (")
		# for succ in successors :
		# 	sys.stdout.write(self.computations[succ][0]+',')
		# print ")\n"
		# sys.stdout.write(")\n successors list = (")
		# for succ in succ_list :
		# 	sys.stdout.write(self.computations[succ][0]+',')
		# print ")\n"
		# print is_connected
		#####
		
		# if it is not the end of the graph
		if len(successors)>0:
			# create the subgraph containing sources and successors nodes
			subgraph_nodes = sources | successors
			subgraph = self.gamma.subgraph(list(subgraph_nodes))
			# if this subgraph is not connected, recursive call
			if not is_connected :
				#print "not connected"
				self.remove_nshape(successors)
			else :
				# if the graph is connected and is not complete, make it complete, transitive reduction, and recursive call
				if not self.is_complete(len(sources),len(successors),subgraph):
					#print "Make complete the subgraph from sources"
					self.make_complete(sources,successors)
					self.remove_nshape(successors)
				# else recursive call
				else :
					#print "next graph"
					self.remove_nshape(successors)
	#-----------------------
	
	#-----------------------
	def is_complete(self,k1,k2,graph):
	#-----------------------
		if len(graph.edges()) == k1 * k2:
			return True
		else:
			return False
	#-----------------------
	
	#-----------------------
	# O(n^2)
	#-----------------------
	def make_complete(self,sources,successors):
	#-----------------------
		for s in sources:
			for succ in successors:
				succ_succ = self.nshape.successors(succ)
				if s!=succ and s not in succ_succ:#(s,succ) not in self.nshape.edges() and (succ,s) not in self.nshape.edges():
					#self.gamma.add_edge(s,succ,label='*')
					self.nshape.add_edge(s,succ,label='*')
					self.adjMat[s][succ]=1
		#drawGamma(self.nshape,"./outputs/nshape_"+str(self.level)+".dot")
		self.transitive_reduction(self.nshape)
		#drawGamma(self.nshape,"./outputs/nshape_"+str(self.level)+"_trans.dot")
	#-----------------------

	#-----------------------
	def tsp(self):
	#-----------------------
		#inverse line digraph with labels on edges = id of the corresponding node
		self.inverse = nx.MultiDiGraph()
		
		#to note the transformation from node to edge
		self.transformed = [[-1 for x in range(0,2)] for x in range(len(self.nshape.nodes()))]
		#to count the index of new nodes
		self.counter_nodes = 0
		
		#to store roots
		self.roots = []
		#to store leaves
		self.leaves = []
		
		#call the recursive function to build self.inverse : the inverse line digraph of self.nshape
		nodes = self.nshape.nodes()
		#we need a loop if there are more than one root in self.nshape
		for no in nodes:
			if self.transformed[no][0]<0 and self.transformed[no][1]<0:
				#to recursively change the source node of a new edge
				source_node = self.counter_nodes
				self.inverse_line(no,source_node)
		
		#print roots and leaves
		# print "Roots = "
		# for r in self.roots:
		# 	print self.computations[r][0]
		# print "\nLeaves = "
		# for l in self.leaves:
		# 	print self.computations[l][0]
		
		#merge roots and leaves to build a unique inverse line digraph
		self.merge_root_leaf()
		
		# initialize order of nodes to build ordered sequences in the final dump
		self.orders_init = {}
		self.global_order_index = 0
		start = self.roots[0]
		self.bfs_orderInit(start)
		
		
		##### DRAW
		saveGraph(self.inverse,"./outputs/inverse.dot")
		
		#array of Digraphs, representing the labels of the series-parallel tree decomposition
		#needed for the last operation self.labelled_reduction
		#initialized for each edge with a simple graph of one node (one computation) in self.inverse_line
		self.toReduce = {}
		#store the root node of each graph in self.toReduce
		self.rootNode = {}
		#global indexes for new sequence and parallel nodes
		self.lab_index = len(self.transformed)
		for i in range(0,len(self.transformed)):
			lab_graph = nx.DiGraph()
			lab_graph.add_node(i,label=self.computations[i][0],order=self.orders_init[(self.transformed[i][0],self.transformed[i][1])])
			#if the edge is not in toReduce add it
			if not (self.transformed[i][0],self.transformed[i][1]) in self.toReduce:
				self.toReduce[(self.transformed[i][0],self.transformed[i][1])] = lab_graph
				self.rootNode[(self.transformed[i][0],self.transformed[i][1])] = i
			#if the edge is already in toReduce, directly reduce the parallel merge
			else :
				self.mergeParallel((self.transformed[i][0],self.transformed[i][1]),self.rootNode[(self.transformed[i][0],self.transformed[i][1])],i,self.toReduce[(self.transformed[i][0],self.transformed[i][1])],lab_graph,False)
		#at this point there is not parallel edges to reduce
		#a parallel reduction can only be created by a sequence reduction (see in self.mergeSequence)
		#drawInverse(self.inverse,"./outputs/inverse2.dot")
		
		#series-parallel tree decomposition, reduction of self.toReduce
		self.startEdge = (0,1)
		self.stop = False
		while len(self.toReduce) > 1:
			#print "restart from " + str(self.startEdge)
			self.labelled_reduction(self.startEdge)
		
		##### DRAW	
		saveGraph(self.toReduce[self.startEdge],"./outputs/tsp.dot")
		
		#self.orders = nx.get_node_attributes(self.toReduce[self.startEdge],'order')
		#self.printBFSSuccessors(self.toReduce[self.startEdge],self.rootNode[self.startEdge],nx.get_node_attributes(self.toReduce[self.startEdge],'label'))
	#-----------------------
	
	#-----------------------
	# for debug
	# print succesors before and after the sort by order
	#-----------------------
	def printBFSSuccessors(self,graph,node,labels):
	#-----------------------
		successors = graph.successors(node)
		print successors
		print "for node "+labels[node]+" orders of successors "		
		print successors
		self.orderSuccessors(successors)
		print successors
		
		print "for node "+labels[node]+" successors "
		for succ in successors:
				print labels[succ]
		for succ in successors:
			self.printBFSSuccessors(graph,succ,labels)
	#-----------------------
	
	#-----------------------
	def bfs_orderInit(self,node):
	#-----------------------
		successors = self.inverse.successors(node)
		for succ in successors:
			self.orders_init[(node,succ)] = self.global_order_index
			self.global_order_index += 1
		for succ in successors:
			self.bfs_orderInit(succ)
	#-----------------------

	#-----------------------
	# node is the node of self.nshape the original graph
	# source_node for the recursion indicate the source node in self.inverse
	#-----------------------
	def inverse_line(self,node,source_node):
	#-----------------------
		#print "Current node of self.nshape to transform to an edge "+self.computations[node][0]
		#if the source node is the first node create it (first call)
		pred = self.nshape.predecessors(node)
		if len(pred) == 0 :
			#print self.computations[node][0]+" - Create source node in self.inverse "+str(source_node)
			self.inverse.add_node(source_node)
			self.counter_nodes = self.counter_nodes +1
			self.roots.append(node)
		
		#detect if in the successors of node, one has already been transformed
		succ = self.nshape.successors(node)
		change_counter = -1
		for i in succ:
			if self.transformed[i][0]>0 and self.transformed[i][1]>0:
				change_counter = i
		
		#in this case change the destination node id
		destination = -1
		
		#if not change_counter a new node can be added
		if change_counter < 0:
			#print "change_counter <0 for node "+self.computations[node][0]
			destination = self.counter_nodes
			#create the destination node
			#print self.computations[node][0]+" - Create source node in self.inverse "+str(destination)
			self.inverse.add_node(destination)
			#add 1 to the counter of nodes in self.inverse
			self.counter_nodes = self.counter_nodes + 1
		#otherwise use the existing one to be connected to
		#change the destination
		else:
			destination = self.transformed[change_counter][0]
			
		#create the edge from source_node to the destination
		#print "Create edge "+str(source_node)+","+str(destination)+" with label "+self.computations[node][0]
		#print self.computations[node][0]+" - Create edge in self.inverse "+ str(source_node)+","+str(destination)
		self.inverse.add_edge(source_node,destination,label=self.computations[node][0])
		#mark the node as transformed
		self.transformed[node] = [source_node,destination]
		
		# new source_node is the one created above
		new_source_node = copy.copy(destination)
		#if change_counter < 0:
			# add one to index of nodes in self.inverse
			#self.counter_nodes = self.counter_nodes + 1
			
		#recursive call to successors
		#print "Successors of "+self.computations[node][0]+" : \n"
		if len(succ)==0:
			self.leaves.append(node)
		else:
			for i in succ:
				#if the successor node to transform to an edge has not been transformed yet
				#recursive call to create it
				if self.transformed[i][0]<0 and self.transformed[i][1]<0:
					#copy counter_nodes to come back to the good value when backtracking recursive calls
					#print "Call inverse_line("+str(i)+","+str(source_node)+")\n\n"
					self.inverse_line(i,new_source_node)
	#-----------------------
	
	#-----------------------
	# inverse line digraph is not unique
	# we take the one with a single root and a single leaf by merging roots and leaves of self.inverse
	#-----------------------
	def merge_root_leaf(self):
		#merge the roots of self.inverse
		for root in self.roots:
			if root != self.roots[0]:
				source = self.transformed[root][0]
				destination = self.transformed[root][1]
				#remove the edge from the graph
				self.inverse.remove_edge(source,destination)
				self.inverse.remove_node(source)
				#add the good edge by merging source with the source of self.roots[0]
				self.inverse.add_edge(self.transformed[self.roots[0]][0],destination,label=self.computations[root][0])
				#change self.transformed
				self.transformed[root][0] = self.transformed[self.roots[0]][0]
		
		#merge the leaves of self.inverse	
		for leaf in self.leaves:
			if leaf != self.leaves[0]:
				source = self.transformed[leaf][0]
				destination = self.transformed[leaf][1]
				#remove the edge from the graph
				self.inverse.remove_edge(source,destination)
				self.inverse.remove_node(destination)
				#add the good edge by merging source with the source of self.roots[0]
				self.inverse.add_edge(source,self.transformed[self.leaves[0]][1],label=self.computations[leaf][0])
				#change self.transformed
				self.transformed[leaf][1] = self.transformed[self.leaves[0]][1]
	#-----------------------
	
	#-----------------------
	# detect a sequence from start edge
	# if there is a sequence reduce it
	# recursive call from the reduced edge or from successors
	#-----------------------
	def labelled_reduction(self,start):
	#-----------------------
		newEdge = ()
		
		if self.isSequence(start) :
			left = start
			right = (start[1],self.inverse.successors(start[1])[0])
			newEdge = (left[0],right[1])
			#print "mergeSequence of edges ("+str(left)+"),("+str(right)+")\n"
			self.mergeSequence(left,right)
			#recursive call from the new edge created
			self.labelled_reduction(newEdge)
		else :
			#recursive call from the successors of start
			succ = self.inverse.successors(start[1])
			for s in succ :
				self.labelled_reduction((start[1],s))
	#-----------------------
	
	#-----------------------
	def isSequence(self,start):
	#-----------------------
		#print "node "+str(start[1])
		#print "successors nb = "+str(len(self.inverse.successors(start[1])))
		#print "predecessors nb = "+str(len(self.inverse.predecessors(start[1])))
		isseq = len(self.inverse.successors(start[1]))==1 and len(self.inverse.predecessors(start[1]))==1 and self.inverse.number_of_edges(start[0],start[1])==1 and self.inverse.number_of_edges(start[1],self.inverse.successors(start[1])[0])==1
		return isseq
	#-----------------------
	
	#-----------------------
	def mergeSequence(self,left,right):
	#-----------------------
		#get the left graph
		left_graph = self.toReduce[left]
		#get the right graph
		right_graph = self.toReduce[right]
		#make the union of the two graphs (which are disjoints) as a new graph
		new_graph = nx.union(left_graph,right_graph)
		# get the orders of the root node of the left and right graph
		# if the order if not correct modify it
		orders = nx.get_node_attributes(new_graph,'order')
		#print orders
		if orders[self.rootNode[left]] > orders[self.rootNode[right]]:
			temp = orders[self.rootNode[left]]
			orders[self.rootNode[left]] = orders[self.rootNode[right]]
			orders[self.rootNode[right]] = temp
			nx.set_node_attributes(new_graph, 'order', orders)
		# happens often for 'P' and 'S' previously added
		elif orders[self.rootNode[left]] == orders[self.rootNode[right]]:
			orders[self.rootNode[right]] += 1
			nx.set_node_attributes(new_graph, 'order', orders)
		#add sequence node and edges
		new_graph.add_node(self.lab_index,label='S',order = 0)
		new_graph.add_edge(self.lab_index,self.rootNode[left])
		new_graph.add_edge(self.lab_index,self.rootNode[right])
		#remove the two edges reduced
		del self.toReduce[left]
		del self.toReduce[right]
		del self.rootNode[left]
		del self.rootNode[right]
		#remove in self.inverse
		self.inverse.remove_edge(left[0],left[1])
		self.inverse.remove_edge(right[0],right[1])
		self.inverse.remove_node(left[1])
		newEdge = (left[0],right[1])
		
		# as in the creation of self.toReduce, verify if the sequence reduction does not create a parallel reduction
		# if not add the new edge
		if not newEdge in self.toReduce:
			self.toReduce[newEdge] = new_graph
			self.rootNode[newEdge] = self.lab_index
			#reduction of the two edges in self.inverse : new edge
			self.inverse.add_edge(newEdge[0],newEdge[1])
		#if it is directly call the parallel reduction
		else :
			self.mergeParallel(newEdge,self.rootNode[newEdge],self.lab_index,self.toReduce[newEdge],new_graph,True)
		
		#increase the index	
		self.lab_index = self.lab_index + 1
		#if the reduction was on the first edge, modify self.startEdge
		if left[0]==0:
			self.startEdge = newEdge
	#-----------------------
	
	#-----------------------
	def mergeParallel(self,edge,root1,root2,up_graph,down_graph,fromSeq):
	#-----------------------
		#print "mergeParallel of double edge "+str(edge)+"\n"
		
		#if the call is from mergeSequence
		#increase the index
		if fromSeq:
			self.lab_index = self.lab_index + 1
		
		#make the union of the two graphs (which are disjoints) as a new graph
		new_graph = nx.union(up_graph,down_graph)
		# get the orders of the root node of the left and right graph
		# if the order if not correct modify it
		orders = nx.get_node_attributes(new_graph,'order')
		if orders[root1] > orders[root2]:
			temp = root1
			orders[root1] = orders[root2]
			orders[root2] = temp
			nx.set_node_attributes(new_graph, 'order', orders)
		# happens often for 'P' and 'S' previously added
		elif orders[root1] == orders[root2]:
			orders[root2] += 1
			nx.set_node_attributes(new_graph, 'order', orders)
		#add sequence node and edges
		new_graph.add_node(self.lab_index,label='P',order=0)
		new_graph.add_edge(self.lab_index,root1)
		new_graph.add_edge(self.lab_index,root2)
		#remove the two edges reduced
		del self.toReduce[edge]
		del self.rootNode[edge]
		#add the edge
		self.toReduce[edge] = new_graph
		self.rootNode[edge] = self.lab_index
		#remove in self.inverse
		while self.inverse.number_of_edges(edge[0],edge[1])>1:
			self.inverse.remove_edge(edge[0],edge[1])
		#increase the index	
		self.lab_index = self.lab_index + 1
	#-----------------------
	
	#-----------------------
	# replace the graph computed in self.tsp by its canonical form
	# each time two successive internal nodes (S or P) have the same type, concat them
	#successors has been ordered before
	#-----------------------
	def canonical(self,node,successors,labels):
	#-----------------------
		# if it is a new node add it
		if node not in self.canonic:
			self.canonic.add_node(node,label=labels[node],order=0)
			self.order_index[node] = 1
			
		# for each successor (already sorted)
		for succ in successors:
			# get the succesors of the succ and ordered them
			ssuccessors = self.toReduce[self.startEdge].successors(succ)
			self.orderSuccessors(ssuccessors)
			if not (labels[node]=='S' and labels[succ]=='S') and not (labels[node]=='P' and labels[succ]=='P'):
				if node not in self.order_index :
					self.order_index[node]=0
				self.canonic.add_node(succ,label=labels[succ],order=self.order_index[node])
				self.order_index[node] += 1
				self.canonic.add_edge(node,succ)
				self.canonical(succ,ssuccessors,labels)
			else:
				self.canonical(node,ssuccessors,labels)
	#-----------------------
	
	#-----------------------
	def cmpOrders(self,x,y):
	#-----------------------
		if self.orders[x] < self.orders[y]:
			return -1
		elif self.orders[x] > self.orders[y]:
			return 1
		else:
			return 0
	#-----------------------
	
	#-----------------------
	def orderSuccessors(self,successors):
	#-----------------------
		successors.sort(cmp=self.cmpOrders)
		
		# for passnum in range(len(successors)-1,0,-1):
		# 	for i in range(passnum):
		# 		if self.orders[successors[i]]>self.orders[successors[i+1]]:
		# 			temp = successors[i]
		# 			successors[i] = successors[i+1]
		# 			successors[i+1] = temp
	#-----------------------
	
	#-----------------------
	# First hybrid dump to L2C component assembly : lad file
	# duplicate the assembly nb_proc times
	#-----------------------
	def dump(self):
	#-----------------------
		print "Compute the dump"
	
		self.lad = "<?xml version='1.0'?>\n"
		self.lad += "<lad xmlns=\"http://www.inria.fr/avalon/lad\"> <!-- lad file of the first version of the heat equation -->\n"
		self.lad += "<mpi>\n"
		
		# create the almost complete template but depending on the processor number
		self.data_template = Template(self.dataTempFile.read())
		self.dico = {}
		self.duplicatesRef = copy.deepcopy(self.duplicates)
		self.duplicatesNew = copy.deepcopy(self.duplicates)
		text_template = self.createTemplate()
		template = Template(text_template)
		
		# for each processor build the assembly by substitution of $proc
		dic = {}
		for p in range(0,self.nb_proc):
			self.lad += "<process>\n"
			dic["proc"] = p
			self.lad += template.substitute(dic)
			self.lad += "</process>\n"
		
		# for each data add all its instances on different processors in a communicator
		if self.nb_proc > 1:
			for d in self.data :
				self.lad += "<communicator>"
				for p in range(0,self.nb_proc):
					self.lad += "<peer instance=\""+d[0]+"_"+str(p)+"\" property=\"comm\"/>"
				self.lad += "</communicator>"
			
		self.lad += "</mpi>\n"
		self.lad += "</lad>\n"
		
		ladFile = open("./outputs/lad"+str(self.nb_proc)+".lad", "w")
		ladFile.write(self.lad)
		ladFile.close()
	#-----------------------
	
	#-----------------------
	# this function builds the complete lad template, from the two files given as input :
	# the data template file
	# the rest of the template file
	# it builds a final complete template with only $proc to substitute
	#-----------------------
	def createTemplate(self):
	#-----------------------
		# 1- dump data
		# recreate the parameter $proc by itself
		self.dico["proc"]="$proc"
		dump_data = ""
		cppref_data = ""
		for d in self.data :
			parseData(self.metaFile,self.dico,d[0])
			#cpp ref data
			cppref_data += "<cppref instance=\""+self.dico["data"]+"_$proc"+"\" property=\"inGo\"/>\n"
			#dump_data
			dump_data += self.data_template.substitute(self.dico)
		dump_data += "\n"
		#print cppref_data
		#print dump_data
		# fill $alldata and $cpprefdata
		parseMetaInf(self.metaFile,self.dico)
		self.dico["nb_proc"] = self.nb_proc
		# $alldata
		self.dico["alldata"] = dump_data
		# $cppref_data to generate
		self.dico["cppref_data"] = cppref_data
		
		# 1- dump computations
		# $computations and $cppref_computations to generate (same for all proc)
		self.dump_comp = ""
		self.cppref_comp = ""
		# when the same component is used more than once, the property names has to be kept
		# save the first instanciation computation index
		self.saveProperty = {}
		
		self.countSync = 0
		if self.dump_type=="hybrid":
			labels = nx.get_node_attributes(self.canonic,'label')
			self.countS = 0
			self.countP = 0
			self.computationDump(labels,self.root)
		elif self.dump_type=="data_base":
			self.computationDumpDataBase()
		
		self.dico["computations"] = self.dump_comp
		self.dico["cppref_computations"] = self.cppref_comp
		#print self.dump_comp
		#print self.cppref_comp
		self.dico["iter"] = self.time
		
		# 3- dump all general template (linked to data and computations)
		self.template = Template(self.templateFile.read())
		#print self.dico
		dump = self.template.substitute(self.dico)
		
		return dump
	#-----------------------	
	
	#-----------------------
	# BFS strategy
	# This function creates the lad dump of the TSP graph computed before
	# each 'S' node is translated to a SEQ component
	# each 'P' node is translated to a PAR component
	# each update computation is translated to a SYNC component
	# each normal computation is translated to its component
	#-----------------------
	def computationDump(self,labels,node):
	#-----------------------
		if labels[node]=='S':
			self.dump_comp += self.makeSequence(labels,node)
			# for the root element it corresponds to cppref_comp, it is linked to Time
			if node==self.root:
				self.cppref_comp = "<cppref instance=\"Seq"+str(self.countS)+"_$proc\" property=\"inGo\"/>\n"
			self.countS += 1
		elif labels[node]=='P':
			self.dump_comp += self.makeParallel(labels,node)
			# for the root element it corresponds to cppref_comp, it is linked to Time
			if node==self.root:
				self.cppref_comp = "<cppref instance=\"Par"+str(self.countP)+"_$proc\" property=\"inGo\"/>\n"
			self.countP += 1
		else :
			if self.computations[node][1]=="update":
				self.dump_comp += self.makeSync(node)
				self.countSync += 1
			else:
				self.dump_comp += self.makeComputation(node)
				
		# print "add node "+labels[node]
		# print self.dump_comp
		
		successors = self.canonic.successors(node)
		# keep the good order of successors
		self.orderSuccessors(successors)
		for succ in successors:
			self.computationDump(labels,succ)
	#-----------------------
	
	#-----------------------
	def computationDumpDataBase(self):
	#-----------------------
		self.dump_comp = "<instance id=\"Seq"+str(0)+"_$proc\" type=\"Sequence\">\n"
		self.dump_comp += "<property id=\"Compute\">\n"
		for comp in range(0,len(self.computations)):
			if self.computations[comp][1]=="update":
				self.dump_comp += self.makeSync(comp)
				self.countSync += 1
			else:
				self.dump_comp += self.makeComputation(comp)
		self.dump_comp += "</property>\n"
		self.dump_comp += "</instance>\n"
		self.cppref_comp = "<cppref instance=\"Seq"+str(0)+"_$proc\" property=\"inGo\"/>\n"
	#-----------------------
	
	#-----------------------
	# Create a sequence component instance in the lad
	#-----------------------
	def makeSequence(self,labels,node):
	#-----------------------
		out = "<instance id=\"Seq"+str(self.countS)+"_$proc\" type=\"Sequence\">\n"
		out += "<property id=\"Compute\">\n"
		successors = self.canonic.successors(node)
		self.orderSuccessors(successors)
		local_countS = self.countS
		local_countP = self.countP
		local_countSync = self.countSync
		for succ in successors:
			if labels[succ]=='S':
				out += "<cppref instance=\"Seq"+str(local_countS)+"_$proc\" property=\"inGo\"/>\n"
				local_countS += 1
			elif labels[succ]=='P':
				out += "<cppref instance=\"Par"+str(local_countP)+"_$proc\" property=\"inGo\"/>\n"
				local_countP += 1
			else:
				if self.computations[succ][1]=="update":
					out += "<cppref instance=\"Sync"+str(local_countSync)+"_$proc\" property=\"inGo\"/>\n"
					local_countSync += 1
				else:
					name = self.computations[succ][0]
					duplicate = int(math.fabs(self.duplicatesRef[name] - self.duplicates[name]))
					out += "<cppref instance=\""+name+"_"+str(duplicate)+"_$proc\" property=\"inGo\"/>\n"
					self.duplicatesRef[name] -= 1
		out += "</property>\n"
		out += "</instance>\n"
		
		return out
	#-----------------------
	
	#-----------------------
	# Create a parallel component instance in the lad
	#-----------------------
	def makeParallel(self,labels,node):
	#-----------------------
		out = "<instance id=\"Par"+str(self.countP)+"_$proc\" type=\"Parallel\">\n"
		out += "<property id=\"Compute\">\n"
		successors = self.canonic.successors(node)
		# print "before"
		# print successors
		self.orderSuccessors(successors)
		# print "after"
		# print successors
		local_countS = self.countS
		local_countP = self.countP
		local_countSync = self.countSync
		for succ in successors:
			if labels[succ]=='S':
				out += "<cppref instance=\"Seq"+str(local_countS)+"_$proc\" property=\"inGo\"/>\n"
				local_countS += 1
			elif labels[succ]=='P':
				out += "<cppref instance=\"Par"+str(local_countP)+"_$proc\" property=\"inGo\"/>\n"
				local_countP += 1
			else:
				if self.computations[succ][1]=="update":
					out += "<cppref instance=\"Sync"+str(local_countSync)+"_$proc\" property=\"inGo\"/>\n"
					local_countSync += 1
				else:
					name = self.computations[succ][0]
					duplicate = int(math.fabs(self.duplicatesRef[name] - self.duplicates[name]))
					out += "<cppref instance=\""+name+"_"+str(duplicate)+"_$proc\" property=\"inGo\"/>\n"
					self.duplicatesRef[name] -= 1
		out += "</property>\n"
		out += "</instance>\n"
		
		return out
	#-----------------------
	
	#-----------------------
	# Create a sync component instance in the lad
	#-----------------------
	def makeSync(self,node):
	#-----------------------
		out = "<instance id=\"Sync"+str(self.countSync)+"_$proc\" type=\"Sync\">\n"
		
		#data read and written
		out += "<property id=\"dataCompute\">\n"
		read = self.computations[node][2]
		# what is written in an update is fake
		for d in read:
			out += "<cppref instance=\""+d+"_$proc\" property=\"services\"/>\n"
		out += "</property>\n"
		out += "</instance>\n"
		
		return out
	#-----------------------
	
	#-----------------------
	# Create a computation component instance in the lad
	#-----------------------
	def makeComputation(self,node):
	#-----------------------
		name = self.computations[node][0]
		duplicate = int(math.fabs((self.duplicatesNew[name]) - self.duplicates[name]))
		# the instance with the name the duplication id and the proc
		# the type with K+name
		out = "<instance id=\""+name+"_"+str(duplicate)+"_$proc\" type=\"K"+name+"\">\n"
		self.duplicatesNew[name] -= 1
		#data read and written
		if name in self.saveProperty :
			read2 = self.computations[self.saveProperty[name]][2]
			written2 = self.computations[self.saveProperty[name]][3]
		else:
			read2 = self.computations[node][2]
			written2 = self.computations[node][3]
			self.saveProperty[name] = node
			
		read = self.computations[node][2]
		written = self.computations[node][3]
		for d,d2 in zip(read,read2):
			# fake link to the update
			if d.find("wu_") < 0 :
				out += "<property id=\""+d2+"\"><cppref instance=\""+d+"_$proc\" property=\"services\"/></property>"
		# for d in read:
		# 	# fake link to the update
		# 	if d.find("wu_") < 0 :
		# 		out += "<property id=\""+d+"\"><cppref instance=\""+d+"_$proc\" property=\"services\"/></property>"
		# 
		if written not in read :
			out += "<property id=\""+written2+"\"><cppref instance=\""+written+"_$proc\" property=\"services\"/></property>"
			#out += "<property id=\""+written+"\"><cppref instance=\""+written+"_$proc\" property=\"services\"/></property>"
		out += "</instance>\n"
		return out
	#-----------------------
	
#===============================================================



#===============================================================
def printHelp():
	print "help"
#===============================================================	



#===============================================================
# Main entry
#===============================================================
#-----------------------
def main():
#-----------------------
	# for i in range(0,len(sys.argv),2):
	# 	if sys.argv[i]=="-msp":
	# 		mspFile = sys.argv[i+1]
	# 	if sys.argv[i]=="--help":
	# 		printHelp()
	# 		break
	# 	else :
	# 		print "argument "+sys.argv[i]+" is not a valid argument"
	# 		printHelp()
	
    compiler = CAC_Compiler("./inputs/SW.msp","./inputs/SW.meta.inf","./inputs/template_skelgis.xml","./inputs/template_skelgis_data.xml")
    compiler.compile()
#-----------------------

#-----------------------
if __name__ == "__main__":
	main()
#-----------------------
