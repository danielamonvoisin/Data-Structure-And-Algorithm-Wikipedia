#ifndef QUERY_H
#define QUERY_H

#define MAX_SUM_LENGTH 150

#include "document.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueryNode { // this will be the node of the linked list that will
                           // store the words entered by the user.
  char *name;              // The text of the query
  struct QueryNode *next;  // Pointer to the next node in the linked list //
                           // Number of documents that contain this word
  bool is_excluded;
} QueryNode;

typedef struct {
  QueryNode *head; // Pointer to the head of the linked list
  int count;       // Number of nodes in the linked list
} Query; // this will be the query linked list that will store the words inputed
         // by the user.

QueryNode *createNode(char *word);
Query *create_string_list(char *input_string);

void FreeQuery(Query *query);
void search_documents(Documents *docs, Query *query);
bool document_matches_query(Document *doc, Query *query);

void search_documents(Documents *docs, Query *query);

char *to_lowercase(const char *str);
bool contains_keyword(char *text, char *keyword);
char *preview_text(const char *text, size_t max_length);
void free_query_list(Query *query);

#endif