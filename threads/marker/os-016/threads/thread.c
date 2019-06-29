#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"
#include <stdbool.h>

/* This is the wait queue structure */
struct wait_queue {
    /* ... Fill this in Lab 3 ... */
};

/* This is the thread control block */
typedef struct thread {
    /* ... Fill this in ... */
    Tid thread_id;
    ucontext_t thread_context;
    struct thread* next;
} threadT;


threadT* running;
threadT* ready_queue;
bool stackNumber[THREAD_MAX_THREADS + 1];

/* thread starts by calling thread_stub. The arguments to thread_stub are the
 * thread_main() function, and one argument to the thread_main() function. */
void
thread_stub(void (*thread_main)(void *), void *arg) {
    Tid ret;

    thread_main(arg); // call thread_main() function with arg
    ret = thread_exit();
    // we should only get here if we are the last thread. 

    assert(ret == THREAD_NONE);
    // all threads are done, so process should exit
    free(ready_queue);

    exit(0);
}

void threadReadyQueue(threadT* new_thread) {

    if (ready_queue == NULL) {
        //head thread
        ready_queue = new_thread;
        new_thread->next = NULL;
    } else {
        // find the 1ast thread in the linked list
        threadT* findThread = ready_queue;
        //printf("findThread = %d\n",findThread->thread_id);

        while (findThread->next != NULL) {

            findThread = findThread->next;
        }
        findThread->next = new_thread;
        new_thread->next = NULL;

    }

}

Tid findTID() {
    int i = 1;
    for (i = 1; i <= THREAD_MAX_THREADS; i++) {
        if (stackNumber[i] == false) {
            break;
        }
    }
    return i;
}

void thread_init(void) {

    /* your optional code here */

    running = (threadT*) malloc(sizeof (threadT));
    //diagnostic information 
    running->thread_id = 0;
    getcontext(&running->thread_context);
    //You will not need to allocate a stack for this thread 
    ready_queue = NULL;

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


    Tid id = findTID();

    if (id == THREAD_MAX_THREADS) {

        return THREAD_NOMORE;
    }


    void* temp = malloc(THREAD_MIN_STACK + 8);
    if (temp == NULL) {
        //thread package could not allocate memory to create a stack of the desired size
        free(temp);
        // free(new_thread);                
        return THREAD_NOMEMORY;
    }

    // volatile int setcontext_called = 0;
    stackNumber[id] = true;
    threadT* new_thread = (threadT*) malloc(sizeof (threadT));

    new_thread->thread_id = id;
    new_thread->next = NULL;

    int err = getcontext(&new_thread->thread_context);
    assert(!err);


    new_thread->thread_context.uc_mcontext.gregs[REG_RSP] = (unsigned long) temp + THREAD_MIN_STACK + 8;

    new_thread->thread_context.uc_mcontext.gregs[REG_RIP] = (unsigned long) thread_stub;
    //Each thread should have a stack of at least THREAD_MIN_STACK size.

    new_thread->thread_context.uc_mcontext.gregs[REG_RDI] = (unsigned long) fn;
    new_thread->thread_context.uc_mcontext.gregs[REG_RSI] = (unsigned long) parg;

    // free(temp);
    threadReadyQueue(new_thread);

    return new_thread->thread_id;


}

