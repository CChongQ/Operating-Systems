#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>
#include "thread.h"
#include "interrupt.h"
#include "test_thread.h"



#define DURATION  60000000
#define NTHREADS       128
#define LOOPS	        10


static void grand_finale();
static void hello(char *msg);
static int fact(int n);
static void suicide();
static void finale();
static int set_flag(int val);
static void do_potato(int num);
static int try_move_potato(int num, int pass);

long *stack_array[THREAD_MAX_THREADS];

/* Important: these tests assume that preemptive scheduling is not enabled,
 * i.e., register_interrupt_handler is NOT called before this function is
 * called. */
void
test_basic()
{
	Tid ret;
	Tid ret2;
	int allocated_space;
	
	printf("starting basic test\n");
	struct mallinfo minfo_start = mallinfo();
	
	assert(thread_id() == 0);
	
	/* Initial thread yields */
	ret = thread_yield(THREAD_SELF);
	assert(thread_ret_ok(ret));
	printf("initial thread returns from yield(SELF)\n");
	/* See thread.h -- initial thread must be Tid 0 */
	ret = thread_yield(0);
	assert(thread_ret_ok(ret));
	printf("initial thread returns from yield(0)\n");
	ret = thread_yield(THREAD_ANY);
	assert(ret == THREAD_NONE);
	printf("initial thread returns from yield(ANY)\n");
	ret = thread_yield(0xDEADBEEF);
	assert(ret == THREAD_INVALID);
	printf("initial thread returns from yield(INVALID)\n");
	ret = thread_yield(16);
	assert(ret == THREAD_INVALID);
	printf("initial thread returns from yield(INVALID2)\n");

	struct mallinfo minfo;
	minfo = mallinfo();
	allocated_space = minfo.uordblks;
	/* create a thread */
	ret = thread_create((void (*)(void *))hello, "hello from first thread");
	minfo = mallinfo();
	if (minfo.uordblks <= allocated_space) {
		printf("it appears that the thread stack is not being"
		       "allocated dynamically\n");
		assert(0);
	}
	printf("my id is %d\n", thread_id());
	assert(thread_ret_ok(ret));
	ret2 = thread_yield(ret);
	assert(ret2 == ret);

	/* store address of some variable on stack */
	stack_array[thread_id()] = (long *)&ret;

	int ii, jj;
	/* we will be using THREAD_MAX_THREADS threads later */
	Tid child[THREAD_MAX_THREADS];
	char msg[NTHREADS][1024];
	/* create NTHREADS threads */
	for (ii = 0; ii < NTHREADS; ii++) {
		ret = snprintf(msg[ii], 1023, "hello from thread %3d", ii);
		assert(ret > 0);
		child[ii] = thread_create((void (*)(void *))hello, msg[ii]);
		assert(thread_ret_ok(child[ii]));
	}
	printf("my id is %d\n", thread_id());
	for (ii = 0; ii < NTHREADS; ii++) {
		ret = thread_yield(child[ii]);
		assert(ret == child[ii]);
	}

	/* destroy NTHREADS + 1 threads we just created */
	printf("destroying all threads\n");
	ret = thread_kill(ret2);
	assert(ret == ret2);
	for (ii = 0; ii < NTHREADS; ii++) {
           // printf("Test = %d\n", ii);
		ret = thread_kill(child[ii]);
		assert(ret == child[ii]);
	}

	/* we destroyed other threads. yield so that these threads get to run
	 * and exit. */
	ii = 0;
	do {
		/* the yield should be needed at most NTHREADS+2 times */
		assert(ii <= (NTHREADS + 1));
		ret = thread_yield(THREAD_ANY);
		ii++;
	} while (ret != THREAD_NONE);

	/*
	 * create maxthreads-1 threads
	 */
	printf("creating  %d threads\n", THREAD_MAX_THREADS - 1);
	for (ii = 0; ii < THREAD_MAX_THREADS - 1; ii++) {
		ret = thread_create((void (*)(void *))fact, (void *)10);
		assert(thread_ret_ok(ret));
	}
	/*
	 * Now we're out of threads. Next create should fail.
	 */
	ret = thread_create((void (*)(void *))fact, (void *)10);
      //  printf("ret = %d\n", ret);
	assert(ret == THREAD_NOMORE);
	/*
	 * Now let them all run.
	 */
	printf("running   %d threads\n", THREAD_MAX_THREADS - 1);
	for (ii = 0; ii < THREAD_MAX_THREADS; ii++) {
		ret = thread_yield(ii);
		if (ii == 0) {
			/* 
			 * Guaranteed that first yield will find someone. 
			 * Later ones may or may not depending on who
			 * stub schedules  on exit.
			 */
			assert(thread_ret_ok(ret));
		}
	}
     
	/* check that the thread stacks are sufficiently far apart */
	for (ii = 0; ii < THREAD_MAX_THREADS; ii++) {
		for (jj = 0; jj < THREAD_MAX_THREADS; jj++) {
			if (ii == jj)
				continue;
			long stack_sep = (long)(stack_array[ii]) -
				(long)(stack_array[jj]);
			if ((labs(stack_sep) < THREAD_MIN_STACK)) {
				printf("stacks of threads %d and %d "
				       "are too close\n", ii, jj);
				assert(0);
			}
		}
	}
      
	/*
	 * They should have cleaned themselves up when
	 * they finished running. Create maxthreads-1 threads.
	 */
	printf("creating  %d threads\n", THREAD_MAX_THREADS - 1);
	for (ii = 0; ii < THREAD_MAX_THREADS - 1; ii++) {  
		//printf("creating ii = %d\n", ii);
                child[ii] = thread_create((void (*)(void *))fact, (void *)10);
           
		assert(thread_ret_ok(child[ii]));
	}
	/*
	 * Now destroy some explicitly and let the others run
	 */
	printf("destroying %d threads\n", THREAD_MAX_THREADS / 2);
	for (ii = 0; ii < THREAD_MAX_THREADS; ii += 2) {               
		ret = thread_kill(child[ii]);
		assert(thread_ret_ok(ret));
	}
          
	for (ii = 0; ii < THREAD_MAX_THREADS; ii++) {   
		ret = thread_yield(ii);              
	}
        
	ret = thread_kill(thread_id());
	assert(ret == THREAD_INVALID);
	printf("testing some destroys even though I'm the only thread\n");
	ret = thread_exit();
        printf("Hello\n");
	assert(ret == THREAD_NONE);
        
	ret = thread_kill(42);
	assert(ret == THREAD_INVALID);
	ret = thread_kill(-42);
	assert(ret == THREAD_INVALID);
	ret = thread_kill(THREAD_MAX_THREADS + 1000);
	assert(ret == THREAD_INVALID);
        
	/*
	 * Create a thread that destroys itself. Control should come back here
	 * after that thread runs.
	 */
	printf("testing destroy self\n");
	int flag = set_flag(0);
	ret = thread_create((void (*)(void *))suicide, NULL);
	assert(thread_ret_ok(ret));
	ret = thread_yield(ret);
	assert(thread_ret_ok(ret));
	flag = set_flag(0);
	assert(flag == 1);	/* Other thread ran */
	/* That thread is gone now */
	ret = thread_yield(ret);
	assert(ret == THREAD_INVALID);

	/* check for memory leaks. at this point, the only thread that should be
	 * running is the main thread, and so no memory should have been
	 * allocated using malloc. */
	/* this assumes that the thread structures are allocated statically,
	 * since the main thread is still running right now. */
	struct mallinfo minfo_end = mallinfo();
	assert(minfo_end.uordblks == minfo_start.uordblks);
	assert(minfo_end.hblks == minfo_start.hblks);
	grand_finale();
	printf("\n\nBUG: test should not get here\n\n");
	assert(0);
}

