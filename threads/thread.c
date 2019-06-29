#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"
#include <stdbool.h>

/* This is the wait queue structure */
struct wait_queue {
    /* ... Fill this in Lab 3 ... */
    struct thread* wait_head;
    Tid targetThread;
};

/* This is the thread control block */
typedef struct thread {
    /* ... Fill this in ... */
    Tid thread_id;
    ucontext_t thread_context;
    struct thread* next;
    void* sp;
} threadT;

//typedef struct exit {
//    threadT* current;
//    struct exit* exit_next;
//} exitThread;


threadT* running;
threadT* ready_queue;
bool stackNumber[THREAD_MAX_THREADS + 1];
threadT* clear_lastQueue;
bool deleteALL;
//exitThread* exit_queue_head;

/* thread starts by calling thread_stub. The arguments to thread_stub are the
 * thread_main() function, and one argument to the thread_main() function. */

void deleteReadyQueue() {
    volatile int setcontext_called = 0;
    int err = getcontext(&running->thread_context);
    assert(!err);
    if (setcontext_called == 1)
        return;
    else {
        while (ready_queue != NULL) {
            threadT* temp = ready_queue;
            ready_queue = ready_queue->next;
            temp->next = NULL;
            //   printf("22222\n");
            // if (temp != NULL) {
            free(temp->sp);
            free(temp);
          
        }

        setcontext_called = 1;
        err = setcontext(&running->thread_context);
        assert(!err);
    }

    // printf("33333\n");

}


void
thread_stub(void (*thread_main)(void *), void *arg) {

    int enable = interrupts_set(1);
    // printf("STUB\n");
    Tid ret;

    thread_main(arg); // call thread_main() function with arg
    interrupts_set(enable);
    ret = thread_exit();
    // we should only get here if we are the last thread. 

    assert(ret == THREAD_NONE);
    // all threads are done, so process should exit
    free(ready_queue);

    exit(0);
}

/*?????*/
void countNumber() {
    int count = 0;
    threadT* findThread = ready_queue;
    while (findThread != NULL) {
        findThread = findThread->next;
        count++;
    }
    printf("countNumber = %d\n", count);
}

void threadReadyQueue(threadT* new_thread) {
    //add the new_thread to the end of ready_queue
  //  printf("new_thread = %d\n", new_thread->thread_id);
    if (ready_queue == NULL) {
        ready_queue = new_thread;
        new_thread->next = NULL;
    } else {
     //   printf("Enter threadReadyQueue\n");
        
        // find the 1ast thread in the linked list
        threadT* findThread = ready_queue;
        int a =0;
        while (findThread->next != NULL) {
            a++;
            findThread = findThread->next;
        }      
        
//         if (findThread->thread_id == 0){
//             printf("a = %d ----\n",a);
//             printf("ready_queue= %d\n", ready_queue->thread_id);
//             
//         }
    //    printf("3333333\n");
        if (findThread->next == NULL) {
            new_thread->next = NULL;
            findThread->next = new_thread;
        }
 
    }
   
}

Tid findTID() {

    int i = 0;
    for (i = 0; i < THREAD_MAX_THREADS; i++) {
        if (stackNumber[i] == false) {
            break;
        }
    }
    return i;
}


void thread_init(void) {

    /* your optional code here */
    clear_lastQueue = NULL;
    ready_queue = NULL;
    deleteALL = false;

    running = (threadT*) malloc(sizeof (threadT));
    //diagnostic information 
    running->thread_id = 0;
    running->next = NULL;
    getcontext(&running->thread_context);
    //You will not need to allocate a stack for this thread 

    stackNumber[0] = true;
    for (int i = 1; i <= THREAD_MAX_THREADS; i++) {
        stackNumber[i] = false;
    }


    
}

Tid
thread_id() {
    return running->thread_id;
}

