default_target: fuzzer
.PHONY: default_target

#=================================================================================#
## Find the LLVM toolchain and set libxml2 include path based on operating system

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LLVM_VERSION := $(shell ls /usr/local/Cellar/llvm)
	LLVM := /usr/local/Cellar/llvm/$(LLVM_VERSION)
	
	XML2_VERSION := $(shell ls /usr/local/Cellar/libxml2)
	INC := -I/usr/local/Cellar/libxml2/$(XML2_VERSION)/include/libxml2
endif

ifeq ($(UNAME), Linux)
	#LLVM := $(shell readlink -f /usr/lib/llvm-??/)
	LLVM := /usr
	INC  := -I/usr/include/libxml2
endif

#=================================================================================#
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

#=================================================================================#
	   
fuzzer: target.cpp
	$(CC) $(INC) $(CXXFLAGS) $(FZZFLAGS) $(LDFLAGS) -o fuzzer target.cpp
	$(RM) -rf fuzzer.dSYM
.PHONY: fuzzer


clean:
	$(RM) *~ fuzzer
.PHONY: clean
