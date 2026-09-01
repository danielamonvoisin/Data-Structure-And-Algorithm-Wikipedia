#include "../src/document.h"
#include "../src/graphs.h"
#include "../src/hashmap.h"
#include "../src/query.h"
#include "../src/record_search.h"
#include "utils.h"

// lab 1 tests:

// test to verify that a document is correctly parsed and initialized.
void test_document_parsing() {
  runningtest("test_document_parsing");

  // create a new document struct with initial zeroed fields.
  Document doc = {0};

  // assign values to the document fields like a student filling a form.
  doc.id = 1;  // student id = 1.
  doc.title = strdup("test document");  // student title: "test document".
  doc.body = strdup("this is the body.");  // student writes body text.
  doc.relevance = 5;  // student relevance score = 5.
  doc.linkCount = 0;  // student has no links yet.
  doc.list_of_links = NULL;  // no linked documents assigned to student.

  // check if the student's id is correctly assigned.
  assertEqualsInt(doc.id, 1);

  // verify that the student's title matches what we assigned.
  assert(strcmp(doc.title, "test document") == 0);

  // confirm the body text the student wrote matches exactly.
  assert(strcmp(doc.body, "this is the body.") == 0);

  // check that the student's relevance score is set correctly.
  assertEqualsInt(doc.relevance, 5);

  // ensure that the student has no links associated.
  assertEqualsInt(doc.linkCount, 0);

  // validate that the list of links for the student is empty.
  assert(doc.list_of_links == NULL);

  // free the dynamically allocated memory used by the student's title and body.
  free(doc.title);
  free(doc.body);

  successtest();  // indicate this student test succeeded.
}


// test 1 on documents linked list.
void test_add_single_document() {
  runningtest("test_add_single_document");

  // allocate memory for documents list and initialize.
  Documents *docs = (Documents *)malloc(sizeof(Documents));
  docs->head = NULL;
  docs->count = 0;

  // create a single document like a student adding one item.
  Document *doc = (Document *)malloc(sizeof(Document));
  doc->id = 42;
  doc->title = strdup("single doc");
  doc->body = strdup("no links here.");
  doc->relevance = 10;
  doc->linkCount = 0;
  doc->list_of_links = NULL;

  // create a node for the document and add it to the documents list.
  DocumentNode *node = CreateDocumentNode(doc);
  docs->head = node;
  docs->count++;

  // check that the documents list is not empty and has one document.
  assert(docs->head != NULL);
  assertEqualsInt(docs->count, 1);
  assertEqualsInt(docs->head->doc->id, 42);
  assert(strcmp(docs->head->doc->title, "single doc") == 0);

  // cleanup all allocated memory.
  FreeDocuments(docs);
  successtest();
}


// test 2 on documents linked list.
void test_add_multiple_documents() {
  runningtest("test_add_multiple_documents");

  // allocate memory for documents list and initialize.
  Documents *docs = (Documents *)malloc(sizeof(Documents));
  docs->head = NULL;
  docs->count = 0;

  // create first document with student-like details.
  Document *doc1 = (Document *)malloc(sizeof(Document));
  doc1->id = 1;
  doc1->title = strdup("doc 1");
  doc1->body = strdup("body 1");
  doc1->relevance = 5;
  doc1->list_of_links = NULL;

  // create second document.
  Document *doc2 = (Document *)malloc(sizeof(Document));
  doc2->id = 2;
  doc2->title = strdup("doc 2");
  doc2->body = strdup("body 2");
  doc2->relevance = 7;
  doc2->list_of_links = NULL;

  // create nodes for documents and link them as a student linking notes.
  DocumentNode *node1 = CreateDocumentNode(doc1);
  DocumentNode *node2 = CreateDocumentNode(doc2);
  node1->next = node2;
  node2->previous = node1;

  // assign head and count to documents list.
  docs->head = node1;
  docs->count = 2;

  // check the linked list structure correctness.
  assert(docs->head != NULL);
  assertEqualsInt(docs->head->doc->id, 1);
  assert(docs->head->next != NULL);
  assertEqualsInt(docs->head->next->doc->id, 2);
  assert(docs->head->next->previous == docs->head);

  // free all allocated documents and nodes.
  FreeDocuments(docs);
  successtest();
}


