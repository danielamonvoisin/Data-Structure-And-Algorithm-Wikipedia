#include "document.h"
#include "graphs.h"
#include "hashmap.h"
#include "query.h"
#include "record_search.h"
#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main() {

  // 1. load all docs into the list
  Documents *docs = LoadAllDocument("datasets/wikipedia5400");
  if (!docs) {
    fprintf(stderr, "Failed to load documents.\n");
    return 1;
  }
  printf("Documents loaded: %d\n", docs->count);

  // 2. init reverse index (hashmap)
  HashMap *map = malloc(sizeof(HashMap));
  if (!map) {
    fprintf(stderr, "Failed to allocate memory for HashMap\n");
    exit(EXIT_FAILURE);
  }
  memset(map->table, 0, sizeof(map->table)); // initialize all pointers to NULL

  DocumentNode *current = docs->head;
  while (current) {
    index_document(map, current->doc);
    current = current->next;
  }

  // 3. init the graph
  DocumentGraph *graph = create_graph(docs->count);
  completing_graph(docs, graph);

  // 4. while the user does not exit
  SearchQueue *mySearchQueue = createSearchQueue();
  char input[1024];

  while (true) {
    // 4.1 ask query
    printf("WELCOME TO POMPEUFARRERS SEARCHER\n");
    printf("What are you searching for? (press ENTER to exit):\n");

    if (!fgets(input, sizeof(input), stdin))
      break;
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) == 0)
      break;

    createSearchingNode(mySearchQueue, input);
    Query *query_list = create_string_list(input);
    print_record_search(mySearchQueue);

    // 4.2 search documents using hashmap
    // 4.3 get the relevance score from the graph
    // 4.4 order documents by relevance score (highest first)
    // 4.5 print the summary of the first 5 documents
    print_top5_docs(map, query_list, docs);

    free_query_list(query_list);

    // 4.6 ask user for an index (0 to 5)
    printf("If there is any document that you want to see, enter its ID (ENTER "
           "to skip):\n");
    if (!fgets(input, sizeof(input), stdin))
      break;
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) == 0)
      continue;

    // 4.7 print all the info about the document
    Document *my_doc = get_document_by_id(docs, atoi(input));
    if (my_doc) {
      printf("\nID\n%d\n\nTITLE\n%s\n\nRELEVANCE SCORE\n%d\n\nBODY\n%s\n",
             my_doc->id, my_doc->title, my_doc->relevance, my_doc->body);
    } else {
      printf("Document not found.\n");
    }
    printf("\n");
    print_record_search(mySearchQueue);
  }

  // free everything
  FreeDocuments(docs);
  free_graph(graph);
  free_hashmap(map);
  free(map);
  freeSearchQueue(mySearchQueue);

  return 0;
}
