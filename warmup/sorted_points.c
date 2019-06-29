#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "point.h"
#include "sorted_points.h"

struct node {
    struct point p;
    struct node* next;
};

/* this structure should store all the points in a list in sorted order. */
struct sorted_points {
	/* you can define this struct to have whatever fields you want. */
    struct node* head;
};

void printList(struct sorted_points *sp) 
{ 
    struct node *temp = sp->head; 
    while(temp != NULL) 
    { 
        printf("(%f, %f)  ", point_X(&(temp->p)), point_Y(&(temp->p))); 
        temp = temp->next; 
    } 
    printf("\n");
} 

/* think about where you are going to store a pointer to the next element of the
 * linked list? if needed, you may define other structures. */

struct sorted_points *
sp_init()
{
    struct sorted_points *sp;

    sp = (struct sorted_points *)malloc(sizeof(struct sorted_points));
    assert(sp);

    sp->head = NULL;
    return sp;
}

void
sp_destroy(struct sorted_points *sp)
{
    if(sp == NULL) return;
    struct node* tmp;
    struct node* head = sp->head;
    
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
    free(sp);
}

// avoid compare value zero
int compare_pointsXY(const struct point *p1, const struct point *p2){
    int result = point_compare(p1, p2);
    if (result == 0) {
        if (point_X(p1) == point_X(p2)) {
            result = point_Y(p1) < point_Y(p2) ? -1 : 1;
        } else {
            result = point_X(p1) < point_X(p2) ? -1 : 1;
        }
    }
    return result;
}

int
sp_add_point(struct sorted_points *sp, double x, double y)
{
    if(sp == NULL) return 0;
    
    struct node *newNode = (struct node*) malloc(sizeof(struct node));
    point_set(&(newNode->p), x, y);
    
    // empty linked list
    if(sp->head == NULL){
        newNode->next = NULL;
        sp->head = newNode;
        return 1;
    }
    // insert at head, newNode value smaller than headNode value
    else if(compare_pointsXY(&(sp->head->p), &(newNode->p)) == 1){
        newNode->next = sp->head;
        sp->head = newNode;
        return 1;
    }
    else{ // insert at proper location
        struct node* curr = sp->head;
        while(curr->next != NULL && 
                compare_pointsXY(&(curr->next->p), &(newNode->p)) <= 0){
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
        return 1;
    }
}

int
sp_remove_first(struct sorted_points *sp, struct point *ret)
{
    if(sp == NULL || sp->head == NULL) return 0;
    
    *ret = sp->head->p;
    
    struct node* temp = sp->head;
    sp->head = sp->head->next;
    free(temp);
    
    return 1;
}

int
sp_remove_last(struct sorted_points *sp, struct point *ret)
{    
    if(sp == NULL || sp->head == NULL) 
        return 0;
    
    if(sp->head->next == NULL){
        *ret = sp->head->p;
        struct node* temp = sp->head;
        sp->head = NULL;
        free(temp);
        return 1;
    }
    
    struct node* prev = sp->head;
    struct node* curr = sp->head->next;
    
    while(curr->next != NULL){
        curr = curr->next;
        prev = prev->next;
    }
    *ret = curr->p;
    free(curr);
    prev->next = NULL;
    return 1;
}

int
sp_remove_by_index(struct sorted_points *sp, int index, struct point *ret)
{
    // empty linked list
    if(sp == NULL || sp->head == NULL) 
        return 0;
    
//    printList(sp);
    
    // remove the first node, index = 0
    if(index == 0){
        return sp_remove_first(sp, ret);
    }
    
    struct node* curr = sp->head;

    // find node before deleted node
    for (int i=0; curr!=NULL && i<index-1; i++) 
         curr = curr->next; 
    
    if(curr == NULL || curr->next == NULL){ // index out of range
        return 0;
    }
    // delete node after curr
    struct node *next = curr->next->next;
    *ret = curr->next->p;
    free(curr->next);
    curr->next = next;
    return 1;
}


int
sp_delete_duplicates(struct sorted_points *sp)
{
    if(sp == NULL || sp->head == NULL){
        return 0;
    }
    struct node* curr = sp->head;
    struct node* next;
    int count = 0;
    
    while(curr->next != NULL){
        //compare current node and its next node
        if(point_compare(&(curr->p), &(curr->next->p)) == 0){
            // delete its next node
            next = curr->next->next;
            free(curr->next);
            count++;
            curr->next = next;
        } else{ // nothing to delete
            curr = curr->next;
        }
    }
    return count;
}

//int
//sp_delete_duplicates(struct sorted_points *sp)
//{
//    int count = 0;
//
//    struct node *i = sp->head;
//    while (i) {
//            struct node *j_curr = i->next;
//            struct node *j_prev = i;
//            while (j_curr) {
//                    if (point_compare(&j_curr->p, &i->p) == 0) {
//                            ++count;
//                            struct node *del = j_curr;
//                            j_prev->next = j_curr = j_curr->next;
//                            free(del);
//                            continue;
//                    }
//                    j_curr = j_curr->next;
//                    j_prev = j_prev->next;
//            }
//            i = i->next;
//    }
//
//    return count;
//}
