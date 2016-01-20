#!/bin/bash
#MSUB -r heat_40k_10k_256         #job name
#MSUB -n 256                    #number of nodes
#MSUB -T 5000                #time out
#MSUB -o outputs/heat_40k_10k_256_%I.o    #output file
#MSUB -e outputs/heat_40k_10k_256_%I.e    #error file
#MSUB -q standard
#MSUB -A gen7317              #project ID

set -x
cd ~/use-local
source load-env-classic-l2c.sh
cd ${BRIDGE_MSUB_PWD}
export LD_LIBRARY_PATH=~/helene/HEAT/Heat_v1/install/lib/:$LD_LIBRARY_PATH
module load mpiprofile/bullxmpi/performance
l2c_loader --mpi -c ${BRIDGE_MSUB_NPROC} lad/heat_40k_10k_256.lad
