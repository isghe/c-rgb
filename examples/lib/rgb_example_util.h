#ifndef RGBORG_EXAMPLE_UTIL_H
#define RGBORG_EXAMPLE_UTIL_H

#ifdef __cplusplus
extern "C"{
#endif

extern void HexToBin (const char * theString, unsigned char * bytes, const int invert);
extern void print_hex(const void *p, size_t len);


#ifdef __cplusplus
}
#endif

#endif