Tid
thread_create(void (*fn) (void *), void *parg) {
    
    //TBD();

    int enable = interrupts_set(0); //block signals 


    if (clear_lastQueue != NULL) {
        Tid killed = thread_kill(clear_lastQueue->thread_id);
        stackNumber[killed] = false;
        clear_lastQueue = NULL;
    }

    Tid id = findTID();


    if (id == THREAD_MAX_THREADS) {
        interrupts_set(enable); //signal unblock 
        return THREAD_NOMORE;
    }

    void* temp = malloc(THREAD_MIN_STACK + 8);
    if (temp == NULL) {
        //thread package could not allocate memory to create a stack of the desired size
        free(temp);
        interrupts_set(enable);
        return THREAD_NOMEMORY;
    }

    // volatile int setcontext_called = 0;
    stackNumber[id] = true;
    threadT* new_thread = (threadT*) malloc(sizeof (threadT));

    new_thread->thread_id = id;
    new_thread->next = NULL;
    new_thread->sp = temp;

    int err = getcontext(&new_thread->thread_context);
    assert(!err);


    new_thread->thread_context.uc_mcontext.gregs[REG_RSP] = (unsigned long) temp + THREAD_MIN_STACK + 8;
    new_thread->thread_context.uc_mcontext.gregs[REG_RIP] = (unsigned long) &thread_stub;
    //Each thread should have a stack of at least THREAD_MIN_STACK size.

    new_thread->thread_context.uc_mcontext.gregs[REG_RDI] = (unsigned long) fn;
    new_thread->thread_context.uc_mcontext.gregs[REG_RSI] = (unsigned long) parg;
    // new_thread->next = NULL;

    // free(temp);
    threadReadyQueue(new_thread);
   // free(temp);

    interrupts_set(enable);
    return new_thread->thread_id;


}


Tid
thread_yield(Tid want_tid) {
    //TBD();

    int enable = interrupts_set(0); //block

    if (want_tid == THREAD_SELF || want_tid == running->thread_id) {

        int setcontext_called = 0;
        int err = getcontext(&running->thread_context);

        assert(!err);
        if (setcontext_called == 1) {
            interrupts_set(enable);
            return (running->thread_id);
        }
        setcontext_called = 1;
        err = setcontext(&running->thread_context);
        assert(!err);

    }

    if (want_tid == THREAD_ANY) {  
        //run the thread at the head of the ready queue
        if (ready_queue == NULL) {
            //there are no more threads, other than the caller, that are available to run
            interrupts_set(enable);
            return THREAD_NONE;
        } else {
            
            volatile int setcontext_called = 0;
            int err = getcontext(&running->thread_context);
            assert(!err);

         //   printf("running = %d\n", running->thread_id);  
            if (setcontext_called == 1) {
          //    printf("Return from THREAD_ANY\n");
                if (running->thread_id == 0){
                    if (deleteALL == true){
                        deleteReadyQueue();
                    }
                  
                    interrupts_set(enable);
                     return THREAD_NONE;
                }
                else{
                    interrupts_set(enable);
                     return running->thread_id;
                }
                
            } else {
                threadT* tempAdd = running;
                running = ready_queue;
                tempAdd->next = NULL;
                threadReadyQueue(tempAdd);
                ready_queue = ready_queue->next;
                running->next = NULL;
                
                setcontext_called = 1;
                setcontext(&running->thread_context);          
            }
        }
    } else {
        //run the thread with identifier tid = want_tid
        if (want_tid > THREAD_MAX_THREADS || want_tid < 0 || ready_queue == NULL) {
            interrupts_set(enable);
            return THREAD_INVALID;
        } else if (stackNumber[want_tid] == false && (want_tid <= THREAD_MAX_THREADS - 1 && want_tid >= 0)) {
            //thread does not exits
            interrupts_set(enable);
            return THREAD_INVALID;
        } else {
            volatile int setcontext_called = 0;
            getcontext(&running->thread_context);

            if (setcontext_called == 1) {
                interrupts_set(enable);
                return want_tid;
            } else {

                threadT* temp = ready_queue;
                threadT* prev = NULL;
               
                while (temp != NULL) {
                    if (temp->thread_id == want_tid)
                        break;
                    else {
                        prev = temp;
                        temp = temp->next;
                    }
                }
                if (temp == NULL) {
                    //not found
                    interrupts_set(enable);
                    return THREAD_INVALID;
                }
                
               // printf("thread_yield, given want_tid = %d\n",want_tid);
                if (temp->thread_id == ready_queue->thread_id) {
                    
                    threadT* temp2 = running; 
                    running = ready_queue;
                    temp2->next = NULL;
                    threadReadyQueue(temp2);                    
                    ready_queue = ready_queue->next;
                    running->next = NULL;
                   
                } else {
                    threadT* temp2 = running; 
                    running = temp;
                    temp2->next = NULL;
                    threadReadyQueue(temp2);
                    prev->next = temp->next;
                    running->next = NULL;
                 
                }
                setcontext_called = 1;
                setcontext(&running->thread_context);

            }
        }

    }


    return THREAD_FAILED;
}

