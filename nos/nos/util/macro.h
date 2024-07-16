#ifndef NOS_UTIL2_MACRO_H
#define NOS_UTIL2_MACRO_H

#define NOS_STRINGIFY(...) #__VA_ARGS__
#define NOS_STRINGIFY2(...) STRINGIFY(__VA_ARGS__)

#define NOS_CONCAT(a, b) a##b
#define NOS_CONCAT2(a, b) NOS_CONCAT(a, b)

#endif
