#ifndef FASTICIONARIOS_H
#define FASTICIONARIOS_H
#include <stdlib.h>
#include <stdio.h>

extern const struct Worker {
    unsigned int id;
    char* name;
    char* surName;
    int salary;
    char area[3];
} worker;

extern void readAreas(FILE* fp);
extern void seekUntil(FILE* fp, char c);
extern char* readUntil(FILE* fp, char c);
extern struct Worker readWorker(FILE* fp);
extern void globalAverage(struct Worker w, int* accumulator);
extern int readSalary(FILE* fp);
extern float getGlobalAverage(int* accumulator);

#endif /* FASTICIONARIOS_H */
