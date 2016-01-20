#!/bin/bash

for i in `seq 1 5`;
do
    ccc_msub taskMSL_1k_500_16.sh
    ccc_msub taskMSL_1k_500_32.sh
    ccc_msub taskMSL_1k_500_64.sh
    ccc_msub taskMSL_1k_500_128.sh
    ccc_msub taskMSL_1k_500_256.sh
    ccc_msub taskMSL_1k_500_512.sh
    ccc_msub taskMSL_1k_500_1024.sh
    ccc_msub taskMSL_1k_500_2048.sh
    ccc_msub taskMSL_1k_500_4096.sh
    ccc_msub taskMSL_1k_500_8192.sh
done