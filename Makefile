TOOLCHAIN_DIR ?= /home/grifcj/toolchains/gcc-arm-none-eabi-10-2020-q4-major
export TOOLCHAIN_DIR

.PHONY: configure build clean

configure:
	cmake -Bbuild -S. -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake

build: configure
	ninja -C build -v

clean:
	rm -rf build

