#!/bin/bash

cd heat_5k_5k
for i in 1 2 3 4
do
	ccc_msub heat_5k_5k_1.sh
	ccc_msub heat_5k_5k_2.sh
	ccc_msub heat_5k_5k_4.sh
	ccc_msub heat_5k_5k_8.sh
	ccc_msub heat_5k_5k_16.sh
	ccc_msub heat_5k_5k_32.sh
	ccc_msub heat_5k_5k_64.sh
	ccc_msub heat_5k_5k_128.sh
	ccc_msub heat_5k_5k_256.sh
	ccc_msub heat_5k_5k_512.sh
	ccc_msub heat_5k_5k_1024.sh
	ccc_msub heat_5k_5k_2048.sh
	ccc_msub heat_5k_5k_4096.sh
done

cd ../heat_5k_500
for i in 1 2 3 4
do
	ccc_msub heat_5k_500_1.sh
	ccc_msub heat_5k_500_2.sh
	ccc_msub heat_5k_500_4.sh
	ccc_msub heat_5k_500_8.sh
	ccc_msub heat_5k_500_16.sh
	ccc_msub heat_5k_500_32.sh
	ccc_msub heat_5k_500_64.sh
	ccc_msub heat_5k_500_128.sh
	ccc_msub heat_5k_500_256.sh
	ccc_msub heat_5k_500_512.sh
	ccc_msub heat_5k_5k_1024.sh
	ccc_msub heat_5k_5k_2048.sh
done

cd ../heat_10k_5k
for i in 1 2 3 4
do
	ccc_msub heat_10k_5k_1.sh
	ccc_msub heat_10k_5k_4.sh
	ccc_msub heat_10k_5k_16.sh
	ccc_msub heat_10k_5k_64.sh
	ccc_msub heat_10k_5k_128.sh
	ccc_msub heat_10k_5k_256.sh
	ccc_msub heat_10k_5k_512.sh
	ccc_msub heat_10k_5k_1024.sh
	ccc_msub heat_10k_5k_2048.sh
	ccc_msub heat_10k_5k_4096.sh
	ccc_msub heat_10k_5k_8192.sh
done

cd ../heat_20k_5k
for i in 1 2 3 4
do
	ccc_msub heat_20k_5k_4.sh
	ccc_msub heat_20k_5k_16.sh
	ccc_msub heat_20k_5k_64.sh
	ccc_msub heat_20k_5k_128.sh
	ccc_msub heat_20k_5k_256.sh
	ccc_msub heat_20k_5k_512.sh
	ccc_msub heat_20k_5k_1024.sh
	ccc_msub heat_20k_5k_2048.sh
	ccc_msub heat_20k_5k_4096.sh
	ccc_msub heat_20k_5k_8192.sh
	ccc_msub heat_20k_5k_16384.sh
done

cd ../heat_40k_10k
for i in 1 2 3 4
do
 	ccc_msub heat_40k_10k_16.sh
 	ccc_msub heat_40k_10k_64.sh
 	ccc_msub heat_40k_10k_128.sh
 	ccc_msub heat_40k_10k_256.sh
 	ccc_msub heat_40k_10k_512.sh
 	ccc_msub heat_40k_10k_1024.sh
 	ccc_msub heat_40k_10k_2048.sh
 	ccc_msub heat_40k_10k_4096.sh
	ccc_msub heat_40k_10k_8192.sh
	ccc_msub heat_40k_10k_16384.sh
 done
