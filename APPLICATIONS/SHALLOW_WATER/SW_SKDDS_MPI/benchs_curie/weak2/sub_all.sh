#!/bin/bash

for i in `seq 1 10`;
do
#    cd SK1
#    ccc_msub FSSK_400_500_1.sh
#    cd ..
#    cd SK2
#    ccc_msub FSSK_400_500_2.sh
#    cd ..
#    cd SK4
#    ccc_msub FSSK_800_500_4.sh
#    cd ..
#    cd SK8
#    ccc_msub FSSK_800_500_8.sh
#    cd ..
    cd SK16
    ccc_msub FSSK_1600_500_16.sh
    cd ..
    cd SK32
    ccc_msub FSSK_1600_500_32.sh
    cd ..
    cd SK64
    ccc_msub FSSK_3200_500_64.sh
    cd ..
    cd SK128
    ccc_msub FSSK_3200_500_128.sh
    cd ..
    cd SK256
    ccc_msub FSSK_6400_500_256.sh
    cd ..
    cd SK512
    ccc_msub FSSK_6400_500_512.sh
    cd ..
    cd SK1024
    ccc_msub FSSK_12800_500_1024.sh
    cd ..
    cd SK2048
    ccc_msub FSSK_12800_500_2048.sh
    cd ..
    cd SK4096
    ccc_msub FSSK_25600_500_4096.sh
    cd ..
    cd SK8192
    ccc_msub FSSK_25600_500_8192.sh
    cd ..
    cd SK16384
    ccc_msub FSSK_51200_500_16384.sh
    cd ..
done