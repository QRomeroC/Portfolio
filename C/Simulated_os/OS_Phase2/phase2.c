#include <usloss.h>
#include <phase2.h>
#include <phase1.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>

//macros
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
void phase2_start_service_processes(void);
int MboxCreate(int slots, int slot_size);
int MboxRelease(int mbox_id);
int MboxSend(int mbox_id, void *msg_ptr, int msg_size);
int MboxRecv(int mbox_id, void *msg_ptr, int msg_max_size);
int MboxCondSend(int mbox_id, void *msg_ptr, int msg_size);
int MboxCondRecv(int mbox_id, void *msg_ptr, int msg_max_size);
void waitDevice(int type, int unit, int *status);
void wakeupByDevice(int type, int unit, int status);
void nullsys(USLOSS_Sysargs *);
int is_kernel_mode(void);
void clockInterruptHandler(int, void*);
void diskInterruptHandler(int, void*);
void terminalDeviceInterruptHandler(int, void*);
void syscallInterruptHandler(int, void*);
unsigned int disable_interrupts(void);
//void enable_interrupts(void);
void set_psr_state(unsigned int old_psr);
int findSlot(); 
/*
--------------------------- End of Phase 2 Prototypes ------------------------------------------
*/

/*
---------------- Start of Phase 2 PCB Struct - members for messages and phase 2 requirements. --
*/
typedef struct PCB {
	//process attrabuites
	int pid;
	//int status;
	struct PCB * nextProducer;
	struct PCB * nextConsumer;
}PCB;
/*
----------------- End of Phase 2 PCB Struct -----------------------------------------------------
*/

/*
--------------- Start of Phase 2 mailbox struct -------------------------------------------------
*/
typedef struct Mailbox{
	//mailbox atrabuites
	int id;
	int allocated;

	//slot atrabuites
	int num_slots;
	int slots_used;
	int slot_size;
	
	//slot list and queue pointers
	struct Mailslot * hd_slot;
	struct PCB * producer_queue_hd_ptr;
	struct PCB * producer_queue_tail_ptr;
	//struct PCB * nextProducer;
	struct PCB * consumer_queue_hd_ptr;
	struct PCB * consumer_queue_tail_ptr;
	//struct PCB * nextConsumer;
	
}Mailbox;
/*
---------------- End of Phase 2 mailbox struct --------------------------------------------------
*/

/*
---------------- Start of Phase 2 slots struct --------------------------------------------------
*/

typedef struct Mailslot{
	//slot atrabuites
	int allocated; 
	int msg_size;
	
	//messeage
	//void * message[MAX_MESSAGE];
	void * message;
	//slot linked list references
	struct Mailslot *nextSlot;
}Mailslot;

/*
---------------- End of Phase 2 slot struct -----------------------------------------------------
*/

/*
----------------- Start of Phase 2 Globals ------------------------------------------------------
*/
static struct PCB process_table[MAXPROC];
void (*systemCallVec[USLOSS_MAX_SYSCALLS])(USLOSS_Sysargs *arg); //whats the right syntax for this?? need to find the macro definition for max syscall 
static struct Mailbox mail_box[MAXMBOX];
static struct Mailslot mail_slot[MAXSLOTS];
char * message_buffer[MAX_MESSAGE];
int remainingSlots = MAXSLOTS;
int remainingBoxes = MAXMBOX;
//block list?

