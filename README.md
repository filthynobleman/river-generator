# River generator
This repository contains an application for procedural generation of heightmaps of
terrains containing rivers.

## Building instructions
The building process is entirely handled with CMake, and the required libraries are
included as external submodules. Despite being a command line application, SFML relies
on the system's ability to create GUI windows, so your system must be able to create a
window. If you are working on WSL, please be sure to update to WSL2.  

First, clone the repository and update the submodules.
```sh
git clone ... --recursive
cd river-generator
git submodule update --init --recursive
```

Before building the application, you need to build the included SFML library.
```sh
mkdir sfml-build
cd sfml-build
cmake ../ext/sfml/ -DSFML_WARNINGS_AS_ERRORS=FALSE -DCMAKE_INSTALL_PREFIX=./install
cmake --build . --config release --parallel
cmake --install .
```

At this point, you can build the actual application
```sh
cd ..
mkdir build
cd build
cmake ..
cmake --build . --config release --parallel
```