#===============================================================
# Helene Coullon
# LIP INRIA
# Types of Python MSP_CAC
#===============================================================

#===============================================================
# Imports
#===============================================================
import sys
import os
sys.path.append(os.path.dirname("./utils/"))
from cac_import import *

Computation = namedtuple("Computation", "id type R w N")
Data = namedtuple("Data", "id domain")