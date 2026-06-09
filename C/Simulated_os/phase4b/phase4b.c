#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <phase3.h>
#include <phase4.h>
#include <phase4_usermode.h>
#include <usyscall.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
-------- PROTOTYPES ---------
*/
void phase4_init(void);
void phase4_start_service_processes(void);
void syscall_sleep(USLOSS_Sysargs *args);
void syscall_term_read(USLOSS_Sysargs *args);
void syscall_term_write(USLOSS_Sysargs *args);
int clock_driver(void *arg);
void syscall_disk_size(USLOSS_Sysargs *args);
void syscall_disk_read(USLOSS_Sysargs *args);
void syscall_disk_write(USLOSS_Sysargs *args);
void check_sleep_queue();
int terminal_driver(void *args);
int disk_driver(void *args);
void enable_terminal_interrupts(int unit);
void handle_term_input(int unit, char ch);
void handler_term_output(int unit);

/*
----------- START OF MACROS -------------
*/
#define NUM_TERMINALS 4
#define NUM_DISKS 2
#define DISK_BLOCKS 16
#define DISK_BLOCK_SZ 512
#define MAX_LINE 80
#define MAX_BUFFERS 10
#define CLOCK_FREQ 5
#define CLOCK_TICKS_PER_SEC 1000000
/*
---------- END OF MACROS ----------------
*/

/*
------- START OF SLEEP REQUEST STRUCT -------
*/
typedef struct SleepRequest{
	int pid;
	int wakeupTime;
	struct SleepRequest *nextSleep;
}SleepRequest;
/*
------ END OF SLEEP REQUEST STRUCT ---------
*/

/*
----- START OF TERMINAL BUFFER STRUCT ------
*/
typedef struct TerminalBuffer{
	int lineHd;
	int lineTail;
	char line[MAX_LINE + 1];
	int length;
}TerminalBuffer;
/*
---- END OF TERMINAL BUFFER STRCUT --------
*/

/*
----- START OF TERMINAL STATE STRUCT -----
*/
typedef struct TerminalState{
	int readMbox;
	int writeMbox;
	
	int lineHd;
	int lineTail;
	int buffCount;
	
	TerminalBuffer lines[MAX_BUFFERS];
	char currentLine[MAX_LINE + 1];
	int currentLength;
	
	char *writeBuff;
	int writeLength;
	int writePos;
	int writeBlockedPID;
	int writeInProgress;
}TerminalState;
/*
------ END OF TERMINAL STATE STRUCT -----
*/

/*
------ START OF DISK REQUEST STRUCT -----
*/
typedef struct DiskRequest {
	int op;
	void *buffer;
	int track;
	int startBlock;
	int numBlocks;
	int pid;
	int status;
	struct DiskRequest *next;	
} DiskRequest;
/*
------ END OF DISK REQUEST STRUCT ------
*/

/*
------ START OF DISK UNIT ------------
*/
typedef struct DiskUnit {
	int mailbox;
	DiskRequest *queueHd;
	int currTrack;
	int blockSize;
	int numTracks;
	int requestInProgress;
	DiskRequest *requestQueueFoward;
	DiskRequest *requestQueueBack;
} DiskUnit;
/*
------ END OF DISK UNIT --------------
*/

/*
------ GLOBALS ------------------
*/
static int clockMailbox;
static int clockTicks;
static SleepRequest *sleepQueueHd = NULL;
static int sleepQueueMutex;
static TerminalState terminals[NUM_TERMINALS];
static DiskUnit disks[NUM_DISKS];

void phase4_init(void){
	//init system call vector 
	systemCallVec[SYS_SLEEP] = syscall_sleep;
	systemCallVec[SYS_TERMREAD] = syscall_term_read;
	systemCallVec[SYS_TERMWRITE] = syscall_term_write;
	systemCallVec[SYS_DISKSIZE] = syscall_disk_size;
	systemCallVec[SYS_DISKREAD] = syscall_disk_read;
	systemCallVev[SYS_DISKWRITE] = syscall_disk_write;

	//init mailboxes
	sleepQueueMutex = MboxCreate(1,0);
	clockMailbox = MboxCreate(1,0);
	clockTicks = 0;
	//go through terminal array and init terminal structs
	int i;
	for (i = 0; i < NUM_TERMINALS; ++i){
		terminals[i].readMbox = MboxCreate(1, 0);
		terminals[i].writeMbox = MboxCreate(1, 0);
		terminals[i].lineHd = terminals[i].lineTail = terminals[i].buffCount = 0;
		terminals[i].currentLength = 0;
		terminals[i].writeBuff = NULL;
		terminals[i].writeLength = 0;
		terminals[i].writePos = 0;
		terminals[i].writeInProgress = 0;
	}
	//fill out disk structs in disk array
	for (i = 0; i < NUM_DISKS; i++){
		disks[i].mailbox = MboxCreate(1,0);
		disks[i].queueHd = NULL;
		disks[i].currTrack = 0;
		disks[i].blockSize = DISK_BLOCK_SZ;
		disks[i].numTracks = DISK_BLOCKS;
		disks[i].requestInProgress = 0;
		disks[i].requestQueueFoward = NULL;
		disks[i].requestQueueBack = NULL;
	}
}

