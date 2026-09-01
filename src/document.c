#include "document.h"

// this function creates a new node to store a Link in a linked list
LinkNode *CreateLinkNode(Link link) {
  LinkNode *node = (LinkNode *)malloc(sizeof(
      LinkNode));    // we allocate memory for a new LinkNode, this means the
                     // memory stays active until we explicitly free it later
  node->data = link; // we copy the Link data into the node, so the node now
                     // contains the Link (ID and text)
  node->next =
      NULL; // since this is a new node, we set its next pointer to NULL, this
            // means it's currently not linked to any other node
  return node; // finally we return a pointer to the new node so we can use it
               // elsewhere
}

// this function adds a new Link to the end of a linked list, the list starts at
// *head, and we insert the new Link at the end
void AddLinkNode(LinkNode **head, Link link) {
  LinkNode *newNode = CreateLinkNode(
      link); // first we create a new node using the link data we received
  if (*head == NULL) { // if the list is empty (the head is NULL), this is the
                       // first element
    *head = newNode; // so we set the head of the list to point to this new node
  } else {           // otherwise, we need to find the end of the list
    LinkNode *current = *head; // we start at the head and keep moving forward
                               // usinf the "next" pointer
    while (current->next != NULL)
      current = current->next; // move to the next node
    current->next =
        newNode; // now the current points to the last node in the list, we
                 // connect the new node to the end of the list
  }
}

// this function reads a document from a file located at the given path and
// converts the file content into a Document structure in memory
Document *document_deserialize(char *path) {
  FILE *f = fopen(path, "r"); // opens the file in read mode ("r")
  assert(f != NULL); // make sure the file was opened correctly, if it wasn't,
                     // the program will crash here with an error

  Document *document = (Document *)malloc(sizeof(
      Document)); // allocat ememory for a new Document structure on the heap,
                  // this means we can use it throughout the program
  document->linkCount =
      0; // initialize the number of links in the document to 0, we will
         // increase it each time we find a new link
  document->relevance =
      0; // initialize the relevance score to 0, this will be updated later

  document->list_of_links =
      NULL; // set the list_of_links pointer to NULL at the beginning, we'll
            // build up the list later as we read the file

  char buffer[262144]; // buffer of 256 KB (256*1024 = 262144 bytes) to safely
                       // hold large document content like full articles
  int bufferIdx = 0; // this will keep track of where we are in the buffer when
                     // adding characters
  char ch;           // a single character variable used to read from the file

  // read the documnet ID (reads first line)
  while ((ch = fgetc(f)) !=
         '\n')                // keep reading characters until we find a newline
    buffer[bufferIdx++] = ch; // add each character to the buffer
  buffer[bufferIdx] = '\0';   // add a null character to mark the end of the
                              // string
  document->id = atoi(buffer); // convert the buffer string to an integer and
                               // assign it to documnet ID

  // read the documnet title (reads the second line)
  bufferIdx = 0;                  // reset buffer to reuse it
  while ((ch = fgetc(f)) != '\n') // read characters until end of the title line
    buffer[bufferIdx++] = ch;
  buffer[bufferIdx] = '\0'; // null-terminate the string
  document->title = strdup(
      buffer); // copy the buffer content into memory and assign it as the title

  // read body and extract links(getting ready to process the rest of the file)
  char linkBuffer[64];      // buffer to store the digits of a link ID
  int linkBufferIdx = 0;    // index for link buffer
  bool parsingLink = false; // flag to know if we're inside a link

  bufferIdx = 0;                   // reuse buffer to build the body text
  while ((ch = fgetc(f)) != EOF) { // read characters until the end of file
    buffer[bufferIdx++] = ch;      // add each character to the body buffer

    if (parsingLink) { // we're currently inside a (...) part after a link text
      if (ch == ')') { // if link ends
        parsingLink = false;
        linkBuffer[linkBufferIdx] = '\0'; // null-terminate the link ID string
        int linkId = atoi(linkBuffer);    // convert string to integer
        Link link = {linkId, NULL}; // create a new Link object (text is NULL)
        AddLinkNode(&document->list_of_links,
                    link);                // add the link to the document's list
        document->linkCount++;            // increase the count of links
        linkBufferIdx = 0;                // reset buffer for next link
      } else if (ch != '(') {             // if it's not the opening parenthesis
        linkBuffer[linkBufferIdx++] = ch; // add the character to the link ID
      }
    } else if (ch == ']') { // if we find the end of a [text] part, the next
                            // part is (linkID)
      parsingLink = true;
    }
  }
  buffer[bufferIdx] = '\0'; // null-terminate the body buffer
  document->body = strdup(
      buffer); // duplicate the buffer into heap memory for the document body

  fclose(f);       // we're done reading the file, so we close it
  return document; // a structure filled with the ID, title, body text, linked
                   // list of links...
}

