#===============================================================
# Helene Coullon
# LIP INRIA
# Drawer of Python MSP_CAC
#===============================================================

#===============================================================
# Imports
#===============================================================
import sys
import os
sys.path.append(os.path.dirname("./utils/"))
from cac_types import *

import matplotlib.pyplot as plt

#===============================================================
# Funtions
#===============================================================
#-----------------------
def drawGamma(graph,pfile):
#-----------------------
    nx.draw_graphviz(graph)
    nx.write_dot(graph,pfile)
    
#-----------------------
def drawInverse(graph,pfile):
#-----------------------
    nx.draw_graphviz(graph)
    nx.write_dot(graph,pfile)