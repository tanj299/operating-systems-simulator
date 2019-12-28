#include "Containers.h"
#include <limits.h>

/******************************************** CONTAINER HELPER FUNCTIONS ********************************************/

Containers::Containers() {}

Process Containers::getProcess (int PID) {
    return processTable_.at(PID);
}

int Containers::getFrontPID() {
    return readyDeque_.front().first; 
}

int Containers::getQueueSize() { 
    return readyDeque_.size(); 
}

/******************************************** PROCESS ALLOCATION FUNCTIONS ********************************************/

// @param: the incoming process 
// check if incoming process should preempt deque.front()
// takes int of deque.front() and process.getPriority()
void Containers::preempt(Process &incomingProcess) {
    int tempPID = 0;
    int tempPriority = 0;

    // checks that the incoming process' priority is greater than current process' priority
    // if true, preempt, shift current process to back of queue and push incoming process to front
    // else, push incoming process to back of queue
    if(readyDeque_.empty()) 
        readyDeque_.push_front(make_pair(incomingProcess.getPID(), incomingProcess.getPriority()));
     
    else if(incomingProcess.getPriority() > readyDeque_.front().second) {
        tempPID = readyDeque_.front().first; 
        tempPriority = readyDeque_.front().second; 
        readyDeque_.pop_front(); 
        readyDeque_.push_front(make_pair(incomingProcess.getPID(), incomingProcess.getPriority()));
        readyDeque_.push_back(make_pair(tempPID, tempPriority));
    }

    else 
        readyDeque_.push_back(make_pair(incomingProcess.getPID(), incomingProcess.getPriority()));    
}

// @param: process to be inserted into the readyQueue_ 
// SN: MUST pass process by reference to avoid creating a copy and creating a new PID#
void Containers::insertToDeque(Process &incomingProcess) {
    if(readyDeque_.empty()) 
        readyDeque_.push_front(make_pair(incomingProcess.getPID(), incomingProcess.getPriority()));

    else
        preempt(incomingProcess);
}

// @param: PID of process and the process itself
// SN: MUST pass process by reference to avoid creating a copy and creating a new PID#
void Containers::insertToMap(Process &incomingProcess) {
    // insert process into our unordered_map where <key = PID, value = Process object> 
    // and if object doesn't exist in map
    if(processTable_.find(incomingProcess.getPID()) == processTable_.end()) {
        processTable_[incomingProcess.getPID()] = incomingProcess;
    }
}

// @param: the new process created 
// inserts the new process into the readyQueue and processTable 
void Containers::queueProcess(Process &incomingProcess) {
    insertToMap(incomingProcess);
    insertToDeque(incomingProcess);
}

// @param: an EXISTING process' PID 
// requeue also preempts using processPID 
void Containers::requeue(int processPID) {
    int tempPID = 0;
    int tempPriority = 0; 
    int copyProcessPID = processPID;;
    int hardDiskProcessPriority = processTable_[processPID].getPriority();

    // checks that the incoming process' priority is greater than current process' priority
    // if true, preempt, shift current process to back of queue and push incoming process to front
    // else, push incoming process to back of queue 

    // checks if CPU is idle
    // CPU idle == empty deque
    if (readyDeque_.empty()) 
        readyDeque_.push_front(make_pair(copyProcessPID, hardDiskProcessPriority));
    
    else if(hardDiskProcessPriority > readyDeque_.front().second) {
        tempPID = readyDeque_.front().first; 
        tempPriority = readyDeque_.front().second; 
        readyDeque_.pop_front(); 
        readyDeque_.push_front(make_pair(copyProcessPID, hardDiskProcessPriority));
        readyDeque_.push_back(make_pair(tempPID, tempPriority));
    }

    else { 
        readyDeque_.push_back(make_pair(copyProcessPID, hardDiskProcessPriority)); 
    }   
}

// displays readyQueue - we use a deque because the underlying container is a vector that can make use of begin() and end() 
void Containers::displayReadyQueue() {
    deque<pair<int, int>>::iterator dequeIt = readyDeque_.begin(); 
    if(!readyDeque_.empty()) {
        cout << "PROCESS CURRENTLY USING CPU: \n" << "PID \tPRIORITY\n" << dequeIt->first << "\t" << dequeIt->second << "\n\n";

        cout << "READY\tQUEUE: \n" << "PID \tPRIORITY\n";
        for(dequeIt = readyDeque_.begin()+1; dequeIt != readyDeque_.end(); dequeIt++) {
            cout << dequeIt->first << "\t" << dequeIt->second << "\n";
        }
    }

    else {
        cout << "PROCESS CURRENTLY USING CPU: \n" << "PID \tPRIORITY\n" << "\n\n";
        cout << "READY\tQUEUE: \n" << "PID \tPRIORITY\n";
    }
}

