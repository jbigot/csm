#!/bin/bash
#MSUB -r weakSK_25600_500_8192         #job name
#MSUB -n 8192                     #number of nodes
#MSUB -T 500                  #time out
#MSUB -o $WORKDIR/results/weak_scaling/400/SKMPI/weakSK_25600_500_8192_%I.o    #output file
#MSUB -e $WORKDIR/results/weak_scaling/400/SKMPI/weakSK_25600_500_8192_%I.e    #error file
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

module load c++/gnu/4.9.1
module load c/gnu/4.9.1
export OMPI_CC=gcc
export OMPI_CXX=g++
#module load mpiprofile/bullxmpi/performance
#ccc_mprun -n ${BRIDGE_MSUB_NPROC} valgrind --leak-check=full --log-file=/ccc/scratch/cont003/gen7470/coullohe/benchs_curie/FSSK/5K_500/leaks_%p.o ../../bin/FullSWOF_2D
ccc_mprun -n ${BRIDGE_MSUB_NPROC} ../../../bin/FullSWOF_2D