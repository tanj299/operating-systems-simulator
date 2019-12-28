output: Process.o Containers.o HardDisk.o main.o
	g++ Process.o Containers.o HardDisk.o main.o -o output

Process.o: Process.cpp
	g++ -c Process.cpp

Containers.o: Containers.cpp
	g++ -c Containers.cpp

HardDisk.o: HardDisk.cpp
	g++ -c HardDisk.cpp

main.o: main.cpp
	g++ -c main.cpp
		
clean: 
	rm *.o output

