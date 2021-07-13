# libFuzzer Template Example: libxml2

## Requirements
- LLVM (>10.0.0) toolchain installed:
  - darwin:  `brew install llvm` 
  - debian:  `sudo apt install llvm-dev` or `sudo apt install llvm-12-dev` (or any >=10)
  - arch:    `sudo pacman -S llvm`

- libxml2 installed:
  - darwin:  `brew install libxml2`
  - debian:  `sudo apt install libxml2`
  - arch:    `sudo pacman -S libxml2`

## Building
The included `Makefile` builds and links libFuzzer and libxml2 for you. Simply run: `make`.

### Manually Building

The equivalent call to clang can be made directly as follows:

```sh
clang -g -O1 \
  -I/usr/local/Cellar/llvm/12.0.0_1/include          \
  -I/usr/local/Cellar/libxml2/2.9.12/include/libxml2 \
  -L/usr/local/Cellar/libxml2/2.9.12/lib             \
  -lxml2                                             \
  -std=c++14                                         \
  -stdlib=libc++                                     \
  -D__STDC_CONSTANT_MACROS                           \
  -D__STDC_FORMAT_MACROS                             \
  -D__STDC_LIMIT_MACROS                              \
  -fsanitize=fuzzer                                  \
  -o fuzzer                                          \
  target.cpp
```


## Running

The following bash script contains some important libFuzzer run-time flags. To see 
all flags, simply run: `./fuzzer -help=1` 

```sh
#!/usr/bin/env bash

# Set AddressSanitizer options via env var
ASAN_OPT="strict_string_checks=1"
ASAN_OPT="${ASAN_OPT}:detect_stack_use_after_return=1"
ASAN_OPT="${ASAN_OPT}:check_initialization_order=1"
ASAN_OPT="${ASAN_OPT}:strict_init_order=1"

# Note: You can also just `export ASAN_OPTIONS="${ASAN_OPT}"`
ASAN_OPTIONS="${ASAN_OPT}" ./fuzzer             \ # Run fuzzer with our ASAN flags and ...
  -seed_inputs="/path/to/seed1,/path/to/seed/2" \ # CSV list of input seeds
  -dict=/path/to/xml.dict                       \ # File format definition, if exists
  -max_len=4096                                 \ # Max length of input buffer
  -jobs=1                                       \ # Number of fuzzing jobs to run
  -workers=1                                    \ # Number of simultaneous worker processes
  /path/to/corpus_dir                           \ # Directory to store transient corpus samples
```
