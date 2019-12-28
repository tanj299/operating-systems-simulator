#include "Process.h"

using namespace std; 

// used for holding temp Processes instantiated 
Process::Process() {
    currentPID_ = 0;
    priority_ = 0;
    memoryUsage_ = 0; 
    parentPID_ = 0; 
    isParent_ = false; 
    isChild_ = false; 
    isZombie_ = false; 
    isWaiting_ = false;
    fileInUse_ = "";  
    startMem_ = 0; 
    endMem_ = 0;
}

// used for creating an actual process with PID / PRIORITY / MEMORY
Process::Process(const int& priority, const long& memSize) {
    currentPID_ = pid_++; 
    priority_  = priority; 
    memoryUsage_ = memSize;
    parentPID_ = 0;
    isParent_ = false; 
    isChild_ = false; 
    isZombie_ = false; 
    isWaiting_ = false; 
    fileInUse_ = "";      
    startMem_ = 0; 
    endMem_ = 0;
}

/**************************************** RULE OF FIVE ****************************************/
// destructor
Process::~Process() {   
    // nothing to call, nothing dynamically allocated 
    // might need to destroy any containers but maybe not 
}

// copy constructor == fork()
Process::Process(const Process &rhs) {
    currentPID_ = rhs.pid_++;
    priority_  = rhs.priority_; 
    memoryUsage_ = rhs.memoryUsage_;
    parentPID_ = rhs.parentPID_;
    isParent_ = false; 
    startMem_ = 0; 
    endMem_ = 0;

    // if fork() is called, then the process created is a child and we set it to true 
    isChild_ = true;  
    isZombie_ = rhs.isZombie_; 
    isWaiting_ = rhs.isWaiting_;
    fileInUse_ = "";  
}

// copy assigment operator 
Process& Process::operator=(const Process &rhs) {
    // check if the process is already equal to the CPU 
    if(&rhs == this) 
        return *this; 

    // set CPU to be current process since CPU is a Process object 
    currentPID_ = rhs.currentPID_;
    priority_  = rhs.priority_; 
    memoryUsage_ = rhs.memoryUsage_;
    parentPID_ = rhs.parentPID_;
    isParent_ = rhs.isParent_; 
    isChild_ = rhs.isChild_; 
    isZombie_ = rhs.isZombie_; 
    isWaiting_ = rhs.isWaiting_;
    fileInUse_ = rhs.fileInUse_; 

    return *this; 
}

// move constructor
// Note: can't really think of a reason why we would need this but this is here for good C++ practice I suppose
Process::Process(Process &&rhs) {
    swap(rhs.currentPID_, currentPID_);
    swap(rhs.priority_, priority_);
    swap(rhs.memoryUsage_, memoryUsage_); 
    swap(rhs.parentPID_, parentPID_);
    swap(rhs.isParent_, isParent_);
    swap(rhs.isChild_, isChild_); 
    swap(rhs.isZombie_, isZombie_);
    swap(rhs.isWaiting_, isWaiting_);
    swap(rhs.fileInUse_, fileInUse_);
    swap(rhs.startMem_, startMem_);
    swap(rhs.endMem_, endMem_);
    rhs.currentPID_ = 0;
    rhs.priority_ = 0;
    rhs.memoryUsage_ = 0; 
    rhs.parentPID_ = 0;
    rhs.isParent_ = false; 
    rhs.isChild_ = false; 
    rhs.isZombie_ = false;
    rhs.isWaiting_ = false;  
    fileInUse_ = "";
}

// move assignment operator 
Process& Process::operator=(Process &&rhs) {
    swap(rhs.currentPID_, currentPID_);
    swap(rhs.priority_, priority_);
    swap(rhs.memoryUsage_, memoryUsage_); 
    swap(rhs.parentPID_, parentPID_);
    swap(rhs.isParent_, isParent_);
    swap(rhs.isChild_, isChild_); 
    swap(rhs.isZombie_, isZombie_);
    swap(rhs.isWaiting_, isWaiting_);
    swap(rhs.fileInUse_, fileInUse_);

    return *this; 
}

/**************************************** GETTER FUNCTIONS ****************************************/

int Process::getPID() {
    return currentPID_; 
}

int Process::getPriority() {
    return priority_;
}

long Process::getMemory() {
    return memoryUsage_;
}

int Process::getParentPID() {
    return parentPID_;
}

string Process::getFile(string currFile) {
    return fileInUse_;
}

bool Process::getParentStatus() {
    return isParent_;
}

bool Process::getChildStatus() {
    return isChild_;
}

bool Process::getZombieStatus() {
    return isZombie_;
}

bool Process::getWaitStatus() {
    return isWaiting_;
}

long Process::getStartMemStatus() {
    return startMem_;
}

long Process::getEndMemStatus() {
    return endMem_;
}

/**************************************** SETTER FUNCTIONS ****************************************/

void Process::setParentStatus(bool parentStat) {
    isParent_ = parentStat; 
}

void Process::setChildStatus(bool childStat) {
    isChild_ = childStat; 
}

void Process::setZombieStatus(bool zombieStat) {
    isZombie_ = zombieStat;
}

void Process::setWaitStatus(bool waitStat) {
    isWaiting_ = waitStat; 
}

void Process::setParentPID(int parentPID) {
    parentPID_ = parentPID;
}

void Process::setStartMemStatus(long startMem) {
    startMem_ = startMem;
}

void Process::setEndMemStatus(long endMem) {
    endMem_ = endMem;
}

// overloading operator< for processes to compare 
bool operator<= (Process &process1, Process &process2) {
    return process1.getPriority() <= process2.getPriority();
}

bool operator> (Process &process1, Process &process2) {
    return process1.getPriority() > process2.getPriority();
}

// class variable for our private variable 
// pid is shared and is visible to all Process objects created 
unsigned int Process::pid_= 1;


