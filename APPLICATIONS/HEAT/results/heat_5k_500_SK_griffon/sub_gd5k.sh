#!/bin/bash

export LIBRARY_PATH=/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/lib/:/home/hcoullon/Heat_v1/install/lib:/home/hcoullon/L2C/INSTALL_L2C/lib/:/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/lib/:/home/hcoullon/L2C/omniORB-4.2.0/INSTALL/lib/:/home/hcoullon/L2C/libtinyxml/INSTALL/lib/
export LD_LIBRARY_PATH=/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/lib/:/home/hcoullon/Heat_v1/install/lib:/home/hcoullon/L2C/INSTALL_L2C/lib/:/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/lib/:/home/hcoullon/L2C/omniORB-4.2.0/INSTALL/lib/:/home/hcoullon/L2C/libtinyxml/INSTALL/lib/
export CPATH=/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/include/:/home/hcoullon/L2C/INSTALL_L2C/include/:/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/include/:/home/hcoullon/L2C/omniORB-4.2.0/INSTALL/include/:/home/hcoullon/L2C/libtinyxml/INSTALL/include
export PATH=/home/hcoullon/L2C/cmake-3.1.0-rc1/INSTALL/bin/:/home/hcoullon/L2C/cmake-3.1.0-rc1/INSTALL/bin/:/usr/local/bin:/usr/bin:/bin:/grid5000/code/bin:/home/hcoullon/L2C/pkgconfig-0.6.0/INSTALL_LN/bin/:/home/hcoullon/L2C/omniORB-4.2.0/INSTALL/bin/:/home/hcoullon/L2C/INSTALL_L2C/bin/:/home/hcoullon/L2C/pkgconfig-0.6.0/INSTALL_LN/bin/:/home/hcoullon/L2C/omniORB-4.2.0/INSTALL/bin/:/home/hcoullon/L2C/INSTALL_L2C/bin/
export BOOST_ROOT=/home/hcoullon/L2C/boost_1_56_0/INSTALL_LN/
export OMNI_DIR=/home/hcoullon/L2C/omniORB-4.2.0/INSTALL/
export TINYXML_DIR=/home/hcoullon/L2C/libtinyxml/INSTALL/
cd /home/hcoullon/Heat_v1/benchmarks/heat_5k_500

l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 1 lad/heat_5k_500_1.lad > outputs/heat_5k_500_11.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 1 lad/heat_5k_500_1.lad > outputs/heat_5k_500_12.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 1 lad/heat_5k_500_1.lad > outputs/heat_5k_500_13.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 1 lad/heat_5k_500_1.lad > outputs/heat_5k_500_14.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 2 lad/heat_5k_500_2.lad > outputs/heat_5k_500_21.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 2 lad/heat_5k_500_2.lad > outputs/heat_5k_500_22.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 2 lad/heat_5k_500_2.lad > outputs/heat_5k_500_23.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 2 lad/heat_5k_500_2.lad > outputs/heat_5k_500_24.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 4 lad/heat_5k_500_4.lad > outputs/heat_5k_500_41.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 4 lad/heat_5k_500_4.lad > outputs/heat_5k_500_42.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 4 lad/heat_5k_500_4.lad > outputs/heat_5k_500_43.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 4 lad/heat_5k_500_4.lad > outputs/heat_5k_500_44.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 8 lad/heat_5k_500_8.lad > outputs/heat_5k_500_81.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 8 lad/heat_5k_500_8.lad > outputs/heat_5k_500_82.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 8 lad/heat_5k_500_8.lad > outputs/heat_5k_500_83.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 8 lad/heat_5k_500_8.lad > outputs/heat_5k_500_84.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 16 lad/heat_5k_500_16.lad > outputs/heat_5k_500_161.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 16 lad/heat_5k_500_16.lad > outputs/heat_5k_500_162.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 16 lad/heat_5k_500_16.lad > outputs/heat_5k_500_163.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 16 lad/heat_5k_500_16.lad > outputs/heat_5k_500_164.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 32 lad/heat_5k_500_32.lad > outputs/heat_5k_500_321.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 32 lad/heat_5k_500_32.lad > outputs/heat_5k_500_322.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 32 lad/heat_5k_500_32.lad > outputs/heat_5k_500_323.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 32 lad/heat_5k_500_32.lad > outputs/heat_5k_500_324.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 64 lad/heat_5k_500_64.lad > outputs/heat_5k_500_641.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 64 lad/heat_5k_500_64.lad > outputs/heat_5k_500_642.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 64 lad/heat_5k_500_64.lad > outputs/heat_5k_500_643.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 64 lad/heat_5k_500_64.lad > outputs/heat_5k_500_644.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 128 lad/heat_5k_500_128.lad > outputs/heat_5k_500_1281.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 128 lad/heat_5k_500_128.lad > outputs/heat_5k_500_1282.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 128 lad/heat_5k_500_128.lad > outputs/heat_5k_500_1283.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 128 lad/heat_5k_500_128.lad > outputs/heat_5k_500_1284.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 256 lad/heat_5k_500_256.lad > outputs/heat_5k_500_2561.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 256 lad/heat_5k_500_256.lad > outputs/heat_5k_500_2562.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 256 lad/heat_5k_500_256.lad > outputs/heat_5k_500_2563.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 256 lad/heat_5k_500_256.lad > outputs/heat_5k_500_2564.o
l2c_loader --mpi -M,--mca,plm_rsh_agent,"oarsh",-machinefile,$OAR_NODEFILE -c 512 lad/heat_5k_500_512.lad > outputs/heat_5k_500_5121.o
#l2c_loader --mpi -c 512 lad/heat_5k_500_512.lad > outputs/heat_5k_500_5122.o
#l2c_loader --mpi -c 512 lad/heat_5k_500_512.lad > outputs/heat_5k_500_5123.o
#l2c_loader --mpi -c 512 lad/heat_5k_500_512.lad > outputs/heat_5k_500_5124.o
