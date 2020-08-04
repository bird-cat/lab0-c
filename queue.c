#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    list_ele_t *cur = q->head;
    list_ele_t *del = NULL;

    while (cur) {
        del = cur;
        cur = cur->next;

        /* free the string */
        free(del->value);

        /* free the list node */
        free(del);
    }

    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;

    /* Return false if q is a NULL queue */
    if (!q) {
        return false;
    }

    /* Allocate space for the new list node */
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    /* Allocate space for the string */
    newh->value = malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    /* copy the string */
    strncpy(newh->value, s, strlen(s) + 1);

    /* insert the new node into head */
    newh->next = q->head;
    q->head = newh;
    if (!q->tail) {
        q->tail = newh;
    }

    /* increment the queue size */
    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newt;
    if (!q) {
        return false;
    }

    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    newt->value = malloc(strlen(s) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s) + 1);

    /* insert the new node into tail */
    newt->next = NULL;

    if (q->tail) {
        q->tail->next = newt;
    } else {
        q->head = newt;
    }
    q->tail = newt;

    /* increment the queue size */
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* Return false if q is NULL or empty */
    if (!q || !q->head) {
        return false;
    }
    /* copy the removed string to *sp */
    if (bufsize > 0) {
        strncpy(sp, q->head->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    /* free the space used by head node and string */
    list_ele_t *del = q->head;
    q->head = q->head->next;
    free(del->value);
    free(del);

    /* decrement the queue size */
    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    /* reset the tail pointer */
    q->tail = q->head;

    /* reverse the list */
    list_ele_t *prev = NULL, *cur = q->head, *fut = q->head->next;

    while (fut) {
        cur->next = prev;
        prev = cur;
        cur = fut;
        fut = fut->next;
    }
    cur->next = prev;

    /* reset the head pointer */
    q->head = cur;
}

/* Merge function called by mergeSort */
/* Merge two sorted lists */
list_ele_t *merge(list_ele_t *h1, list_ele_t *h2)
{
    list_ele_t *head, *tail;

    if (strcmp(h1->value, h2->value) < 0) {
        head = tail = h1;
        h1 = h1->next;
    } else {
        head = tail = h2;
        h2 = h2->next;
    }

    while (h1 && h2) {
        if (strcmp(h1->value, h2->value) < 0) {
            tail->next = h1;
            tail = tail->next;
            h1 = h1->next;
        } else {
            tail->next = h2;
            tail = tail->next;
            h2 = h2->next;
        }
    }

    while (h1) {
        tail->next = h1;
        tail = tail->next;
        h1 = h1->next;
    }

    while (h2) {
        tail->next = h2;
        tail = tail->next;
        h2 = h2->next;
    }
    return head;

}

/* Merge sort function called by q_sort */
list_ele_t *mergeSort(list_ele_t *begin, list_ele_t *end)
{
    /* check initial condition */

    if (begin == end) {
        return begin;
    }

    /* Find the middle node and split the list */
    list_ele_t *fast = begin, *slow = begin, *prev = NULL;

    while (fast) {
        fast = fast->next;
        if (fast) {
            fast = fast->next;
        }
        prev = slow;
        slow = slow->next;
    }
    prev->next = NULL;

    /* Sort the two half individually and merge them */
    return merge(mergeSort(begin, prev), mergeSort(slow, end));
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    /* call the mergeSort function */
    q->head = mergeSort(q->head, q->tail);

    /* reset the tail pointer */
    list_ele_t *cur = q->head;

    while (cur->next) {
        cur = cur->next;
    }
    q->tail = cur;
}
