#!/bin/bash

for i in `seq 1 5`;
do
    ccc_msub weakMSL_3200_200_16.sh
    ccc_msub weakMSL_3200_200_32.sh
    ccc_msub weakMSL_6400_200_64.sh
    ccc_msub weakMSL_6400_200_128.sh
    ccc_msub weakMSL_12800_200_256.sh
    ccc_msub weakMSL_12800_200_512.sh
    ccc_msub weakMSL_25600_200_1024.sh
    ccc_msub weakMSL_25600_200_2048.sh
    ccc_msub weakMSL_51200_200_4096.sh
    ccc_msub weakMSL_51200_200_8192.sh
    ccc_msub weakMSL_102400_200_16384.sh
done