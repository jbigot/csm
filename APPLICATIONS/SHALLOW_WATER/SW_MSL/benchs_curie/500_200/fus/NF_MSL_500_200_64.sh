#!/bin/bash
#MSUB -r fus_MSL_500_200_64         #job name
#MSUB -n 64                     #number of nodes
#MSUB -T 500                  #time out
#MSUB -o $WORKDIR/results/task_scaling/500_200/fus/fus_MSL_500_200_64_%I.o    #output file
#MSUB -e $WORKDIR/results/task_scaling/500_200/fus/fus_MSL_500_200_64_%I.e    #error file
#MSUB -q standard
#MSUB -A gen7470               #project ID

set -x
cd ${BRIDGE_MSUB_PWD}

# BOOST
export BOOST_ROOT=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/boost_1_58_0/INSTALL_LN/
export LD_LIBRARY_PATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/boost_1_58_0/INSTALL_LN/lib/:$LD_LIBRARY_PATH
export LIBRARY_PATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/boost_1_58_0/INSTALL_LN/lib/:$LIBRARY_PATH
export CPATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/boost_1_58_0/INSTALL_LN/include/:$CPATH

# L2C
export LD_LIBRARY_PATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/L2C/INSTALL/lib/:$LD_LIBRARY_PATH
export LIBRARY_PATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/L2C/INSTALL/lib/:$LIBRARY_PATH
export CPATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/L2C/INSTALL/include/:$CPATH
export PATH=/ccc/cont003/home/gen7470/coullohe/LIBRARIES/L2C/INSTALL/bin/:$PATH

# APP
export LD_LIBRARY_PATH=/ccc/cont003/home/gen7470/coullohe/APPLICATIONS/git/ShallowWater_fusion_new/install/lib/:$LD_LIBRARY_PATH
export LIBRARY_PATH=/ccc/cont003/home/gen7470/coullohe/APPLICATIONS/git/ShallowWater_fusion_new/install/lib/:$LIBRARY_PATH
export CPATH=/ccc/cont003/home/gen7470/coullohe/APPLICATIONS/git/ShallowWater_fusion_new/install/include/:$CPATH

module load c++/gnu/4.9.1
module load c/gnu/4.9.1
export OMPI_CC=gcc
export OMPI_CXX=g++
#module load mpiprofile/bullxmpi/performance
ccc_mprun -n ${BRIDGE_MSUB_NPROC} /ccc/cont003/home/gen7470/coullohe/LIBRARIES/L2C/INSTALL/bin/llcmcpp_loader  -- /ccc/cont003/home/gen7470/coullohe/LIBRARIES/L2C/INSTALL/lib/lad/l2c/cppmpiloader.lad -- $SCRATCHDIR/ladfiles/fus_500_200_64.lad