static void
grand_finale()
{
	int ret;

	printf("for my grand finale, I will destroy myself\n");
	printf("while my talented assistant prints \"basic test done\"\n");
	ret = thread_create((void (*)(void *))finale, NULL);
	assert(thread_ret_ok(ret));
	thread_exit();
	assert(0);

}

static void
hello(char *msg)
{
	Tid ret;
	char str[20];

	printf("message: %s\n", msg);
	ret = thread_yield(THREAD_SELF);
	assert(thread_ret_ok(ret));
	printf("thread returns from  first yield\n");

	/* we cast ret to a float because that helps to check
	 * whether the stack alignment of the frame pointer is correct */
	sprintf(str, "%3.0f\n", (float)ret);

	ret = thread_yield(THREAD_SELF);
	assert(thread_ret_ok(ret));
	printf("thread returns from second yield\n");

	while (1) {
		thread_yield(THREAD_ANY);
	}

}

static int
fact(int n)
{
	/* store address of some variable on stack */
	stack_array[thread_id()] = (long *)&n;
	if (n == 1) {
		return 1;
	}
	return n * fact(n - 1);
}

static void
suicide()
{
	int ret = set_flag(1);
	assert(ret == 0);
	thread_exit();
	assert(0);
}

static int flag_value;

/* sets flag_value to val, returns old value of flag_value */
static int
set_flag(int val)
{
	return __sync_lock_test_and_set(&flag_value, val);
}

