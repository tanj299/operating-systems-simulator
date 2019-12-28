#include <unordered_map>
#include <deque>
#include <utility>
#include <list>
#include "Process.h"

#ifndef CONTAINERS_H
#define CONTAINERS_H

class Containers {
private: 
	// unordered_map is used for process table with all processes hashed using 
	// unordered_map params: <int PID, Process PROCESS_OBJECT> where key: PID of process and value: the Process itself
	unordered_map<int, Process> processTable_;

	// deque is used to iterate over ready queue since we cannot iterate over priority_queue 
	// to traverse, simply use the underlying iterator begin() and end() 
	// object in deque is a pair that holds <int PID, int PRIORITY> of process	
	deque<pair<int, int>> readyDeque_;

	// vector is used for processes that have issued wait()
	// waitingVector params: <int PID>
	vector<int> waitingVector_;

	// list is used for memory management with all processes 
	// tuple params: <int PID, long PROCESS_MEM_SIZE> 
	list<pair<int, long>> memoryList_;

	// totalMemSize is used for memory management; declared when asking user what is total bytes in memory
	long totalMemory_;

	// constTotalMemory is used for memory management; we do not alter this we just use it to compare 
	long constTotalMemory_;

public:

	/************ CONTAINER HELPER FUNCTIONS************/

	Containers(); // ok
	Process getProcess(int PID); // ok
	int getFrontPID();
	int getQueueSize();

	/************ PROCESS ALLOCATION FUNCTIONS************/

	void preempt(Process &incomingProcess); // ok
	void insertToDeque(Process &incomingProcess); // ok
	void insertToMap(Process &incomingProcess); // ok
	void queueProcess(Process &incomingProcess);
	void requeue(int processPID);
	void displayReadyQueue(); // ok
	void popFront();
	void setStatus(int &PID, string member, bool change);

	/************ MEMORY ALLOCATION FUNCTIONS************/

	long getTotalMemorySize();
	long getConstTotalMemorySize();
	void setTotalMemorySize(long totalMem);
	void setConstTotalMemorySize(long totalMem);
	void reduceTotalMemorySize(long processMem);
	void increaseTotalMemorySize(long processMem);
	void setMemoryRange(int processPID, long processMem);
	void setMemoryRangeBegin(int processPID, long processMem);	
	void insertToList(int processPID, long processMem);
	void displayMemoryList();
	int findPositionOfPID();
	void removeFromMemList(int processPID); // for testing

	/************ FORK / WAIT / EXIT FUNCTIONS************/

	void fork(); // ok 
	void exit();
	void terminateFromProcessTable();
	void terminateFromDeque();
	void terminateFromMemList();
	void terminateFromWaitVector();
	void exitFromContainers();
}; 

#endif // CONTAINERS_H