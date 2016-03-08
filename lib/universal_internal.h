/*

universal_internal.h
universal-c

Copyright (c) 2016 Coletiv Studio

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef __UNIVERSAL_C_INTERNAL_H__
#define __UNIVERSAL_C_INTERNAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <assert.h>
#include <stddef.h>

typedef int32_t universal_error_t;

#if !defined(UNIVERSAL_EXTERN_C_BEGIN)
#if defined(__cplusplus)
#define UNIVERSAL_EXTERN_C_BEGIN extern "C" {
#define UNIVERSAL_EXTERN_C_END   }
#else
#define UNIVERSAL_EXTERN_C_BEGIN
#define UNIVERSAL_EXTERN_C_END
#endif
#endif

#if defined(DEBUG) || defined(TEST)
#define Logc(format, ...) printf(("universal: " format "\n"), ## __VA_ARGS__)
#define LogErrorc(format, ...) printf(("universal: *error* " format "\n"), ## __VA_ARGS__)
#define PrettyLogc printf("universal: * %s\n", __PRETTY_FUNCTION__)
#else
#define Logc(format, ...)
#define LogErrorc(format, ...)
#define PrettyLogc
#ifndef NS_BLOCK_ASSERTIONS // Block Assertions in Release versions
#define NS_BLOCK_ASSERTIONS
#endif
#endif

#endif /* __UNIVERSAL_INTERNAL_H__ */