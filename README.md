# Linux Heap Memory Manager

## Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Authors](#authors)

## About <a name = "about"></a>

This is a user space memory management project tailored for Linux systems. This project explores various approaches to memory allocation and deallocation, serving as an introductory exploration into fundamental memory management principles.

### Versions

- **Naive Implementation :** Establishes a fundamental understanding of memory management principles, based on linked list.
  
- **Optimized Naive Version :** Enhances performance and resource utilization through optimization.
  
- **Buddy Allocator :** Incorporates the logic of the Buddy Allocator, known for its efficiency in managing memory fragments.


## Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.
### Prerequisites

* **Make v4.2+**
```bash
sudo apt install make
```

* **GNU GCC**
```bash
sudo apt install gcc
```

### Install
First, clone the repository locally :

```bash
git clone https://github.com/IgorGreenIGM/HeapMemoryManager
```

### Compile & Launch

A makefile with all compile commands is provided, simply run : 
```bash
make <version> 
```
Where <b><i>version</i></b> is the version of the allocator you wish to compile : 
- *v1* : naive
- *v2* : naive optimized
- *v3* : binary buddy allocator

Also, a stress program is proposed for testing allocators : 
```bash
make stress
```

And finally start any program with the allocator :
```bash
LD_PRELOAD=./bin/malloc.so <path to your program>
``` 

## Authors <a name = "authors"></a>

- [@IgorGreenIGM](https://github.com/IgorGreenIGM) - Idea & Initial work
