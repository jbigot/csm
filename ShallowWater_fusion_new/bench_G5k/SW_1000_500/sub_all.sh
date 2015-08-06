#!/bin/bash

export LIBRARY_PATH=/home/hcoullon/LIBRARIES/boost_1_56_0/INSTALL_LN:$LIBRARY_PATH
export LD_LIBRARY_PATH=/home/hcoullon/LIBRARIES/boost_1_56_0/INSTALL_LN/lib/:$LD_LIBRARY_PATH
export CPATH=/home/hcoullon/LIBRARIES/boost_1_56_0/INSTALL_LN/include/:$CPATH
export PATH=/home/hcoullon/LIBRARIES/pkg-config-0.18.1/INSTALL_LN/bin/:/home/hcoullon/LIBRARIES/cmake-3.3.0/INSTALL/bin/:/usr/local/bin:/usr/bin:/bin:/grid5000/code/bin:$PATH
export BOOST_ROOT=/home/hcoullon/LIBRARIES/boost_1_56_0/INSTALL_LN/

# export for L2C
export LIBRARY_PATH=/home/hcoullon/LIBRARIES/L2C/INSTALL/lib/:$LIBRARY_PATH
export LD_LIBRARY_PATH=/home/hcoullon/LIBRARIES/L2C/INSTALL/lib/:$LD_LIBRARY_PATH
export CPATH=/home/hcoullon/LIBRARIES/L2C/INSTALL/include:$CPATH
export PATH=/home/hcoullon/LIBRARIES/L2C/INSTALL/bin/:$PATH

# export for applications                                                                                              
export LIBRARY_PATH=/home/hcoullon/APPLICATIONS/ShallowWater_fusion_new/install/lib/:$LIBRARY_PATH
export LD_LIBRARY_PATH=/home/hcoullon/APPLICATIONS/ShallowWater_fusion_new/install/lib/:$LD_LIBRARY_PATH

l2c_loader --mpi -c 1 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_1.lad > FusNew_1000_500_1_1.o 2>&1
l2c_loader --mpi -c 1 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_1.lad > FusNew_1000_500_2_1.o 2>&1
l2c_loader --mpi -c 2 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_2.lad > FusNew_1000_500_1_2.o 2>&1
l2c_loader --mpi -c 2 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_2.lad > FusNew_1000_500_2_2.o 2>&1
l2c_loader --mpi -c 4 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_4.lad > FusNew_1000_500_1_4.o 2>&1
l2c_loader --mpi -c 4 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_4.lad > FusNew_1000_500_2_4.o 2>&1
l2c_loader --mpi -c 8 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_8.lad > FusNew_1000_500_1_8.o 2>&1
l2c_loader --mpi -c 8 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_8.lad > FusNew_1000_500_2_8.o 2>&1
l2c_loader --mpi -c 16 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_16.lad > FusNew_1000_500_1_16.o 2>&1
l2c_loader --mpi -c 16 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_16.lad > FusNew_1000_500_2_16.o 2>&1
l2c_loader --mpi -c 32 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_32.lad > FusNew_1000_500_1_32.o 2>&1
l2c_loader --mpi -c 32 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_32.lad > FusNew_1000_500_2_32.o 2>&1
l2c_loader --mpi -c 64 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_64.lad > FusNew_1000_500_1_64.o 2>&1
l2c_loader --mpi -c 64 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_64.lad > FusNew_1000_500_2_64.o 2>&1
l2c_loader --mpi -c 128 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_128.lad > FusNew_1000_500_1_128.o 2>&1
l2c_loader --mpi -c 128 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_128.lad > FusNew_1000_500_2_128.o 2>&1
l2c_loader --mpi -c 256 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_256.lad > FusNew_1000_500_1_256.o 2>&1
l2c_loader --mpi -c 256 -M,-machinefile,$OAR_NODEFILE,--mca,plm_rsh_agent,"oarsh" ../../lad/FusNew_1000_500_256.lad > FusNew_1000_500_2_256.o 2>&1