// function to create a new node document
DocumentNode *CreateDocumentNode(Document *doc) {
  DocumentNode *node = (DocumentNode *)malloc(
      sizeof(DocumentNode)); // to allocate memory for a new DocumentNode
  node->doc = doc;       // here we just have to assign the document to the node
  node->next = NULL;     // and set the next pointer to NULL
  node->previous = NULL; // and set the previous pointer to NULL
  return node;
}

Documents *
LoadAllDocument(char *folderPath) { // this function will basically be for
                                    // loading all documents into a linked list
  // we first have to initiate the string that will contain the path to the
  // index.txt file that contains the number of documents
  char listFilePath[512];
  sprintf(listFilePath, "%s/index.txt",
          folderPath); // This just builds the full path string to a file called
                       // index.txt inside a folder.

  // once we have the open the file that contains the number of documents
  FILE *f = fopen(listFilePath, "r");
  assert(f != NULL); // Ensure the file was opened successfully

  // we will use the fscanf function to read the index number that the
  // folderpath has, which will tell us how may iterations the function has to
  // do, for example if we input as an argument "datasets/wikipedia12", %d = 12.
  int count;
  fscanf(f, "%d", &count);
  fclose(f); // Close the index.txt file

  // Then with the count variable setted we will allocate memory for the
  // structure that will store the documents
  Documents *docs = (Documents *)malloc(sizeof(Documents));
  docs->head = NULL; // And ofcourse we have to initialize the list as empty
  docs->count = 0;   // Same, initialize the document count

  // We will have to set the pointer to the last node in the list, to make it
  // clear, tail is a pointer to a pointer to a node and the idea is that tail
  // will keep track of the last node's next pointer, so we can add new nodes to
  // the end of the list efficiently.
  DocumentNode **tail = &docs->head;
  DocumentNode *prev = NULL;

  // ONce we have all setted we will iterate over the number of documents
  // founded befores.
  for (int i = 0; i < count; i++) {
    // Again we have to build the path to each document's file (the ones inside
    // the wikipedia folder).
    char docPath[512];
    sprintf(docPath, "%s/%d.txt", folderPath, i);

    // here we will call the function Deserialize the document from the file we
    // have loaded its path.
    Document *doc = document_deserialize(docPath);

    // so then we will create a new node for the document that we ave just
    // created.
    DocumentNode *node = CreateDocumentNode(doc);

    // and finally we have to add the node to the end of the list without the
    // necessity of traversing the list because we already setted a pointer to a
    // pointer of the last’s next node.

    node->previous = prev;

    *tail =
        node; // Initially, tail points to docs->head (which is NULL), then when
              // we add the first node: *tail = node sets docs->head = node
    tail = &node->next; // we update tail to point to node->next, and move the
                        // pointer to the next node, so that it points to the
                        // new last node.
    prev = node;
    docs->count++; // increment the document count
  }

  return docs; // finally return the list of documents
}

// Of Course we have to free memory of the documents
void FreeDocuments(Documents *docs) {
  DocumentNode *current = docs->head;

  // we will iterate through the list of documents already built.
  while (current) {
    Document *doc = current->doc;

    // then here we will free the memory of the document data (title and body)
    free(doc->title);
    free(doc->body);

    // and also free the memory of the list of links
    LinkNode *link = doc->list_of_links;
    while (link) {
      LinkNode *next = link->next; // Save the next link
      free(link);                  // Free the link node
      link = next;                 // Move to the next link
    }

    // then we also have to free the memory of the document itself
    free(doc);

    // and free the memory of the document node
    DocumentNode *nextDoc = current->next;
    free(current);
    current = nextDoc;
  }
  free(docs);
}

Document *get_document_by_id(Documents *docs, int target_id) {
  if (!docs || !docs->head)
    return NULL;

  DocumentNode *current = docs->head;
  while (current) {
    if (current->doc->id == target_id) {
      return current->doc;
    }
    current = current->next;
  }

  return NULL; // Not found
}

// Finally, free the memory of the documents structure

// lab 2
