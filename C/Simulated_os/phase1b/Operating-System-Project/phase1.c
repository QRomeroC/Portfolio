#include <phase1.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define READY		0
#define RUNNING 	1
#define BLOCKED 	2
#define TERMINATED 	3

#define PSR_CURRENT_MODE 		0x1
#define PSR_CURRENT_INTERRUPTS  0x2
#define PSR_PREV_MODE    		0x4
#define PSR_PREV_INTERRUPTS     0x8

#define TIMEMAX 80000

// The following struct is in the header, here for ref
typedef struct Process {
  int pid;
  char * name;
  char * stack;
  unsigned int PSR; //<- not needed? since we dont need psr state for long term
  int process_state;

  int exit_status; 
  int status; 
  int wasZapped;
 
  long run_time; 
  long start_time;
  int allocated;
  int priority;
  struct Process * next_priority;
  struct Process * next_block;
  struct Process * parent;
  struct Process * child;
  struct Process * elderSib;
  struct Process * jrSib;
  struct USLOSS_Context context;
  int( * fp)(void * );
  void * args;
} Process;

/**** Function declaration ****/
void add_to_tail(Process * queue, Process * new);
void printChildren(Process * parent);
int  testcase_wrapper(void*);
void init_wrapper(void);
void func_wrapper();

unsigned int disable_interrupts(void);
unsigned int enable_interrupts(void);
unsigned int psr_on(unsigned int psr_flag);
unsigned int psr_off(unsigned int psr_flag);
void set_psr_state(unsigned int old_psr);

void set_kernal_mode(void);
void check_kernal_mode(void);
int check_has_child(void);

//void dispatcher();
void  add_To_Priority_Queue(Process * new_process);
void add_process_next(Process * adding, Process * r_queue);
int get_process_next();



int init_main(void *);
int is_kernel_mode();

//void blockMe();
void add_to_block_list(Process * process_to_block);
//int unblockProc(int pid);
//void zap(int pid);
/***********************************/



//proc pointer for current process
Process * currProcRunning = NULL;
//context pointers for context switching (could be current and next)
USLOSS_Context * parent_context;
USLOSS_Context * child_context;

//process table
Process PCB[MAXPROC];
//process id counter
int nextPID = 1;
int avaiable_PCB_slots = MAXPROC;
//priority queues
Process * pq_Level_1 = NULL;
Process * pq_Level_2 = NULL;
Process * pq_Level_3 = NULL;
Process * pq_Level_4 = NULL;
Process * pq_Level_5 = NULL;
Process * pq_Level_6 = NULL;
Process * pq_Level_7 = NULL;
//block list
Process * blockList = NULL;

//phase 1 initalize makes the init proc stuct and parks it in PCB[1]
void phase1_init(void) {
  USLOSS_Console("phase2_start_service_processes() called -- currently a NOP\n");
  USLOSS_Console("phase3_start_service_processes() called -- currently a NOP\n");
  USLOSS_Console("phase4_start_service_processes() called -- currently a NOP\n");
  USLOSS_Console("phase5_start_service_processes() called -- currently a NOP\n");
  //memset the pcb with size of a pcb struct
  memset(PCB, 0, sizeof(PCB));

  int PCB_index = nextPID % MAXPROC;
  
  PCB[PCB_index].pid = nextPID;
  PCB[PCB_index].name = strdup("init");
  //PCB[PCB_index].process_state = READY;
  PCB[PCB_index].priority = 6;
  PCB[PCB_index].allocated = 1;
  PCB[PCB_index].fp = init_main;
  PCB[PCB_index].exit_status = 0xDEAD; // if we ever see 57005 we screwed up. 
  

  PCB[PCB_index].stack = (char *)malloc(USLOSS_MIN_STACK);
  if (!PCB[PCB_index].stack) {
    USLOSS_Console("Error: Memory allocation failed for stack\n");
    USLOSS_Halt(1);
  }

  USLOSS_ContextInit(&PCB[PCB_index].context, PCB[PCB_index].stack, USLOSS_MIN_STACK, NULL, init_wrapper);
  avaiable_PCB_slots--;
  parent_context = &PCB[PCB_index].context;

  add_To_Priority_Queue(&PCB[PCB_index]);
}