// test 3 on documents linked list.
void test_get_document_by_id() {
  runningtest("test_get_document_by_id");

  // allocate documents list with one student document.
  Documents *docs = (Documents *)malloc(sizeof(Documents));
  docs->head = NULL;
  docs->count = 0;

  // create a document with id 99 to simulate student search.
  Document *doc = (Document *)malloc(sizeof(Document));
  doc->id = 99;
  doc->title = strdup("target doc");
  doc->body = strdup("find me!");
  doc->relevance = 3;
  doc->list_of_links = NULL;

  // add document node to documents list.
  DocumentNode *node = CreateDocumentNode(doc);
  docs->head = node;
  docs->count = 1;

  // test if we can find the document by id correctly.
  Document *found = get_document_by_id(docs, 99);
  assert(found != NULL);
  assert(strcmp(found->title, "target doc") == 0);

  // test searching for a document that does not exist.
  Document *not_found = get_document_by_id(docs, 1000);
  assert(not_found == NULL);

  // free all allocated memory.
  FreeDocuments(docs);
  successtest();
}


// test 1 on links linked list.
void test_add_single_link() {
  runningtest("test_add_single_link");

  // start with an empty links list like a student starting fresh.
  LinkNode *head = NULL;

  // create a link and add it to the list.
  Link link = {42, strdup("link to 42")};
  AddLinkNode(&head, link);

  // check if the link was added correctly.
  assert(head != NULL);
  assertEqualsInt(head->data.ID, 42);

  // free memory allocated for link text and node.
  free(head->data.text);
  free(head);
  successtest();
}


// test 2 on links linked list.
void test_add_multiple_links() {
  runningtest("test_add_multiple_links");

  // start empty link list.
  LinkNode *head = NULL;

  // add multiple links like a student adding multiple notes.
  AddLinkNode(&head, (Link){1, strdup("first")});
  AddLinkNode(&head, (Link){2, strdup("second")});
  AddLinkNode(&head, (Link){3, strdup("third")});

  // iterate through list checking if all links are correctly added.
  LinkNode *current = head;
  int expected = 1;
  while (current) {
    assertEqualsInt(current->data.ID, expected);
    expected++;
    LinkNode *next = current->next;
    free(current->data.text);
    free(current);
    current = next;
  }

  successtest();
}


// test 3 on links linked list.
void test_link_list_empty() {
  runningtest("test_link_list_empty");

  // start with empty link list, no links yet.
  LinkNode *head = NULL;
  assert(head == NULL); // the list is empty.

  // add one link and check.
  AddLinkNode(&head, (Link){5, strdup("only one")});
  assert(head != NULL);
  assertEqualsInt(head->data.ID, 5);

  // free allocated memory.
  free(head->data.text);
  free(head);
  successtest();
}


// lab 2 tests:


// test 1 on query linked list.
void test_query_parsing_multiple_words() {
  // this test will check if the query parser correctly splits a string into
  // a linked list of words like a student splitting a sentence into words.

  runningtest("test_query_parsing_multiple_words");
  {
    Query *query = create_string_list("domestic dog cat");

    // verify the query is not null and words are in correct order.
    assert(query != NULL);
    assertEqualsInt(strcmp(query->head->name, "domestic"), 0);
    assertEqualsInt(strcmp(query->head->next->name, "dog"), 0);
    assertEqualsInt(strcmp(query->head->next->next->name, "cat"), 0);
    assertNull(query->head->next->next->next);

    free_query_list(query);
  }
  successtest();
}




// test 2 on query linked list.
void test_query_parsing() {
  // this test checks that none of the nodes or names are null, so strcmp won't crash.
  // a simpler version of the multiple-word query test.

  runningtest("test_query_parsing");
  {
    Query *query = create_string_list("domestic dog"); // input string.

    // check that first and second words are correct and not null.
    assert(query != NULL);
    assert(query->head != NULL);
    assert(query->head->name != NULL);
    assertEqualsInt(strcmp(query->head->name, "domestic"), 0);

    assert(query->head->next != NULL);
    assert(query->head->next->name != NULL);
    assertEqualsInt(strcmp(query->head->next->name, "dog"), 0);

    // check no extra words.
    assert(query->head->next->next == NULL);

    free_query_list(query);
  }
  successtest();
}


// test 3 on query linked list.
void test_empty_query() {
  runningtest("test_empty_query");

  Query *query = create_string_list("");
  assert(query == NULL);

  Query *query2 = create_string_list(NULL);
  assert(query2 == NULL);

  successtest();
}




