# File Finder

## Introduction

File Finder is a personal project written in C designed to help locate files within a directory structure. 
The goal of this project is to provide a simple and efficient tool for file searching with readable c code. 
This is an ongoing personal project and contributions to improve it are always welcome.

**Note:** This project does **not** aim to compete or compare with tools like `grep` or `ripgrep`. It focuses solely on basic file finding capabilities.

## Project Structure

```bash

$ tree .
.
├── ff
├── include
│   └── ff.h
├── main.c
├── Makefile
├── utils

```

- `include/ff.h` — Header files with function declarations.
- `main.c` — Main program source code.
- `ff` — Source files related to core file finding functionality.
- `utils` — Utility functions/helpers used in the project.
- `Makefile` — Build instructions to compile the project.

## Building the Project

To compile the program, simply run:

    make

It will generate the executable (usually named `ff` or similar as per the Makefile).

## Usage

Run the compiled executable from the command line, followed by the directory or filename pattern you want to search. 
Specific usage depends on the implemented options (please refer to source comments or future documentation).

## Contributing

Feel free to fork this repository, improve the code, add new features, or optimize existing ones. Pull requests are welcome!


---

Thank you for checking out File Finder!