int init_main(void *) {
  set_psr_state(PSR_CURRENT_MODE);
	
  int retVal = spork("testcase_main", testcase_wrapper, "testcase_main", USLOSS_MIN_STACK, 3);
  /*
  if (retVal == -2) {
    printf("Error: stackSize is less than USLOSS_MIN_STACK\n");
  } else if (retVal == -1) {
    printf("Error: No empty slots, priority out of range, or invalid parameters\n");
  }*/
  //printf("RetVal(Spork): %d\n",retVal);
  dispatcher();
  //TEMP_switchTo(retVal);
  //quit(0);
  return -2;
}

void init_wrapper(void) {
  set_psr_state(PSR_CURRENT_MODE);
 
  init_main(NULL);
  dispatcher();
  quit(0);
  
}

int testcase_wrapper(void *) {	
  unsigned int oldPSR = enable_interrupts();

  testcase_main();
  dispatcher();
  if(PCB[nextPID%MAXPROC].allocated)
	quit(0);    
  else 
	USLOSS_Halt(0);
  set_psr_state(oldPSR);
  
  return 0;
}
int spork(char * name, int( * func)(void * ), void * arg, int stacksize, int priority) {
  // Check Kernal mode
  check_kernal_mode();
  // disableInterrupts
  unsigned int oldPSR = disable_interrupts();

  if (stacksize < USLOSS_MIN_STACK) {
    return -2;
  }

  
  //check avaiable PCB slots for room
  if(avaiable_PCB_slots < 1)
	return -1;
  
  while (PCB[nextPID % MAXPROC].allocated) {
	//printf("\tCurrent Slot %d   : PID %d\n", nextPID, PCB[nextPID% MAXPROC].pid );
	nextPID++;
  }
  //check avaiable PCB slots for room
  avaiable_PCB_slots--;
  int slot = nextPID % MAXPROC;
  //printf("slot: %d\n",nextPID);
  //printf("pid(inside spork): %d\n",nextPID);
  PCB[slot].pid = nextPID;
  PCB[slot].name = strdup(name);
  //PCB[nextPID% MAXPROC].process_state = READY;
  PCB[slot].priority = priority;
  PCB[slot].parent = currProcRunning;
  PCB[slot].allocated = 1;
  PCB[slot].exit_status = 0xDEAD;
  PCB[slot].fp = func;
  PCB[slot].args = arg;

  PCB[slot].stack = (char *)malloc(stacksize);
  if (!PCB[slot].stack) {
    USLOSS_Console("Error: Memory allocation failed for stack\n");
    return -2;
  }
  //printf("currProcRunning: %s\n",currProcRunning->name);
  if (!currProcRunning->child) {
    currProcRunning->child = &PCB[slot];
  } else {
    Process * currChild = currProcRunning->child;
    while (currChild->jrSib)
      currChild = currChild->jrSib;

    currChild->jrSib = &PCB[slot];
  }

  USLOSS_ContextInit(&PCB[slot].context, PCB[slot].stack, stacksize, NULL, func_wrapper);

  set_psr_state(oldPSR);
  add_To_Priority_Queue(&PCB[slot]);
  dispatcher();
  return PCB[nextPID% MAXPROC].pid;
}

void func_wrapper(void) {
   //kMode = USLOSS_PsrGet() & 0x1;

  unsigned int oldPSR = enable_interrupts();
  PCB[nextPID % MAXPROC].fp(PCB[nextPID % MAXPROC].args);
  set_psr_state(oldPSR);
  quit(0);
   	
}

