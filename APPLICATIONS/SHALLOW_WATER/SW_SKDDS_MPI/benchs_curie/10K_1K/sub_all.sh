#!/bin/bash

for i in `seq 1 5`;
do
    ccc_msub strongSK_10K_1K_32.sh
    ccc_msub strongSK_10K_1K_64.sh
    ccc_msub strongSK_10K_1K_128.sh
    ccc_msub strongSK_10K_1K_256.sh
    ccc_msub strongSK_10K_1K_512.sh
    ccc_msub strongSK_10K_1K_1024.sh
    ccc_msub strongSK_10K_1K_2048.sh
    ccc_msub strongSK_10K_1K_4096.sh
    ccc_msub strongSK_10K_1K_8192.sh
    ccc_msub strongSK_10K_1K_16384.sh
done