static void
finale()
{
	int ret;
	printf("finale running\n");
	ret = thread_yield(THREAD_ANY);
	assert(ret == THREAD_NONE);
	ret = thread_yield(THREAD_ANY);
	assert(ret == THREAD_NONE);
	printf("basic test done\n");
	/* 
	 * Stub should exit cleanly if there are no threads left to run.
	 */
	return;
}

#define NPOTATO  NTHREADS

static int potato[NPOTATO];
static int potato_lock = 0;
static struct timeval pstart;

void
test_preemptive()
{

	int ret;
        long ii;
	Tid potato_tids[NPOTATO];

	unintr_printf("starting preemptive test\n");
	unintr_printf("this test will take %d seconds\n", DURATION / 1000000);
	gettimeofday(&pstart, NULL);
	/* spin for sometime, so you see the interrupt handler output */
	spin(SIG_INTERVAL * 5);
	interrupts_quiet();

	potato[0] = 1;
	for (ii = 1; ii < NPOTATO; ii++) {
		potato[ii] = 0;
	}

	for (ii = 0; ii < NPOTATO; ii++) {
		potato_tids[ii] =
			thread_create((void (*)(void *))do_potato, (void *)ii);
		assert(thread_ret_ok(potato_tids[ii]));
	}

	spin(DURATION);

	unintr_printf("cleaning hot potato\n");

	for (ii = 0; ii < NPOTATO; ii++) {
		assert(interrupts_enabled());
		ret = thread_kill(potato_tids[ii]);
		assert(thread_ret_ok(ret));
	}

	unintr_printf("preemptive test done\n");
	/* we don't check for memory leaks because while threads have exited,
	 * they may not have been destroyed yet. */
}

static void
do_potato(int num)
{
	int ret;
	int pass = 1;

	unintr_printf("0: thread %3d made it to %s\n", num, __FUNCTION__);
	while (1) {
		ret = try_move_potato(num, pass);
		if (ret) {
			pass++;
		}
		spin(1);
		/* Add some yields by some threads to scramble the list */
		if (num > 4) {
			int ii;
			for (ii = 0; ii < num - 4; ii++) {
				assert(interrupts_enabled());
				ret = thread_yield(THREAD_ANY);
				assert(thread_ret_ok(ret));
			}
		}
	}
}

static int
try_move_potato(int num, int pass)
{
	int ret = 0;
	int err;
	struct timeval pend, pdiff;

	assert(interrupts_enabled());
	err = __sync_bool_compare_and_swap(&potato_lock, 0, 1);
	if (!err) {	/* couldn't acquire lock */
		return ret;
	}
	if (potato[num]) {
		potato[num] = 0;
		potato[(num + 1) % NPOTATO] = 1;
		gettimeofday(&pend, NULL);
		timersub(&pend, &pstart, &pdiff);
		unintr_printf("%d: thread %3d passes potato "
			      "at time = %9.6f\n", pass, num,
			      (float)pdiff.tv_sec +
			      (float)pdiff.tv_usec / 1000000);
		assert(potato[(num + 1) % NPOTATO] == 1);
		assert(potato[(num) % NPOTATO] == 0);
		ret = 1;
	}
	err = __sync_bool_compare_and_swap(&potato_lock, 1, 0);
	assert(err);
	return ret;
}

static struct wait_queue *queue;
static int done;
static int nr_sleeping;

static void
test_wakeup_thread(int num)
{

	int i;
	int ret;
	struct timeval start, end, diff;

	for (i = 0; i < LOOPS; i++) {
		int enabled;
		gettimeofday(&start, NULL);
                

		/* track the number of sleeping threads with interrupts
		 * disabled to avoid wakeup races. */
		enabled = interrupts_off();
		assert(enabled);
		__sync_fetch_and_add(&nr_sleeping, 1);
		ret = thread_sleep(queue);
		assert(thread_ret_ok(ret));
		interrupts_set(enabled);

		gettimeofday(&end, NULL);
		timersub(&end, &start, &diff);
                
		/* thread_sleep should wait at least 4-5 ms */
		if (diff.tv_sec == 0 && diff.tv_usec < 4000) {
			unintr_printf("%s took %ld us. That's too fast."
				      " You must be busy looping\n",
				      __FUNCTION__, diff.tv_usec);
			goto out;
		}
	}
out:
	__sync_fetch_and_add(&done, 1);
}

