#include "phase2.h"

#define UNALLOCATED 0
#define ALLOCATED 1

/*
The following are the methods and their signatures found in phase 1 spec, should be the ones 
we can call from libphase1.a:
phase1_init()
 - void phase1 init(void)
spork()
 - int spork(char *name, int (*startFunc)(void*), void *arg, int stackSize, int priority)
join()
 - int join(int *status)
quit()
 - void quit(int status)
zap()
 - void zap(int pid)
getpid()
 - int getpid(void)
dumpProcesses()
 - void dumpProcesses(void)
blockMe()
 - void blockMe()
unblockProc()
 - int unblockProc(int pid)
*/

/*
---------------------------- Strat of Phase 2 Prototypes --------------------------------------
*/
void phase2_init(void);
void phase2_start_service_process(void);
int MboxCreate(int slots, int slot_size);
int MboxRelease(int mbox_id);
int MboxSend(int mbox_id, void *msg_ptr, int msg_size);
int MboxRecv(int mbox_id, void *msg_ptr, int msg_max_size);
int MboxCondSend(int mbox_id, void *msg_ptr, int msg_size);
int MboxCondRecv(int mbox_id, void *msg_ptr, int msg_max_size);
void waitDevice(int type, int unit, int *status);
void wakeupByDevice(int type, int unit, int status);
void (*systemCallVec[])(USLOSS_Sysargs *args);
void nullsys(void);
int is_kernel_mode(void);
void clockInterruptHandler(int, void*);
void diskInterruptHandler(int, void*);
void terminalDeviceInterruptHandler(int, void*);
void syscallInterruptHandler(int, void*);
//int slotsFull(); //<- iter through mailbox array and if one or more mailboxes are allocated, check the sum of slot sizes
/*
--------------------------- End of Phase 2 Prototypes ------------------------------------------
*/

/*
---------------- Start of Phase 2 PCB Struct - members for messages and phase 2 requirements. --
*/
struct Phase2_PCB {
  int pid;
  MailBox * mailbox;//?
  Message * msg;//?
  struct Phase2_PCB *nextBlocked;
}Phase2_PCB;
/*
----------------- End of Phase 2 PCB Struct -----------------------------------------------------
*/

/*
--------------- Start of Phase 2 mailbox struct -------------------------------------------------
*/
struct mailbox{
	int id;
	int allocated;
	//struct Phase2_PCB sender;??
	//struct Phase2_PCB receiver;
	int num_slots;
	int slot_size;
	//if the slot array holds an array of mail_msg structs then that's the data type of the pointer here?
	struct mailslot * hd_slot;
	struct mailbox * next_producer;
	struct mailbox * next_consumer;
	//struct mailslot * next_slot;
	//struct mail;
}mailbox;
/*
---------------- End of Phase 2 mailbox struct --------------------------------------------------
*/
/*

---------------- Start of Phase 2 slots struct --------------------------------------------------
*/

struct mailslot{
	int allocated; 
	char * msg;
	struct slot *nextSlot;//??
}mailslot;

/*
---------------- End of Phase 2 slot struct -----------------------------------------------------
*/

//************** depercated **********************
/*
---------------- Start of Phase 2 syscall struct ------------------------------------------------
*/
/*
struct phase2_syscall{
		void * fp;
}phase2_syscall;
*/
//************* depercated ***********************
/*
---------------- End of Phase 2 syscall struct --------------------------------------------------
*/

//*************** Depercated ***************************
/*-------------- Start of mail msg struct -------------------------------------------------------
*/
/*
struct mail_msg{
	//int allocated;??
	char * message[MAX_MESSAGE];
	struct mail_msg *nextSlot;//??
}mail_msg;
*/

/*
---------------- End of mail msg struct ---------------------------------------------------------
*/

/*
----------------- Start of Phase 2 Globals ------------------------------------------------------
*/
static Phase2_PCB Phase2_Proc_Table[MAXPROC];
void * systemCallVec[USLOSS_MAX_SYSCALLS]; //whats the right syntax for this?? need to find the macro definition for max syscall 
static MailBoxes[MAXMBOX];
static MailSlots[MAXSLOTS];
char * Message[MAX_MESSAGE];
int totalBoxes = MAXMBOX;
//reciver queue??
MailBox * consumerQueue;
MailBox * producerQueue;
//consumer queue??
//block list?
/*
----------------- End of Phase 2 Globals --------------------------------------------------------
*/