Tid
thread_exit() {
    //TBD();

    int enable = interrupts_set(0); //block
    
      if (running->thread_id == 0) {
           printf("thread_exit,ready->thread_id = %d\n", ready_queue->thread_id);
            interrupts_set(enable);
            return THREAD_NONE;
       }

    if (ready_queue == NULL) {
       // printf("thread_exit, ready_queue == NULL\n");
        interrupts_set(enable);
        return THREAD_NONE;
    } else {
            printf("ready = %d\n", ready_queue->thread_id);
             printf("running = %d\n", running->thread_id);
             
             
      //  printf("thread_exit\n");
        Tid current_running = thread_yield(THREAD_ANY);
        //   printf("current_running = %d\n", current_running);
        Tid killed = -1; 
        if (current_running > 0)
            killed = thread_kill(current_running - 1);

        if (ready_queue != NULL && ready_queue->next == NULL) {
            stackNumber[running->thread_id] = false;
            clear_lastQueue = running;
        }
        
        interrupts_set(enable);
        return killed;
    }
}

Tid
thread_kill(Tid tid) {
  
    int enable = interrupts_set(0);
    printf("=========thread_kill= %d\n", tid);
    if (tid < 0 || tid > THREAD_MAX_THREADS || ready_queue == NULL || tid == running->thread_id || tid ==0) {
    //    printf("THREAD_INVALID;\n");
        interrupts_set(enable);
        return THREAD_INVALID;
    } else {
        //find the thread    

        threadT* toBeDeleted = ready_queue;
        threadT* prev = NULL;
        int count = 0;
  
        while (toBeDeleted != NULL) {
            if (toBeDeleted->thread_id == tid) {
             //   found = true;
                break;
            } else {
                prev = toBeDeleted;
                toBeDeleted = toBeDeleted->next;
            }
            count++;
        }
       
        if (toBeDeleted == NULL) return THREAD_INVALID;
       
  
        if (toBeDeleted == ready_queue) {
            ready_queue = ready_queue->next;
        } else {
            prev->next = toBeDeleted->next;
        }
        toBeDeleted->next = NULL;


        stackNumber[toBeDeleted->thread_id] = false;
        //       
        Tid delete = toBeDeleted->thread_id;

        free(toBeDeleted);
      //  printf("++++++++++++=ready_queue= %d\n", ready_queue->thread_id);
        interrupts_set(enable);
        return delete;
    }

}

/*******************************************************************
 * Important: The rest of the code should be implemented in Lab 3. *
 *******************************************************************/

/* make sure to fill the wait_queue structure defined above */
struct wait_queue *
wait_queue_create() {
    struct wait_queue *wq;

    wq = malloc(sizeof (struct wait_queue));
    assert(wq);

    wq->wait_head = NULL;
    wq->targetThread = -1;

    // TBD();

    return wq;
}

void
wait_queue_destroy(struct wait_queue *wq) {
 //    countNumber();
   // printf("Enter wait_queue_destroy\n");
    if (wq->wait_head == NULL){
        //wq doesn't have wait threads
        free(wq);
        wq = NULL;
    } else {
        threadT* loop = wq->wait_head;
        while (loop != NULL) {
            threadT* delete = loop;
            loop = loop->next;
            free(delete);
            delete = NULL;

        }
        free(wq);
        wq = NULL;
    }
    
//     if (wq == NULL) {
//          printf("SUCCESS\n");
//     }
       
        
    return;
   }
int waitCount(struct wait_queue *queue) {
    int count = 0;
    threadT* findThread = queue->wait_head;
    while (findThread != NULL) {
        findThread = findThread->next;
        count++;
    }
    return count;
   // printf("waitCount = %d\n", count);
}

Tid
thread_sleep(struct wait_queue *queue) {
    // primitive blocks or suspends a thread when it is waiting on an event
    //such as a mutex lock becoming available or the arrival of a network packet.
  //   printf("thread_sleep\n");
     
    int enable = interrupts_set(0);
    if (queue == NULL) {
        interrupts_set(enable);
        return THREAD_INVALID;
    }
    if (ready_queue == NULL) {
     //   printf("ALL sleep\n");
        interrupts_set(enable);
        return THREAD_NONE;
    }
   //  printf("ready = %d\n", ready_queue->thread_id);
    //suspend running, run the fist one in ready_queue, put running into wait_queue
    //change running, so use getcontext
    volatile int setcontext_called = 0;
    getcontext(&running->thread_context);
    
    if (setcontext_called == 1) {
         //waitCount(queue);
        interrupts_set(enable);
        return running->thread_id;
    } else {
        if (queue->wait_head == NULL) {
            queue->wait_head = running;
            running = ready_queue;
            ready_queue = ready_queue->next;
           // waitCount(queue);
        } else {
            //find the last one in wait queue

            threadT* find_last = queue->wait_head;

            while (find_last->next != NULL) {
             
                find_last = find_last->next;
            }
             if (find_last->next == NULL) {
                find_last->next = running;
                find_last->next->next = NULL;

                running = ready_queue;
                ready_queue = ready_queue->next;
                running->next = NULL;
            }
            
        }
   //   printf("Running = %d\n", running->thread_id);
//        if (ready_queue == NULL) {
//            printf("wait-------------------- = %d\n",waitCount(queue));
//        }
       //printf("wait = %d\n",waitCount(queue));
        setcontext_called = 1;
        setcontext(&running->thread_context);
    }

    interrupts_set(enable);
    return THREAD_FAILED;
}



/* when the 'all' parameter is 1, wakeup all threads waiting in the queue.
 * returns whether a thread was woken up on not. */
int
thread_wakeup(struct wait_queue *queue, int all) {
  //    printf("thread_wakeup\n");
    int enable = interrupts_set(0);
    if (queue == NULL) {
        interrupts_set(enable);
        return 0;
    }

    if (queue->wait_head == NULL) {
        interrupts_set(enable);
        return 0;
    } else {
        //wake up threads in wait_queue, put them into ready_queue
        if (all == 0) {
      //    printf("ONE\n");
            //wake up one thread, FIFO
            threadT* FI = queue->wait_head;
            queue->wait_head = queue->wait_head->next;
            FI->next = NULL;

            if (ready_queue == NULL) ready_queue = FI;
            else {
                threadT* findLast = ready_queue;
                while (findLast->next != NULL)
                    findLast = findLast->next;
                 if (findLast->next == NULL) findLast->next = FI;
            }
            deleteALL = true;
            
            interrupts_set(enable);
            return 1;
        } else {
         //   printf("ALL\n");
            deleteALL = false;
            int count = 0;
            while (queue->wait_head != NULL) {
                threadT* head = queue->wait_head;
                queue->wait_head = queue->wait_head->next;
                head->next = NULL;

                if (ready_queue == NULL) ready_queue = head;
                else {
                    threadT* findLast = ready_queue;
                    while (findLast->next != NULL)
                        findLast = findLast->next;
                   if (findLast->next == NULL) findLast->next = head;
                }
                count++;
            }    
            interrupts_set(enable);
            return count;
        }
    }




     interrupts_set(enable);
    return 0;
}