void
test_wakeup(int all)
{
	Tid ret;
	long ii;
	static Tid child[NTHREADS];
	unintr_printf("starting wakeup test\n");
	struct mallinfo minfo_start = mallinfo();

	done = 0;
	nr_sleeping = 0;

	queue = wait_queue_create();
	assert(queue);

	/* initial thread sleep and wake up tests */
	ret = thread_sleep(NULL);
	assert(ret == THREAD_INVALID);
	unintr_printf("initial thread returns from sleep(NULL)\n");

	ret = thread_sleep(queue);
	assert(ret == THREAD_NONE);
	unintr_printf("initial thread returns from sleep(NONE)\n");

	ret = thread_wakeup(NULL, 0);
	assert(ret == 0);
	ret = thread_wakeup(queue, 1);
	assert(ret == 0);
       
	/* create all threads */
	for (ii = 0; ii < NTHREADS; ii++) {  
		child[ii] = thread_create((void (*)(void *))test_wakeup_thread,
					  (void *)ii);
		assert(thread_ret_ok(child[ii]));
             //    printf("PASSSSSSSS\n");
	}
        
out:
	while (__sync_fetch_and_add(&done, 0) < NTHREADS) {
           
		if (all) {
                     
			/* wait until all threads have slept */
			if (__sync_fetch_and_add(&nr_sleeping, 0) < NTHREADS) {
				goto out;
			}
			/* we will wake up all threads in the thread_wakeup
			 * call below so set nr_sleeping to 0 */
			nr_sleeping = 0;
                  
		} else {
                 //   printf("ALL = 0\n");
			/* wait until at least one thread has slept */
			if (__sync_fetch_and_add(&nr_sleeping, 0) < 1) {
				goto out;
			}
			/* wake up one thread in the wakeup call below */
			__sync_fetch_and_add(&nr_sleeping, -1);
		}
		/* spin for 5 ms. this allows testing that the sleeping thread
		 * sleeps for at least 5 ms. */
		spin(5000);
              //  printf("Passing\n");
		/* tests thread_wakeup */
		assert(interrupts_enabled());
		ret = thread_wakeup(queue, all);
		assert(interrupts_enabled());
		assert(ret >= 0);
		assert(all ? ret == NTHREADS : ret == 1);
              //  printf("END\n");
             //   printf("ALL = %d\n",__sync_fetch_and_add(&done, 0));
	}
        
	/* we expect nr_sleeping is 0 at this point */
	assert(nr_sleeping == 0);
	assert(interrupts_enabled());
	/* no thread should be waiting on queue */
	wait_queue_destroy(queue);
	/* wait for other threads to exit */
        //   printf("Test\n");
	while (thread_yield(THREAD_ANY) != THREAD_NONE) {
	}
       // printf("NON\n");
	struct mallinfo minfo_end = mallinfo();
	assert(minfo_end.uordblks == minfo_start.uordblks);
	assert(minfo_end.hblks == minfo_start.hblks);

	unintr_printf("wakeup test done\n");
}

static Tid wait[NTHREADS];

static void
test_wait_thread(int num)
{
	int rand = ((double)random())/RAND_MAX * 1000000;

	/* make sure that all threads are created before continuing */
	/* we use atomic operations for synchronization because we assume that
	 * lock/cv have not been implemented yet */
	while (__sync_fetch_and_add(&done, 0) < 1) {
	}
	/* spin for a random time between 0-1 s */
	spin(rand);
	if (num > 0) {
		assert(interrupts_enabled());
		/* wait on previous thread */
		thread_wait(wait[num-1]);
		assert(interrupts_enabled());
		spin(rand/10);
		/* id should print in ascending order, from 1-127 */
		unintr_printf("id = %d\n", num);
	}
}

