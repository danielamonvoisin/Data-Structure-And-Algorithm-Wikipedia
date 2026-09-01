
#include "graphs.h"

DocumentGraph *create_graph(int size) { //creates a new graph represented by an adjacency matrix of the given size.
  DocumentGraph *graph = (DocumentGraph *)malloc(sizeof(DocumentGraph));
  graph->size = size;

  // Allocate the 2D matrix
  graph->matrix = (int **)malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++) {
    graph->matrix[i] = (int *)calloc(size, sizeof(int)); // initialize with 0s
  }

  return graph;
}

void add_link(DocumentGraph *graph, int from, int to) { //adds a directional link from document from to document to in the graph.
  graph->matrix[from][to] += 1;
}

void print_graph(DocumentGraph *graph) { //prints the full adjacency matrix of the graph.
  printf("Document Graph (adjacency matrix):\n");
  for (int i = 0; i < graph->size; i++) {
    for (int j = 0; j < graph->size; j++) {
      printf("%d ", graph->matrix[i][j]);
    }
    printf("\n");
  }
}

void completing_graph(Documents *docs, DocumentGraph *graph) { //goes through all documents and their links, adds the links to the graph, and increases the relevance of each linked document.
  DocumentNode *fromID = docs->head;

  while (fromID) {
    LinkNode *toIDs = fromID->doc->list_of_links;
    while (toIDs) {
      add_link(graph, fromID->doc->id, toIDs->data.ID);
      DocumentNode *Update_relevance = docs->head;
      for (int i = 0; i <= (toIDs->data.ID) - 1;
           i++) { // iterate in the Documents structure until it matches the ID
                  // number.
        Update_relevance =
            Update_relevance->next; // so Update_relevance is the document
                                    // structure we want to reach.
      }
      Update_relevance->doc->relevance++;
      toIDs = toIDs->next;
    }
    fromID = fromID->next;
  }
}

//function to free the graph
void free_graph(DocumentGraph *graph) {
  for (int i = 0; i < graph->size; i++) {
    free(graph->matrix[i]);
  }
  free(graph->matrix);
  free(graph);
}