/* suspend current thread until Thread tid exits */
Tid
thread_wait(Tid tid) {
    //if invokes thread_wait, put on wait_queue
    //if invokes wake up, makeup
    if (tid < 0 || tid > THREAD_MAX_THREADS || tid == running->thread_id || tid == THREAD_SELF){
        return THREAD_INVALID;
    }

    return 0;
}

struct lock {
    /* ... Fill this in ... */
    struct wait_queue * lock_wait_queue;
    Tid acquired_thread_id;
    int lock_state;
};

struct lock *
lock_create() {
    //disable signals, store the previous signal state in "enabled"
    int enabled = interrupts_set(0);
    
    struct lock *lock;

    lock = malloc(sizeof (struct lock));
    assert(lock);
    
    lock->acquired_thread_id = -1;
    lock->lock_wait_queue = wait_queue_create();
    lock->lock_state=LOCK_AVALIABLE;

    //quit critical section
    interrupts_set(enabled);
    return lock;
}

void
lock_destroy(struct lock *lock) {
    //disable signals, store the previous signal state in "enabled"
    int enabled = interrupts_set(0);
    
    assert(lock != NULL);
    
//    //release the lock if it is available
    while((lock->lock_state!=LOCK_AVALIABLE)&&(lock->lock_wait_queue->wait_queue_counter!=0)){
       
    }
    if((lock->lock_state==LOCK_AVALIABLE)&&(lock->lock_wait_queue->wait_queue_counter==0)){
        wait_queue_destroy(lock->lock_wait_queue);
        free(lock);
    }
    //quit critical section
    interrupts_set(enabled);
    return;
}

void
lock_acquire(struct lock *lock) {
    //disable signals, store the previous signal state in "enabled"
    int enabled = interrupts_set(0);
    
    assert(lock != NULL);
    
    while(lock->lock_state==LOCK_UNAVALIABLE){
        thread_sleep(lock->lock_wait_queue);
    }
    lock->lock_state=LOCK_UNAVALIABLE;
    lock->acquired_thread_id = current_running_thread->thread_id;
    
    //quit critical section
    interrupts_set(enabled);
    return;
}

void
lock_release(struct lock *lock) {
     //disable signals, store the previous signal state in "enabled"
    int enabled = interrupts_set(0);
    
    assert(lock != NULL);
    
    if(lock->lock_state==LOCK_UNAVALIABLE){
        thread_wakeup(lock->lock_wait_queue, 1);
        lock->lock_state = LOCK_AVALIABLE;
        lock->acquired_thread_id = -1;
    }
    
    //quit critical section
    interrupts_set(enabled);
    return;
}
struct cv {
    /* ... Fill this in ... */
};

struct cv *
cv_create() {
    struct cv *cv;

    cv = malloc(sizeof (struct cv));
    assert(cv);

    TBD();

    return cv;
}

void
cv_destroy(struct cv *cv) {
    assert(cv != NULL);

    TBD();

    free(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock) {
    assert(cv != NULL);
    assert(lock != NULL);

    TBD();
}

void
cv_signal(struct cv *cv, struct lock *lock) {
    assert(cv != NULL);
    assert(lock != NULL);

    TBD();
}

void
cv_broadcast(struct cv *cv, struct lock *lock) {
    assert(cv != NULL);
    assert(lock != NULL);

    TBD();
}
