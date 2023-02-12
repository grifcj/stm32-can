# Overview
This repo contains code for testing CAN functionality with Renode.

---
**NOTE**

These examples haven't been tested on real hardware. Though they may
demonstrate CAN communication in Renode, there may be issues when run on actual
devices.

---

### Tool versions
Major tools/resources and their versions used while testing

* ubuntu 20.04
* make 4.2.1
* cmake 3.22.1
* gcc-arm-none-eabi 10.3-2021.10
* mono 6.12.0.182
* renode 1.13.2
* vscode 1.75.1 (For developing Renode)

# Cross compiler and BSP
The example assumes the use of [GNU RM
toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
and directly uses resources from STM32CubeF4 package (V1.25.2). The cube
resources have been imported directly into this repo, so no need to grab it separately.
The build is targeting the STM324xG_EVAL development board with a STM32F407 MCU.

# Build Embedded Binaries
The build is CMake based. A simple make wrapper with clean and build target is
provided for convenience.

To build the embedded binaries, the crosscompiler must be on the path. Either
modify path or set via option to make.
```
> export PATH=~/toolchains/gcc-arm-none-eabi-10.3-2021.10/bin:$PATH
> make build
```
or
```
> TOOLCHAIN_DIR=~/toolchains/gcc-arm-none-eabi-10.3-2021.10/bin make build
```

Options can be set via command line or persistently via options file. Just
create an options file in root directory.
```
> cat options.mk
TOOLCHAIN_DIR ?= /path/to/gnu/rm/toolchain
BUILD_DIR ?= /path/to/build/dir
> make build
```

# Install Renode

Releases can be obtained [here](https://github.com/renode/renode).

# Build Renode

Building Renode is documented
[here](https://renode.readthedocs.io/en/latest/advanced/building_from_sources.html).

# Run Examples

Assume:
1) Renode installed and on path
2) CWD is root of stm32-can repo

Examples can be run with following commands

```
# One machine in loopback with polling
renode \
    -e 'include @renode-loopback.resc' \
    -e 'start' \
    -e 'runMacro $sendCANMessage'

# One machine in loopback with interrupts
renode \
    -e 'set bin @build/loopback_interrupt' \
    -e 'include @renode-loopback.resc' \
    -e 'start' \
    -e 'runMacro $sendCANMessage'

# Three machines communicating over can hub with interrupts. One sends message,
# two receive it.
renode \
    -e 'include @renode-canhub.resc' \
    -e 'start' \
    -e 'runMacro $sendCANMessage'
```

# Run Robot Tests

```
renode-test tests.robot
```
