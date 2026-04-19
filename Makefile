BUILD_DIR  ?= build
BUILD_TYPE ?= Release
GENERATOR  ?= Ninja
BENCH_ARGS ?=

VERBS   := all build test bench clean configure format help
PROJECT := $(firstword $(filter-out $(VERBS),$(MAKECMDGOALS)))

CMAKE_FLAGS := -S . -B $(BUILD_DIR) -G "$(GENERATOR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

.PHONY: $(VERBS)

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
	@for bin in $(BUILD_DIR)/projects/*/*_bench; do \
	  [ -x "$$bin" ] && echo "==> $$bin" && "$$bin" $(BENCH_ARGS); \
	done
else
	@$(BUILD_DIR)/projects/$(PROJECT)/$(PROJECT)_bench $(BENCH_ARGS)
endif

all: build test bench

clean:
ifeq ($(PROJECT),)
	@rm -rf $(BUILD_DIR)
else
	@rm -rf $(BUILD_DIR)/projects/$(PROJECT)
endif

format:
	@pre-commit run --all-files

help:
	@echo "Usage: make <verb> [project]"
	@echo "  verbs:   build | test | bench | all | clean"
	@echo "  project: toy name (e.g. 1_hello_world); omit to act on all"
	@echo "  vars:    BUILD_TYPE=Debug|Release   BENCH_ARGS=..."

%:
	@:
