#!/bin/bash

for i in `seq 1 10`;
do
#    ccc_msub weakMSL_400_500_1.sh
#    ccc_msub weakMSL_400_500_2.sh
#    ccc_msub weakMSL_800_500_4.sh
#    ccc_msub weakMSL_800_500_8.sh
    ccc_msub weakMSL_1600_500_16.sh
    ccc_msub weakMSL_1600_500_32.sh
    ccc_msub weakMSL_3200_500_64.sh
    ccc_msub weakMSL_3200_500_128.sh
    ccc_msub weakMSL_6400_500_256.sh
    ccc_msub weakMSL_6400_500_512.sh
    ccc_msub weakMSL_12800_500_1024.sh
    ccc_msub weakMSL_12800_500_2048.sh
    ccc_msub weakMSL_25600_500_4096.sh
    ccc_msub weakMSL_25600_500_8192.sh
    ccc_msub weakMSL_51200_500_16384.sh
done