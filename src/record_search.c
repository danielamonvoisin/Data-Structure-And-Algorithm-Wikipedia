#include "record_search.h"

void createSearchingNode(SearchQueue *queue, char *string_search) {
  SearchNode *currentNode = (SearchNode *)malloc(
      sizeof(SearchNode));  // allocates memory for a new search node
  currentNode->next = NULL; // initializes next to null (it will be the newest)
  currentNode->previous = NULL; // initializes previous to null
  currentNode->name =
      strdup(string_search); // stores a copy of the search string

  if (queue->head ==
      NULL) { // if the queue is empty, set both head and tail to the new node
    queue->head = currentNode; // only node, so it's both oldest
    queue->tail = currentNode; // and newest
  } else {
    currentNode->previous =
        queue->tail; // link current node to the current tail (newest so far)
    queue->tail->next = currentNode; // link old tail forward to the new node
    queue->tail = currentNode;       // update the tail to the newest node
  }
  queue->size++; // increase the size of the queue

  // if the queue has more than 3 searches, remove the oldest (head)
  if (queue->size > 3) {
    SearchNode *toDelete =
        queue->head; // store the current head (oldest) to be removed
    queue->head = toDelete->next; // move head to the next oldest node
    queue->head->previous = NULL; // the new head has no previous
    free(toDelete->name);         // free the search string
    free(toDelete);               // free the node itself
    queue->size--;                // decrease the queue size
  }
}

SearchQueue *createSearchQueue() {
  SearchQueue *currentQueue = (SearchQueue *)malloc(
      sizeof(SearchQueue)); // allocates memory for the queue structure
  currentQueue->head =
      NULL; // initializes the head to null (no oldest search yet)
  currentQueue->tail =
      NULL;               // initializes the tail to null (no newest search yet)
  currentQueue->size = 0; // sets initial size to 0
  return currentQueue;    // returns the new empty queue
}

void print_record_search(SearchQueue *queue) {
  printf("*********************** RECENT SEARCHES **********************\n");

  SearchNode *current = queue->tail;
  while (current) {
    // Limit display to 58 characters (1 for '*', 1 for '*')
    char preview[59];
    strncpy(preview, current->name, 58);
    preview[58] = '\0'; // null-terminate

    int name_len = strlen(preview);
    int padding = 58 - name_len;

    printf("* %s%*s *\n", preview, padding, "");
    current = current->previous;
  }

  printf("**************************************************************\n");
}

void freeSearchQueue(SearchQueue *queue) {
  if (!queue)
    return;

  SearchNode *current = queue->head;
  while (current) {
    SearchNode *next = current->next;
    free(current->name); // Free the name string
    free(current);
    current = next;
  }

  free(queue);
}
