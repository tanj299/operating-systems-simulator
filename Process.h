#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream> 
#include <string>
#include <queue>            // needed for queue
#include <deque>            // needed for deque
#include <unordered_map>    // needed for hashmap
#include <utility>          // needed for pair

using namespace std; 

class Process {
private:
    /************************ PROCESS CONTROL BLOCK PRIVATE VARIABLES ************************/
    
    // declare a static variable since all instances of a class static variables are shared  
    static unsigned int pid_; 

    // this is the PID we are setting for each instance of class 
    unsigned int currentPID_; 

    // this is the priority 
    unsigned int priority_;
    
    // this is the process' memory usage
    long memoryUsage_;    

    // if a child exists, we maintain its parent PID as well
    // should only be set if a process calls fork 
    // we default it to 0 since pid all starts at 1 
    int parentPID_ = 0;
    
    // @return: if process is zombie, true; else, false
    bool isZombie_ = false;

    // @return: if process is a parent process, true; else, false
    bool isParent_ = false; 

    // @return: if process is a child process, true; else, false
    bool isChild_ = false;

    // @return: if process is waiting, true; else, false
    bool isWaiting_ = false;  

    // should totalMem_ be in this class or in main? 
    long totalMem_; 

    // file should be an empty string at the moment
    string fileInUse_ = ""; 

    // start of process; used for memory management
    long startMem_ = 0; 

    // end of process; used for memory management
    long endMem_ = 0; 

    // list of children - only append if parent is set to true
    vector<int> listOfChildren;
    
public:
    // default constructor = create() process 
    // invoked when creating new process; expected behavior: 
    // 1) when new process is created/arrives, initialize it's PCB (pid, priority, memory usage, isParent_, etc. )
    // 2) check priority -> process should be placed in ready-queue or CPU if it can preempt current process 
    // 3) allocate memory for process -> subtract memoryUsage from totalMem
    // 4) PIDs should NOT be reused 
    // 5) every new process should be hashed and placed in an unordered_map
    Process();  

    // parameterized constructor 
    // invoked when creating new process; expected behavior: 
    // 1) when new process is created/arrives, initialized it's PCB by incrementing it's pid automatically
    // 2) set the priorty level and memory usage and assign to process 
    Process(const int& priority, const long& memSize);


/**************************************** RULE OF FIVE ****************************************/
    // destructor != exit 
    // it just destroys the object goes out of scope / end of lifetime 
    ~Process();  
    
    // copy constructor = fork()  
    // assign new PID but maintain priority and memoryUsage
    Process(const Process& rhs);

    // copy assignment = assign current process to CPU
    // set CPU to be current process since CPU is a Process object 
    Process& operator=(const Process& rhs);

    // move constructor
    Process(Process &&rhs);

    // move assignment operator=
    Process& operator=(Process &&rhs);

/**************************************** ADDITIONAL FUNCTIONS ****************************************/

    int getPID();
    int getPriority(); 
    long getMemory(); 
    string getFile(string currFile);
    bool getParentStatus();
    bool getChildStatus(); 
    bool getZombieStatus();
    bool getWaitStatus(); 
    long getStartMemStatus();
    long getEndMemStatus();
    int getParentPID();
    void setParentStatus(bool parentStat);
    void setChildStatus(bool childStat); 
    void setZombieStatus(bool zombieStat);
    void setWaitStatus(bool waitStat); 
    void setStartMemStatus(long startMem);
    void setEndMemStatus(long endMem);
    void setParentPID(int parentPID);

/**************************************** CHILDREN VECTOR FUNCTIONS ****************************************/
    int getVectorSize();
    void addChildToVector(int childPID);
    void clearChildrenVector();  


    friend bool operator<= (Process &process1, Process &process2);
    friend bool operator> (Process &process1, Process &process2);

    // wait() command 
    void wait();
}; 

#endif // PROCESS_H_