#!/bin/bash
#MSUB -r heat_5k_5k_32         #job name
#MSUB -n 32                    #number of nodes
#MSUB -T 300                #time out
#MSUB -o outputs/heat_5k_5k_32_%I.o    #output file
#MSUB -e outputs/heat_5k_5k_32_%I.e    #error file
#MSUB -q standard
#MSUB -A gen7317              #project ID

set -x
cd ~/use-local
source load-env-classic-l2c.sh
cd ${BRIDGE_MSUB_PWD}
export LD_LIBRARY_PATH=~/helene/HEAT/src-l2c/install/lib/:$LD_LIBRARY_PATH
module load mpiprofile/bullxmpi/performance
l2c_loader --mpi -c ${BRIDGE_MSUB_NPROC} lad/heat_5k_5k_32.lad