int join(int *status) {
  //printf("ENTER Join w/ status %d\n", *status);
   //CHECK kMode? set kmode! check and set?	
  check_kernal_mode();
  
  unsigned int oldPSR = disable_interrupts();
  if (status == NULL) {
    return -3; // Invalid argument
  }
  //printChildren(currProcRunning);
  Process *currChild = currProcRunning->child;
  
  
  if(currChild == NULL)
    return -2; 
  
  //printf("Current Child %s status %d \n",currProcRunning->child->name, currProcRunning->child->process_state);

  while (currChild!= NULL){
	if(currChild->process_state == TERMINATED){
	  //printf("Found termeinated process: %s ", currChild-> name);
	  // Only child 
	  if( (currChild == currProcRunning->child) && (currChild->jrSib == NULL)){
		//printf("only Child\n");
		currProcRunning->child = NULL;   	
	  }else if(currChild->jrSib != NULL){
		//printf("Has siblings\n");
		//Eldest 
		if(currChild == currProcRunning->child){
	      currProcRunning->child = currChild->jrSib;
		  currChild->jrSib->elderSib =NULL; // child'S jr'S elder == null
		}
		//Middle child
		else{
		  currChild->elderSib->jrSib = currChild->jrSib;
		  currChild->jrSib->jrSib = currChild->elderSib;
		} 
		//Youngest
	  
	  }else{
		//printf("No siblings\n");
		currChild->elderSib->jrSib = NULL;
	  }
	  // Free Memory
	  
	  int pid = currChild -> pid;
	  //printf("currChild pre: %s  ",currChild->name );
	  *status = currChild->exit_status;
	  free(currChild -> stack);
	  memset(currChild, 0 ,sizeof(Process));
	  //printf("post: %s  \n",currChild->name );
	  avaiable_PCB_slots++;
	  
	  //*status = currProcRunning->pid; 
	  
	  return pid;
	}else{
	  currChild = currChild->jrSib;
    }
  }
  set_psr_state(oldPSR);
  *status = -1;
  return currProcRunning->pid; 

}


 void quit(int status) {
 printf("ENTER QUIT w/ status %d\n", status);
	int oldPSR = disable_interrupts();
	printf("Current running process: %s %d %d\n", currProcRunning-> name, currProcRunning-> pid, currProcRunning->process_state);
	currProcRunning->process_state = TERMINATED;
	currProcRunning->exit_status = status;
	if (currProcRunning->child){
        USLOSS_Console("Process pid %d called quit() while it still had children\n",currProcRunning->pid);
        USLOSS_Halt(1);
    }
	currProcRunning = NULL;
	dispatcher();
	
	oldPSR++; // this negates the warning for unused var....
	assert(0);
}


int getpid(void) {
  return currProcRunning->pid;
}
/*
	Name: blockMe()
	Purpose: update current process running's process state as blocked and add it to the block
	         list. Dispatcher already removes current process from its queue.
	Return: void
*/
void blockMe(){
		if (!currProcRunning){
			//temp print for double checking logic
			printf("should never happen since only the current process can be blocked\n");
		}
		else {
			//remove_PQ(currProcRunning);
			//process_to_block.process_state = BLOCKED;
			add_to_block_list(currProcRunning);
			currProcRunning->process_state = BLOCKED;
			dispatcher();
		}
}
/*
void remove_PQ(Process *process_to_remove){
		int process_priority = process_to_remove->priority;
		
}
*/
/*
	Name: add_to_block_list(process *)
	Purpose: takes as an argument the a process pointer represent the 
	process to add to the block list.
	Return: void
*/
void add_to_block_list(Process * process_to_block){
		//process_to_block.process_state = BLOCKED;
		if (!blockList){
			blockList = process_to_block;
		}else{
			process_to_block->next_block = blockList;
			blockList = process_to_block;
		}
}
/*
	Name: unblockProc(int pid)
	Purpose: takes as an argument a process id value that represents the process
	to unblock. It finds the process given the pid, sets the process_state flag to Ready
	and updates the block list
	Return: -2 : if element not in blockList (i.e. not blocked)
	         0 : otherwise
*/
int unblockProc(int pid){
		
		if (!blockList){
			return -2;
		}
		//should we check if its the right process here??
		Process process_to_unblock = PCB[pid%MAXPROC];
		if (process_to_unblock.process_state != BLOCKED){
			return -2;
		}
		else {
			//should we check if the recoved process matches pids?
			process_to_unblock.process_state = READY;
			Process * prev = NULL;
			Process * blockList_ptr = blockList;
			while (blockList_ptr){
				if (blockList_ptr->pid == pid){
					if (!prev){
						blockList = blockList_ptr->next_block;
						add_To_Priority_Queue(blockList_ptr);
						return 0;
					}
					else{
						prev->next_block = blockList_ptr->next_block;
						add_To_Priority_Queue(blockList_ptr);
						return 0;
					}
				}
				prev = blockList_ptr;
				blockList_ptr = blockList_ptr->next_block;
			}
		}
		return -2;
}

