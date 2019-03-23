#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
 
#include "rgb_example_util.h"

void HexToBin (const char * theString, unsigned char * bytes, const int invert);
void print_hex(const void *p, size_t len);

static int HexToBinCoupleChar (const char c1, const char c2){
	assert (isxdigit (c1));
	assert (isxdigit (c2));
	const char str [] = {c1, c2, 0};
	return strtol (str, NULL, 16);
}

void HexToBin (const char * theString, unsigned char * bytes, const int invert){
	assert (NULL != theString);
	assert (NULL != bytes);

	const size_t len = strlen (theString);
	
	if (invert){
		char * aInvert = malloc (len + 1);
		assert (NULL != aInvert);

		aInvert [len] = 0;

		// "1234" -> "3412"
		for (size_t i = 0; i < len; i +=2){
			aInvert [i+0] = theString [len - i - 2];
			aInvert [i+1] = theString [len - i - 1];
		}
		
		for (size_t i = 0; i < len; i +=2){
			bytes [i/2] = HexToBinCoupleChar (aInvert [i+0], aInvert [i+1]);
		}
		free (aInvert);
	}
	else{
		for (size_t i = 0; i < len; i +=2){
			bytes [i/2] = HexToBinCoupleChar (theString [i+0], theString [i+1]);
		}
	}
}

void print_hex(const void *p, size_t len) {
	assert (NULL != p);

    for (size_t i = 0; i < len; ++i){
        printf("%02hhx", *((uint8_t *) p + i));
	}

    printf("\n");
}
