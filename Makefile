# Define options persistently in options file
-include options.mk

TOOLCHAIN_DIR ?= ${HOME}/toolchains/gcc-arm-none-eabi-10-2020-q4-major/bin
BUILD_DIR ?= build

export PATH := ${PATH}:${TOOLCHAIN_DIR}

.PHONY: configure build clean

configure:
	cmake -B${BUILD_DIR} -S. -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake

build: configure
	ninja -C ${BUILD_DIR} -v

clean:
	rm -rf ${BUILD_DIR}

