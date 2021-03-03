# Overview
This repo contains code for testing CAN functionality with Renode. See below
for instructions on building embedded binaries.

At time of this writing, STMCAN peripheral and CANHub has some minor errors
which cause Renode to crash. Instructions to build custom renode are provided.

Once you have binaries and custom renode, then the test scripts can be
executed.

### Tool versions
For reference, the major tools/resources and their versions

* ubuntu 20.04
* make 4.2.1
* cmake 3.17.2
* gcc-arm-none-eabi 10-2020-q4-major
* mono 6.12.0.122
* monodevelop 7.8.4 (build2)

# Cross compiler and BSP
The example assumes the use of [GNU RM
toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
and directly uses resources from STM32CubeF4 package (V1.25.2). The cube
resources have been imported directly into source build so it is
self-contained. The build is targeting the STM324xG_EVAL development board with
a STM32F407 MCU.

# Build Embedded Binaries
The build is CMake based. A simple make wrapper with clean and build target is
provided for convenience.

To build the embedded binaries, the crosscompiler must be on the path. Either
modify path or set via option to make.
```
> export PATH=~/toolchains/gcc-arm-none-eabi-10-2020-q4-major/bin:$PATH
> make build
```
or
```
> TOOLCHAIN_DIR=~/toolchains/gcc-arm-none-eabi-10-2020-q4-major/bin make build
```

Options can be set via command line or persistently via options file. Just
create an options file in root directory.
```
> cat options.mk
TOOLCHAIN_DIR ?= /path/to/gnu/rm/toolchain
BUILD_DIR ?= /path/to/build/dir
```

# Build Renode
Building Renode is documented
[here](https://renode.readthedocs.io/en/latest/advanced/building_from_sources.html).

Our changes to Renode are only in the infrastracture submodule. Clone our
[fork]() of Renode, which will pull down the custom infrastructure module. It can be built
according to normal instructions, i.e. just execute `./build.sh`.

To run the custom renode, mono will be required.
mono

# Debugging Renode
