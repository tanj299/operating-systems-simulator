# Operating Systems Simulator


## ABOUT:
The purpose of this project is to simulate an operating system and listen to commands from users. The project uses a first-come, first-serve preemptive CPU scheduling algorithm. Initially, the project will ask the user two questions:

a) How much memory should be allocated? (Up to 4 billion bytes)
b) How many hard disks exist

The program will start listening for command input by users including:

#### Create new process: 
```
>> A <INT PRIORITY> <LONG MEMORY>
```

#### Move current process to hard disk number to "perform" I/O operations with file (read/write)
```
>> d <INT HARD-DISK-NUMBER> <STRING FILE-NAME>
```

#### Current process in CPU exits: 
```
>> exit
```

#### Process completes I/O operations with file
```
>> D <INT HARD-DISK-NUMBER>
```

#### Copy current process using CPU: 
```
>> fork
```

#### Display current process and ready queue: 
```
>> S r 
```

#### Display current hard disks and the process with the files each process is using: 
```
>> S i
```

#### Display all processes and memory allocation:

```
>> S m
```


### Compiled on: gcc (Ubuntu 7.4.0-1ubuntu-18.04.1) 7.4.0

### To compile program:

1) Unzip and extract files into a convenient directory; ex: the Desktop, Documents, home, etc.

2) Change directory into /operating-systems-simulator

3) In terminal, change directory into ../operating-systems-simulator

4) Use command:
	make clean
	make
	./output