void Containers::popFront() {
    readyDeque_.pop_front();
}


void Containers::setStatus(int &PID, string member, bool change) {
    if(member == "parent") {
        processTable_[PID].setParentStatus(change);        
    }
}

/******************************************** MEMORY ALLOCATION FUNCTIONS ********************************************/

long Containers::getTotalMemorySize() {
    return totalMemory_;
}

long Containers::getConstTotalMemorySize() {
    return constTotalMemory_;
}

void Containers::setTotalMemorySize(long totalMem) {
    totalMemory_ = totalMem;
}

void Containers::setConstTotalMemorySize(long totalMem) {
    constTotalMemory_ = totalMem;
}

void Containers::reduceTotalMemorySize(long processMem) {
    totalMemory_ -= processMem;
}

void Containers::increaseTotalMemorySize(long processMem) {
    totalMemory_ += processMem;
}

// @return: the correct position of inserting the node using worst-fit 
int Containers::findPositionOfPID() {
    int currentPID = 0; 
    // we determine the gaps at:
    // 1) the front 2) the middle 3) the end
    list<pair<int, long>>::iterator listIt; 
    long tempMax = 0;
    long remainderAtEnd = constTotalMemory_ - (processTable_[ (prev(memoryList_.end(), 1))->first ].getEndMemStatus()) - 1;

    // @assert: there's a gap at the front
    if(processTable_[memoryList_.begin()->first].getStartMemStatus() != 0) {
        tempMax = processTable_[memoryList_.begin()->first].getStartMemStatus();
        currentPID = memoryList_.begin()->first;
        // cout << "start is not 0" << endl;

    }

    // @assert: there's a gap in the middle
    list<pair<int, long>>::iterator prevIt = memoryList_.begin(); 
    long gap = 0;
    for(listIt = memoryList_.begin()++; listIt != memoryList_.end(); listIt++) {
        gap = processTable_.at(listIt->first).getStartMemStatus() - processTable_.at(prevIt->first).getEndMemStatus() - 1; 
        // tempMax = max(gap, tempMax);
        if(gap > tempMax) {
            tempMax = gap; 
            currentPID = listIt->first;
        }
        // need to keep track of prev at end
        prevIt = listIt; 
        // cout << "find me the gap and id to insert: " << currentPID << endl;
    }

    // @assert: final check: if the gap at the end is greater than tempMax, then we return -1
    if(remainderAtEnd > tempMax)
        currentPID = -1; 

    // cout << "insert at pid: " << currentPID << endl;
    return currentPID; 
}

void Containers::insertToList(int processPID, long processMem) {
    // catchall for process memory allocation
    if(processMem > totalMemory_) {
        cout << "Insufficient amount of memory necessary for allocation - try again\n";
        return; 
    }

    // reduce total memory size with process size 
    reduceTotalMemorySize(processMem);

    // if list is empty, directly push 
    if(memoryList_.empty()) {
        memoryList_.push_back(make_pair(processPID, processMem));
        processTable_[processPID].setStartMemStatus(0); 
        processTable_[processPID].setEndMemStatus(processMem - 1); 
    }

    // if list has one element
    // we have to check the beginning and the end of list: 
    // x-@-y, where @ is the node and x is the gap at front and y is the gap at end
    else if(memoryList_.size() == 1) {
        // if the first and only node starts at 0, then we just append to the end of the list
        // set new node's start == prev.end()+1 and new node's end == start + processMem - 1
        if(processTable_[memoryList_.begin()->first].getStartMemStatus() == 0) {
            memoryList_.push_back(make_pair(processPID, processMem));
            long startOfNewMem = processTable_[memoryList_.begin()->first].getEndMemStatus() + 1;
            processTable_[processPID].setStartMemStatus(startOfNewMem); 
            processTable_[processPID].setEndMemStatus(startOfNewMem + processMem - 1); 
        }

        // else, we check if the size of the beginning is greater than total mem left
        // if it is, we append to front, else, we append to back
        else {
            if(processTable_[memoryList_.begin()->first].getStartMemStatus() > totalMemory_) {
                memoryList_.push_front(make_pair(processPID, processMem));
                processTable_[processPID].setStartMemStatus(0);
                processTable_[processPID].setEndMemStatus(processMem-1);
            }

            else {
                memoryList_.push_back(make_pair(processPID, processMem));
                long startOfNewMem = processTable_[memoryList_.begin()->first].getEndMemStatus() + 1;
                processTable_[processPID].setStartMemStatus(startOfNewMem); 
                processTable_[processPID].setEndMemStatus(startOfNewMem + processMem - 1); 
            }
        }
    }

    // else, there are at least two nodes to which we can determine the gap
    // we determine the position of where to insert the node
    else {
        int insertPosition = findPositionOfPID();
        // if position == -1, then we append to the back and assert there is no gap greater than total memory
        if(insertPosition == -1) {
            memoryList_.push_back(make_pair(processPID, processMem));
            long startOfNewMem = processTable_[prev(memoryList_.end(), 2)->first].getEndMemStatus() + 1;
            processTable_[processPID].setStartMemStatus(startOfNewMem);
            processTable_[processPID].setEndMemStatus(startOfNewMem + processMem - 1); 
        }

        // else, iterate through the list and find the position of the gap
        // insert new node at position
        else {
            list<pair<int, long>>::iterator listIt;
            list<pair<int, long>>::iterator prevIt = memoryList_.begin();
            for(listIt = memoryList_.begin()++; listIt != memoryList_.end(); listIt++) {
                if(listIt->first == insertPosition) {
                    memoryList_.insert(listIt, make_pair(processPID, processMem));

                    // take prevIt's end 
                    long startOfNewMem = processTable_[prevIt->first].getEndMemStatus() + 1;
                    processTable_[processPID].setStartMemStatus(startOfNewMem);
                    processTable_[processPID].setEndMemStatus(startOfNewMem + processMem-1);
                }
                
                prevIt = listIt;
            }
        }
    }
}

