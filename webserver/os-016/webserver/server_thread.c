#include "request.h"
#include "server_thread.h"
#include "common.h"
#include <pthread.h>

struct server {
    int nr_threads;
    int max_requests;
    int max_cache_size;
    int exiting;
    /* add any other parameters you need */
    int* buffer; // FIFO
    pthread_t* worker_thraed;

};
pthread_mutex_t mutex;
pthread_cond_t Full;
pthread_cond_t Empty;
int itemCount = 0;
int in;
int out;
/* static functions */
/* initialize file data */
static struct file_data *
file_data_init(void) {
    struct file_data *data;

    data = Malloc(sizeof (struct file_data));
    data->file_name = NULL;
    data->file_buf = NULL;
    data->file_size = 0;
    return data;
}

/* free all file data */
static void
file_data_free(struct file_data *data) {
    free(data->file_name);
    free(data->file_buf);
    free(data);
}

static void
do_server_request(struct server *sv, int connfd) {
    int ret;
    struct request *rq;
    struct file_data *data;

    data = file_data_init();

    /* fill data->file_name with name of the file being requested */
    rq = request_init(connfd, data);
    if (!rq) {
        file_data_free(data);
        return;
    }
    /* read file, 
     * fills data->file_buf with the file contents,
     * data->file_size with file size. */
    ret = request_readfile(rq);
    if (ret == 0) { /* couldn't read file */
        goto out;
    }
    /* send file to client */
    request_sendfile(rq);
out:
    request_destroy(rq);
    file_data_free(data);
}

/* entry point functions */
void Consumer(struct server *sv) {

    while (1) {

        pthread_mutex_lock(&mutex); //mutex lock

        while (itemCount == 0 && sv->exiting == 0) {
            //A worker thread must wait if the buffer is empty.
            pthread_cond_wait(&Empty, &mutex);
        }
        if (sv->exiting == 1) {
            pthread_mutex_unlock(&mutex);
            return;
        }

        //remove item from buffer
        int connfd = sv->buffer[out];
        itemCount--;
        if (itemCount == (sv->max_requests - 1)) {
            //wake up producer
            pthread_cond_broadcast(&Full);
        }
        out = (out + 1) % (sv->max_requests);
        pthread_mutex_unlock(&mutex);
        do_server_request(sv, connfd);
    }
}

struct server *
server_init(int nr_threads, int max_requests, int max_cache_size) {
    // pthread_mutex_lock(&mutex);
    struct server *sv;

    sv = Malloc(sizeof (struct server));
    sv->nr_threads = nr_threads;
    sv->max_requests = max_requests + 1;
    sv->max_cache_size = max_cache_size;
    sv->exiting = 0;
    in = 0;
    out = 0;


    pthread_cond_init(&Empty, NULL);
    pthread_cond_init(&Full, NULL);
    pthread_mutex_init(&mutex, NULL);

    if (nr_threads > 0 || max_requests > 0 || max_cache_size > 0) {
        /* Lab 4: create queue of max_request size when max_requests > 0 */
        if (max_requests > 0) {
            sv->buffer = (int*) malloc(sizeof (int)* (sv->max_requests));
        }
        else sv->buffer = NULL;

        /* Lab 4: create worker threads when nr_threads > 0 */
        if (nr_threads > 0) {
            sv->worker_thraed = (pthread_t*) malloc(sizeof (pthread_t) * nr_threads);
            for (int i = 0; i < nr_threads; i++) {
                pthread_create(&sv->worker_thraed[i], NULL, (void*) Consumer, (void *) sv);
            }
        } else sv->worker_thraed = NULL;
    }

    /* Lab 5: init server cache and limit its size to max_cache_size */


    return sv;
}

void
server_request(struct server *sv, int connfd) {
    if (sv->nr_threads == 0) { /* no worker threads */
        do_server_request(sv, connfd);
    } else {
        /*  Save the relevant info in a buffer and have one of the
         *  worker threads do the work. */
        pthread_mutex_lock(&mutex);

        while (itemCount == sv->max_requests) {
            //Master thread must block and wait if the buffer is full
            pthread_cond_wait(&Full, &mutex);
        }
        //buffer is not full, produce
        sv->buffer[in] = connfd;
        itemCount++;
        if (itemCount == 1) {
            //wake up consumer 
            pthread_cond_broadcast(&Empty);
        }
        in = (in + 1) % (sv->max_requests);
        pthread_mutex_unlock(&mutex);
    }
}

void
server_exit(struct server *sv) {
    /* when using one or more worker threads, use sv->exiting to indicate to
     * these threads that the server is exiting. make sure to call
     * pthread_join in this function so that the main server thread waits
     * for all the worker threads to exit before exiting. */

    //pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);
    sv->exiting = 1;
    //wake up all threads currently blocked by Empty and Full
    pthread_cond_broadcast(&Empty);
    pthread_cond_broadcast(&Full);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < sv->nr_threads; i++) {
        //wait for all work thread to exit
        pthread_join(sv->worker_thraed[i], NULL);
    }
    //all work thread exit

    /* make sure to free any allocated resources */

    if (sv->max_requests > 0) {
        free(sv->buffer);
    }
    if (sv->nr_threads > 0) {
        free(sv->worker_thraed);
    }

    free(sv);
}

