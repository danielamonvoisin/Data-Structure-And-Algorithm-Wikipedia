#include "document.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int **matrix;
  int size;
} DocumentGraph;

DocumentGraph *create_graph(int size);
void add_link(DocumentGraph *graph, int from, int next);
void print_graph(DocumentGraph *graph);
void free_graph(DocumentGraph *graph);
void completing_graph(Documents *docs, DocumentGraph *graph);
