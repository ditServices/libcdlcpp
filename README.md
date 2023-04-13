# CDL 

CDL is a simple library for reading and writing Colour Decision List files in C++, the project is in active development and currently is at version 0.1. 

The library presents a parsing and writing function that is made available via a series of constructors. The CDL file is a specification that is based on a predefined XML structure, the libary uses Pugixml to manage this aspect of reading and writing. 

## Build instructions

The library can be built via cmake and make. Please ensure to clone recursively to ensure the submodules are built. 

- `git clone https://github.com/ditServices/libcdlcpp`
- `git submodule update --init --recursive`
- `mkdir build`
- `cmake ..`
- `make`