// clock timer 
int clock_message_timer = 0; // elapsed time since last message sent. 
int clock_tics = 0; // counted every 20 mil when the clock interrupt fires. 
#define MESSAGE_WAIT_TIME 100 

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
	memset(process_table, 0, sizeof(PCB) * MAXPROC);
	memset(mail_box, 0, sizeof(Mailbox) * MAXMBOX);
	memset(mail_slot, 0, sizeof(Mailslot) * MAXSLOTS);
	
	//fill system call vector array
	for (int i = 0; i < USLOSS_MAX_SYSCALLS; i++){
			systemCallVec[i] = nullsys;
	}
	
	//initialize USLOSS_IntVec for interupts
	USLOSS_IntVec[USLOSS_CLOCK_INT] = clockInterruptHandler;
	//USLOSS_IntVec[USLOSS_DISK_INT] = disk_handler;
	//USLOSS_IntVec[USLOSS_TERM_INT] = terminal_hadler;
	//USLOSS_IntVec[USLOSS_SYSCALL_INT] = syscall_handler;
	//USLOSS_IntVec[USLOSS_ALARM_INT]
	//USLOSS_IntVec[USLOSS_MMU_INT]
	//USLOSS_IntVec[USLOSS_ILLEGAL_INT]
	
	for (int i = 0; i < 7; i++){
		MboxCreate(1,sizeof(int));
	}
	
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
void phase2_start_service_processes(void){
	return;
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
	Return: -1 - numSlots/slotSize is negative or exced allowable sleep
		  >= 0 - ID of allocated mailbox 
*/
int MboxCreate(int slots, int slot_size){
	//disable interrupts
	unsigned int oldPSR = disable_interrupts();

	//check kernel mode 
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode\n");
		USLOSS_Halt(1);
	}
	
	//check if slots is negative or too large
	if (slots < 0 || slot_size < 0 || slot_size > MAX_MESSAGE || slots > MAXSLOTS){
		set_psr_state(oldPSR);
		return -1;
	}
	
	//check if we have space in mailbox array
	if (remainingBoxes < 1){
		return -1;
	}
	
	//starting from first index find first open mailbox
	int i = 0;
	while (mail_box[i].allocated == ALLOCATED && i < MAXMBOX){
		i++;
	}
	if (i >= MAXMBOX){
		return -1;
	}
	
	//get a reference to the mailbox at i
	Mailbox * new_mailbox = &mail_box[i];
	//update id
	new_mailbox->id = i;
	//update status
	new_mailbox->allocated = ALLOCATED;
	//remember number of slots
	new_mailbox->num_slots = slots;
	//remember the size of each slot
	new_mailbox->slot_size = slot_size;
	//set pointer references to NULL -- memset should have done this?
	
	//update box count
	remainingBoxes--;
	
	//restore interrupts
	set_psr_state(oldPSR);
	//return mailbox id
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
	//disable interrupts
	unsigned int oldPSR = disable_interrupts();
	
	//check kernel mode 
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	
	//check if vaild mail box id
	if(!mail_box[mbox_id].allocated){
		return -1;
	}
	
	//get a reference to the mailbox at mailbox_id
	Mailbox * mailbox = &mail_box[mbox_id];
	//get a reference to the first slot mailbox is holding if any
	Mailslot * curr_slot = mailbox->hd_slot;
	//if curr_slot is null it will skip
	while (curr_slot){
		//if not null find if we have any more slots
		Mailslot * temp = curr_slot->nextSlot;
		//mark free
		curr_slot->allocated = UNALLOCATED;
		//free space(i dont think we need actually);
		//maybe free curr_slot->message since that the only thing in the struct with memory
		free(curr_slot);
		//increase the number of free slots
		remainingSlots++;
		//update curr_slot reference
		curr_slot = temp;
		
		/*
			need to add logic to deal with blocked process if they are in the mailbox to be realeased.
		*/
	}

	//by here mailbox has no slots liked to it
	//we might need to clear out the queues johhny boy we always have to clean out the damn queues
	
	//mark as free
	mailbox->allocated = UNALLOCATED;
	//bring box count back up by one
	remainingBoxes++;
	
	//restore interrupts
	set_psr_state(oldPSR);
	
	//return success
	return 0;
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
	//disable interrupts
	unsigned int oldPSR = disable_interrupts();
	
	//check kernel mode 
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	
	//check if vaild mailbox id
	if(mail_box[mbox_id].allocated == UNALLOCATED){
		return -1;
	}
	//get a reference to the mailbox
	struct Mailbox * mailbox = &mail_box[mbox_id];
	
	//check if the message being sent on this mailbox is too big
	//should also screen out zero sized messages??
	if(msg_size > mailbox->slot_size){
		return -1;
	}
	
	//block case 1: mailbox is full
	if(mailbox->slots_used == mailbox->num_slots){
		int new_pid = getpid();
		PCB * new_PCB = &process_table[new_pid % MAXPROC];
		new_PCB->pid = new_pid;
		
		if(mailbox->producer_queue_hd_ptr){
			PCB * temp = mailbox->producer_queue_hd_ptr;
			while(temp->nextProducer){
				temp = temp->nextProducer;
			}
			
			temp->nextProducer = new_PCB;
		}
		else {
			mailbox->producer_queue_hd_ptr = new_PCB;
		}
		blockMe();
	}
	
	//block case 2: mailbox has a producer queued
	if(mailbox->producer_queue_hd_ptr){
		int new_pid = getpid();
		PCB * new_PCB = &process_table[new_pid % MAXPROC];
		new_PCB->pid = new_pid;
		PCB * temp = mailbox->producer_queue_hd_ptr;
		while (temp->nextProducer){
			temp = temp->nextProducer;
		}
		temp->nextProducer = new_PCB;
		blockMe();
	}

	//zero slot mailbox
	/*
	if (mailbox->num_slots == 0){
		if(!mailbox.consumer_queue_hd_ptr){
			if(!mailbox.producer_queue_hd_ptr){
				mailbox.producer_queue_hd_ptr = PCB[getpid() % MAXPROC];
				mailbox.producer_queue_tail_ptr = producer_queue_hd_ptr->nextProducer;
				blockMe();
			}
			else{
				PCB * temp = mailbox.producer_queue_hd_ptr;
				while(temp->nextProducer){
					temp = temp->nextProducer;
				}
				temp->nextProducer = PCB[getpid() % MAXPROC];
				mailbox->producer_queue_tail_ptr = temp->nextProducer;
				blockMe();
			}
		}
		else{
			//deque head of consumer queue and wake(unblock)?
		}	
	}
	*/	

	//find the index of the first free slot
	int free_slot_index = findSlot();
	
	//if -1 then no free slots -- return -2...look i know but if it works its not stupid...just refactorable
	if (free_slot_index == -1){
		return -2;
	}
	
	//get a reference to the slot at free_slot_index
	Mailslot * free_slot = &mail_slot[free_slot_index];
	
	//should I malloc?
	free_slot->message = malloc(msg_size);
	//check if malloc failed -- standard protocal when using malloc
	if(free_slot->message == NULL){
		return -2;
	}
	
	//will need to double check syntax
	memcpy(free_slot->message, msg_ptr,msg_size);
	
	//update slot member attrabuites
	free_slot->allocated = ALLOCATED;
	free_slot->msg_size = msg_size;

	//if the mailbox currently has no messages pending the put the front of the list
	if(!mailbox->hd_slot){
		mailbox->hd_slot = free_slot;
	} 
	//else a messeage already exist, add to the tail to maintain ordering
	else {
		Mailslot * temp = mailbox->hd_slot;
		//we don't want to fall off the end of our list so look ahead to next
		while(temp->nextSlot){
			temp = temp->nextSlot;
		}
		temp->nextSlot = free_slot;
	}
	
	//update mailbox attrabuites
	mailbox->slots_used++;
	
	//check if we should wake someone up
	if(mailbox->consumer_queue_hd_ptr){
		PCB * process_to_unblock = mailbox->consumer_queue_hd_ptr;
		mailbox->consumer_queue_hd_ptr = mailbox->consumer_queue_hd_ptr->nextConsumer;
		unblockProc(process_to_unblock->pid);
	}
	
	//need to add checks earlier in this logic nighmare to querry mailbox slots used or num slot 
	//to see if the mail box is full 
	
	//restore interrupts
	set_psr_state(oldPSR);
	
	//return success
	return 0;
	
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
	unsigned int oldPSR = disable_interrupts();
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	
	//check if valid mailbox id
	if(mail_box[mbox_id].allocated == UNALLOCATED){
		return -1;
	}
	
	//IF A CONSUMER ALREADY EXISTS OR IF MESSAGES DONT EXIST IS WHERE IT GETS WONKY.
	
	//get a ref to the mailbox at mailbox id
	struct Mailbox * mailbox = &mail_box[mbox_id];
	//block case 1: recieving on an empty mailbox
	if(!mailbox->hd_slot){
		int new_pid = getpid();
		PCB * new_PCB = &process_table[new_pid % MAXPROC];
		new_PCB->pid = new_pid;
		if(mailbox->consumer_queue_hd_ptr){
			PCB * temp = mailbox->consumer_queue_hd_ptr;
			while(temp->nextConsumer){
				temp = temp->nextConsumer;
			}
			temp->nextConsumer = new_PCB;
		}
		else{
			mailbox->consumer_queue_hd_ptr = new_PCB;
		}
		blockMe();
	}
	//block case 2: a blocked consumer exists ahead of us
	if(mailbox->consumer_queue_hd_ptr){
		int new_pid = getpid();
		PCB * new_PCB = &process_table[new_pid % MAXPROC];
		new_PCB->pid = new_pid;
		if(mailbox->consumer_queue_hd_ptr){
			PCB * temp = mailbox->consumer_queue_hd_ptr;
			while(temp->nextConsumer){
				temp = temp->nextConsumer;
			}
			temp->nextConsumer = new_PCB;
		}
		else{
			mailbox->consumer_queue_hd_ptr = new_PCB;
		}
		blockMe();
	}
	
	//get a ref to the first slot
	struct Mailslot * hd_slot = mailbox->hd_slot;

	//if the message in slot is too big
	if (hd_slot->msg_size > msg_max_size){
		return -1;
	}
	
	/*
	if (mailbox->consumer_queue_hd_ptr != NULL){
		int new_pid = getpid();
		PCB * new_PCB = &process_table[new_pid % MAXPROC];
		new_PCB->pid = new_pid;
		PCB * temp = mailbox->consumer_queue_hd_ptr;
		while (temp->nextConsumer){
			temp = temp->nextConsumer;
		}
		temp->nextConsumer = new_PCB;
		blockMe();
	}
	*/
	
	//copy to msg_ptr buffer message held in the hd_slot.
	memcpy(msg_ptr,hd_slot->message,hd_slot->msg_size);
	
	//for sure we have to update mailbox to have correct counts
	free(hd_slot->message);
	hd_slot->message = NULL;
	hd_slot->allocated = UNALLOCATED;
	mailbox->hd_slot = hd_slot->nextSlot;
	
	//mailbox->num_slots++; <- same reason as in send, prob should not modify num_slots just how many of them are being used.
	mailbox->slots_used--;
	
	//check if we should wake someone up
	if (mailbox->producer_queue_hd_ptr){
		PCB * unblock_process = mailbox->producer_queue_hd_ptr;
		mailbox->producer_queue_hd_ptr = unblock_process->nextProducer;
		unblockProc(unblock_process->pid);
	}
	
	//restore interrupts
	set_psr_state(oldPSR);
	
	//return the size of message recieved.
	return hd_slot->msg_size;
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
	unsigned int oldPSR = disable_interrupts();
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	Mailbox * mailbox = &mail_box[mbox_id];
	//pre check if a block would occure
	if (mailbox->allocated == UNALLOCATED || msg_size > mailbox->slot_size) {
        return -1;
    }

    if (mailbox->slots_used == mailbox->num_slots && mailbox->consumer_queue_hd_ptr == NULL) {
        //Block would occur, return without receiving
		return -2; 
    }
	
	//set_psr_state(oldPSR);
	//if no block then invoke regular MboxSend
    return MboxSend(mbox_id, msg_ptr, msg_size);
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
	unsigned int oldPSR = disable_interrupts();
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	Mailbox * mailbox = &mail_box[mbox_id];
	//same as above we pre check condition of mailbox and if would block we do nothing
    if (mailbox->allocated == UNALLOCATED) {
        return -1;
    }

    if (mailbox->hd_slot == NULL && mailbox->producer_queue_hd_ptr == NULL) {
        // Block would occur, return without receiving
		return -2; 
    }
	
	//set_psr_state(oldPSR);
	//if no block would occure we invoke regular MboxRecv
    return MboxRecv(mbox_id, msg_ptr, msg_max_size);
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
	unsigned int oldPSR = disable_interrupts();
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	
	int mailbox_id;
	//identify the correct mailbox id relative to the device causing the wait
    if (type == USLOSS_CLOCK_INT) {
		//clock is mailbox[0] with no offset
        mailbox_id = 0;
    } else if (type == USLOSS_DISK_INT && unit >= 0 && unit <= 1) {
		//disk is mailbox[1] with offset {0,1}
        mailbox_id = 1 + unit;
    } else if (type == USLOSS_TERM_INT && unit >= 0 && unit <= 3) {
		//terminal is mailbox[3] with ofset {0,1,2,3}
        mailbox_id = 3 + unit;
    } else {
        USLOSS_Console("Error: Invalid device type or unit.\n");
        USLOSS_Halt(1);
    }
	
	//set_psr_state(oldPSR);
	//call recieve which should block on device until a message (interrput) is sent on the 
	//corresponding mailbox
    MboxCondRecv(mailbox_id, status, sizeof(int));
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
	unsigned int oldPSR = disable_interrupts();
	//check kernel mode through USLOSS
	if (!is_kernel_mode()){
		USLOSS_Console("Invalid Operation: Not in Kernel Mode");
		USLOSS_Halt(1);
	}
	
	if (!is_kernel_mode()) {
        USLOSS_Console("Error: Invalid Operation - Not in Kernel Mode\n");
        USLOSS_Halt(1);
    }
	//same as wait but instead of waiting to recieve, we send the message
    int mailbox_id;
	//similar to wait we get the right mailbox id first same base and offsets
    if (type == USLOSS_CLOCK_INT) {
        mailbox_id = 0;
    } else if (type == USLOSS_DISK_INT && unit >= 0 && unit <= 1) {
        mailbox_id = 1 + unit;
    } else if (type == USLOSS_TERM_INT && unit >= 0 && unit <= 3) {
        mailbox_id = 3 + unit;
    } else {
        USLOSS_Console("Error: Invalid device type or unit.\n");
        USLOSS_Halt(1);
    }
	
	//set_psr_state(oldPSR);
	//then call send, if there's a consumer waiting they'll be woken up
    MboxCondSend(mailbox_id, &status, sizeof(int));
}

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
void nullsys(USLOSS_Sysargs *){
	unsigned int oldPSR = disable_interrupts();
	//USLOSS_Sysargs *args = (USLOSS_Sysargs*)argVoid;
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
  return(USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet());
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
	//unsigned int oldPSR = disable_interrupts();
	//should we check kernel mode and disable interupts in this function
	//also need to make it static maybe since it is only called as a helper to kernel code
	if (remainingSlots < 1){
		return -1;
	}
	
	int i = 0;
	
	while (mail_slot[i].allocated){
		i++;
	}
	
	//set_psr_state(oldPSR);
	return i;
}

