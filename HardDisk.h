#ifndef HARD_DISK_H
#define HARD_DISK_H

#include <vector>
#include <deque>
#include <string>
#include <utility>
using namespace std;

class HardDisk {
private:
	// vector to hold all hard disks with size inputted by user
	// each index holds a deque since a hard disk is an I/O queue
	// the deque holds a pair<int PID, string FILENAME>
	vector<deque<pair<int, string>>> hardDisk_; 
public: 
	HardDisk(); 
	HardDisk(int numOfHardDisk); 
	int totalHardDiskCount();
	int getHardDiskSize(int hardDiskNumber); 
	int getFrontProcessPID(int hardDiskNumber);
	void popFrontProcess(int hardDiskNumber);
	void insertToHardDisk(int hardDiskNumber, pair<int,string> processToUseHardDisk); 
	void displayAllHardDisks();	
	void terminateFromHardDisk();
};

#endif // HARD_DISK_H