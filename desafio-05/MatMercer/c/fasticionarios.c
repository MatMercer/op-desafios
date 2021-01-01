#include <malloc.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fasticionarios.h"

#define AREA_TABLE_SIZE 32
void readAreas(FILE* fp) {
    const char* areasKeyword = ":\"saera\"";
    const int areasKeywordSize = 7;

    fseek(fp, -1, SEEK_END);

    int foundIdx = 0;
    
    while(1) {
        int c = fgetc(fp);
        fseek(fp, -2, SEEK_CUR);

        if (c == areasKeyword[foundIdx]) {
            if (++foundIdx == areasKeywordSize) {
                seekUntil(fp, ':');
                break;
            }
        } else {
            foundIdx = 0;
        }

#ifdef DEBUG
        static int count = 0;
        count += 1;
        if (count > 2048) {
            fprintf(stderr, "!!! COULD NOT FIND AREAS !!!");
            break;
        }
#endif 
    }

    hcreate(AREA_TABLE_SIZE);
    ENTRY e;

    while(!feof(fp)) {
        seekUntil(fp, '\"');
        seekUntil(fp, ':');
        seekUntil(fp, '\"');
        char *areaCode = readUntil(fp, '\"');

        seekUntil(fp, '\"');
        seekUntil(fp, ':');
        seekUntil(fp, '\"');
        char *areaName = readUntil(fp, '\"');

        e.key = areaCode;
        e.data = (void*) areaName;
        hsearch(e, ENTER);

#ifdef DEBUG
        printf("Area detected: %s: %s\n", areaCode, areaName);
#endif
    }
}

void seekUntil(FILE* fp, char c) {
    while (!feof(fp)) {
        if (fgetc(fp) == c) {
            break;
        }
    }
}

#define MAX_BUFF 256 // todo: I can calculate the buffer size with ftell(fp)
char* readUntil(FILE* fp, char c) {
    char* buffer;

    buffer = (char*)malloc(MAX_BUFF); // slow, allocates every time

    int count = 0;

    while(!feof(fp)) {
        char rc = fgetc(fp);
        if (rc == c) {
            break;
        }

        buffer[count++] = rc;
    }

    return buffer;
}

// todo: need to optimize it, malloc is slow and requiring to much bytes
struct Worker readWorker(FILE* fp) {
    struct Worker w;

    seekUntil(fp, ':');
    w.id = atoi(readUntil(fp, ','));

    seekUntil(fp, ':');
    seekUntil(fp, '"');
    w.name = (char*) malloc(256);
    strcpy(w.name, readUntil(fp, '"'));

    seekUntil(fp, ':');
    seekUntil(fp, '"');
    w.surName = (char*) malloc(256);
    strcpy(w.surName, readUntil(fp, '"'));

    seekUntil(fp, ':');
    w.salary = atof(readUntil(fp, ','));

    seekUntil(fp, ':');
    seekUntil(fp, '"');
    strcpy(w.area, readUntil(fp, '"'));

    return w;
}