void phase4_start_service_processes(void){
	
	//spork clock deamons
	spork("ClockDriver",clock_driver,NULL,USLOSS_MIN_STACK,2);
	//spork terminal deamons
	for (int i = 0; i < NUM_TERMINALS; ++i){
		spork("TerminalDriver",terminal_driver, (void *)(long)i,USLOSS_MIN_STACK,2);
	}
	//spork disk deamons
	for (int i = 0; i < NUM_DISKS; i++){
		spork("DiskDriver", disk_driver, (void *)(long)i, USLOSS_MIN_STACK, 2);
	}
}
/*
	Name: syscall_sleep
	Purpose: handle the system call for sleeping, handles book keeping for sleep
	Return: void 
*/
void syscall_sleep(USLOSS_Sysargs *args){
	int seconds = (int)(long) args->arg1;
	int pid = getpid();
	//check is neg
	if (seconds < 0){
		args->arg4 = (void *)(long)-1;
		return;
	}
	//check if 0
	if (seconds == 0){
		args->arg4 = (void *)(long)0;
		return;
	}
	//calc time to wake
	int wakeupTime = clockTicks + seconds * CLOCK_TICKS_PER_SEC;
	//mallac a new request to sleep
	SleepRequest *newRequest = (SleepRequest *)malloc(sizeof(SleepRequest));
	//check if malloc failed
	if (newRequest == NULL){
		USLOSS_Console("syscall_sleep: malloc failed\n");
		USLOSS_Halt(1);
	}
	//fillout sleep request members
	newRequest->wakeupTime = wakeupTime;
	newRequest->pid = pid;
	newRequest->nextSleep = NULL;
	//check if sleep queue is empty or if new request will wake earlier
	if(sleepQueueHd == NULL || wakeupTime < sleepQueueHd->wakeupTime){
		newRequest->nextSleep = sleepQueueHd;
		sleepQueueHd = newRequest;
	}
	else{
		//else find spot for request
		SleepRequest *curr = sleepQueueHd;
		while(curr->nextSleep != NULL && curr->nextSleep->wakeupTime <= wakeupTime){
			curr = curr->nextSleep;
		}
		newRequest->nextSleep = curr->nextSleep;
		curr->nextSleep = newRequest;
	}
	//hang on recv
	MboxRecv(clockMailbox,NULL,0);
	//map args
	args->arg4 = (void *)(long)0;
	
}

/*
	Name: clock_driver
	Purpose: act as the clock driver
	Return: uses out arguments normally
*/
int clock_driver(void *arg){
	int status;
	while(1){
		//call to wait
		waitDevice(USLOSS_CLOCK_DEV, 0, &status);
		//check if failed
		if (status != 0){
			USLOSS_Console("Clock_Driver Failure: halting\n");
			USLOSS_Halt(1);
		}
		//clock ticks
		clockTicks++;
		//check queue
		check_sleep_queue();
	}
	
	return 0;
	
}

/*
	Name: check_sleep_queue
	Purpse: helper function to clock_driver, to see if any process are in the sleep
	queue
	Return: void
*/
void check_sleep_queue(){
	//while queue is not empty and its time to wake
	while(sleepQueueHd != NULL && sleepQueueHd->wakeupTime <= clockTicks){
		//get a ref to the element to wake
		SleepRequest *toWake = sleepQueueHd;
		//update queue ref
		sleepQueueHd = toWake->nextSleep;
		//send a call to wake
		MboxSend(clockMailbox,NULL,0);
		//unblock the woke process
		unblockProc(toWake->pid);
		//free ref
		free(toWake);
	}
	
}

