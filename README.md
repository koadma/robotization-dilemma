# robotization-dilemma

A futuristic turn-based realistic space-shooter with robots.

# Installation

## Windows, Visual Studio

The "root folder" or `./` will be the folder containing the `.sln` file.

Clone or download this repository.
Clone or download [NGin](https://github.com/marci07iq/NGin)
Place NGin (or simlink) it to the `./../../NGin` folder, (such that the `NGin.h` file is at `./../../NGin/NGin.h`)

In the `./Debug, ./Release/` output directories place the `.lib,.dll,.exp` files from `./robotization dilemma` and copy or simlink the entire `./robotization dilemma/html` folder.
Depending on the build parameters, you might need to rename `freeglutd.dll` to `freeglut.dll`

Build with Visual Studio

# Usage

Ensure that 2 clients, and 1 server is running.
In Play FFa -> Join FFa server connect both clients
Use the GUI to manage your ship.

# Contributing

## Code Style

Don't use tabs, use spaces for indentation!
Use  `lowerCamelCase` for variable names!
For global constants:  `USE_THIS_STYLE`
For Class names: `UpperCamelCase`
For private member variables `_lowerCamelCase`

## Commits

[commit style](http://chris.beams.io/posts/git-commit/)

