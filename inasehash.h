
// INASE HASH reference implementation.
// Created by Markus Wanke on 2015.11.05

#ifndef INASEHASH_H
#define INASEHASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


#define HASH_LENGTH 11

/**
 * INASEHASH - Turns 64bit unsigned integers into convinient short hash like strings.
 * The strings have always 11 chracters and consist of [A-Za-z0-9_-]
 *
 * @para intID: The integer ID to hash
 * @para buf: A buffer to store the hash string. 12 Bytes are needed.
 * @return: buf pointer for convinience reasons
 */
char *inasehash(uint64_t intID, char *buf);


/**
 * INASEHASH - Turns 64bit unsigned integers into convinient short hash like strings.
 * The strings should have always 11 chracters and consist of [A-Za-z0-9_-]
 *
 * @para intID: The integer ID to hash
 * @return: A STATIC allocated result string. 
 *
 * WARNING The result string is allocated in static space. Means if you call 
 * inasehash() twice the previous string will be overwritten. This is super 
 * handy if only need to call this function once, but highly error prone if 
 * you need to call this function more than once. Make sure to use 
 * strdup(inasehash_static(555)) to allocate memory for the result before calling 
 * inasehash again!
 */
char *inasehash_static(uint64_t intID);

/**
 * Reverse the process of hashing. Returns the original integer value for
 * a given hash. Use is_valid_hash_string() before calling this function. 
 * Calling this function with non valid hashstrings results in undefined behaviour.
 *
 * @para hash: 11 digit NUL terminated c-string. containing [a-zA-Z0-9_-]
 * @return: Original integer value
 */
uint64_t inaseunhash(char *hash);


/**
 * To test if an string is an valid hash string.
 */
bool is_valid_hash_string(char *s);

/**
 * To test if an char is a valid hash char that could appear in a hash string
 */
bool is_valid_hash_char(char s);


/**
 * For debugging
 */
uint64_t linear_congruential_generator(uint64_t input);
uint64_t linear_congruential_generator_reverse(uint64_t input);

#ifdef __cplusplus
}
#endif

#endif /* INASEHASH_H */
