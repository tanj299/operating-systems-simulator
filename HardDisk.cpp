#include "HardDisk.h"
#include "Process.h"

HardDisk::HardDisk() {}

HardDisk::HardDisk(int size) {
	// resize the vector size of hardDisk_ entered by user
	hardDisk_.resize(size);
}

int HardDisk::getHardDiskSize(int hardDiskNumber) {
	return hardDisk_[hardDiskNumber].size();
}

int HardDisk::totalHardDiskCount() {
	return hardDisk_.size();
}

int HardDisk::getFrontProcessPID(int hardDiskNumber) {
	return hardDisk_[hardDiskNumber].front().first; 
}

void HardDisk::popFrontProcess(int hardDiskNumber) {
	hardDisk_[hardDiskNumber].pop_front();
}

void HardDisk::insertToHardDisk(int hardDiskNumber, pair<int,string> processToUseHardDisk) {
	hardDisk_[hardDiskNumber].push_back(processToUseHardDisk);
}

void HardDisk::displayAllHardDisks() {
	// vector<pair<int, string>>::iterator = hardDiskIt; 
	// hardDisk_ is a vector of each hard disk
	// each hard disk is also a vector of pairs
	// iterate through hardDisk_ (the vector of all hardDisks)
	deque<pair<int, string>>::iterator ioQueueIt; 
	for(int i = 0; i < hardDisk_.size(); ++i) {
		// iterate hardDisk_[HARD DISK NUMBER]'s size 
		if(!hardDisk_[i].empty()) {
			cout << "Hard Disk: " << i << "\n";
			cout << "Current process in hard disk: " 
				 << hardDisk_[i].front().first << "\t" << " | Filename: " << hardDisk_[i].front().second << "\n";
		
			for(ioQueueIt = hardDisk_[i].begin()+1; ioQueueIt != hardDisk_[i].end(); ioQueueIt++) {
				// output pair.first, pair.second
				cout << "Processes in I/O queue: "<< ioQueueIt->first << "\t | Filename: " << ioQueueIt->second << "\n";
			}
			cout<<"\n";
		}

		else {
			cout << "Hard Disk " << i << " is empty" << "\n";
		}
	}
}


