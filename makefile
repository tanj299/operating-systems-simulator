output: Process.o Containers.o HardDisk.o main.o
	g++ Process.o Containers.o HardDisk.o main.o -o output

Process.o: Process.cpp
	g++ -std=c++11 -c Process.cpp

Containers.o: Containers.cpp
	g++ -std=c++11 -c Containers.cpp

HardDisk.o: HardDisk.cpp
	g++ -std=c++11 -c HardDisk.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp
		
clean: 
	rm *.o output