/*
	Name: zap(int pid)
	Purpose: takes as argument a integer pid that represents the process id 
	to zap. Guards against bad zap calls:
		zap against init
		zap against itself
		zap against non-existing/terminated process
	Return: void
*/
void zap(int pid){
	//Guard case 1: zapping init
	if (pid == 1){
		USLOSS_Console("Cannot zap init\n");
		USLOSS_Halt(1);
	}
	//Guard case 2: zapping one's self
	if (currProcRunning->pid == pid){
		USLOSS_Console("Cannot zap one's self\n");
		USLOSS_Halt(1);
	}
	Process * process_to_zap = &PCB[pid % MAXPROC];
	//Guard case 3: zapping a null or terminated process
	if (!process_to_zap->allocated || process_to_zap->process_state == TERMINATED){
		USLOSS_Console("Invalid process to zap: process either terminated or does not exist\n");
		USLOSS_Halt(1);
	}
	
	process_to_zap->wasZapped = 1;
	
}
void dumpProcesses(){
  unsigned int oldPSR = disable_interrupts();
  //USLOSS_Console("**************** Calling dumpProcesses() *******************\n");
  USLOSS_Console(" PID  PPID  NAME\t\t\t  PRIORITY\t STATE\n");
  for(int i =0; i < MAXPROC; i++){    
	  if (PCB[i].allocated) {
		USLOSS_Console("  %d\t", PCB[i].pid);
		if(PCB[i].parent)
		  USLOSS_Console("%d   ", PCB[i].parent->pid);
	    else 
		  USLOSS_Console("0   ");
	    USLOSS_Console("%s", PCB[i].name);
		if(strlen(PCB[i].name)< 6)
			USLOSS_Console("\t\t\t  ");
		else
			USLOSS_Console("\t\t  ");
        USLOSS_Console("%d \t\t ", PCB[i].priority);
        USLOSS_Console("%d ", PCB[i].process_state);
	    if(PCB[i].process_state == TERMINATED)
		  USLOSS_Console("(%d)",PCB[i].exit_status);
     
	  USLOSS_Console("\n");
	}
  }
  set_psr_state(oldPSR);
}
	

void dumpProcesses_old(void) {
  //oldPSR = disableInterups();?? make oldPSR an unsigned int?
  //unsigned int oldPSR = USLOSS_PsrGet();
  //USLOSS_PsrSet(0x1);
  unsigned int oldPSR = disable_interrupts();
  
  for (int i = 0; i < MAXPROC; i++) {
    if (PCB[i].allocated) {
      printf("\n\nProcess #%d\n", i);
      printf("PID: %d \n", PCB[i].pid);
      printf("NAME: %s \n", PCB[i].name);
      printf("process_stateE: %d \n", PCB[i].process_state);
      printf("PRIORITY: %d \n", PCB[i].priority);
     // printf("PARENT_PID: %d \n", PCB[i].parent ? PCB[i].parent->pid : -1);
      printChildren(&PCB[i]);
    }
  }

  set_psr_state(oldPSR);
  
}

void printChildren(Process * parent) {
  Process * currChild = parent->child;
  printf("Child(s):\tPID[");
  while (currChild != NULL) {
    printf(" %d, ", currChild->pid);
    currChild = currChild->jrSib; 
  }
  printf("]\n");
}
unsigned int enable_interrupts(){
  //printf("Enable Interrupts: \n");
  unsigned int oldPSR = USLOSS_PsrGet();
  unsigned int no_need = 0; 
  no_need = USLOSS_PsrSet(oldPSR | PSR_CURRENT_INTERRUPTS);
  no_need++;
  return oldPSR;
}
unsigned int disable_interrupts(){
  //printf("Disable Interrupts: \n");
  unsigned int oldPSR = USLOSS_PsrGet();
  unsigned int no_need = 0; 
  no_need = USLOSS_PsrSet(oldPSR & ~PSR_CURRENT_INTERRUPTS);
  no_need++;
  return oldPSR;
}
int is_kernel_mode(){  
  return(PSR_CURRENT_MODE & USLOSS_PsrGet() );
}
void check_kernal_mode(){
  if(!is_kernel_mode()){
	USLOSS_Console("ERROR: not Kernal Mode\n");
	USLOSS_Halt(1);
}}
/*
*  Set PSR State appered in both psr on and off as a simple error check. 
*  pulled out to use indepently of other psr on/off logic. 
*/
void set_psr_state(unsigned int old_psr){
  if( USLOSS_PsrSet(old_psr) != USLOSS_DEV_OK){
	USLOSS_Console("Error: PSR set state \n");
	USLOSS_Halt(1);
  }
}




