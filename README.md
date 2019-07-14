# COMP9242/AOS Public
[course project specification](https://www.cse.unsw.edu.au/~cs9242/19/project/index.shtml)

## Overview
The goal of the course is to implement sos - Simple Operating System. This repo
is here for me to play around with it in my own way.

## Language

The infrastructure is for C. Students have done it in Rust, some regretting the
choice, some recommending it. I plan to play around with it in Rust.

* As a personal choice, Rust is a beautiful language, engineered to eliminate
memory and data-race bugs when `safe`.
* When `safe` is not an option, as is common with OS work, the structure of the
language provides an excelent framework to isolate the risk of `unsafe` code.
* Understanding how to go between `rust` and `C` is a goal of mine.
* Rust is fun in a [masochistic sort of way](https://www.cse.unsw.edu.au/~cs9242/19/project/index.shtml#advanced-components-aka-stuff-for-masochists). 


## Infrastructure

AOS uses a CMake based build-generator, which also manages dependencies. It is
entirely self-contained, using musllibc, and other libraries, from which it can
`#include <>`

Building the Operating System "personallity" is what this project is all about.
The starting code comes with little more than startup diagnostics and a hacky
implementation to spawn a process that is used to complete the equivilent of 
"hello world".

The all work is done in `projects/aos`, with root-server code being in `aos/sos/src/main.c`.

## Linking in the root-server

The Cmake system modifies the `_start` semantics for the root-server so it sets
up the entry points as defined by `_sel4_start`. Other servers use the standard
`_start`. `_sel4_start` is defined in `sos/crt/sel4_crt0.S`.

## Dependancy management
This is all done using `CmakeLists.txt` files in each directory.
* Exactly one project is the root server using `DeclareRootserver($projName)`
 * $projName=sos in the starting codes root-server
* To `#include <libname.h>`, `target_link_libraries($projName Configuration libname)`
* TODO

# Aim

To set up the infrastructure such that it Just Works as much for Rust, as it does
for C, and to begin implementation under that infrastructure as a test.
