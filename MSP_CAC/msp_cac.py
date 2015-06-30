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
# Class msp_to_tsp
# This class represent the actual compiler from the DSL to 
# the series-parallel tree decomposition.
# This tree decomposition can then be dumped to different solutions.
# INPUT = file to parse in the msp language
# OUTPUT = series-parallel tree decomposition and intermediate graphs
#===============================================================
class msp_to_tsp:
	#-----------------------
	def __init__(self,file):
	#-----------------------
		self.pfile = file
	#-----------------------

	#-----------------------
	def compile(self):
	#-----------------------
		# parse the input file
		self.read_dsl()
		# compute gamma data : separate updates
		#self.gamma_data() #it seems it does not work for SW entirely
		# compute gamma data : do not separate updates
		self.gamma_data_seq()
		# compute gamma hybrid
		self.gamma_hyb()
		# first transitive reduction
		self.transitive_reduction(self.gamma)
		##### DRAW
		#drawGamma(self.gamma,"./outputs/trans.dot")
		# compute sources/roots of self.gamma
		self.sources()
		# remove N shapes
		#self.nshape = self.gamma.copy()
		self.nshape = self.gamma
		self.level = 0
		self.remove_nshape(self.sources)
		#self.transitive_reduction(self.nshape)
		##### DRAW
		drawGamma(self.nshape,"./outputs/nshape.dot")
		# series-parallel tree decomposition
		self.tsp()
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
		
		#see parser.py for more details
		parse(self.pfile,self.mesh,self.data,self.time,self.computations)
		
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
	#-----------------------
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
					name_wu = "wu_"+self.computations[current][0]+"_"+str(counter)
					self.computations.insert(current,Computation("upd_"+self.computations[current][0]+"_"+str(counter),"update",set(up),name_wu,""))
					toadd.add(name_wu)
					counter = counter+1
					current = current+1
				for el in toadd:
					self.computations[current][2].add(el)
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
	#-----------------------
	def gamma_data_seq(self):
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
				name_wu = "wu_"+self.computations[current][0]
				self.computations.insert(current,Computation("upd_"+self.computations[current][0],"update",toupdate,name_wu,""))
				current = current+1
				self.computations[current][2].add(name_wu)
				
			current = current+1
				
		##### PRINT
		#for elem in self.computations:
			#print elem
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
		#drawGamma(self.gamma,"./outputs/dag.dot")
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
		#drawGamma(self.gamma,"./outputs/trans.dot")
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
	# recursive BFS by successors O(n)+O(n^2)
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
		
		#remove from sources the nodes also present in destination
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
		#else :
			#print "End of graph"
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
	# O(n^3) ou O(n^4) WRONG !!!
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
		
		##### DRAW
		drawInverse(self.inverse,"./outputs/inverse.dot")
		
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
			lab_graph.add_node(i,label=self.computations[i][0])
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
		nx.draw_graphviz(self.toReduce[self.startEdge])
		nx.write_dot(self.toReduce[self.startEdge],"./outputs/tsp.dot")
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
		#add sequence node and edges
		new_graph.add_node(self.lab_index,label='S')
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
		
		#print self.inverse.edges()
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
		#add sequence node and edges
		new_graph.add_node(self.lab_index,label='P')
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
#===============================================================
# Class dump
# 
#===============================================================
#class dump:

#===============================================================
# Main entry
#===============================================================
if __name__ == "__main__":
    compiler = msp_to_tsp("./SW.msp")
    compiler.compile()
