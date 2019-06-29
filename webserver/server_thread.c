#include "request.h"
#include "server_thread.h"
#include "common.h"
#include <pthread.h>
#include <stdbool.h>

struct node {
    struct file_data* file;
    int workingFlag;
    bool deleted;
    struct node* next;
};
typedef struct node table_node;

typedef struct table {
    int table_size;
    int current_size;
    table_node** nodes;
} cache_table;

typedef struct LRU_node {
    char* file_name;
    struct LRU_node *prev;
    struct LRU_node * next;
} LRU_node;

typedef struct LRU_queue {
    LRU_node* front; //MRU node
    LRU_node* end; //LRU node

} LRU_list;

LRU_list* LRU_Queue = NULL;

struct server {
    int nr_threads;
    int max_requests;
    int max_cache_size;
    int exiting;
    /* add any other parameters you need */
    int* buffer; // FIFO
    pthread_t* worker_thraed;
    /***Lab5***/
    cache_table* cacheTable;
};


pthread_mutex_t mutex;
pthread_cond_t Full;
pthread_cond_t Empty;
int itemCount = 0;
int in;
int out;


unsigned long hash_djb2(char *str);
table_node* cache_lookup(struct server* sv, char* file_name);
table_node* cache_insert(struct server* sv, struct file_data* file);
int cache_evict(struct server* sv, int evict_size);
void findInsert(struct server *sv, table_node* new_node, int hashValue);
void updateLRUList(char* file_name, bool newNode);
void updateDeletedLRU(struct server *sv, table_node* deleted);


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

    /*****************Lab5*****************/
    if (sv != NULL && sv->max_cache_size > 0) {

        pthread_mutex_lock(&mutex);

        table_node* lookup_Result = cache_lookup(sv, data->file_name);

        if (lookup_Result != NULL) {
            //found in cache table 
            lookup_Result->workingFlag++;

            request_set_data(rq, lookup_Result->file);
            //update LRU list
            updateLRUList(data->file_name, false);
        } else {
            //not found in cache
            pthread_mutex_unlock(&mutex);

            /* read file, 
             * fills data->file_buf with the file contents,
             * data->file_size with file size. */
            ret = request_readfile(rq);
            if (ret == 0) { /* couldn't read file */
                goto out;
            }

            pthread_mutex_lock(&mutex);

            //insert into cache table
            lookup_Result = cache_lookup(sv, data->file_name);
            if (lookup_Result == NULL) {
                table_node* temp = cache_insert(sv, data);
                if (temp != NULL) {
                    temp->workingFlag++;
                    updateLRUList(data->file_name, true);
                }
            } else {
                lookup_Result->workingFlag++;
                request_set_data(rq, lookup_Result->file);
                updateLRUList(data->file_name, false);
            }
        }

        pthread_mutex_unlock(&mutex);
        request_sendfile(rq);
out:
        if (lookup_Result != NULL) {
            pthread_mutex_lock(&mutex);
            lookup_Result->workingFlag--;

            pthread_mutex_unlock(&mutex);
        }
        request_destroy(rq);
        file_data_free(data);
    } 
    else {
        /* reads file, 
         * fills data->file_buf with the file contents,
         * data->file_size with file size. */
        ret = request_readfile(rq);
        if (!ret)
            goto out_noCache;
        /* sends file to client */
        request_sendfile(rq);
out_noCache:
        request_destroy(rq);
        file_data_free(data);
    }
}
/********************Lab5********************/

/**Hash function From Lab1**/
unsigned long hash_djb2(char *str) {

    unsigned long hash = 5381;
    int i = 0;
    while (str[i] != '\0')
        hash = hash * 33 + str[i++];
    return hash;
}