/*
	Name: syscall_term_read
	Purpose: preforms a read of one bit at a time from user buffer. finds the 
	correct terminal from terminal array and updates sysargs structs
	Return: void
*/
void syscall_term_read(USLOSS_Sysargs *args){
	//get elements from args struct members
	char *userBuff = (char *)args->arg1;
	int buffSize = (int)(long)args->arg2;
	int unit = (int)(long)args->arg3;
	//check for bad values
	if (userBuff == NULL || buffSize <= 0 || unit < 0 || unit >= NUM_TERMINALS){
		args->arg4 = (void *)(long)-1;
		return;
	}
	//find terminal
	TerminalState *term = &terminals[unit];
	//while we dont have any chars in the buffer space
	while (term->buffCount == 0){
		MboxRecv(term->readMbox, NULL, 0);
	}
	//get the terminals terminal buffer line member
	TerminalBuffer *line = &term->lines[term->lineHd];
	//if line lenght < buffer size line lenthing is the same
	//else we update it to buffer sie
	int toCopy = (line->length < buffSize) ? line->length : buffSize;
	//copy user buffer into terminal buffer
	memcpy(userBuff, line->line, toCopy);
	//update members of terminal
	term->lineHd = (term->lineHd + 1) % MAX_LINE;
	term->buffCount--;
	//update members of args struct
	args->arg2 = (void *)(long)toCopy;
	args->arg4 = (void *)(long)0;
	
}

/*
	Name: syscall_term_write
	Purpose: writes a character from buffer to terminal
	Return: void
*/
void syscall_term_write(USLOSS_Sysargs *args){
	char *userBuff = (char *)args->arg1;
	int buffSize = (int)(long)args->arg2;
	int unit = (int)(long)args->arg3;
	
	if (userBuff == NULL || buffSize <= 0 || unit < 0 || unit >= NUM_TERMINALS){
		args->arg4 = (void *)(long)-1;
		return;
	}
	//find terminal
	TerminalState *term = &terminals[unit];
	//update members
	term->writeBuff = userBuff;
	term->writeLength = buffSize;
	term->writePos = 0;
	term->writeInProgress = 1;
	//get a ref to status for an address to update
	int status;
	//call usloss device input
	USLOSS_DeviceInput(USLOSS_TERM_DEV, unit, &status);
	//check if xmit is ready
	if (USLOSS_TERM_STAT_XMIT(status) == USLOSS_DEV_READY){
		//get a control bit ready with xmit and recv inabled
		int control = 0x7;
		//bit shit the first bit in 
		control |= (userBuff[0] << 8);
		//pass to device output
		USLOSS_DeviceOutput(USLOSS_TERM_DEV, unit, (void *)(long)control);
		//addvance the position
		term->writePos++;
	}
	//call to mbox recive on the terminal write mailbox
	MboxRecv(term->writeMbox, NULL, 0);
	//update args struct
	args->arg2 = (void *)(long)buffSize;
	args->arg4 = (void *)(long)0;
	
}

/*
	Name: terminal_driver
	Purpose: acts as the terminal driver to enable terminal interrupts (both xmit and recv).
	checks on loop if ther is someting waiting to recv or something to xmit
	Return: uses out arguments to update status
*/
int terminal_driver(void *args){
	//get which terminal 
	int unit = (int)(long)args;
	//make a status to have wait device write too
	int status;
	//enable terminal read and xmit
	enable_terminal_interrupts(unit);
	
	while(1){
		//call wait
		waitDevice(USLOSS_TERM_DEV, unit, &status);
		//read status
		//if failed
		if (status != 0){
			USLOSS_Console("Terminal Driver failed: wait device failed for unit %d\n", unit);
			USLOSS_Halt(1);
		}
		//if recv busy
		if (USLOSS_TERM_STAT_RECV(status) == USLOSS_DEV_BUSY){
			char currChar = USLOSS_TERM_STAT_CHAR(status);
			handle_term_input(unit, currChar);
		}
		//if ready to xmit
		if (USLOSS_TERM_STAT_XMIT(status) == USLOSS_DEV_READY){
			handler_term_output(unit);
		}
	}
	return 0;
}

/*
	Name: enable_terminal_interrupts
	Purpose: acts as controler of the status register, makes value 1 and shifts 1 into the 
	corrisponding bit position (for 'send char', xmit, and recv)
	Return: void
*/
void enable_terminal_interrupts(int unit){
	int control = 0;
	control |= 0x1 << 0;
	//recv bit
	control |= 0x1 << 1;
	//xmit bit
	control |= 0x1 << 2;
	
	USLOSS_DeviceOutput(USLOSS_TERM_DEV, unit, (void *)(long)control);
}

