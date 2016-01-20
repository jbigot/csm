#!/bin/bash
#MSUB -r heat_20k_5k_1024         #job name
#MSUB -n 1024                    #number of nodes
#MSUB -T 400                #time out
#MSUB -o outputs/heat_20k_5k_1024_%I.o    #output file
#MSUB -e outputs/heat_20k_5k_1024_%I.e    #error file
#MSUB -q standard
#MSUB -A gen7317              #project ID

set -x
cd ~/use-local
source load-env-classic-l2c.sh
cd ${BRIDGE_MSUB_PWD}
export LD_LIBRARY_PATH=~/helene/HEAT/Heat_v1/install/lib/:$LD_LIBRARY_PATH
module load mpiprofile/bullxmpi/performance
l2c_loader --mpi -c ${BRIDGE_MSUB_NPROC} lad/heat_20k_5k_1024.lad
