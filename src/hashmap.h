#define HASH_SIZE 50021 // or some prime number
// Hash function parameters
#include "document.h"
#include "query.h"
#define INT_MAX 10000000

typedef struct DocListNode {
  Document *doc;
  // Document* doc; // Pointer to the document structure
  struct DocListNode *next;
  struct DocListNode *previous;
} DocListNode;

typedef struct WordEntry {
  char *word;             // lowercase version
  DocListNode *docs_head; // linked list of doc IDs
  int doc_count;          // number of documents that contain this word
  struct WordEntry *next; // next in hash table bucket (chaining)
  DocListNode *tail;
} WordEntry;

typedef struct relevancesInQueryNODE { // all this relevances are from ID's that
                                       // contain all the words in the Query
  Document *docsFiltered;
  struct relevancesInQueryNODE *next;
  struct relevancesInQueryNODE *previous;
} relevancesInQueryNODE;

typedef struct {
  WordEntry *table[HASH_SIZE];
  size_t size;
} HashMap;

HashMap *init_hashmap(size_t size);

unsigned long hash_fnv1a(const char *str);
void add_word_to_index(HashMap *map, const char *raw_word, Document *doc);
void index_document(HashMap *map, Document *doc);
DocListNode *get_docs_for_word(HashMap *map, const char *word_raw);
void print_IDs_query(HashMap *map, Query *query_list);
bool doc_in_list(int doc_id, DocListNode *head);
void print_top5_docs(HashMap *map, Query *query_list, Documents *docs);

void sortByRelevanceDescending(relevancesInQueryNODE *head);

bool is_query_or_docs_invalid(Query *query_list, Documents *docs);
int get_doc_lists_for_query_words(HashMap *map, Query *query_list,
                                  DocListNode **doc_lists, int max_words);
int find_smallest_doclist_index(DocListNode **doc_lists, int word_count);
relevancesInQueryNODE *
filter_docs_in_all_lists(DocListNode *min_list, DocListNode **include_lists,
                         int include_count, int min_index,
                         DocListNode **exclude_lists, int exclude_count,
                         int *found_count);
void print_top5_filtered_docs(relevancesInQueryNODE *head);
void free_relevances_list(relevancesInQueryNODE *head);
void free_hashmap(HashMap *map);
void print_hashmap_contents();