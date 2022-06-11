#pragma once

#ifdef DEBUG
#include <stdio.h>
#define _printf printf
#define LOG(...)                                                               \
    {                                                                          \
        _printf("(/%s:%d)", __FILE__, __LINE__);                               \
        _printf(__VA_ARGS__);                                                  \
    }
#define ERROR(...)                                                             \
    {                                                                          \
        fprintf(stderr, "(/%s:%d)", __FILE__, __LINE__);                       \
        fprintf(stderr, __VA_ARGS__);                                          \
    }

#define NLOG(...)                                                              \
    { _printf(__VA_ARGS__); }

#define NERROR(...)                                                            \
    { fprintf(stderr, __VA_ARGS__); }

#else
#define LOG(...)
#define ERROR(...)
#define LOG(...)
#define NERROR(...)
#define printf(...)
#define fprintf(...)
#endif
