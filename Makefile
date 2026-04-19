BUILD_DIR  ?= build
BUILD_TYPE ?= Release
GENERATOR  ?= Ninja
PROJECT    ?=
BENCH_ARGS ?=

CMAKE_FLAGS := -S . -B $(BUILD_DIR) -G "$(GENERATOR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

.PHONY: all configure build test bench run clean format help

all: build

configure:
	@cmake $(CMAKE_FLAGS)

build: configure
ifeq ($(PROJECT),)
	@cmake --build $(BUILD_DIR)
else
	@cmake --build $(BUILD_DIR) --target $(PROJECT)_tests $(PROJECT)_bench
endif

test: build
ifeq ($(PROJECT),)
	@ctest --test-dir $(BUILD_DIR) --output-on-failure
else
	@ctest --test-dir $(BUILD_DIR) --output-on-failure -L $(PROJECT)
endif

bench: build
ifeq ($(PROJECT),)
	$(error PROJECT=<name> is required for `make bench`)
endif
	@$(BUILD_DIR)/projects/$(PROJECT)/$(PROJECT)_bench $(BENCH_ARGS)

run: test

clean:
	@rm -rf $(BUILD_DIR)

format:
	@pre-commit run --all-files

help:
	@echo "Targets:"
	@echo "  make build [PROJECT=name] [BUILD_TYPE=Debug|Release]"
	@echo "  make test  [PROJECT=name]"
	@echo "  make bench  PROJECT=name  [BENCH_ARGS=...]"
	@echo "  make run   [PROJECT=name]     # configure + build + test"
	@echo "  make clean | make format"