void
test_wait(void)
{
	Tid ret;
	long i;

	unintr_printf("starting wait test\n");
	srandom(0);
	struct mallinfo minfo_start = mallinfo();

	/* initial thread wait tests */
	ret = thread_wait(thread_id());
	assert(ret == THREAD_INVALID);
	unintr_printf("initial thread returns from waiting on self\n");
	
	ret = thread_wait(THREAD_SELF);
	assert(ret == THREAD_INVALID);
	unintr_printf("initial thread returns from waiting on THREAD_SELF\n");

	ret = thread_wait(110);
	assert(ret == THREAD_INVALID);
	unintr_printf("initial thread returns from waiting on 110\n");

	done = 0;
	/* create all threads */
	for (i = 0; i < NTHREADS; i++) {
		wait[i] = thread_create((void (*)(void *))test_wait_thread,
					 (void *)i);
		assert(thread_ret_ok(wait[i]));
	}

	__sync_fetch_and_add(&done, 1);
	for (i = 0; i < NTHREADS; i++) {
		thread_wait(wait[i]);
	}

	struct mallinfo minfo_end = mallinfo();
	assert(minfo_end.uordblks == minfo_start.uordblks);
	assert(minfo_end.hblks == minfo_start.hblks);

	unintr_printf("wait test done\n");
}

static void
test_wait_kill_thread(Tid parent)
{
	Tid ret = thread_kill(parent); /* ouch */
	assert(ret == parent);
	unintr_printf("its over\n");
}

void
test_wait_kill(void)
{
	Tid child;
	Tid ret;

	unintr_printf("starting wait_kill test\n");
	/* create a thread */
	child = thread_create((void (*)(void *))test_wait_kill_thread,
			      (void *)(long)thread_id());
	assert(thread_ret_ok(child));
	ret = thread_wait(child);

	/* child kills parent! we shouldn't get here */
	assert(ret == child);
	unintr_printf("wait_kill test failed\n");
}

static struct lock *testlock;
static struct cv *testcv_signal[NTHREADS];
static struct cv *testcv_broadcast;

static volatile unsigned long testval1;
static volatile unsigned long testval2;
static volatile unsigned long testval3;

#define NLOCKLOOPS    100

static void
test_lock_thread(unsigned long num)
{
	int i, j;

	for (i = 0; i < LOOPS; i++) {
		for (j = 0; j < NLOCKLOOPS; j++) {
			int ret;
			
			assert(interrupts_enabled());
			lock_acquire(testlock);
			assert(interrupts_enabled());
			
			testval1 = num;

			/* let's yield to make sure that even when other threads
			 * run, they cannot access the critical section. */
			assert(interrupts_enabled());
			ret = thread_yield(THREAD_ANY);
			assert(thread_ret_ok(ret) || ret == THREAD_NONE);

			testval2 = num * num;

			/* yield again */
			assert(interrupts_enabled());
			ret = thread_yield(THREAD_ANY);
			assert(thread_ret_ok(ret) || ret == THREAD_NONE);

			testval3 = num % 3;
			
			assert(testval2 == testval1 * testval1);
			assert(testval2 % 3 == (testval3 * testval3) % 3);
			assert(testval3 == testval1 % 3);
			assert(testval1 == num);
			assert(testval2 == num * num);
			assert(testval3 == num % 3);

			assert(interrupts_enabled());
			lock_release(testlock);
			assert(interrupts_enabled());
		}
		unintr_printf("%d: thread %3d passes\n", i, num);
	}
}

void
test_lock()
{
	long i;
	Tid result[NTHREADS];

	unintr_printf("starting lock test\n");
	struct mallinfo minfo_start = mallinfo();

	assert(interrupts_enabled());
	testlock = lock_create();
	assert(interrupts_enabled());
	for (i = 0; i < NTHREADS; i++) {
		result[i] = thread_create((void (*)(void *))test_lock_thread,
					  (void *)i);
		assert(thread_ret_ok(result[i]));
	}

//	for (i = 0; i < NTHREADS; i++) {
//		thread_wait(result[i]);
//	}

	assert(interrupts_enabled());
	lock_destroy(testlock);
	assert(interrupts_enabled());

	struct mallinfo minfo_end = mallinfo();
	assert(minfo_end.uordblks == minfo_start.uordblks);
	assert(minfo_end.hblks == minfo_start.hblks);

	unintr_printf("lock test done\n");
}

