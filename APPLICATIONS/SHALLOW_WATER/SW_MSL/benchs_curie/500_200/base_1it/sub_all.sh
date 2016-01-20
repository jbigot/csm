#!/bin/bash

for i in `seq 1 10`;
do
    ccc_msub NF_MSL_500_1_1.sh
    ccc_msub NF_MSL_500_1_2.sh
    ccc_msub NF_MSL_500_1_4.sh
    ccc_msub NF_MSL_500_1_8.sh
    ccc_msub NF_MSL_500_1_16.sh
    ccc_msub NF_MSL_500_1_32.sh
    ccc_msub NF_MSL_500_1_64.sh
    ccc_msub NF_MSL_500_1_128.sh
    ccc_msub NF_MSL_500_1_256.sh
    ccc_msub NF_MSL_500_1_512.sh
    ccc_msub NF_MSL_500_1_1024.sh
    ccc_msub NF_MSL_500_1_2048.sh
#    ccc_msub NF_MSL_500_1_4096.sh
done