void  add_To_Priority_Queue(Process * new_process){
  //printf("Adding %s to Queue %d to  ", new_process->name,new_process->priority);
  int priority = new_process-> priority;
  Process ** current_queue = NULL; 
  switch(priority){
    case 1 : current_queue = &pq_Level_1; break; 	
    case 2 : current_queue = &pq_Level_2; break; 	
    case 3 : current_queue = &pq_Level_3; break; 	
    case 4 : current_queue = &pq_Level_4; break; 	
    case 5 : current_queue = &pq_Level_5; break; 	
    case 6 : current_queue = &pq_Level_6; break; 
    default:
	  USLOSS_Console("ERROR: Invalid Priority %d\n", priority);
	  USLOSS_Halt(1);
	}
	
	// found the right queue now add to the tail. 
  if(*current_queue == NULL){
	// queue is empty and we just add new_process	
	// printf("Head of the list\n");
	*current_queue = new_process;
  }
  else {
	// We find the tail and add it there.
	Process * tail = *current_queue;
    while (tail->next_priority)
      tail = tail->next_priority;
    tail->next_priority = new_process; 
	// printf("Tail of the list\n");
  }
  new_process-> next_priority = NULL;  //should already be NULL from memset but just incase. 
}


void dispatcher(){
  //check kernel mode or end
  check_kernal_mode();
  
  //Check that current running has exceded it's time limit. else return. 
  if(currProcRunning){
	currProcRunning->run_time += currentTime() - currProcRunning->start_time;
	if(currProcRunning->run_time < TIMEMAX){
	  return; 
	}
	currProcRunning->process_state = READY;
	add_To_Priority_Queue(currProcRunning);
  }
  
  
  //printf("ENTER Dispatcher\n");
  //printf("Current Process %s\n", currProcRunning ? currProcRunning->name : "NULL");
  
  
  Process ** queue_head = NULL; 
  // tight group of ifs all looking for the first queue with process.
  if(pq_Level_1 != NULL)	   { queue_head = &pq_Level_1; }
  else if( pq_Level_2 != NULL) { queue_head = &pq_Level_2; }
  else if( pq_Level_3 != NULL) { queue_head = &pq_Level_3; }
  else if( pq_Level_4 != NULL) { queue_head = &pq_Level_4; }
  else if( pq_Level_5 != NULL) { queue_head = &pq_Level_5; }
  else if( pq_Level_6 != NULL) { queue_head = &pq_Level_6; }
  // IDK if we need to check queue 7 
  else if( pq_Level_7 != NULL){ queue_head = &pq_Level_7; }
  else{
	// No process are queued. 
	USLOSS_Console("Dispatcher has no runnable process\n");
	//dumpProcesses();
	USLOSS_Halt(1);
  }
  // Remove head of Queue
  Process * next = *queue_head;
  *queue_head = next->next_priority;
  next->next_priority = NULL;
	

  Process * oldProc = currProcRunning;
  currProcRunning = next;
  next->start_time = currentTime();
  next->process_state = RUNNING;
  if (!oldProc){
	//printf("NO old process switching to next. \n");
	USLOSS_ContextSwitch(NULL, &next->context);
  }
  else{
	
	//printf("Old procees was %s \n", oldProc->name);
	USLOSS_ContextSwitch(&oldProc->context, &next->context);
  }	
  // Timer Stuff?  
  //printf("\tdispatcher is switching from %d -> %d\n",((oldProc)?oldProc->pid:-1), next->pid );
  //switch_context(currProcRunning->pid, next->pid, READY);
	
	// put it back? 

}
