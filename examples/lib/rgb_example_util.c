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
/*
con python ho girato le due stringhe
(`'.join([x + y for (x,y) in zip(a[::2], a[1::2])[::-1]]`)
dove a è la stringa hex) e funziona
*/
	assert (NULL != theString);
	assert (NULL != bytes);

	const size_t len = strlen (theString);
	
	if (invert){
	/*
		1234
		2143
		4321
		3412
	*/
		char * aCopy = malloc (len + 1);
		aCopy [len] = 0;
		for (size_t i = 0; i < len; ++i){
			const char aChar = theString [len - i -1];
			aCopy [i] = aChar;
		}
		printf ("%s;\n", theString);
		printf ("%s;\n", aCopy);
		for (size_t i = 0; i < len; i +=2){
			const char temp = aCopy [i+0];
			aCopy [i+0] = aCopy [i+1];
			aCopy [i+1] = temp;
		}
		printf ("%s;\n", aCopy);
		
		for (size_t i = 0; i < len; i +=2){
			bytes [i/2] = HexToBinCoupleChar (aCopy [i+0], aCopy [i+1]);
		}
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