void Containers::removeFromMemList(int processPID) {
    list<pair<int, long>>::iterator listIt; 
    for(listIt = memoryList_.begin(); listIt != memoryList_.end(); listIt++) {
        if(listIt->first == processPID) {
            increaseTotalMemorySize(listIt->second);

            // avoid incrementing invalidated iterator
            listIt = memoryList_.erase(listIt);
        }
    }
    cout << "After removing, totalMemory_ left: " << totalMemory_ << endl;
}

void Containers::displayMemoryList(){
    if(memoryList_.empty()) {
        cout << "There is nothing in memory" << endl;
        return;
    }

    list<pair<int, long>>::iterator listIt; 
    for(listIt = memoryList_.begin(); listIt != memoryList_.end(); listIt++) {
        cout << "PID: " << listIt->first << " | Memory Range: " <<
        // lookup with PID on process table to get startMem and endMem
        processTable_.at(listIt->first).getStartMemStatus() << " -> " << processTable_.at(listIt->first).getEndMemStatus() << "\n"; 
    }
}


/******************************************** FORK / WAIT / EXIT FUNCTIONS ********************************************/

// creates a copy of the current process - we call it childProcess
// childProcess: setChildStatus = true 
// parentProces: setParentStatus = true 
void Containers::fork() {
    // call copy constructor to copy current process
    // a new PID is also created for the copied process 
    int currentPID = readyDeque_.front().first;
    Process childProcess = processTable_.at(currentPID);

    // we set the forked process' child status to true since it's a child
    childProcess.setChildStatus(true);

    // we set the current process' parent status to true because it is a parent of the forked process
    processTable_[currentPID].setParentStatus(true);

    // queue up the child process
    queueProcess(childProcess);

    // allocate memory for the child process
    reduceTotalMemorySize(childProcess.getMemory());
}

// remove process from process table
void Containers::terminateFromProcessTable() {
    int pidToErase = getFrontPID();
    processTable_.erase(pidToErase);
}

// remove process from memory in system
void Containers::terminateFromMemList() {
    // we just set the pid to -1 to "erase" from our list
    // since we need to ensure there's a gap
    int pidToErase = getFrontPID();
    list<pair<int, long>>::iterator listIt; 
    for(listIt = memoryList_.begin(); listIt != memoryList_.end(); listIt++) {
        if(listIt->first == pidToErase) {
            listIt->first = -1;
        }
    }
}

// remove process from wait vector
void Containers::terminateFromWaitVector() {}

// exit current process
// NOT including cascading termination; used for testing
void Containers::exit() {
    int processPID = readyDeque_.front().first;
    list<pair<int, long>>::iterator listIt;
    for (listIt = memoryList_.begin(); listIt != memoryList_.end(); listIt++){
        if (listIt->first == processPID){
            increaseTotalMemorySize(listIt->second);

            // avoid incrementing invalidated iterator
            listIt = memoryList_.erase(listIt);
        }
    }

    popFront();
}

// remove processes from all locations except hard disk
// hard disk is in its own class
void Containers::exitFromContainers() {
    // if process is a child, check if process has a parent 
    // if process has a parent check if parent is waiting
    // else, cascade terminate process, including the process in the processTable
    // if its parent is waiting, cascade terminate process and bring parent back to readyQueue or CPU
    // if its parent is NOT waiting, cascade terminate process
    int pidToErase = getFrontPID(); 
    while (processTable_.at(pidToErase).getChildStatus()) {

    }

    // check if it's a parent

    // if it's neither a parent nor child

    popFront(); // equivalent to terminating from deque
    terminateFromProcessTable();
    terminateFromMemList();
    terminateFromWaitVector();
}