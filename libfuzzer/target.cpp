/*
 * Building: 
 *     clang++ -g -O1 \
 *       -I/usr/local/Cellar/llvm/12.0.0_1/include          \
 *       -I/usr/local/Cellar/libxml2/2.9.12/include/libxml2 \
 *       -L/usr/local/Cellar/libxml2/2.9.12/lib             \
 *       -lxml2                                             \
 *       -std=c++14                                         \
 *       -stdlib=libc++                                     \
 *       -D__STDC_CONSTANT_MACROS                           \
 *       -D__STDC_FORMAT_MACROS                             \
 *       -D__STDC_LIMIT_MACROS                              \
 *       -fsanitize=fuzzer                                  \
 *       -o fuzzer                                          \
 *       target.cpp
 *
 * Running:
 *     # file: run.sh
 *
 *     # Set AddressSanitizer options via env var
 *     ASAN_OPT="strict_string_checks=1"
 *     ASAN_OPT="${ASAN_OPT}:detect_stack_use_after_return=1"
 *     ASAN_OPT="${ASAN_OPT}:check_initialization_order=1"
 *     ASAN_OPT="${ASAN_OPT}:strict_init_order=1"
 *     
 *     # Note: You can also just `export ASAN_OPTIONS="${ASAN_OPT}"`
 *     ASAN_OPTIONS="${ASAN_OPT}" \
 *     ./fuzzer \
 *       \ # CSV list of input seeds
 *       -seed_inputs="/path/to/seed1,/path/to/seed/2" \
 *       \ # File format definition, if exists
 *       -dict=/path/to/xml.dict                       \
 *       \ # Max length of input buffer
 *       -max_len=4096                                 \
 *       \ # Number of fuzzing jobs to run
 *       -jobs=1                                       \
 *       \ # Number of simultaneous worker processes
 *       -workers=1                                    \
 *       \ # Directory to store transient corpus samples
 *       /path/to/corpus_dir
 *
 */

// Include library APIs for any relevant functions, structs, enums, namespaces
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// This is our "hook" for LLVM/libFuzzer
// Without this, our fuzz target (i.e., `xmlReadMemory()`) could not be tested
/* fuzz_target
 * @data: the content of the document
 * @size: the length in bytes
 */
static int fuzz_trgt(const uint8_t *data, size_t size) {
    int result = 1;

    // Instruments libFuzzer to only generate fuzz input with length >= 6
    if (size > 6) {
        // Instruments libFuzzer to match the (explicit) XML magic bytes, below
        if(data[0] == 0x3c &&
           data[1] == 0x3f &&
           data[2] == 0x78 &&
           data[3] == 0x6d &&
           data[4] == 0x6c &&
           data[5] == 0x20)
        
        {
            // See: http://xmlsoft.org/html/libxml-parser.html#xmlReadMemory
            xmlDocPtr doc = xmlReadMemory(
                // Pointer to our (fuzz value) char array
                //reinterpret_cast<const char*>(data),
                (const char*)(data), 
                // Size of associated fuzzed value array
                //static_cast<int>(size),
                (int)(size),
                // Base URL to use for the document, or NULL
                (const char*)NULL,
                // Document encoding, or NULL
                (const char*)NULL,
                // xmlParserOption flags
                // http://xmlsoft.org/html/libxml-parser.html#xmlParserOption
                (int)0
            );

            xmlFreeDoc(doc);
            xmlCleanupParser();
            result = 0;               
        }
    }
    return result;
}

void ignore (void* ctx, const char* msg, ...) {
  // Error handler to avoid spam of error messages from libxml parser.
  // Without this, libxml will spam stdout
}

// Yes, this has to be the function name and signature
extern "C" 
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    xmlSetGenericErrorFunc(NULL, &ignore);

    // `libFuzzer` is going to repeatedly call the body of fuzz_trgt(...) 
    // with a slice of pseudo-random bytes, until it hits an error
    // condition, e.g., segfault, panic, interrupt, etc.
    // 
    // Write your fuzz_trgt(...) body to hit the entry point you need. 
    //  ➜ We want xmlReadMemory(...) to crash, 
    fuzz_trgt(data, size);
    return 0;
}
