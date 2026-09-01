#include "hashmap.h"

unsigned long hash_fnv1a(const char *str) {
  unsigned long hash = 14695981039346656037UL; //start with a fixed offset basis
  while (*str) {
    hash ^= (unsigned char)(*str++); //xor byte into the hash
    hash *= 1099511628211UL; //multiply by the FNV prime
  }
  return hash; //return the computed hash value
}

HashMap *init_hashmap(size_t size) {
  HashMap *map = malloc(sizeof(HashMap)); //allocate memory for the hashmap

  if (!map)
    return NULL;  //return null if allocation failed

  map->size = size; //store the size in the struct

  for (int i = 0; i < HASH_SIZE; i++) {
    map->table[i] = NULL; //initialize all table entries to null
  }

  return map; //return the initialized hashmap

}

void add_word_to_index(HashMap *map, const char *raw_word, Document *doc) {

  if (!raw_word || !doc)
    return;    //exit if the word or doc is null

  char *word = to_lowercase(raw_word); //convert word to lowercase
  // size_t slot = hash_fnv1a(word) % HASH_SIZE;
  unsigned long hash = hash_fnv1a(word) % HASH_SIZE; //compute slot using hash function
  WordEntry *entry = map->table[hash]; //get the entry at the computed slot

  while (entry) {
    if (strcmp(entry->word, word) == 0) //check if word already exists
      break;
    entry = entry->next; //go to next entry in case of collision
  }

  if (!entry) {
    entry = malloc(sizeof(WordEntry)); //create a new entry if word was not found
    entry->word = word; //store the word
    entry->docs_head = NULL; //initialize document list
    entry->next = map->table[hash]; //insert entry at the beginning of the slot's list
    entry->doc_count = 0; //initialize doc count
    entry->tail = NULL; //initialize tail
    map->table[hash] = entry; //update the table to point to new entry
  } else {
    free(word); //update the table to point to new entry
  }

  DocListNode *current = entry->docs_head;
  while (current) {
    if (current->doc->id == doc->id)  //check if document is already in the list
      return;
    current = current->next;
  }

  DocListNode *node = malloc(sizeof(DocListNode)); //allocate new node for the doc
  node->doc = doc; //link the doc to the node
  node->next = entry->docs_head; //insert node at the beginning of the list
  if (entry->docs_head)
    entry->docs_head->previous = node; //update previous pointer if list was not empty
  node->previous = NULL; //new node is now the head, so no previous
  entry->docs_head = node; //update head of the list
  entry->doc_count++; //increment doc count
  if (!entry->tail)
    entry->tail = node; //set tail if this is the first doc
}

void index_document(HashMap *map, Document *doc) {
  if (!doc)
    return; //if the document is null, do nothing

  //index words in the title
  char *title_copy = to_lowercase(doc->title); //make a lowercase copy of the title
  char *token_title = strtok(title_copy, " \t\n.,;:!?()[]\"'"); //tokenize the title using common delimiters
  while (token_title) {
    add_word_to_index(map, token_title, doc); //add each word from the title to the hashmap
    token_title = strtok(NULL, " \t\n.,;:!?()[]\"'"); //move to the next word
  }
  free(title_copy); //free the temporary lowercase title copy
  
  //index words in the body
  char *body_copy = to_lowercase(doc->body); //make a lowercase copy of the body
  char *token = strtok(body_copy, " \t\n.,;:!?()[]\"'"); //tokenize the body using common delimiters

  while (token) {
    add_word_to_index(map, token, doc); //add each word from the body to the hashmap
    token = strtok(NULL, " \t\n.,;:!?()[]\"'"); //move to the next word
  }
  free(body_copy); //free the temporary lowercase body copy
}