Tid
thread_yield(Tid want_tid) {
    //TBD();

    if (want_tid == THREAD_SELF || want_tid == running->thread_id) {

        int setcontext_called = 0;
        int err = getcontext(&running->thread_context);


        assert(!err);
        if (setcontext_called == 1) {
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

            return THREAD_NONE;
        } else {

            volatile int setcontext_called = 0;
            int err = getcontext(&running->thread_context);
            assert(!err);

            if (setcontext_called == 1) {

                return running->thread_id;
            } else {


                threadReadyQueue(running);
                running = ready_queue;
                ready_queue = ready_queue->next;
                running->next = NULL;
                //  printf("now running = %d\n",running->thread_id);
                setcontext_called = 1;
                setcontext(&running->thread_context);
                // return running->thread_id;
            }

        }
    } else {
        //run the thread with identifier tid = want_tid

        if (want_tid > THREAD_MAX_THREADS || want_tid < 0 || ready_queue == NULL) {

            return THREAD_INVALID;
        } else if (stackNumber[want_tid - 1] == false && (want_tid <= THREAD_MAX_THREADS - 1 && want_tid >= 0)) {
            //thread does not exits
            return THREAD_INVALID;
        } else {
            volatile int setcontext_called = 0;
            getcontext(&running->thread_context);

            if (setcontext_called == 1) {

                return want_tid;
            }
            else {
                threadT* temp = ready_queue;
                threadT* prev = NULL;
                // printf("pre running = %d\n",running->thread_id);
                while (temp != NULL) {
                    if (temp->thread_id == want_tid)
                        break;
                    else {
                        prev = temp;
                        temp = temp->next;
                    }
                }
                //  printf("temp_id = %d\n",temp->thread_id);
                if (temp->thread_id == ready_queue->thread_id) {
                    threadReadyQueue(running);
                    running = ready_queue;
                    ready_queue = ready_queue->next;
                    running->next = NULL;
                } else {
                    threadReadyQueue(running);
                    running = temp;
                    prev->next = temp->next;
                    temp->next = NULL;
                }

                setcontext_called = 1;
                setcontext(&running->thread_context);
                //return want_tid;
            }
        }

    }


    return THREAD_FAILED;
}

Tid
thread_exit() {
    //TBD();

    if (ready_queue == NULL) {
        
        return THREAD_NONE;
    } else {
       //current thread does not run
       //thread that is created later should be able to reuse this thread's identifier
        Tid delete = thread_yield(THREAD_ANY);
        thread_kill(delete - 1);

        return thread_exit();
    }

    return THREAD_FAILED;


}

Tid
thread_kill(Tid tid) {


    if (tid < 0 || tid > THREAD_MAX_THREADS || tid == running->thread_id || ready_queue == NULL) {

        return THREAD_INVALID;
    } else {
        //find the thread
        threadT* toBeDeleted = ready_queue;
        threadT* prev = NULL;

        while (toBeDeleted->next != NULL) {
            if (toBeDeleted->thread_id == tid)
                break;
            else {
                prev = toBeDeleted;
                toBeDeleted = toBeDeleted->next;
            }
        }

        if (toBeDeleted == ready_queue) {

            ready_queue = ready_queue->next;
        } else {
            prev->next = toBeDeleted->next;
        }

        stackNumber[toBeDeleted->thread_id] = false;
        //       
        Tid delete = toBeDeleted->thread_id;

        free(toBeDeleted);

        return delete;
    }


    return THREAD_FAILED;
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

    TBD();

    return wq;
}

void
wait_queue_destroy(struct wait_queue *wq) {
    TBD();
    free(wq);
}

Tid
thread_sleep(struct wait_queue *queue) {
    TBD();
    return THREAD_FAILED;
}

/* when the 'all' parameter is 1, wakeup all threads waiting in the queue.
 * returns whether a thread was woken up on not. */
int
thread_wakeup(struct wait_queue *queue, int all) {
    TBD();
    return 0;
}

/* suspend current thread until Thread tid exits */
Tid
thread_wait(Tid tid) {
    TBD();
    return 0;
}

struct lock {
    /* ... Fill this in ... */
};

struct lock *
lock_create() {
    struct lock *lock;

    lock = malloc(sizeof (struct lock));
    assert(lock);

    TBD();

    return lock;
}

void
lock_destroy(struct lock *lock) {
    assert(lock != NULL);

    TBD();

    free(lock);
}

void
lock_acquire(struct lock *lock) {
    assert(lock != NULL);

    TBD();
}

void
lock_release(struct lock *lock) {
    assert(lock != NULL);

    TBD();
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