/*
	Name: phase2_init
	Kernel Mode: Yes
	May Block: No
	May Context Switch: No
	Purpose: similar to phase1_init, function is called by testcases during bootstrap,
	before any processes are running. Used to initialize data structures. Must not attempt
	to spork any process or use any other process specific functions.
	Return: N/A <- pretty sure it never returns
*/
void phase2_init(void){
	//USLOSS should initalize in kernel mode
	if(!is_kernel_mode()){
		USLOSS_Console("Error: Invalid access not in kernel mode.\n");
		USLOSS_Halt(1);
	}
	//initials data structures
	memset(Phase2_Proc_Table, 0, sizeof(Phase2_PCB));
	memset(MailBoxes, 0, sizeof(mailbox));
	memset(MailSlots, 0, sizeof(mail_msg));
	//memset(systemCallVec, 0, sizeof(phase2_syscall));
	//fill system call vector array
	int i;
	for (i = 0; i < NUM_SYSCALLS; i++){
			systemCallVec[i] = nullsys();
	}
	//pretty sure memset will do the bellow already?
	//for (i = 0; i < MAXSLOTS; i++){
	//	MailSlots[i].allocated = UNALLOCATED;
	//}
	
}

/*
	Name: phase2_start_service_process
	Kernel Mode: Yes
	May Block: No
	May Context Switch: Yes
	Purpose: Called by Phase 1 init, once processes are running but before the testcases begin.
	If phase 2 init requires any service process to be running this method will invoke spork to 
	create them.
*/
void phase2_start_service_process(void){
	//return;
}

/*
	Name: MboxCreate
	Kernel Mode: Yes
	May Block: No
	May Context Switch: No
	Purpose: Create a new mailbox. Assigns mailbox id to index of mailbox array.
	Arg: 
		int slots (numSlots) - the maximum number of slots that may be used to queue up
		messages from this mailbox
	Return: -1 - numSlots/slotSize is negative or exced allowable size
		  >= 0 - ID of allocated mailbox 
*/
int MboxCreate(int slots, int slot_size){
	//disableInterupts();???
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	//check if slots is negative or too large
	if (slots < 0 || slot_size < 0 || slot_size > MAX_MESSAGE || slots > MAXSLOTS){
		return -1;
	}
	
	mailbox * new_mailbox = NULL;
	int i = 0;
	if (totalBoxes < 1){
		return -1;
	}
	while (MailBox[i].allocated){
		i++;
	}
	

	new_mailbox = MailBox[i];
	new_mailbox.id = i;
	new_mailbox.allocated = ALLOCATED;
	new_mailbox.num_slots = slots;
	new_mailbox.slot_size = slot_size;
	totalBoxes--;
	//could make aouther helper that does this
	/*
	int j = findSlot();
	new_mailbox.hd_slot = MailSlots[j];
	int c = 1;
	mail_msg * temp_msg = new_mailbox.hd_slot;
	while(c < slots){//<- dont know if <= or just <
		temp_msg.nextSlot = MailSlots[getSlot()];
		temp_msg = temp_msg.nextSlot;
		c++;
	}
	
	remainingSlots -= slots;
	*/
	
	return i;		
}

/*
	Name: MboxRelease
	Kernel Mode: Yes
	May Block: No
	May Context Switch: Yes 
	Purpose: Destroys a mailbox. All slots consumed by the mailbox will be freed
	All blocked producers and consumers will be unblocked and return -1.
	Once a mailbox has been marked as destroyed no process will be allowed to 
	block on it. Any attempt to send or recive on it will return -1.
	Return: -1 - The ID is not a mailbox that is currently in use
			 0 - Success 
*/
int MboxRelease(int mbox_id){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	//check if vaild mail box id
	if(!MailBox[mbox_id].allocated){
		return -1;
	}
	MailBox * mailbox = MailBox[mbox_id];
	//do we need to check null?
	if (!mailbox){
		return -1;
	}
	
	mail_msg * curr_slot = mailbox.hd_slot;
	while (curr_slot){
		mail_msg * temp = curr_slot.next_slot;
		free(curr_slot);
		curr_slot = temp;
	}
	totalBoxes++;
	mailbox.allocated = UNALLOCATED;
	
}

/*
	Name: MboxSend
	Kernel Mode: Yes
	May Block: Yes
	May Context Switch: Yes
	Purpose: sends a message through a mailbox. If message is delivered directly to
	a consumer or queued up in a mail slot, then this function will not block. It may 
	context switch if it wakes up a higher-priority process. If there are no consumers
	queued and no space available to queue a message, then the process will block until
	message can be delivered - either to a consumer or to a mail slot.
	Return: -2 - The system has run out of global mailbox slots, message could not be queued
			-1 - Illegal values given as arguements, including:
				invalid mailbox id or mailbox was released before send happened
			0 - success
*/
int MboxSend(int mbox_id, void *msg_ptr, int msg_size){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
}