/*
	Name: handle_term_input
	Purpose: meant to be a helper to read, finds the terminal checks the buffer if newline, or too long
	null terminates string, and copy over buffer, uses cond send to prevernt blocking
	Return: void
*/
void handle_term_input(int unit, char ch){
	//address of terminal from terminal array
	TerminalState *term = &terminals[unit];
	//check the character written
	if (ch == '\n' || term->currentLength >= MAX_LINE){
		//map to null terminator
		term->currentLine[term->currentLength] = '\0';
		//continue coping while buff count is less then max
		if (term->buffCount < MAX_BUFFERS){
			strcpy(term->currentLine[term->lineTail], term->currentLine);
			term->lineTail = (term->lineTail + 1) % MAX_LINE;
			term->buffCount++;
		}
		//reset current length
		term->currentLength = 0;
		//use a mailbox
		MboxCondSend(term->readMbox, NULL, 0);
	}
	else {
		//else we are just reading in the character
		term->currentLine[term->currentLength++] = ch;
	}
	
}

/*
	Name: handler_term_output
	Purpose: meant to be a helper to write, finds terminal, checks if terminal is not writing, validates
	the current buffer position and the total write length then shift the buffer intot the buffer space
	calls usloss device output.
	Return: Void
*/
void handler_term_output(int unit){
	//find the terminal
	TerminalState *term = &terminals[unit];
	//check if theres a write
	if (!term->writeInProgress){
		return;
	}
	//if we have not hit the end
	if (term->writePos < term->writeLength){
		//base control with xmit, recv interupts inaabled
		int control = 0x7;
		//or'd with the bit from buff and shifted right to have the bit in the character portion
		control |= (term->writeBuff[term->writePos++] << 8);
		//call to usloss
		USLOSS_DeviceOutput(USLOSS_TERM_DEV, unit, (void *)(long)control);
	}
	else{
		//else flag as not writing
		term->writeInProgress = 0;
		//use a mailbox
		MboxCondSend(term->writeMbox, NULL, 0);
	}
}

/*
	Name: disk_driver
	Purpose: checks if pending calls to disk are present and utilize corrosponding mailboxes
	to wakeup and sleep process interacting with disk, act as the driver for disk functions
	Return: uses args struct to push information out, return 0 if disk operation fail.
*/
int disk_driver(void *args){
	int unit = (int)(long)args;
	int status;
	
	while (1){
		//recive on mailbox
		MboxRecv(disks[unit].mailbox, NULL, 0);
		//while disk has a pending req
		while (disks[unit].queueHd != NULL){
			//get request
			DiskRequest *req = disks[unit].queueHd;
			//check if req needs to seek
			if(req->startBlock != disks[unit].currTrack){
				//make a ref to a usloss seek request
				USLOSS_DeviceRequest seekReq;
				//fillout request
				seekReq.opr = USLOSS_DISK_SEEK;
				seekReq.reg1 = (void *)(long)req->startBlock;
				seekReq.reg2 = NULL;
				//fire off request
				USLOSS_DeviceOutput(USLOSS_DISK_DEV, unit, &seekReq);
				//wait on request
				waitDevice(USLOSS_DISK_DEV, unit, &status);
				//update disk postion
				disks[unit].currTrack = req->startBlock;
			}
		
			//once in correct track
			int i;
			for (i = 0; i < req->numBlocks; i++){
				//make a r/w request
				USLOSS_DeviceRequest rwReq;
				//update request
				rwReq.opr = req->op;
				rwReq.reg1 = (void *)(long)(req->startBlock + 1);
				rwReq.reg2 = (char *)req->buffer + i * DISK_BLOCK_SZ;
				//fire off request
				USLOSS_DeviceOutput(USLOSS_DISK_DEV, unit, &rwReq);
				//wait on request
				waitDevice(USLOSS_DISK_DEV, unit, &status);
				//check if failed
				if (status == USLOSS_DEV_ERROR){
					req->arg->arg1 = (void *)(long)USLOSS_DEV_ERROR;
					//if failed break out of loop
					break;
				}
			}
			//update arg members
			req->args->arg1 = (void *)(long)0;
			req->args->arg4 = (void *)(long)0;
			//send on mailbox 
			MboxSend(disks[unit].mailbox, NULL, 0);
			//update queue
			disks[unit].requestQueueFoward = req->next;
			//recv on mailbox 
			MboxRecv(disks[unit].mailbox, NULL, 0);
			//unblock
			unblockProc(req->pid);
			//free old request
			free(req);
		}
	}
	//if loop fails
	return 0;
}