DocListNode *get_docs_for_word(HashMap *map, const char *word_raw) { //searches the hashmap to find which documents contain a specific word. Calculates the corresponding hash slot, then searches through that slot’s linked list to find the word. If found, it returns the list of documents (a DocListNode*) associated with that word. 
  if (!word_raw)
    return NULL; //if the input word is null, return no results
  char *word = to_lowercase(word_raw); //convert the word to lowercase for case-insensitive matching
  size_t slot = hash_fnv1a(word) % HASH_SIZE; //compute the slot index using the hash function and table size

  WordEntry *entry = map->table[slot]; //get the first entry in the linked list at that slot

  while (entry) { //iterate through the list of entries at that hash slot
    if (strcmp(entry->word, word) == 0) { //if the word matches exactly
      free(word);  //free the lowercase copy (we no longer need it)
      return entry->docs_head; //return the list of documents containing the word
    }
    entry = entry->next; //move to the next entry in the list if not matched
  }
  free(word); //free the lowercase copy if word was not found
  return NULL; //return null if no entry matched
}

bool doc_in_list(int doc_id, DocListNode *head) { //this function checks if a document with a specific ID exists in a linked list of documents. 
  while (head) {
    if (head->doc->id == doc_id) //check if the current document's id matches the target id
      return true;
    head = head->next;
  }
  return false;
}

void sortByRelevanceDescending(relevancesInQueryNODE *head) { //this function sorts a linked list of documents in descending order of relevance, using a simple bubble sort approach. 
  if (!head)
    return;
  bool swapped;
  do {
    swapped = false;
    relevancesInQueryNODE *curr = head;
    while (curr->next) {
      if (curr->docsFiltered->relevance < curr->next->docsFiltered->relevance) { //if current doc has less relevance than the next one, swap them
        Document *tmp = curr->docsFiltered;
        curr->docsFiltered = curr->next->docsFiltered;
        curr->next->docsFiltered = tmp;
        swapped = true;
      }
      curr = curr->next;
    }
  } while (swapped);
}

bool is_query_or_docs_invalid(Query *query_list, Documents *docs) {
  return (!docs || !docs->head || !query_list || !query_list->head);
}

int get_doc_lists_for_query_words(HashMap *map, Query *query_list,
                                  DocListNode **doc_lists, int max_words) { //this function takes a query (with keywords) and finds the list of documents for each word in the query using the hashmap. It stores each document list in an array. 
  int word_count = 0;
  QueryNode *current = query_list->head;

  while (current && word_count < max_words) {
    DocListNode *list = get_docs_for_word(map, current->name); //get the list of documents that contain the current word
    if (!list) //if no documents contain this word, return -1 (query cannot be fulfilled)
      return -1;
    doc_lists[word_count++] = list; //store the list in the array and increment the word counter
    current = current->next;
  }
  return word_count;
}

int find_smallest_doclist_index(DocListNode **doc_lists, int word_count) {
  int min_index = 0, min_size = INT_MAX;
  for (int i = 0; i < word_count; i++) {
    int size = 0;
    for (DocListNode *node = doc_lists[i]; node; node = node->next) // count the nodes in the current list
      size++;
    if (size < min_size) { //if this list is smaller than the previously smallest one update the smallest size
      min_size = size;
      min_index = i;
    }
  }
  return min_index;
}

relevancesInQueryNODE *
filter_docs_in_all_lists(DocListNode *min_list, DocListNode **include_lists,
                         int include_count, int min_index,
                         DocListNode **exclude_lists, int exclude_count,
                         int *found_count) {

  relevancesInQueryNODE *head = NULL;
  *found_count = 0;

  while (min_list) {
    bool in_all = true;

    //check if the document is in all the other inclusion lists
    for (int i = 0; i < include_count; i++) {
      if (i == min_index)
        continue; //skip the list we're iterating over
      if (!doc_in_list(min_list->doc->id, include_lists[i])) {
        in_all = false; //if not in one, skip it
        break;
      }
    }

    //exclude the document if it's in any of the exclusion lists
    if (in_all) {
      for (int i = 0; i < exclude_count; i++) {
        if (doc_in_list(min_list->doc->id, exclude_lists[i])) {
          in_all = false;
          break;
        }
      }
    }

    //if passed all filters, add it to the result list
    if (in_all) {
      // Add this doc to result list
      relevancesInQueryNODE *node = malloc(sizeof(relevancesInQueryNODE));
      node->docsFiltered = min_list->doc;
      node->previous = NULL;
      node->next = NULL;

      if (!head) {
        head = node; //if it's the first result, set head
      } else {
        relevancesInQueryNODE *last = head;
        while (last->next)
          last = last->next;
        last->next = node;
        node->previous = last;
      }
      (*found_count)++;
    }

    min_list = min_list->next;
  }

  return head;
}

