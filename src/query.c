#include "query.h"
#include "hashmap.h"

QueryNode *createNode(char *word) { // basic function to create a Node (the
                                    // keyword) in our linked list (query).
  QueryNode *my_word = (QueryNode *)malloc(
      sizeof(QueryNode)); // as always we have to allocate space for the node
                          // (of type QueryNode) in the heap.
  my_word->name = strdup(word); // update the pointer of this new node, copying
                                // (= allocating space in the memory heap) the
                                // string entered by the user (the keyword).
  my_word->next = NULL; // set the next pointer to NULL because it will be the
                        // last node in the linked list every time we create a
                        // new node (like enqueue at the end).
  return my_word; // and return this new node with its updated information.
}
Query *create_string_list(char *input_string) {
  Query *my_query = malloc(sizeof(Query));
  my_query->head = NULL;
  my_query->count = 0;

  // here we will have to make a mutable copy of the input string
  char *input_copy = strdup(input_string);
  if (input_copy == NULL)
    return NULL;

  char *token = strtok(input_copy, " ");
  QueryNode **tail = &my_query->head;

  while (token != NULL) {
    QueryNode *node = malloc(sizeof(QueryNode));
    node->is_excluded = false;

    // Detect -excluded
    if (token[0] == '-') {
      node->is_excluded = true;
      token++; // skip the '-' sign
    }

    node->name = strdup(token); // copy the word
    node->next = NULL;

    *tail = node;
    tail = &node->next;
    my_query->count++;

    token = strtok(NULL, " ");
  }

  free(input_copy); // clean up the space of the copy (because it was addes with
                    // strdup
  return my_query;
}

void FreeQuery(
    Query *query) { // this function will help us to free the memory allocated
                    // for the linked list of the keywords entered by the user.
  QueryNode *current = query->head; // create a temporal pointer to the head of
                                    // the linked list (the first node).
  while (current) {
    QueryNode *next = current->next; // Save the next link
    free(current->name);             // Free the link node
    free(current);                   // Free the link node
    current = next;                  // Move to the next link
  }
  free(query); // Finally, free the memory of the documents structure
}

// LAB2: funtions to search the strings entered by the user.

char *to_lowercase(
    const char *str) { // this function will help us to convert to lowercase the
                       // string entered by the user and the body text obtained
                       // by the parsing function.
  char *lower = strdup(str); // we will allocate space (in the memory heap) and
                             // copy string because Trying to change str[i]
                             // directly would result in undefined behavior,
  for (int i = 0; lower[i]; i++) { // we will iterate over the string (its
                                   // characters) to convert it to lowercase.
    lower[i] =
        tolower(lower[i]); // apply the tolower function to each character.
  }
  return lower; // return the lowercase string.
}

bool contains_keyword(
    char *text,
    char *keyword) { // this function will help us check if the keyword is
                     // actually in the text or not (TRUE or FALSE).
  char *lower_text =
      to_lowercase(text); // firstly convert the text (the pointer that points
                          // to the body of the document) to lowercase.
  char *lower_keyword = to_lowercase(
      keyword); // firstly convert the keyword (the pointer that points to the
                // word entered by the user) to lowercase.

  bool found =
      strstr(lower_text, lower_keyword) !=
      NULL; // here then we will check if the keyword is in the text or not.

  free(lower_text); // since "strdup" allocates new memory on the heap, its like
                    // mandatory for us to free the memory allocated for the
                    // lowercase text.
  free(lower_keyword); // same thing, free the memory allocated for the
                       // lowercase keyword.
  return found; // return the result of the search, if the keyword is in the
                // text, return TRUE, otherwise return FALSE.
}

bool document_matches_query(
    Document *doc, Query *query) { // this fucntion will help us to check if the
                                   // document matches the query or not.
  QueryNode *current =
      query->head; // because our goal is to iterate over the linked list (the
                   // query, the one that contains the words entered by the
                   // user) and see what documents have the keyword.
  while (current !=
         NULL) { // so we will iterate until we have reached the end of the
                 // linked list, where the current node will be NULL.
    if (!contains_keyword(
            doc->body,
            current->name)) { // so basically it will consist on checking i the
                              // keyword is in the body of the docuemnt or not.
      return false; // if its not on the document we will just return false to
                    // finish this loop and move on to the next document.
    }
    current = current->next; // if the keyword is in the document, we will move
                             // to the next node of the linked list (the next
                             // word entered by the user).
  }
  return true; // if the document reaches to the end of the linked list, it
               // means that all the keywords are in the document, so we will
               // return true.
}

void search_documents(Documents *docs,
                      Query *query) { // this function will help us to search
                                      // the documents that match the query.
  DocumentNode *current =
      docs->head; // we have to create a pointer to the head of the linked list
                  // (the documents) to iterate over it.
  int found = 0;  // we have to ste the count of how many document where founded
                  // because later in the result count will be useful.

  while (current != NULL &&
         found < 5) { // because the maximum number of document to be printed is
                      // 5, we will iterate until we reach the end of the linked
                      // list or we have printed 5 documents.
    if (document_matches_query(
            current->doc, query)) { // we will extract the body of the current
                                    // document with its corresponding pointer
                                    // and check if it matches the query.

      char *preview = preview_text(current->doc->body, MAX_SUM_LENGTH);
      printf("(%d) %s\n---\n%s\n---\n\n", current->doc->id, current->doc->title,
             preview); // so if it was true we will just
                       // print the document with its ID,
                       // title and part of the body.
      free(preview);   // free the memory allocated for the preview.
      found++;         // increment the count of documents found.
    }
    current = current->next; // move to the next document in the linked list of
                             // the parsed documents.
  }
  printf("[%d "
         "results]\n-----------------------------------------------------------"
         "-\n\n",
         found);

  if (found ==
      0) { // if no document was found, we will print a message to the user.
    printf("No matching documents found.\n");
  }
}

char *
preview_text(const char *text,
             size_t max_length) { // this function will help us to preview the
                                  // text of the document, it will print a part
                                  // of the body of the document.
  if (strlen(text) <=
      max_length) { // Boundary condition --> if the length of the text is less
                    // than or equal to the maximum length, we will just return
                    // the text.
    return strdup(text); // there will be no need to summarize it.
  }

  char *summary = (char *)malloc(
      (max_length + 4) *
      sizeof(char)); // because we want to return the summary string, we will
                     // have to allocate memory on the heap for the summary, for
                     // it to be returned safely, and we will add 3 to the
                     // maximum length to add the ellipsis.
  if (!summary) {
    fprintf(stderr,
            "Memory allocation failed\n"); // this is just if there was an error
                                           // allocating memory space.
    return NULL;
  }

  strncpy(summary, text,
          max_length + 3); // Copy the first max_length characters from the text
                           // to the summary.
  summary[max_length] =
      '.'; // append manually the "..." to the end of the summary.
  summary[max_length + 1] =
      '.'; // append manually the "..." to the end of the summary.
  summary[max_length + 2] =
      '.'; // append manually the "..." to the end of the summary.
  summary[max_length + 3] =
      '\0'; // finally the null-terminate the string added.

  return summary;
}

void free_query_list(Query *query) {
  if (!query)
    return;

  QueryNode *current = query->head;
  while (current) {
    QueryNode *next = current->next;
    free(current->name); // Free the string in the node
    free(current);
    current = next;
  }

  free(query);
}
