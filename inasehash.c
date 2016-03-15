
// INASE HASH reference implementation.
// Created by Markus Wanke on 2015.11.05

#include <string.h> // strnlen
#include "inasehash.h"

typedef uint64_t u64;
typedef uint8_t byte;


// Random permutation of "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyzyz-_"
// This needs to have 64 unique characters and the order is final.
// If you change this, change the reverse_lookup_table as well.
static char lookup_table[64] = 
{
	'I','N','A','S','E','i','r','F',
	'n','M','b','T','W','f','G','Q',
	'a','O','4','B','v','c','h','K',
	's','0','7','5','6','D','q','z',
	'e','V','C','X','3','8','9','R',
	'o','k','m','U','Y','p','x','1',
	'J','Z','l','t','2','y','j','d',
	'P','L','H','w','-','_','u','g'
};

// At the bottom of this file is a small python script to generate this table 
// from the lookup_table automatic.
static byte reverse_lookup_table[256] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
    0x19, 0x2f, 0x34, 0x24, 0x12, 0x1b, 0x1c, 0x1a, 
    0x25, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x02, 0x13, 0x22, 0x1d, 0x04, 0x07, 0x0e, 
    0x3a, 0x00, 0x30, 0x17, 0x39, 0x09, 0x01, 0x11, 
    0x38, 0x0f, 0x27, 0x03, 0x0b, 0x2b, 0x21, 0x0c, 
    0x23, 0x2c, 0x31, 0x00, 0x00, 0x00, 0x00, 0x3d, 
    0x00, 0x10, 0x0a, 0x15, 0x37, 0x20, 0x0d, 0x3f, 
    0x16, 0x05, 0x36, 0x29, 0x32, 0x2a, 0x08, 0x28, 
    0x2d, 0x1e, 0x06, 0x18, 0x33, 0x3e, 0x14, 0x3b, 
    0x2e, 0x35, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



u64 linear_congruential_generator(u64 input)
{
    // Magic? No Math! https://ja.wikipedia.org/wiki/%E7%B7%9A%E5%BD%A2%E5%90%88%E5%90%8C%E6%B3%95
    return input * 6364136223846793005u + 1442695040888963407u;   // will overflow which means mod 2^64
}


u64 linear_congruential_generator_reverse(u64 input)
{
	// 13877824140714322085u is the inverse of 6364136223846793005u in the 2^64 space
    return (input - 1442695040888963407u) * 13877824140714322085u;
}




bool is_valid_hash_char(char c)
{
	return c == lookup_table[0] || reverse_lookup_table[(byte)c] != 0x00;
}

bool is_overflow_char_on_last_position(char c)
{
    // the last hash digit has only 4 significant bits instead of 6. (11 chars a 6 bit = 66 bit from a 64 bit number)
    // so it must be one of the first 16 chars in the lookup_table
    return reverse_lookup_table[(byte)c] > 0xF;
}

bool is_valid_hash_string(char *s) 
{
	if (!s)
		return false;

	if (strnlen(s, HASH_LENGTH) != HASH_LENGTH)
		return false;

	for(int i = 0; i < HASH_LENGTH; i++) 
	{
		if (!is_valid_hash_char(s[i]))
			return false;
	}

    if(is_overflow_char_on_last_position(s[HASH_LENGTH-1]))
        return false;

	return true;
}


// is_valid_hash_string() should always be called before this function is called. 
u64 inaseunhash(char *hash)
{
	u64 bitmap = 0x0;
	char *end = hash + HASH_LENGTH;	
	
	while(end-- > hash)
	{
		bitmap <<= 6;
		bitmap |= (u64)reverse_lookup_table[ (byte)*end ];
	}

	return linear_congruential_generator_reverse(~bitmap);
}



char *inasehash_static(u64 id)
{
	static char buf[ HASH_LENGTH + 1 ];
	return inasehash(id, buf);
}

// short version
char *inasehash(u64 id, char *buf)
{
	id = ~linear_congruential_generator(id);

	for(int i = 0; i < HASH_LENGTH; ++i, id >>= 6 )
		buf[i] = lookup_table[ id & 0x3F ];

	buf[HASH_LENGTH] = '\0';
    return buf;
}

// long version, explaining what happens:  
/*
char *inasehash(u64 id, char *buf)
{
	// turns 00000001 into kinda random 11001010 without ever creating the same value
	id = linear_congruential_generator(id);

	// lets just flip all bits because its fun
	id = ~id;

    for(int i = 0; i < HASH_LENGTH; ++i)
    {
		// now we chop the 64bit id into 11 parts. 10 times 6 bit, and one time 4 bit
		// 6 bits have 64 possible combinations. Perfect for our lookup table.

		// shift the bits in ID i times 6. In the first loop i = 0 and nothing is shifted
		u64 tmp = id >> (i * 6);

		// we only need the last 6 bytes so we cut them off. 
		// Results in a number always between 0 an 63 :)
		u64 pos = tmp & 0x3F;   // 0x3F = 00111111

		// Warning C does know the differnece between arithmetic shift and logic shift.
		// Does your language? PHP for example does not!! So if you programm this in 
		// PHP or so your last line must look like this
		// u64 pos = tmp & ((i < HASH_LENGTH) ? 0x3F : 0x0F);   // 0x3F in binary: 00111111, 0x0F in binary 00001111
		// this will keep left inserted sign bits 0 in the last iteration. For more information:
		// https://en.wikipedia.org/wiki/Arithmetic_shift

		// Copy the corrosponding chracter from the lookup table into the result string
        buf[i] = lookup_table[pos];
    }

	// the same code more compact, C knows not to use arithmetic shift on unsigned integers
	// for(int i = 0; i < HASH_LENGTH; ++i, id >>= 6 )
		// buf[i] = lookup_table[ id & 0x3F ];


	// the same code more compact, with arithmetic shift protection. Implement this code in
	// other languages which do not have logic shift un unsigned integers. PHP for example
	// for(int i = 0; i < HASH_LENGTH-1; ++i, id >>= 6 )
		// buf[i] = lookup_table[ id & 0x3F ];
	// buf[10] = lookup_table[ id & 0x0F ];

	
	// this is a C string, so we need an zero byte at the end or horrible things will happen
	buf[HASH_LENGTH] = '\0';
    return buf;
}
*/





/* GENERATE THE REVERSE LOOKUP TABLE WITH THIS PYTHON 3 SCRIPT:

mapping = [ 'I','N','A','S','E','i','r','F',
			'n','M','b','T','W','f','G','Q',
			'a','O','4','B','v','c','h','K',
			's','0','7','5','6','D','q','z',
			'e','V','C','X','3','8','9','R',
			'o','k','m','U','Y','p','x','1',
			'J','Z','l','t','2','y','j','d',
			'P','L','H','w','-','_','u','g' ]

res = []

for i in range(0, 256):
    try:
        value = mapping.index(chr(i))
        res.append("0x" + format(value, '02x'))
    except:
        res.append("0x00")

    if i % 8 == 0:
        res[-1] = "\n    " + res[-1]

print("{" + ", ".join(res) + "\n}")

*/


