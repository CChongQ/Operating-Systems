#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "wc.h"
#include <string.h>
#include <ctype.h>

#define NUM_HASH_TABLE_BIN 100000

struct entry {
    char *key;
    int value;
    struct entry *next;
};

struct wc {
    int size;
    struct entry **table;
};

unsigned long hash_djb2(char *str) {

        unsigned long hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash;
}

/* Insert a word into a hash table. */
void wc_add(struct wc *wc, char *key) {
	struct entry *next = NULL;
	struct entry *last = NULL;

	unsigned long hashval = hash_djb2(key);
        int bin = hashval % wc->size;
	next = wc->table[bin];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair, increase its value */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {
            (next->value)++;
	} 
        else { /* cannot find the key, add in linked list */
            struct entry *newEntry = malloc(sizeof(struct entry));
            newEntry->key = key;
            newEntry->value = 1;
            newEntry->next = NULL;

            /* start of linked list */
            if( next == wc->table[ bin ] ) {
                    newEntry->next = next;
                    wc->table[ bin ] = newEntry;
            /* end of linked list */
            } else if ( next == NULL ) {
                    last->next = newEntry;
            } else  {
                    newEntry->next = next;
                    last->next = newEntry;
            }
	}
}

struct wc *
wc_init(char *word_array, long size)
{
	struct wc *wc;
        // allocate table itself
	wc = (struct wc *)malloc(sizeof(struct wc));
	assert(wc);

        // allocate pointers
	wc->table = malloc(sizeof(struct entry*) * NUM_HASH_TABLE_BIN);
        for(int i; i<NUM_HASH_TABLE_BIN; i++){
            wc->table[i] = NULL;
        }
        wc->size = NUM_HASH_TABLE_BIN;
        // end of initiating hash table

        // let word_array be const
        char * const copy = strdup(word_array);
	char *word = strtok(copy, " \t\n\f\r");
        
	while (word != NULL)
	{
                wc_add(wc, word);
		word = strtok(NULL, " \t\n\f\r"); // split by whitespace
	}

	return wc;
}

void
wc_output(struct wc *wc)
{
    int i = 0;
    struct entry *temp;
    while(i != NUM_HASH_TABLE_BIN){
        temp = wc->table[i];
        while(temp != NULL){
            printf("%s:%d\n", temp->key, temp->value);
            temp = temp->next;
        }
        i++;
    }
}

void
wc_destroy(struct wc *wc)
{
    // free memory of each bin and linked list
    struct entry *temp;
    int i = 0;
    while(i != NUM_HASH_TABLE_BIN){
        while((wc->table[i]) != NULL){
            temp = wc->table[i];
            wc->table[i] = wc->table[i]->next;
            free(temp);
        }
        i++;
    }
    // free memory of word count hash table itself
    free(wc);
}
