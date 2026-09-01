![LINK GITHUB](https://github.com/martinagallemi01/edaii-2025-P102-19.git)

# Authors

Group number: (19)

- (daniela.monvoisin01@estudiant.upf.edu)----
- (aran.josa01@estudiant.upf.edu)
- (martina.gallemi01@estudiant.upf.edu)

# Plagiarism Disclaimer
- You **MUST NOT** look at anyone else's solutions, including previous year students and external tutors.
- You **MUST** make your repository private.
- You **MAY** discuss the assignments with other students, but you may not look at or copy each others' code.
- You **MUST** commit frequently to the repository (at least 1 commit per exercise).
- You **MUST NOT** share your code with other students even if they ask you to.
- You **MUST** add a link next to any code you copy from external sources.
- You **MUST NOT** ask other students for their project or fragments of their code.

# Repository Contents

- [.github/workflows/unit-tests.yml](./.github/workflows/unit-tests.yml): script to run tests when pushing to GitHub (CI)
- [datasets](./datasets/): datasets of documents you can use to test your search engine
    - [wikipedia12](./datasets/wikipedia12/): a dataset of 13 documents about animals
    - [wikipedia270](./datasets/wikipedia270/): a dataset of 271 documents about different topics
    - [wikipedia540](./datasets/wikipedia540/): a dataset of 541 documents about different topics
    - [wikipedia5400](./datasets/wikipedia5400/): a dataset of 5401 documents about different topics
- [src](./src/): folder with all `.c` and `.h` files to run your program
    - [main.c](./src/main.c): entrypoint to your program
    - [sample_lib.c](./src/sample_lib.c): an example of how to write `.c` files
    - [sample_lib.h](./src/sample_lib.h): an example of how to write `.h` files
- [test](./test/): folder with all `.c` and `.h` files to unit test your program
    - [test.c](./test/test.c): test runner entrypoint
    - [sample_lib_test.c](./test/sample_lib_test.c): an example of how to write unit tests
- [Makefile](./Makefile): file defining what `make` commands are used to run the code from the CLI
