#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"


void parse_args(int argc, char **argv)
{
    const struct option table[] = 
    {
        { "bin"     , required_argument, NULL, 'b'},
        { "diff"    , required_argument, NULL, 'd'},
        { "elf"     , required_argument, NULL, 'e'},
    };
    char o;

    while((o = getopt_long(argc, argv, "b:d:e:", table, NULL)) != -1)
    {
        switch(o)
        {
            case 'b': bin_file = optarg; break;
            case 'e': elf_file = optarg; break;
            case 'd': diff_so_file = optarg; break;
            default:
                printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
                printf("\t-b,--bin=filename       image file\n");
                printf("\t-e,--elf=filename       elf file\n");
                printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
                printf("\n");
                exit(0);
        }
    }
}