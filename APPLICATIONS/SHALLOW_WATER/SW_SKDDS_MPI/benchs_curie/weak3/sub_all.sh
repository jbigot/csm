#!/bin/bash

for i in `seq 1 5`;
do
    cd SK16
    ccc_msub FSSK_3200_200_16.sh
    cd ..
    cd SK32
    ccc_msub FSSK_3200_200_32.sh
    cd ..
    cd SK64
    ccc_msub FSSK_6400_200_64.sh
    cd ..
    cd SK128
    ccc_msub FSSK_6400_200_128.sh
    cd ..
    cd SK256
    ccc_msub FSSK_12800_200_256.sh
    cd ..
    cd SK512
    ccc_msub FSSK_12800_200_512.sh
    cd ..
    cd SK1024
    ccc_msub FSSK_25600_200_1024.sh
    cd ..
    cd SK2048
    ccc_msub FSSK_25600_200_2048.sh
    cd ..
    cd SK4096
    ccc_msub FSSK_51200_200_4096.sh
    cd ..
    cd SK8192
    ccc_msub FSSK_51200_200_8192.sh
    cd ..
    cd SK16384
    ccc_msub FSSK_102400_200_16384.sh
    cd ..
done