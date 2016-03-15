
// INASE HASH reverence implementation.
// Created by Markus Wanke on 2015.11.05

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>

#include "inasehash.h"

void usage(char *execname) 
{
    fprintf(stderr, "Usage: %s [id to hash (64 bit integers in base 10)] | -d [HASH] \n", execname);
    fprintf(stderr, "\t-d : Decode hashvalue back to original id\n");
    fprintf(stderr, "\tThe hash format is [a-zA-Z0-9_-]{11}\n");
}

uint64_t parse_arg(char *str)
{
    char *endptr = NULL;
    uint64_t val;

    errno = 0;
    val = strtoull(str, &endptr, 10);

    if (errno != 0 || endptr == str || *endptr != '\0')
    {
        perror("Failed to parse command line argument.");
        exit(EXIT_FAILURE);
    }

    return val;
}

int unhash(int argc, char** argv) 
{
    if (argc == 3) 
    {
        if(!is_valid_hash_string(argv[2]))
        {
            fprintf(stderr, "The String '%s' is not in the correct hash string format. Should be: [a-zA-Z0-9_-]{11}\n", argv[2]);
            return EXIT_FAILURE;
        }

        printf("%"PRIu64"\n", inaseunhash(argv[2]));
        return EXIT_SUCCESS;
    }

    for(int i = 2; i < argc; ++i)
    {
        if(!is_valid_hash_string(argv[i]))
        {
            fprintf(stderr, "The String '%s' is not in the correct hash string format. Should be: [a-zA-Z0-9_-]{11}\n", argv[i]);
            return EXIT_FAILURE;
        }
        printf("%s:%8"PRIu64"\n", argv[i], inaseunhash(argv[i]));
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) 
{
    if (argc < 2) 
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if ( argc >= 3 && argv[1][0] == '-' && argv[1][1] == 'd' && argv[1][2] == '\0')
    {
        return unhash(argc, argv);
    }

    if (argc == 2) 
    {
        puts(inasehash_static(parse_arg(argv[1])));
    }
    else 
    {
        for(int i = 1; i < argc; ++i)
        {
            char *hash = inasehash_static(parse_arg(argv[i]));
            printf("%8s: %s\n", argv[i], hash);
        }
    }

    return EXIT_SUCCESS;
}