/*find the file in hash table*/
table_node* cache_lookup(struct server *sv, char* file_name) {

    int hash_val = hash_djb2(file_name) % (sv->cacheTable->table_size);

    if (sv->cacheTable->nodes[hash_val] == NULL)
        return NULL;
    else {
        // find the node with the same hash value and the same file name
        table_node* temp = sv->cacheTable->nodes[hash_val];
        while (temp != NULL) {
            if (strcmp(temp->file->file_name, file_name) == 0)      		
                return temp;
            else
                temp = temp->next;
        }
        return temp; //temp == NULL here
    }
}

/*Insert new node into hash table*/
table_node* cache_insert(struct server *sv, struct file_data *file) {

    //sizes of all the cached files must be less than or equal to max_cache_size
    if (file->file_size > sv->max_cache_size) {
        return NULL;
    }

    if (file->file_size > (sv->max_cache_size - sv->cacheTable->current_size)) {
        //start to evict LRU file
        int size_extra = sv->cacheTable->current_size + file->file_size - sv->max_cache_size;
        int temp = cache_evict(sv, size_extra);
        if (temp > 0) {
            //evict unsuccessful 
            return NULL;
        }
    }


    sv->cacheTable->current_size = sv->cacheTable->current_size + file->file_size;

    //add to the cache table
    int hash_val = hash_djb2(file->file_name) % (sv->cacheTable->table_size);

    table_node* new_node = (table_node*) malloc(sizeof (table_node));
    new_node->file = file_data_init();
    new_node->file->file_name = strdup(file->file_name);
    new_node->file->file_buf = strdup(file->file_buf);
    new_node->file->file_size = file->file_size;


    new_node->next = NULL;
    new_node->workingFlag = 0;



    if (sv->cacheTable->nodes[hash_val] == NULL) {
        //empty table entry
        sv->cacheTable->nodes[hash_val] = new_node;
        return new_node;
    } else {
        table_node* temp = sv->cacheTable->nodes[hash_val];
        while (temp != NULL) {
            temp = temp->next;
        }
        //no space, add new conflict element to end of the list
        temp->next = new_node;
        return new_node;
    }
}

/*Evict LRU node from hash table*/
int cache_evict(struct server *sv, int evict_size) {


    if (LRU_Queue == NULL)
        return 10;
    else {
        int current_evict_size = evict_size;
        LRU_node * end = LRU_Queue->end;

        //find the LRU node in the hash table, check whether its file size > evict_size
        //if < evict_size, continue to evict
        while (current_evict_size > 0 && end != NULL) {

            table_node* temp = cache_lookup(sv, end->file_name);

            //skip working nodes
            while (end != NULL && temp->workingFlag > 0) {
                end = end->prev;
                if (end != NULL)
                    temp = cache_lookup(sv, end->file_name);
            }

            if (end != NULL) {
                current_evict_size = current_evict_size - temp->file->file_size;
                sv->cacheTable->current_size = sv->cacheTable->current_size - temp->file->file_size;

                //update LRU list
                if (end->prev != NULL) {
                    end->prev->next = end->next;
                    if (end->next != NULL)
                        end->next->prev = end->prev;
                } else {
                    //last node is the head
                    LRU_Queue->front = LRU_Queue->front->next;
                    if (end->next != NULL)
                        end->next->prev = end->prev;
                }

                LRU_node *toBeDeleted = end;
                end = end->prev;
                toBeDeleted->prev = NULL;
                toBeDeleted->next = NULL;
                free(toBeDeleted); // free the LRU node


                file_data_free(temp->file);
                temp->file = NULL;
                updateDeletedLRU(sv, temp);
            } else {
                return 10;
            }
        }
        return current_evict_size;
    }
}


void updateDeletedLRU(struct server *sv, table_node* deleted) {
    int hash_val = hash_djb2(deleted->file->file_name) % (sv->cacheTable->table_size);

    table_node* temp = sv->cacheTable->nodes[hash_val];
    table_node* prev = NULL;

 
    while (temp != NULL) {
        if (strcmp(temp->file->file_name, deleted->file->file_name) == 0) {
            if (prev != NULL) {
                prev->next = temp->next;
            } else {
                sv->cacheTable->nodes[hash_val] = temp->next;
            }
            temp->next = NULL;
            free(temp);
            break;
        } else {
            prev = temp;
            temp = temp->next;
        }
    }

}

