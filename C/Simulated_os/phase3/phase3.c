#include <usloss.h>
#include <phase3.h>
#include <phase3_usermode.h>
#include <usyscall.h>
#include <string.h>

/*
sysargs struct for reference
typedef struct sysargs
{
	int number;
	void *arg1;
	void *arg2;
	void *arg3;
	void *arg4;
} sysargs;
*/

/*
----------- Start of Prototypes ----------------
*/
void phase3_init(void);
void phase3_start_service_processes(void); 
void syscall_spawn(USLOSS_Sysargs *args);
void syscall_wait(USLOSS_Sysargs *args);
void syscall_terminate(USLOSS_Sysargs *args);
void syscall_getpid(USLOSS_Sysargs *args);
void syscall_gettimeofday(USLOSS_Sysargs *args);
void syscall_dumpprocesses(USLOSS_Sysargs *args);
int kernSemCreate(int value, int *semID);
void syscall_semcreate(USLOSS_Sysargs *args);
void sem_gain(int semID);
void sem_release(int semID);
int kernSemP(int semID);
int kernSemV(int semID);
void syscall_semp(USLOSS_Sysargs *args);
void syscall_semv(USLOSS_Sysargs *args);
/*
---------- End of Prototypes -----------
*/



/*
---------- start of semaphore struct declaration -------------
*/
typedef struct semaphore {
	//flag to identify as in use
	int allocated;
	//current value of semaphore
	int value;
	//mutual exclusion will utilize a mailbox
	int mutex;
	//queue will utilize a mailbox
	int blockQueue;
} semaphore;
/*
--------- End of semaphore struct declaration
*/

/*
--------- Start of GLobals ---------------------------------------
*/
void (*systemCallVec[USLOSS_MAX_SYSCALLS])(USLOSS_Sysargs *args);
static semaphore semTable[MAXSEMS];
/*
--------- End of Globals -----------------------------------------
*/

/*
	Name: phase3_init
	Purpose: initalize the syscall vector with the syscalls 
	Return: N/A <- void - init never returns
*/
void phase3_init(void){
	//init system call vector 
	systemCallVec[SYS_SPAWN] = syscall_spawn;
	systemCallVec[SYS_WAIT] = syscall_wait;
	systemCallVec[SYS_TERMINATE] = syscall_terminate;
	systemCallVec[SYS_GETPID] = syscall_getpid;
	systemCallVec[SYS_GETTIMEOFDAY] = syscall_gettimeofday;
	systemCallVec[SYS_DUMPPROCESSES] = syscall_dumpprocesses;	
	systemCallVec[SYS_SEMCREATE] = syscall_semcreate;
	systemCallVec[SYS_SEMP] = syscall_semp;
	systemCallVec[SYS_SEMV] = syscall_semv;
}

/*
	Name: phase3_start_service_processes
	Purpose: initalize the semaphore table, init all sempaphores to unallocated
	and map the mailboxes for mutex/queue.
*/
void phase3_start_service_processes(void) {
	//set up semaphore table and assign a mailbox for its use
	for (int i = 0; i < MAXSEMS; i++){
		semTable[i].allocated = 0;
		semTable[i].mutex = MboxCreate(1,0);
		semTable[i].blockQueue = MboxCreate(0,0);
		MboxSend(semTable[i].mutex, NULL, 0);
	}	
}

/*
	Name: syscall_spawn
	Purpose: system call for spawn to create a new process that runs in user mode
	Return: N/A <- void
*/
void syscall_spawn(USLOSS_Sysargs *args){
	//args assignments <- per spec pg 6
	char *name = (char *) args->arg5;
	int (*func)(void *) = (int (*)(void *)) args->arg1;
	void *arg = (void *) args->arg2;
	int stack_size = (int) ((long)args->arg3);
	int priority = (int) ((long)args->arg4);
	
	//validate data check 
	if (name == NULL || func == NULL || stack_size < 1 || priority < 1 || priority > 6){
		args->arg1 = (void *) -1;
		args->arg4 = (void *) -1;
		return;
	}
	
	int pid = -1;
	//actual call to spawn
	int status = Spawn(name, func, arg, stack_size, priority, &pid);
	//check spawn return and assign according to spec
	if (status == 0){
		args->arg1 = (void *) pid;
		args->arg4 = (void *) 0;
	}
	else {
		args->arg1 = (void *) -1;
		args->arg4 = (void *) -1;
	}
}
/*
	Name: Spawn <- do i write spawn?
*/
/*
int Spawn (char *name, int (*func)(void *), void *arg, int stack_size, int priority, int *pid){
	//spawn should be the wrapper to sycall spawn right??
	
}
*/

/*
	Name: syscall_wait
	Purpose: system call for wait, which uses join
	Return: -2 : if no children
	        >= 0: pid of the cleaned up process
*/
void syscall_wait(USLOSS_Sysargs *args){
	int pid, status;
	int result = join(&pid, &status);
	
	if (result == -2){
		args->arg4 = (void *) -2;
		return;
	}
	//args assignments <- spec
	args->arg1 = (void *) pid;
	args->arg2 = (void *) status;
	args->arg4 = (void *) 0;
}

