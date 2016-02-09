#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

void push(node_t** head, int val) {
    node_t* new = malloc(sizeof(node_t));
    new->val = val;
    new->next = NULL;
    if(*head == NULL) {
      *head = new;
      return;
    }
    node_t * current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new;
}

int pop(node_t ** head) {
    int retval = -1;
    node_t * next_node = NULL;
    if (*head == NULL)
        return -1;
    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;
    return retval;
}

int remove_by_value(node_t ** head, int val) {
    node_t *previous, *current;
    if (*head == NULL)
        return -1;
    if ((*head)->val == val)
        return pop(head);
    previous = *head;
    current = (*head)->next;
    while (current) {
        if (current->val == val) {
            previous->next = current->next;
            free(current);
            return val;
        }
        previous = current;
        current  = current->next;
    }
    return -1;
}

void print_list(node_t * head, char* s) {
  node_t * current = head;
  while (current != NULL) {
    sprintf(s + strlen(s), "%d,", current->val);
    current = current->next;
  }
}

int is_empty(node_t* head) {
  return (head == NULL);
}

void delete_list(node_t *head) {
    node_t  *current = head, *next = head;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}
