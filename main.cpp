#include <iostream>
#include <deque>
#include <sstream>
#include <utility>
#include "Process.h"
#include "Containers.h"
#include "HardDisk.h"
using namespace std; 

// @param: the readyQueue and the processTable_
// listen for user inputs 
void readCommand(Containers &queueContainers, HardDisk &hardDiskContainer) {
	string command = ""; 
	string instruction = "";
	string filename = "";
	char options;
	int hardDiskNumber = 0;
	int priority = 0; 
	long memoryUsage = 0; 

	cout << "\nProceed to enter commands:\n";

	// input line of command to parse later for correct instructions 
	getline(cin, command); 
	istringstream parseCommand(command);
	parseCommand >> instruction; 

	// **************************** PROCESS RELATED COMMANDS ****************************	
	// >> A <PRIORITY> <SIZE>  
	if(instruction == "A") {
		parseCommand >> priority;
		parseCommand >> memoryUsage; 

		// check if there is space in memory before we create new process
		if(priority <= 0 || memoryUsage <= 0 || queueContainers.getTotalMemorySize() < memoryUsage) {
			cout << "Incorrect memory or priority for process - please reenter \n"; 
			return; 
		}

		// create process
		Process incomingProcess(priority, memoryUsage);

		// put process into readyQueue and processTable
		queueContainers.queueProcess(incomingProcess);

		// put process into memory list
		queueContainers.insertToList(incomingProcess.getPID(), memoryUsage);
	}

	// >> fork()
	else if(instruction == "fork") {
		queueContainers.fork();
	}

	// >> exit()
	else if(instruction == "exit") {
		if(queueContainers.getQueueSize() == 0) {
			cout << "No process left to exit\n";
			return;
		}

		queueContainers.exit();
	}

	// >> wait()
	else if(instruction == "wait") 
		cout << "wait" << endl;

	// **************************** HARD DISK RELATED COMMANDS ****************************
	// >> d <HARD DISK NUMBER> <FILE NAME>
	else if(instruction == "d") {
		parseCommand >> hardDiskNumber;
		parseCommand >> filename;
		int currentProcessPID = 0;
		if(filename == "") {
			cout << "No file to read or write - check file name\n";
			return;
		}
		// get the currentProcess' PID
		// check if CPU / ready queue is empty first 
		if(queueContainers.getQueueSize() != 0) {
			currentProcessPID = queueContainers.getFrontPID();
		}
		
		else {
			cout << "There is nothing in CPU - try again\n";
			return;
		}
		// remove current process using CPU
		// in other words, pop the front of the deque 
		queueContainers.popFront(); 
		pair<int, string> processToUseHardDisk(currentProcessPID, filename);
		hardDiskContainer.insertToHardDisk(hardDiskNumber, processToUseHardDisk);
	}

	// >> D <HARD DISK NUMBER>
	// hard disk number finish work for one process
	// pop front of hardDisk queue, find the process in processTable, queueProcess again
	else if(instruction == "D") {
		parseCommand >> hardDiskNumber;

		if(hardDiskNumber < 0 || hardDiskNumber >= hardDiskContainer.totalHardDiskCount()) {
			cout << "Inaccessible hard disk number - try again\n";
			return;
		}

		if(hardDiskContainer.getHardDiskSize(hardDiskNumber) == 0) {
			cout << "Nothing in hard disk " << hardDiskNumber << "\n";
			return; 
		}
		// retrieve front process number in hard disk number
		int tempPID = hardDiskContainer.getFrontProcessPID(hardDiskNumber);
		// queue process again
		// Process tempProcess = move(queueContainers.getProcess(hardDiskContainer.getFrontProcessPID(hardDiskNumber)));
		// pop front of hard disk number
		hardDiskContainer.popFrontProcess(hardDiskNumber);
		queueContainers.requeue(tempPID);
	}

	// **************************** SNAPSHOT RELATED COMMANDS ****************************
	
	// S <OPTIONS>; S is mostly a display command 
	else if(instruction == "S") {
		parseCommand >> options; 
		// display current process in CPU and ready queue
		if(options == 'r')
			queueContainers.displayReadyQueue();
		else if(options == 'i')
			hardDiskContainer.displayAllHardDisks();
		else if(options == 'm') {
			queueContainers.displayMemoryList();
		}
	}

	else 
		cout << "Invalid instruction - try again\n"; 

	// clear instruction for next input 
	instruction = "";
}

int main () {
	long ramMem = 0; 
	int numOfHardDisks = 0; 

	// check for valid input of memory / memory amount 
	// memory should be of type long and should be greater than 0 
	// cin.clear() will clear the error flag if the input is invalid
	// cin.ignore() will remove buffer from input of up to 10000 characters until newline
	// 10000 is just an arbitrary number amount of character inputs

	cout << "How much RAM memory is there on the simulated computer?\n"; 
	while( (!(cin >> ramMem)) || (ramMem < 0)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Incorrect amount of memory, try again\n";
	} 

	cout << "How many hard disks does the simulated computer have?\n"; 
	while( (!(cin >> numOfHardDisks)) || (numOfHardDisks < 0)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Incorrect amount of hardDisks, try again" << endl;
	} 

	// @init: HardDisk object
	HardDisk hardDiskContainer(numOfHardDisks);

	// @init Containers object; used for: 
	// ready queue (deque), process table (unordered_map), and memory management (list)
	Containers queueContainers; 
	queueContainers.setTotalMemorySize(ramMem);
	queueContainers.setConstTotalMemorySize(ramMem);
	long check = queueContainers.getTotalMemorySize();

	// flush out whitespace from cin to listen to correct inputs
	cin.clear();

	// ignores 256 characters up until '\n'
	// please do not put 256 spaces after our memory size 
	cin.ignore(256, '\n');

	while(true) {
		// read user inputs
		readCommand(queueContainers, hardDiskContainer);
	}
}