/*
	Name: MboxRecv
	Kernel Mode: Yes
	May Block: Yes
	May Context Switch: Yes
	Purpose: waits to receive a message through a mailbox. If there is a message already
	queued in a mail slot, it may read it directly and return. Otherwise it will block until
	a message is available.
	Return: -1 - Illegal values given as arguement, including:
				invalid mailbox ID or message was too large for buffer
				or mailbox was released before recive could happened
		  >= 0 - The size of the message recieved.
*/
int MboxRecv(int mbox_id, void *msg_ptr, int msg_max_size){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	
}

/*
	Name: MboxCondSend
	Kernel Mode: Yes 
	May Block: No
	May Context Switch: Yes
	Purpose: Similar to MboxSend except it refuses to block. If at any point it would block
	it will return -2 instead. While it may not block it can context switch, if a process with 
	higher priority is woken up.
	Return: 0 - if successful          <- taken from .h
			1 - if mailbox is full     <- taken from .h
		   -1 - if illegal arguements  <- taken from .h
		   -2 - if block
*/
int MboxCondSend(int mbox_id, void *msg_ptr, int msg_size){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
}

/*
	Name: MboxCondRecv
	Kernel Mode: Yes 
	May Block: No
	May Context Switch: Yes
	Purpose: Similar to MboxRecv but may not block. If at any point it would block
	it will return -2 instead. While it may not block it can context switch, if a process with 
	higher priority is woken up.
	Return: 0 - if successful           <- taken from .h
			1 - if mailbox is full      <- taken from .h
		   -1 - if illegal arguements   <- taken from .h
		   -2 - if block
*/
int MboxCondRecv(int mbox_id, void *msg_ptr, int msg_max_size){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
}

/*
	Name: waitDevice
	Kernel Mode: Yes 
	May Block: Yes
	May Context Switch: Yes
	Purpose: Waits for an interrupt to fire on a given device. Only three device types
	are valid: Clock, Disk, and Terminal Devices. Unit field must be valid:
	       (0 - clock), (0,1 - Disk), (0,1,2,3 - Terminal). If invalid report error and halt
	waitDevice will Recieve from the proper mailbox for the divice, when the message is recieved
	it will store the status (status is in message payload) into the out parameter and return
	Return: *status - wait will update status with status recieved in payload
	
	Args:
	int type = interrupt device type
	int unit = # of device (when more than one) <- must be valid
	int *status = where interrupts handler puts device status register
*/
void waitDevice(int type, int unit, int *status){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
}

/*
	Name: wakeupByDevice
	Kernel Mode: Yes
	May Block: Yes
	May Context Switch: Yes
	Purpose: wakes up a blocked or sleeped process by a device interupt?
	Return: N/A -- VOID <- wakes up the process
	
	Args:
	int type = interrupt device type
	int unit = # of device (when more than one) <- must be valid
	int *status = where interrupts handler puts device status register
*/
void wakeupByDevice(int type, int unit, int status){
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
}

/*
	Name: (*systemCallVec[]) <- function pointer **IS THIS NEEDED**
	Kernel Mode: ?
	May Block: ?
	May Context Swtich: ? 
	Purpose: a function pointer to an array of system call functions
	acts as a function wrapper, to interface with USLOSS and have a signature
	USLOSS can utilize
	Return: N/A -- VOID <- maps a given system call to a USLOSS valid signature?
*/
void (*systemCallVec[USLOSS_MAX_SYSCALLS])(USLOSS_Sysargs *args);
/*
	Name: nullsys
	Kernel Mode: No
	May Block: No 
	May Context Switch: No
	Purpose: A default system call function, nullsys is the initialized system call
	function that all indices in our syscall table will map to. If any syscall happens
	in phase 2 nullsys call will be invoked.
	Return: N/A -- VOID <- prints error and halts.
*/
void nullsys(void){
		USLOSS_Console("Error: Enter Error Message Here");
		USLOSS_Halt(1);
}

/*
	Name: is_kernel_mode
	Kernel Mode: No
	May Block: No
	May Context Switch: No
	Purpose: verify if OS is currently in kernel mode
	Return: The current process status register value masked with PsrGet()
*/
int is_kernel_mode(void){  
  return(PSR_CURRENT_MODE & USLOSS_PsrGet() );
}

/*
	Name: findSlot
	Kernel Mode: Yes
	May Block: No
	May Context Switch: No
	Purpose: find the next open slot in MailSlots Table
	Return: >= 0 - the index of the first slot that does not have a mail_msg struct 
				   inside of it.
			-1 - if no free slots exisit <- should not happen because we check if 
			     enough unallocated slots exist before trying to find an open slot
*/
int findSlot(){
	int i = 0;
	while (MailSlots[i] && i <= MAXSLOTS){
		i++;
	}
	if (i > MAXSLOTS){
		return -1;
	}
	
	return i;
}
			