/*
	Name: syscall_terminate
	Purpose: system call for terminate, terminates the current process with a given status
	Return: void
*/
void syscall_terminate(USLOSS_Sysargs *args){
	int status = (int)(long) args->arg1;
	
	int child_pid, child_status;
	//join till no children to join
	while (1){
		int result = join(&child_pid, &child_status);
		if (result == -2){
			break;
		}
	}
	//quit on self with status from args
	quit(status);
}

/*
	Name: syscall_getpid
	Purpose: utilizes getpid from phase1, returns through out perams
	Return: void <- uses out perams
*/
void syscall_getpid(USLOSS_Sysargs *args){
	int pid;
	//call to getpid with address of var pid
	GetPID(&pid);
	//map into struct
	args->arg1 = (void *)(long) pid;
}

/*
	Name: syscall_gettimeofday
	Purpose: gets current time, similar to get pid uses out peram
	Return: void <- maps current time to args1
*/
void syscall_gettimeofday(USLOSS_Sysargs *args){
	//call to current time
	int time = currentTime();
	//map time to struct
	args->arg1 = (void *)(long) time;
}

/*
	Name: syscall_dumpprocesses
	Purpose: calls dumpProcess
	Return: void
*/
void syscall_dumpprocesses(USLOSS_Sysargs *args){
	DumpProcesses();
}

/*
	Name: kernSemCreate
	Purpose: creates and initalizes members of a semaphore from sem table, signature from 
	phase3_kernelInterfaces.h
	Return: -1: if unable to create
			0: if successful
*/
int kernSemCreate(int value, int *semID){
	//attempt to init a semaphore with negative value
	if (value < 0){
		return -1;
	}
	//fillout memebers
	for (int i = 0; i < MAXSEMS; i++){
		if (!semTable[i].allocated){
			semTable[i].allocated = 1;
			semTable[i].value = value;
			*semID = i;
			
			return 0;
		}
	}
	//semTable is full
	return -1;
}

/*
	Name: syscall_semcreate
	Purpose: system call for sem create
	Return: void
*/
void syscall_semcreate(USLOSS_Sysargs *args){
	//get the value of the semaphore to create
	int initValue = (int)(long) args->arg1;
	//declare var for semaphore id
	int semID;
	//pass to kernel sem create
	int retVal = kernSemCreate(initValue, &semID);
	//if = map semid and 0
	if (retVal == 0){
		args->arg1 = (void *)(long) semID;
		args->arg4 = (void *) 0;
	}
	else{
		//map -1 
		args->arg1 = (void *) -1;
		args->arg4 = (void *) -1;
	}
}

/*
	Name: sem_gain
	Purpose: recvs on the semaphore mutex mailbox to gain the lock 
	Return: void
*/
void sem_gain(int semID){
	MboxRecv(semTable[semID].mutex, NULL, 0);
}

/*
	Name: sem_release
	Purpose: sends on the semaphore mutex mailbox to release lock
	Return: void
*/
void sem_release(int semID){
	MboxSend(semTable[semID].mutex, NULL, 0);
}

/*
	Name: kernSemP
	Purpose: dec semaphore value by 1
	Return: -1: if unsuccessful
			0: if successful
*/
int kernSemP(int semID){
	//screen out bad ids
	if (semID < 0 || semID >= MAXSEMS || !semTable[semID].allocated){
		return -1;
	}
	//get lock
	sem_gain(semID);
	//find semaphore in sem table and dec value
	semTable[semID].value--;
	//if value went negative
	if(semTable[semID].value < 0){
		sem_release(semID);
		//block on sempahore
		MboxRecv(semTable[semID].blockQueue, NULL, 0);
		return 0;
	}
	//else relase the lock
	sem_release(semID);
	//and return
	return 0;
}

/*
	Name: kernSemV
	Purpose: dec semaphore value by 1
	Return: -1: if unsucessful
	        0: if successful
*/
int kernSemV(int semID){
	//screen out bad ids
	if (semID < 0 || semID >= MAXSEMS || !semTable[semID].allocated){
		return -1;
	}
	//gain lock
	sem_gain(semID);
	//inc value
	semTable[semID].value++;
	//if value was negative and stays negative block case
	if(semTable[semID].value <= 0){
		MboxSend(semTable[semID].blockQueue, NULL, 0);
	}
	//else release lock
	sem_release(semID);
	//and return
	return 0;
}

/*
	Name: syscall_semp
	Purpose: call kernSemP
	Return: void
*/
void syscall_semp(USLOSS_Sysargs *args){
	int semID = (int)(long) args->arg1;
	
	int retVal = kernSemP(semID);
	
	if (retVal == 0){	
		args->arg4 = (void *)(long) 0;
	}
	else {
		args->arg4 = (void *)(long) -1;
	}
}

/*
	Name: syscall_semv
	Purpose: call kernSemV
	Return: void
*/
void syscall_semv(USLOSS_Sysargs *args){
	int semID = (int)(long) args->arg1;
	
	int retVal = kernSemV(semID);
	if (retVal == 0){
		args->arg4 = (void *)(long) 0;
	}
	else {
		args->arg4 = (void *)(long) -1;
	}
}	