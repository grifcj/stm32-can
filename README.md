# Overview
This repo contains code for testing CAN functionality with Renode.

Currently, the STMCAN peripheral and CANHub provided with Renode have some minor errors
which cause Renode to crash. Instructions to build custom renode are provided.

Once you have embedded binaries and custom renode, then the renode scripts can be
executed which demonstrate CAN communication.

### Tool versions
Major tools/resources and their versions used while testing

* ubuntu 22.04
* make 4.3
* cmake 3.22.1
* gcc-arm-none-eabi 10.3.1 20210621
* mono 6.12.0.182

# Cross compiler and BSP
The example assumes the use of [GNU RM
toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
and directly uses resources from STM32CubeF4 package (V1.25.2). The cube
resources have been imported directly into this repo, so no need to grab it seperately. 
The build is targeting the STM324xG_EVAL development board with a STM32F407 MCU.

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
> make build
```

# Build Renode
Building Renode is documented
[here](https://renode.readthedocs.io/en/latest/advanced/building_from_sources.html).

The fixes for CAN in Renode are only in the infrastracture submodule. Recursively clone our
[fork](https://github.com/grifcj/renode.git) of Renode, which will pull down the custom infrastructure module.

It can be built according to normal instructions, which for linux is simply
executing `./build.sh`.

```
> git clone --recursive https://github.com/grifcj/renode.git /path/to/renode/src
> cd /path/to/renode/src
> ./build.sh
```

# Run Renode

Assuming one has built renode and the embedded binaries, and we are in this
directory, examples can be run like so.

Assume renode has been checked out and built in `~/src/renode`

```
# Example one machines in loopback with polling
mono ~/src/renode/output/bin/Release/Renode.exe \
    -e 'include @renode-loopback.resc' \
    -e 'start' \
    -e 'runMacro $sendCANMessage'

# Example one machines in loopback with interrupts
mono ~/src/renode/output/bin/Release/Renode.exe \
    -e 'set bin @build/loopback_interrupt' \
    -e 'include @renode-loopback.resc' \
    -e 'start' \
    -e 'runMacro $sendCANMessage'

# Example two machines communicating over can hub with interrupts
mono ~/src/renode/output/bin/Release/Renode.exe \
    -e 'include @renode-canhub.resc' \
    -e 'start' \
    -e 'runMacro $sendCANMessage'
```

# Run Robot Tests
Again, assume renode has been checked out and built in `~/src/renode`

```
renode-test --robot-framework-remote-server-full-directory \
    ~/src/renode/output/bin/Release \
    tests.robot
```
