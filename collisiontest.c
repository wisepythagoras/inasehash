

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>
#include <time.h>

#include "inasehash.h"

typedef uint64_t u64;

typedef struct trie trie;

struct trie
{
    char key;
    trie *next;
    trie *children;
};

trie *newnode(char s)
{
    trie *t = malloc(sizeof(trie));
    t->key=s;
    t->next = t->children = NULL;
    return t;
}

void insert(trie **t, char *s, int start)
{
    if(s[start] == '\0')
    {
        *t = newnode('#');
        return;
    } 

    if(*t == NULL)
    {
        *t = newnode(s[start]);
        insert(&(*t)->children, s, start+1);
    }       

    if((*t)->key == s[start])
        insert(&(*t)->children, s, start+1);
    else
        insert(&(*t)->next, s, start);
}


bool search(trie *t ,char *s,int start)
{
    if(t == NULL)
        return false;

    if(t->key == '#' && s[start] == '\0')
        return true;

    if((t->key != '#' && s[start] == '\0') || (t->key == '#' && s[start] != '\0'))
        return false;

    if(t->key == s[start])
        return search(t->children,s,start+1);
    else
        return search(t->next,s,start);

    return false;
}



u64 lcg(u64 input)
{
    return input * 6364136223846793005 + 1442695040888963407;
}

u64 lcgr(u64 input)
{
    u64 ii = 13877824140714322085U;
    return (input - 1442695040888963407) * ii;
}

char *int2bin(u64 u)
{
    // dangerous if you don't know what it means to return static pointers
    static char str[sizeof(u64) * CHAR_BIT + 1];
    size_t bits = sizeof(u64) * CHAR_BIT;

    for(; bits--; u >>= 1)
        str[bits] = u & 0x1 ? '1' : '0';

    str[bits] = '\0';
    return str;
}



// int main(int argc, char** argv) 
// {
// // u64 i = 0;

// for(u64 i = 0; i < 100; i++ )
// {
// if(i % 1000000000 == 0)
// {
// printf(".");
// fflush(stdout);
// }

// u64 j = lcgr(lcg(i));
// if (i != j) 
// {
// printf("Meh i: %"PRIu64" j: %"PRIu64"\n", i, j);
// printf("I: %s\n", int2bin(i));
// printf("J: %s\n", int2bin(j));
// break;
// }
// }
// }

int collision_test() 
{
    trie *t = NULL;
    char buf[100];

    for(u64 i = 1; i <= 10000000000000; ++i)
    {
        if( i && i % 100000 == 0){
            // printf(".");
            printf("Database ID: %10"PRIu64"  Bit Matrix: %s  Hash: %s\n", i, int2bin(~linear_congruential_generator(i)), inasehash(i, buf));
            fflush(stdout);
        }
        char *tmp = inasehash(i, buf);

        if(search(t, tmp, 0))
        {
            printf("\n DAMN! a collision at:%"PRIu64"\n Hash: %s", i, tmp);
            return EXIT_FAILURE;
        }
        insert(&t, tmp, 0);
    }

    return EXIT_SUCCESS;
}



int unhash_test()
{
    u64 ran = time(NULL);

    for(u64 i = 1; i <= 10000000000000; ++i)
    {
        if( i % 1000000 == 0)
        {
            printf(".");
            fflush(stdout);
        }

        ran = linear_congruential_generator(ran);

        char *hash = inasehash_static(ran);

        if (!is_valid_hash_string(hash)) 
        {
            printf("\n DAMN! non valid hash string created at:%"PRIu64"\n with Hash: %s", ran, hash);
            return EXIT_FAILURE;
        }

        u64 result = inaseunhash(hash);

        if (ran != result) 
        {
            printf("\n DAMN! ununhashable value at:%"PRIu64"\n with Hash: %s", ran, hash);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}


int main(int argc, char** argv) 
{
    return unhash_test();
}
