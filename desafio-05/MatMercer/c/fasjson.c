#include <stdio.h>
#include <stdlib.h>
#include "fasticionarios.h"

int main(int argc, char **argv) {
#ifdef DEBUG
    if (argc != 2) {
        fprintf(stderr, "usage: fasjson [file.json]\n");
    }
#endif

    char* fileName = argv[1];

    FILE *fp;

    fp = fopen(fileName, "r");

    readAreas(fp);

    return 0;
}

