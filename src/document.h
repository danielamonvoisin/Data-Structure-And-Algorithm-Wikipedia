#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a link
// Each link has an ID and a text
// The ID is used to identify the link, and the text is the actual content of
// the link
typedef struct {
  int ID; // the one I contain
  char *text;
} Link;

// Structure to represent a node in a linked list of links
// Each node contains a Link and a pointer to the next node
typedef struct LinkNode {
  Link data;
  struct LinkNode *next;
} LinkNode;

// Structure to represent a document
// Each document has a title, an ID, a list of links (as a linked list),
// relevance score, and body text
typedef struct {
  char *title;
  int id;
  LinkNode *list_of_links; // Linked list of links
  // Each document can have multiple links
  // The list_of_links is a pointer to the head of the linked list
  int relevance;
  char *body;
  int linkCount;
} Document;

// Structure to represent a node in a linked list of documents
// Each node contains a Document and a pointer to the next node
typedef struct DocumentNode {
  Document *doc;
  struct DocumentNode *next;
  struct DocumentNode *previous;
} DocumentNode;

// Structure to represent a linked list of documents
// Each list contains a pointer to the head of the linked list and a count of
// the number of documents
typedef struct {
  DocumentNode *head;
  int count;
} Documents;

// Function prototypes
// These functions are used to deserialize, load, and free documents and their
// linked lists
Document *document_deserialize(char *path);
Documents *LoadAllDocument(char *folderPath);
void FreeDocuments(Documents *docs);
Document *get_document_by_id(Documents *docs, int target_id);
void AddLinkNode(LinkNode **head, Link link); 
LinkNode *CreateLinkNode(Link link); 
DocumentNode *CreateDocumentNode(Document *doc); 



#endif