void free_relevances_list(relevancesInQueryNODE *head) {
  while (head) {
    relevancesInQueryNODE *tmp = head;
    head = head->next;
    free(tmp);
  }
}

void print_top5_filtered_docs(relevancesInQueryNODE *head) {
  sortByRelevanceDescending(head); //sort documents by relevance (highest first)
  relevancesInQueryNODE *curr = head;
  for (int i = 0; i < 5 && curr; i++, curr = curr->next) {
    char *preview = preview_text(curr->docsFiltered->body, MAX_SUM_LENGTH);
    printf("(%d) %s\n---\n%s\n---\nRelevance score(%d)\n---\n\n",
           curr->docsFiltered->id, curr->docsFiltered->title, preview,
           curr->docsFiltered->relevance);
    free(preview); // free the memory
  }
}

void print_top5_docs(HashMap *map, Query *query_list, Documents *docs) { //parse the query into include/exclude words, find documents that match all include and no exclude words, sort them by relevance, print the top 5.
  if (is_query_or_docs_invalid(query_list, docs)) {
    printf("No documents loaded or no query provided.\n");
    return;
  }

  const int MAX_QUERY_WORDS = 100;
  DocListNode *doc_lists[MAX_QUERY_WORDS]; //temporary array to hold doc lists for query words
  int word_count = get_doc_lists_for_query_words(map, query_list, doc_lists,
                                                 MAX_QUERY_WORDS);

  if (word_count == -1) {
    printf("No documents contain all the words.\n");
    return;
  }

  DocListNode *include_lists[MAX_QUERY_WORDS];
  DocListNode *exclude_lists[MAX_QUERY_WORDS];
  int include_count = 0, exclude_count = 0;

  QueryNode *current = query_list->head;
  while (current) {
    DocListNode *list = get_docs_for_word(map, current->name); //retrieve docs for each word

    if (!list && !current->is_excluded) //if required word has no match
      return; // required word missing

    if (current->is_excluded) {
      exclude_lists[exclude_count++] = list; //collect exclusion lists
    } else {
      include_lists[include_count++] = list; //collect inclusion lists
    }

    current = current->next;
  }

  int min_index = find_smallest_doclist_index(include_lists, include_count);
  DocListNode *min_list = include_lists[min_index];
  int found = 0;

  relevancesInQueryNODE *head =
      filter_docs_in_all_lists(min_list, include_lists, include_count,
                               min_index, exclude_lists, exclude_count, &found); //filter by inclusion + exclusion

  if (!found) {
    printf("No documents contain all the words.\n");
  } else {
    print_top5_filtered_docs(head);
  }

  free_relevances_list(head);
}

void free_hashmap(HashMap *map) {
  for (int i = 0; i < HASH_SIZE; i++) {
    WordEntry *entry = map->table[i];
    while (entry) {
      WordEntry *next_entry = entry->next;

      // Free the word
      free(entry->word);

      // Free the linked list of documents
      DocListNode *doc_node = entry->docs_head;
      while (doc_node) {
        DocListNode *next_doc = doc_node->next;
        free(doc_node);
        doc_node = next_doc;
      }

      // Free the entry itself
      free(entry);

      entry = next_entry;
    }
    map->table[i] = NULL; // Just to be safe
  }
}