//  test 1 for the linear search functionality
void test_document_matches_simple() {
  runningtest("test_document_matches_simple");

  // Allocate a new Document and fill in sample data.
  Document *doc = malloc(sizeof(Document));
  doc->id = 1;
  doc->title = strdup("Animal Study");                   // Title: Animal Study
  doc->body = strdup("The domestic dog is a common pet."); // Body contains "dog"

  // Create a Query linked list from the string "dog".
  Query *query = create_string_list("dog");

  // Assert that document_matches_query finds the word "dog" in the document body.
  assert(document_matches_query(doc, query));  // Expected: true

  // Free allocated strings and structs to avoid memory leaks.
  free(doc->title);
  free(doc->body);
  free(doc);
  free_query_list(query);

  successtest();  // Mark test as successful.
}



//  test 2 for the linear search functionality
void test_document_does_not_match() {
  runningtest("test_document_does_not_match");

  // allocate Document with body that does NOT contain the word "dog".
  Document *doc = malloc(sizeof(Document));
  doc->id = 2;
  doc->title = strdup("Tech News");
  doc->body = strdup("This article talks about artificial intelligence.");

  // here we create a Query with the word "dog" which is NOT in the document body.
  Query *query = create_string_list("dog");

  // Assert that document_matches_query returns false (no match).
  assert(!document_matches_query(doc, query));  // Expected: false

  // and then free allocated memory.
  free(doc->title);
  free(doc->body);
  free(doc);
  free_query_list(query);

  successtest();
}





// LAB 3 TEST:



void test_hashmap_index_and_lookup() {
  // This test skips using a full document and directly tests the function
  // `add_word_to_index`.

  runningtest("test_hashmap_index_and_lookup");
  {
    HashMap *map = init_hashmap(1009); // small prime number, good for testing
    Document doc;
    doc.id = 123;
    doc.title = "Test Title";
    doc.body = "Body content";
    doc.relevance = 10;
    // We create a doc with ID 123 and add the word "socialist"
    // to the hashmap manually using that function.

    add_word_to_index(map, "socialist", &doc);

    DocListNode *list = get_docs_for_word(map, "socialist");
    assertEqualsInt(list->doc->id, 123); // Then we check that if we look up
    // "socialist" it supposedly have to give us that document and no other
    // results (list->next == NULL).
    assertNull(list->next);

    free_hashmap(map);
  }
  successtest();
}




void test_hashmap_add_and_get() { // this test wil check if our indexing system
                                  // (basucally the hashmap) can
  // store a document correctly, and retrieve it later using the words in its
  // body. For example we have created a document with ID 99 and body "dog cat".
  // Then we call index_document, which is supposed to add each word from the
  // body into the hashmap. Later, we try to retrieve the list of documents for
  // "dog" and "cat" and then make sure they include our document  which it must
  // be ID 99.
  runningtest("test_hashmap_add_and_get");
  {

    HashMap *map = init_hashmap(1009); // small prime number, good for testing

    Document doc;
    doc.id = 99;
    doc.title = "Hashmap test";
    doc.body = "dog cat";
    doc.relevance = 10;

    index_document(map, &doc);

    DocListNode *dog_list = get_docs_for_word(map, "dog");
    DocListNode *cat_list = get_docs_for_word(map, "cat");

    assert(dog_list != NULL);
    assert(cat_list != NULL);
    assertEqualsInt(dog_list->doc->id, 99);
    assertEqualsInt(cat_list->doc->id, 99);

    free_hashmap(map);
  }
  successtest();
}



void test_breed_word_in_doc_9() {
  // This test is about verifying that the word "breed" gets indexed properly.
  runningtest("test_breed_word_in_doc_9");
  {
    HashMap *map = init_hashmap(1009); // small prime number, good for testing
    // we create a document with ID 9 and a body that includes the word "breed".
    Document doc;
    doc.id = 9;
    doc.title = "Test doc";
    doc.body = "The breed of this animal is unknown";
    doc.relevance = 5;

    // call the function index document, to correctly index it.
    index_document(map, &doc);

    // then we look up the word "breed" in the hashmap and check that one of the
    // results is our document (ID 9).
    DocListNode *list = get_docs_for_word(map, "breed");

    // we walk through the list to see if doc ID 9 is there
    bool found = false;
    while (list) {
      if (list->doc->id == 9) {
        found = true;
        break;
      }
      list = list->next;
    }

    assertEqualsInt(found, 1); // here it should find it,

    free_hashmap(map); // free the memory
  }
  successtest();
}



