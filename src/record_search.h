#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SearchNode { // this will be the node of the linked list that
                            // will store the words entered by the user.
  char *name;               // The text of the query.
  struct SearchNode *next;  // Pointer to the next node in the linked list.
  struct SearchNode
      *previous; // Pointer to the previous node in the linked list.
} SearchNode;

typedef struct { // this will be the query linked list that will store the words
                 // inputed by the user.
  int size;
  SearchNode *head; // Pointer to the head of the linked list.
  SearchNode *tail; // Pointer to the tail of the linked list.
} SearchQueue; // this will be the query linked list that will store the words
               // inputed by the user.

void createSearchingNode(SearchQueue *queue, char *string_search);
SearchQueue *createSearchQueue();
void print_record_search(SearchQueue *queue);
void freeSearchQueue(SearchQueue *queue);