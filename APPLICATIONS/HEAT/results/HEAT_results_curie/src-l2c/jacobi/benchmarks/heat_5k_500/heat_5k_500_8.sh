#!/bin/bash
#MSUB -r heat_5k_500_8         #job name
#MSUB -n 8                     #number of nodes
#MSUB -T 100                  #time out
#MSUB -o outputs/heat_5k_500_8_%I.o    #output file
#MSUB -e outputs/heat_5k_500_8_%I.e    #error file
#MSUB -q standard
#MSUB -A gen7317               #project ID

set -x
cd ~/use-local
source load-env-classic-l2c.sh
cd ${BRIDGE_MSUB_PWD}
export LD_LIBRARY_PATH=~/helene/HEAT/src-l2c/install/lib/:$LD_LIBRARY_PATH
module load mpiprofile/bullxmpi/performance
l2c_loader --mpi -c ${BRIDGE_MSUB_NPROC} lad/heat_5k_500_8.lad