// LAB 4 TEST:

void test_graph_links() {
  // Here we are trying to build a Graph using two documents where doc0 has a
  // link to doc1 ( so they are adjacent), then it builds the graph and checks
  // if the adjacency matrix reflects that doc0 links to doc1 (so matrix[0][1]
  // should be 1), and that doc1 does not link back to doc0 ( so matrix[1][0] =
  // 0).

  runningtest("test_graph_links");
  {
    Documents *docs = malloc(sizeof(Documents));
    docs->count = 2;
    docs->head = NULL;

    Document *doc1 = malloc(sizeof(Document));
    doc1->id = 0;
    doc1->title = "Doc 0";
    doc1->body = "hello world";
    doc1->relevance = 3;
    doc1->linkCount = 1;
    doc1->list_of_links = malloc(sizeof(LinkNode));
    doc1->list_of_links->data.ID = 1;
    doc1->list_of_links->next = NULL;

    Document *doc2 = malloc(sizeof(Document));
    doc2->id = 1;
    doc2->title = "Doc 1";
    doc2->body = "linking back";
    doc2->relevance = 4;
    doc2->linkCount = 0;
    doc2->list_of_links = NULL;

    docs->head = malloc(sizeof(DocumentNode));
    docs->head->doc = doc1;
    docs->head->next = malloc(sizeof(DocumentNode));
    docs->head->next->doc = doc2;
    docs->head->next->next = NULL;

    DocumentGraph *graph = create_graph(2);
    completing_graph(docs, graph);

    assertEqualsInt(graph->matrix[0][1], 1); // doc0 → doc1 (they are linked)
    assertEqualsInt(graph->matrix[1][0],
                    0); // doc1 → doc0( they are not linked)

    free_graph(graph);
    FreeDocuments(docs);
  }
  successtest();
}



void test_graph_link_and_relevance_update() {
  runningtest("test_graph_link_and_relevance_update");

  // Create fake documents
  Document docA = {.id = 0,
                   .title = "A",
                   .body = "test",
                   .relevance = 0,
                   .list_of_links = NULL};
  Document docB = {.id = 1,
                   .title = "B",
                   .body = "test",
                   .relevance = 0,
                   .list_of_links = NULL};

  // tests that a graph is correctly updated when one document (A) links to
  // another (B). That the relevance score of B increases when it is linked to.
  LinkNode *linkAB = malloc(sizeof(LinkNode));
  linkAB->data.ID = 1;
  linkAB->next = NULL;
  docA.list_of_links = linkAB;

  // It creates two dummy documents, links A → B, and builds the graph. Then it
  // checks that the graph reflects the link and that docB’s relevance went from
  // 0 to 1.
  DocumentNode *nodeA = malloc(sizeof(DocumentNode));
  DocumentNode *nodeB = malloc(sizeof(DocumentNode));
  nodeA->doc = &docA;
  nodeB->doc = &docB;
  nodeA->next = nodeB;
  nodeB->next = NULL;

  Documents docs; // we will create an instance of a Documents structure.
  docs.head = nodeA;
  docs.count = 2;

  // we have to create indeed and complete graph
  DocumentGraph *graph = create_graph(2);
  completing_graph(&docs, graph);

  // we test that A links to B (matrix[0][1] == 1)
  assertEqualsInt(graph->matrix[0][1], 1);

  // we test that the relevance of B was incremented
  assertEqualsInt(docB.relevance, 1);

  // we will have to free all the memory dinamically aloocated:
  free(linkAB);
  free(nodeA);
  free(nodeB);
  free_graph(graph);

  successtest();
}

void eda_engine_test() {
  running("eda_engine_test");
  {
    void test_document_parsing();
    void test_add_single_document(); 
    void test_add_multiple_documents(); 
    void test_get_document_by_id(); 
    void test_add_single_link(); 
    void test_add_multiple_links(); 
    void test_link_list_empty(); 
    void test_query_parsing_multiple_words(); 
    void test_query_parsing(); 
    void test_empty_query(); 
    void test_document_matches_simple(); 
    void test_document_does_not_match(); 
    void test_hashmap_index_and_lookup(); 
    void test_hashmap_add_and_get(); 
    void test_breed_word_in_doc_9(); 
    void test_graph_links(); 
    void test_graph_link_and_relevance_update(); 
  }
  success();
}