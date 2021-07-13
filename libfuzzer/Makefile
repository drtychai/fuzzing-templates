default_target: fuzzer
.PHONY: default_target

#=========================================================================================#
## Find the LLVM toolchain prefix and set libxml2 include path based on operating system

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LLVM_VERSION := $(shell ls /usr/local/Cellar/llvm)
	XML2_VERSION := $(shell ls /usr/local/Cellar/libxml2)

	LLVM := /usr/local/Cellar/llvm/$(LLVM_VERSION)
	INC  := -I/usr/local/Cellar/libxml2/$(XML2_VERSION)/include/libxml2
endif

ifeq ($(UNAME), Linux)
	# Are LLVM binaries in our PATH
	LLVM_PREFIX := $(shell llvm-config --prefix 2>/dev/null)

	# ** Avoid this block by simpling ensuring `llvm-config` is in your PATH **
	ifeq ($(LLVM_PREFIX), )
		# If not in our PATH, search for it
		#
		# Ugly one-liner for: `/path/to/bin/llvm-config --prefix`
		# If, for some reason, more than one version is installed, then default to most recent
		LLVM_PREFIX := $(shell \
								`find /usr -type f -path '**/bin/llvm-config' 2>/dev/null \
									| sort \
									| head -n 1` \
								--prefix)
	endif

	LLVM := $(LLVM_PREFIX)
	INC  := -I/usr/include/libxml2
endif

#=========================================================================================#
## Agnostic Global Variables

CC  := $(LLVM)/bin/clang
CXX := $(LLVM)/bin/clang++

# Clang compilation flags
CFLAGS   := $(shell $(LLVM)/bin/llvm-config --cflags)
CXXFLAGS := $(shell $(LLVM)/bin/llvm-config --cxxflags)

# LLVM hook for libFuzzer and ASAN, respectively
FZZFLAGS := -fsanitize=fuzzer,address

# Linker flags
LDFLAGS  := -lxml2

#=========================================================================================#

fuzzer: target.cpp
	@$(CC) $(INC) $(CXXFLAGS) $(FZZFLAGS) $(LDFLAGS) -o fuzzer target.cpp
	@$(RM) -rf fuzzer.dSYM
	@echo -e '\033[1;32m[*] Target built successfully\033[0m'
.PHONY: fuzzer


clean:
	@$(RM) *~ fuzzer
.PHONY: clean