/*
	Name: disable_interrupts
	Purpose: update USLOSS PSR to be in kernel mode with interupts disabled.
	Return: an unsigned integer that represents the old PSR value.
*/
unsigned int disable_interrupts(){
  //printf("Disable Interrupts: \n");
  unsigned int oldPSR = USLOSS_PsrGet();
  //printf("oldPSR = %d\n", oldPSR);
  if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0){
	  USLOSS_Console("Error: Invalid Request - Not in Kernel Mode\n");
	  USLOSS_Halt(1);
  }
  USLOSS_PsrSet(USLOSS_PsrGet() & ~USLOSS_PSR_CURRENT_INT);
  //printf("new psr: %d\n",USLOSS_PsrGet());
  return oldPSR;
}

/*
---------------- depercated ----------------------
	Name: enable_interrupts
	Purpose: update USLOSS PSR to enable interrupts
	Return: N/A <- VOID 
*/
/*
void enable_interrupts(){
  if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0){
	  USLOSS_Console("Error: Invaild Request - Not in Kernel Mode\n");
	  USLOSS_Halt(1);
  }
  USLOSS_PsrSet(USLOSS_PsrGet() | USLOSS_PSR_CURRENT_INT);
 
}
*/
/*
	Name: set_psr_state
	Purpose: restore psr state to a assigned value 
	Return: N/A <- VOID
*/
void set_psr_state(unsigned int old_psr){
  //printf("welcom to set_psr_state\n");
  if(USLOSS_PsrSet(old_psr) != USLOSS_DEV_OK){
	USLOSS_Console("Error: PSR set state \n");
	USLOSS_Halt(1);
  }
}

/*
	Name: clockInterruptHandler
	Purpose: manage an interupt on the clock
	Return: N/A <- VOID
*/

void clockInterruptHandler(int dev,void *arg){
	unsigned int oldPSR = disable_interrupts();
	//check dev match correct device for clock interrupt
	if (dev != USLOSS_CLOCK_INT){
		USLOSS_Console("Error: Invalid clock interrupt device.\n");
		USLOSS_Halt(1);
	}
	
	//manage wake up timing - check elapsed time
	clock_tics++;

	int current_time = clock_tics * 20; 
	if (current_time - clock_message_timer >= MESSAGE_WAIT_TIME) {
		//conditionally send message 
		int status =  0;
		MboxCondSend(0, &status, sizeof(int));
		clock_message_timer = current_time;
	}
	
}

/*
	Name: diskInterruptHandler
	Purpose: identify the unit a disk interrupt occured and send a message on the corresponding mailbox
	Return: N/A <- VOID
*/
void diskInterruptHandler(int dev, void *arg) {
	unsigned int oldPSR = disable_interrupts();
	//check if vaild device
    if (dev != USLOSS_DISK_INT) {
        USLOSS_Console("Error: Invalid disk interrupt device.\n");
        USLOSS_Halt(1);
    }
	//syntax for the right memeber?
    int unit = (int)arg; 
	//validate unit value
    if (unit < 0 || unit > 1) {
        USLOSS_Console("Error: Invalid disk unit.\n");
        USLOSS_Halt(1);
    }

    int status;
    USLOSS_DeviceInput(USLOSS_DISK_INT, unit, &status);
	//conditionally send status to the corresponding terminal mailbox
	//base mailbox for disk is mailbox[1] unit can be {0,1}
	//max index in mailbox would be 1 + 1 = mailbox[2]
    MboxCondSend(1 + unit, &status, sizeof(int)); 
}

/*
	Name: terminalDeviceInterruptHandler
	Purpose: identify the unit a terminal interrupt occured and send a message on the corresponding mailbox
	Return: N/A <- VOID
*/
void terminalDeviceInterruptHandler(int dev, void *arg) {
	unsigned int oldPSR = disable_interrupts();
	//check if vaild device
    if (dev != USLOSS_TERM_INT) {
        USLOSS_Console("Error: Invalid terminal interrupt device.\n");
        USLOSS_Halt(1);
    }
	//syntax for arg struct memeber?
    int unit = (int)arg; // Terminal unit (0 to 3)
    if (unit < 0 || unit > 3) {
        USLOSS_Console("Error: Invalid terminal unit.\n");
        USLOSS_Halt(1);
    }

    int status;
    USLOSS_DeviceInput(USLOSS_TERM_INT, unit, &status);
	//conditionally send status to the corresponding terminal mailbox
	//base mailbox for terminals is mailbox[3] unit can be {0,1,2,3} 
	//max index in mailbox would be 3 + 3 = mailbox[6]
    MboxCondSend(3 + unit, &status, sizeof(int)); 
}

