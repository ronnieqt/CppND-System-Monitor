.PHONY: all
all: format build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: test
test:
	g++ -std=c++17 -o build/test -I ./include/ test/main.cpp src/system.cpp src/process.cpp src/processor.cpp src/linux_parser.cpp src/format.cpp

.PHONY: clean
clean:
	rm -rf build