/*Update status of LRU cache*/
void updateLRUList(char* file_name, bool newNode) {

    if (newNode == false) {
        //move existed node
        LRU_node* temp = LRU_Queue->front;

        while (temp != NULL) {
            if (strcmp(temp->file_name, file_name) != 0) {
                temp = temp->next;
            } else {
                //add to the front of the LRU list
                if (temp->prev != NULL) {
                    // temp is not head of LRU list 
                    temp->prev->next = temp->next;
                    if (temp->next != NULL)
                        temp->next->prev = temp->prev;

                    temp->next = LRU_Queue ->front;
                    temp->prev = NULL;
                    LRU_Queue->front = temp;
                }
                //else {already the head, do not need to update} 
                break;
            }

        }
        return;
    } else {
        //add a new node into the LRU Queue
        LRU_node* temp = (LRU_node*) malloc(sizeof (LRU_node));
        temp->file_name = strdup(file_name);
        temp->prev = NULL;
        temp->next = NULL;
        if (LRU_Queue == NULL) {
            LRU_Queue = (LRU_list*) malloc(sizeof (LRU_list));
            LRU_Queue->front = temp;
            LRU_Queue->end = temp;
        } else {
            temp->next = LRU_Queue->front;
            LRU_Queue->front = temp;
        }
    }
    return;

}


/********************Lab4********************/

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

        bool nocache = true;
        if (sv->max_cache_size > 0)
            nocache = false;
        pthread_mutex_unlock(&mutex);
        if (!nocache)
            do_server_request(sv, connfd); //do reuqest after unlock, multiple threads can do this at the same time, no critical section invlved 
        else
            do_server_request(NULL, connfd);
    }
}

struct server *
server_init(int nr_threads, int max_requests, int max_cache_size) {
    pthread_mutex_lock(&mutex);
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
        } else sv->buffer = NULL;

        /* Lab 4: create worker threads when nr_threads > 0 */
        if (nr_threads > 0) {
            sv->worker_thraed = (pthread_t*) malloc(sizeof (pthread_t) * nr_threads);
            for (int i = 0; i < nr_threads; i++) {
                pthread_create(&sv->worker_thraed[i], NULL, (void*) Consumer, (void *) sv);
            }
        } else sv->worker_thraed = NULL;

        /* Lab 5: init server cache and limit its size to max_cache_size */
        if (max_cache_size > 0) {
            sv->cacheTable = (cache_table*) malloc(sizeof (cache_table));
            //assert(sv->cache);
            sv->cacheTable->table_size = max_cache_size;
            sv->cacheTable->nodes = (table_node **) malloc(sizeof (table_node *) * max_cache_size);
            int i;
            for (i = 0; i < sv->cacheTable->table_size; i++)
                sv->cacheTable->nodes[i] = NULL;
        } else if (max_cache_size == 0) {
            sv->cacheTable = NULL;
        }

    }



    pthread_mutex_unlock(&mutex);
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

    if (sv->cacheTable != NULL) {
        for (int i = 0; i < sv->max_cache_size; i++) {
            if (sv->cacheTable->nodes[i] != NULL) {
                table_node* loop = sv->cacheTable->nodes[i];
                while (loop != NULL) {
                    table_node* temp = loop;
                    loop = loop->next;
                    free(temp);
                }
            }
        }
        free(sv->cacheTable);
    }

    if (LRU_Queue != NULL) {
        LRU_node* loop = LRU_Queue->end;
        while (loop != NULL) {
            LRU_node* temp = loop;
            loop = loop->prev;
            free(temp);
        }
        free(LRU_Queue);
    }

    free(sv);
}

