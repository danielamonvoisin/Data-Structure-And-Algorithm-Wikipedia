# Report: Building a search engine like Google

> [!NOTE]  
> Complete the report in this file before delivering the project.
> The lab guide has a list of the questions you must answer.


## 1. C4 Component Diagram – Introduction

![diagram](https://github.com/user-attachments/assets/b9fbb975-4c97-43d7-928f-fa2e054b202e)

In our system, the input documents are considered **persistent components**, as they are read from disk and remain unchanged after program execution. On the other hand, the data structures we use for searching—such as the **reverse index (hashmap)**, the **document graph**, and the **recent search queue**—are **volatile**, as they reside in memory and are freed when the program exits.

---

## 2. Runtime Complexity Table

| Description                                                           | Big-O      | Justification                                                                                                                                         |
|----------------------------------------------------------------------|------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| Parsing a document into the struct (including adding links to the list) | O(L)       | Each document of length L is parsed character by character, and each link is added to the list in constant time per link.                            |
| Parsing a query into the struct                                      | O(Q)       | Each query of length Q is parsed word by word, storing each keyword in the struct, with constant time per keyword.                                   |
| Counting the neighbours in the graph                                 | O(N + E)   | Counting all neighbours in the graph requires visiting each node (N) and each edge (E) once.                                                          |
| Counting the neighbours of a document in the graph                   | O(D)       | For a single document, counting neighbours involves traversing its adjacency list, which has D neighbours.                                           |
| Finding the documents that contain a keyword in the reverse-index    | O(1 + K)   | Lookup in the hashmap is O(1), and iterating over K documents containing the keyword is O(K).                                                        |
| Finding the documents that match all keywords in the query           | O(M × Q)   | For Q keywords, intersecting the lists of matching documents (each of size up to M) takes O(M × Q) in the worst case.                                |
| Sorting the documents according to the relevance score               | O(D log D) | Sorting D documents by relevance uses a comparison-based sort, which is O(D log D).                                                                  |


## 3. Search Time: With vs. Without Reverse Index


It clearly shows how the reverse index keeps search time almost constant, while the traditional approach becomes much slower as the dataset grows.

![Captura de pantalla 2025-05-30 220532](https://github.com/user-attachments/assets/c7ccefab-97a6-401d-974d-699419a7b4b9)

---

## 4. Initialization Time for Varying Hashmap Slot Counts


As the number of slots increases, the initialization time gets longer. That’s because the system has to allocate more memory. Each new slot in the hashmap takes time to set up. The difference is small (0.01–0.02 seconds), but it adds up when the slot count doubles.

![Captura de pantalla 2025-05-30 220319](https://github.com/user-attachments/assets/36b21f1e-a5c8-4ef1-9584-b819f8e561e3)

---

## 5. Search Time for Varying Hashmap Slot Counts


We measured how long it takes to search in a hashmap (a data structure used to store words and their matching documents) using different sizes for the hashmap.

Bigger hashmaps make searches faster, especially when the size is small at the beginning. But after a certain point, making it larger doesn’t improve things much, and it might just waste memory.

![Captura de pantalla 2025-05-30 214529](https://github.com/user-attachments/assets/be8923f3-fb4b-461e-97f0-b6e0afa12a31)

---

## 6. Suggested Improvement to the Reverse Index

One possible way to improve the reverse index would be to replace the hash map with a **trie** (also known as a prefix tree). A trie lets you store words by their characters, and it’s especially useful if many of the words in the documents share common beginnings, like `"cat"`, `"cats"`, `"catwalk"`, etc.

With this change, prefix-based searches (for example, searching for all words that start with `"cat"`) could be done much faster.

However, this structure uses more memory than a hash map, because it needs to keep a node for every character in every word.




