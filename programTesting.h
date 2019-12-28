Program Testing:

Input memory consumption:
>> 100 memory

Input hard disk size:
>> 3
// PROCESS 
>> A 5 10 // CPU: PID: 1 PRIORITY: 5 - RQ: <EMPTY>
>> A 8 15 // CPU: PID: 2 PRIORITY: 8 - RQ: PID: 1 PRIORITY: 5
>> S r 
>> A 3 65
>> S r 
>> d 1 asd
>> d 1 QWER
>> S i // disk 1: PID 2 FILE: asd 
	   // I/O queue: PID 1 FILE: QWER
	   // optional to not show idle disks
>> D 1 // CPU: PID: 2 PRIORITY: 8 - RQ: PID 3 PRIORITY: 3
>> S i // disk 1: PID 1 FILE: QWER
>> D 1 
>> S r // CPU: 2/8 - RQ: 3/3, 1/5

// MEMORY MANAGEMENT
>> S m 
/* 
0 - 9 PID 1 
10 - 24: PID 2
25 - 89: PID 3 
90 - 99: EMPTY 
*/
>> exit // CPU: 1/5 - RQ: 3/3 <--- this might be wrong... if current process exits then next process goes up so it should be 3/3
>> A 1 8
>> S m 
>> S m 
/* 
0 - 9 PID 1 
10 - 17: PID 4
25 - 89: PID 3 
90 - 99: EMPTY 
*/

// NEW TEST
// TEST FOR IDLE CPU
>> A 5 10
>> d 1 asd
>> D 1 
>> S r 

// NEW TEST
// WAIT / FORK / EXIT
>> A 5 10
>> fork
>> fork 
>> fork // CPU: 1/5 - RQ: 2/5, 3/5, 4/5
>> d 0 asd.txt
>> exit
>> exit 
>> d 1 QWER 
>> D 0
// CPU: 1/5 
// disk0: IDLE
// disk1: 4

>> wait 
>> S r // CPU: 1/5, process should still be running
>> wait
>> S r // CPU: 1/5; process should still be running since there are two zombies
>> wait // CPU: IDLE - RQ: IDLE, PID 1 should be in waiting vector but it should STILL exist in memory
>> D 1 // CPU: PID 4 
> exit // CPU: 1/5, process 4 exited

// NEw TEST - CASCADING TERMINATION
>> 100
>> 3
>> A 5 10
>> fork 
>> S r // CPU: 1/5 - RQ: 2/5
>> d 1 asd // CPU: 2/5 | disk1: 2/asd
>> fork // CPU 2/5 - RQ: 3/5
>> d 0 QWER.txt
>> D 0 
>> D 1 
>> exit // PID 3 exits from system; at this point, 3 is in IO queue so exit affects I/O queue too 
		// PID 2 is a zombie of 1 
		// CPU: 1/5 but process 2 has exited but its a zombieProcess 
>> wait