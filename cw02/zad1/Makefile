all: compile 4bytes1000000 4bytes2500000 512bytes10000 512bytes25000 4096bytes10000 4096bytes25000 8192bytes10000 8192bytes25000

compile:
	gcc -Wall -std=gnu11 main.c -o main

4bytes1000000:
	echo System generate 1000000 records 4 bytes each \ > wyniki.txt
	time ./main sys generate datafile 1000000 4 2>> wyniki.txt
	cp datafile datafile1
	echo System shuffle 1000000 records 4 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 1000000 4 2>> wyniki.txt
	echo System sort 1000000 records 4 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 1000000 4 2>> wyniki.txt
	echo Library shuffle 1000000 records 4 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 1000000 4 2>> wyniki.txt
	echo Library sort 1000000 records 4 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 1000000 4 2>> wyniki.txt
4bytes2500000:
	echo System generate 2500000 records 4 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 2500000 4 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 2500000 records 4 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 2500000 4 2>> wyniki.txt
	echo System sort 2500000 records 4 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 2500000 4 2>> wyniki.txt
	echo Library shuffle 2500000 records 4 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 2500000 4 2>> wyniki.txt
	echo Library sort 2500000 records 4 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 2500000 4 2>> wyniki.txt
512bytes10000:
	echo System generate 10000 records 512 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 10000 512 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 10000 records 512 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 10000 512 2>> wyniki.txt
	echo System sort 10000 records 512 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 10000 512 2>> wyniki.txt
	echo Library shuffle 10000 records 512 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 10000 512 2>> wyniki.txt
	echo Library sort 10000 records 512 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 10000 512 2>> wyniki.txt
512bytes25000:
	echo System generate 25000 records 512 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 25000 512 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 25000 records 512 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 25000 512 2>> wyniki.txt
	echo System sort 25000 records 512 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 25000 512 2>> wyniki.txt
	echo Library shuffle 25000 records 512 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 25000 512 2>> wyniki.txt
	echo Library sort 25000 records 512 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 25000 512 2>> wyniki.txt
4096bytes10000:
	echo System generate 10000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 10000 4096 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 10000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 10000 4096 2>> wyniki.txt
	echo System sort 10000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 10000 4096 2>> wyniki.txt
	echo Library shuffle 10000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 10000 4096 2>> wyniki.txt
	echo Library sort 10000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 10000 4096 2>> wyniki.txt
4096bytes25000:
	echo System generate 25000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 25000 4096 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 25000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 25000 4096 2>> wyniki.txt
	echo System sort 25000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 25000 4096 2>> wyniki.txt
	echo Library shuffle 25000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 25000 4096 2>> wyniki.txt
	echo Library sort 25000 records 4096 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 25000 4096 2>> wyniki.txt
8192bytes10000:
	echo System generate 10000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 10000 8192 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 10000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 10000 8192 2>> wyniki.txt
	echo System sort 10000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 10000 8192 2>> wyniki.txt
	echo Library shuffle 10000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 10000 8192 2>> wyniki.txt
	echo Library sort 10000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 10000 8192 2>> wyniki.txt
8192bytes25000:
	echo System generate 25000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys generate datafile 25000 8192 2>> wyniki.txt
	cp datafile dafatile1
	echo System shuffle 25000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile 25000 8192 2>> wyniki.txt
	echo System sort 25000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys sort datafile 25000 8192 2>> wyniki.txt
	echo Library shuffle 25000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys shuffle datafile1 25000 8192 2>> wyniki.txt
	echo Library sort 25000 records 8192 bytes each \ >> wyniki.txt
	time ./main sys sort datafile1 25000 8192 2>> wyniki.txt