static void
test_cv_signal_thread(unsigned long num)
{
	int i;
	struct timeval start, end, diff;

	for (i = 0; i < LOOPS; i++) {
		assert(interrupts_enabled());
		lock_acquire(testlock);
		assert(interrupts_enabled());
		while (testval1 != num) {
			gettimeofday(&start, NULL);
			assert(interrupts_enabled());
			cv_wait(testcv_signal[num], testlock);
			assert(interrupts_enabled());
			gettimeofday(&end, NULL);
			timersub(&end, &start, &diff);

			/* cv_wait should wait at least 4-5 ms */
			if (diff.tv_sec == 0 && diff.tv_usec < 4000) {
				unintr_printf("%s took %ld us. That's too fast."
					      " You must be busy looping\n",
					      __FUNCTION__, diff.tv_usec);
				goto out;
			}
		}
		unintr_printf("%d: thread %3d passes\n", i, num);
		testval1 = (testval1 + NTHREADS - 1) % NTHREADS;

		/* spin for 5 ms */
		spin(5000);

		assert(interrupts_enabled());
		cv_signal(testcv_signal[testval1], testlock);
		assert(interrupts_enabled());
		lock_release(testlock);
		assert(interrupts_enabled());
	}
out:
	;
}

void
test_cv_signal()
{

	long i;
        int result[NTHREADS];

	unintr_printf("starting cv signal test\n");
	struct mallinfo minfo_start = mallinfo();
	unintr_printf("threads should print out in reverse order\n");

	for (i = 0; i < NTHREADS; i++) {
		testcv_signal[i] = cv_create();
	}
	testlock = lock_create();
	testval1 = NTHREADS - 1;
	for (i = 0; i < NTHREADS; i++) {
		result[i] = thread_create((void (*)(void *))
					  test_cv_signal_thread, (void *)i);
		assert(thread_ret_ok(result[i]));
	}

	for (i = 0; i < NTHREADS; i++) {
		thread_wait(result[i]);
	}

	assert(interrupts_enabled());
	for (i = 0; i < NTHREADS; i++) {
		cv_destroy(testcv_signal[i]);
	}
	assert(interrupts_enabled());
	lock_destroy(testlock);

	struct mallinfo minfo_end = mallinfo();
	assert(minfo_end.uordblks == minfo_start.uordblks);
	assert(minfo_end.hblks == minfo_start.hblks);

	unintr_printf("cv signal test done\n");
}

static void
test_cv_broadcast_thread(unsigned long num)
{
	int i;
	struct timeval start, end, diff;

	for (i = 0; i < LOOPS; i++) {
		assert(interrupts_enabled());
		lock_acquire(testlock);
		assert(interrupts_enabled());
		while (testval1 != num) {
			gettimeofday(&start, NULL);
			assert(interrupts_enabled());
			cv_wait(testcv_broadcast, testlock);
			assert(interrupts_enabled());
			gettimeofday(&end, NULL);
			timersub(&end, &start, &diff);

			/* cv_wait should wait at least 4-5 ms */
			if (diff.tv_sec == 0 && diff.tv_usec < 4000) {
				unintr_printf("%s took %ld us. That's too fast."
					      " You must be busy looping\n",
					      __FUNCTION__, diff.tv_usec);
				goto out;
			}
		}
		unintr_printf("%d: thread %3d passes\n", i, num);
		testval1 = (testval1 + NTHREADS - 1) % NTHREADS;

		/* spin for 5 ms */
		spin(5000);

		assert(interrupts_enabled());
		cv_broadcast(testcv_broadcast, testlock);
		assert(interrupts_enabled());
		lock_release(testlock);
		assert(interrupts_enabled());
	}
out:
	;
}

void
test_cv_broadcast()
{

	long i;
        int result[NTHREADS];

	unintr_printf("starting cv broadcast test\n");
	struct mallinfo minfo_start = mallinfo();

	unintr_printf("threads should print out in reverse order\n");

	testcv_broadcast = cv_create();
	testlock = lock_create();
	testval1 = NTHREADS - 1;
	for (i = 0; i < NTHREADS; i++) {
		result[i] = thread_create((void (*)(void *))
					  test_cv_broadcast_thread, (void *)i);
		assert(thread_ret_ok(result[i]));
	}

	for (i = 0; i < NTHREADS; i++) {
		thread_wait(result[i]);
	}

	assert(interrupts_enabled());
	cv_destroy(testcv_broadcast);
	lock_destroy(testlock);
	assert(interrupts_enabled());

	struct mallinfo minfo_end = mallinfo();
	assert(minfo_end.uordblks == minfo_start.uordblks);
	assert(minfo_end.hblks == minfo_start.hblks);

	unintr_printf("cv broadcast test done\n");
}
