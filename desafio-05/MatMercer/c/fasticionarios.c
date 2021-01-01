#include <malloc.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct Worker {
    unsigned int id;
    char* name;
    char* surName;
    float salary;
    char area[3];
} worker;

void readAreas(FILE* fp);
void seekUntil(FILE* fp, char c);
char* readUntil(FILE* fp, char c);
struct Worker readWorker(FILE* fp);

#ifndef TEST_BINARY // test binary
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
#endif // end of test binary

#ifdef TEST_BINARY

void assertReadUntil();
void assertSeekArea();
void assertSeekUntil();
void assertReadWorker();

void assertStringEqual(char* msg, char* expected, char* yielded);
void assertIntEqual(char* msg, int expected, int yielded);
void assertFloatEqual(char* msg, float expected, float yielded);
void pass(char* msg);

int main() {
    assertSeekUntil();
    assertReadUntil();

    assertSeekArea();
    assertReadWorker();
    
    return 0;
}

#define ASSERT_AREA(AREA, NAME) e.key = AREA; assertStringEqual("maps " AREA " to " NAME, NAME, (char*)hsearch(e, FIND)->data);
void assertSeekArea() {
    const char* file10K = "Funcionarios-10K.json";

    FILE *f10p;

    f10p = fopen(file10K, "r");

    readAreas(f10p);

    ENTRY e;
    e.key = "A1";
    ASSERT_AREA("A1", "Área 1");
    ASSERT_AREA("A2", "Área 2");
    ASSERT_AREA("A3", "Área 3");
    ASSERT_AREA("CC", "Controladoria Central");
    ASSERT_AREA("D1", "Departamento 1");
    ASSERT_AREA("D2", "Departamento 2");
    ASSERT_AREA("D3", "Departamento 3");
    ASSERT_AREA("DC", "Departamento Comercial");
    ASSERT_AREA("DJ", "Departamento Jurídico");
    ASSERT_AREA("ES", "Engenharia Social");
    ASSERT_AREA("GA", "Gestão de Ativos");
    ASSERT_AREA("GT", "Governança de TI");
    ASSERT_AREA("ME", "Manutenção Elétrica");
    ASSERT_AREA("MH", "Manutenção Hidráulica");
    ASSERT_AREA("MM", "Manutenção Mecânica");
    ASSERT_AREA("MP", "Manutenção Predial");
    ASSERT_AREA("PE", "Pagadoria Externa");
    ASSERT_AREA("PI", "Pagadoria Interna");
    ASSERT_AREA("PM", "Produção Multimídia");
    ASSERT_AREA("PP", "Produção de Panfletos");
    ASSERT_AREA("PV", "Programação Visual");
    ASSERT_AREA("RH", "Recursos Humanos");
    ASSERT_AREA("DS", "Desenvolvimento de Software");
    ASSERT_AREA("GS", "Gerenciamento de Software");
    ASSERT_AREA("UD", "Designer de UI/UX");
}

void assertSeekUntil() {
    const char* f = "test_seekUntil_0.txt";
    FILE *fp;
    fp = fopen(f, "r");
    seekUntil(fp, 'b');

    assertIntEqual("must seek to char correctly", 10, ftell(fp));
}

void assertReadUntil() {
    const char* f = "test_readUntil_0.txt";
    FILE *fp;
    
    fp = fopen(f, "r");
    char* result = readUntil(fp, '"');

    assertStringEqual("readUntil must read corretly", "the content of a string", result);
}

void assertReadWorker() {
    const char* file10K = "Funcionarios-10K.json";
    FILE *f10p;

    f10p = fopen(file10K, "r");

    seekUntil(f10p, '[');

    struct Worker w = readWorker(f10p);

    assertIntEqual("worker: maps worker id", 1, w.id);
    assertStringEqual("worker: maps worker name", "Aahron", w.name);
    assertStringEqual("worker: maps worker surName", "Abaine", w.surName);
    assertFloatEqual("worker: maps worker salary", 24048.75, w.salary);
    assertStringEqual("worker: maps worker area", "A2", w.area);
    }

void assertStringEqual(char* msg, char* expected, char* yielded) {
    if(strcmp(expected, yielded) != 0) {
        fprintf(stderr, "ERROR: '%s', expected '%s' but got '%s'\n", msg, expected, yielded);
    } else {
        pass(msg);
    }
}

void assertIntEqual(char* msg, int expected, int yielded) {
    if (expected != yielded) {
        fprintf(stderr, "ERROR: '%s', expected '%d' but got '%d'\n", msg, expected, yielded);
    } else {
        pass(msg);
    }
}

void assertFloatEqual(char* msg, float expected, float yielded) {
    if (expected != yielded) {
        fprintf(stderr, "ERROR: '%s', expected '%f' but got '%f'\n", msg, expected, yielded);
    } else {
        pass(msg);
    }
}

void pass(char* msg) {
    fprintf(stderr, "PASS: %s\n", msg);
}
#endif

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

struct Worker readWorker(FILE* fp) {
    struct Worker w;

    seekUntil(fp, ':');
    w.id = atoi(readUntil(fp, ','));

    seekUntil(fp, ':');
    seekUntil(fp, '"');
    strcpy(w.name, readUntil(fp, '"'));

    seekUntil(fp, ':');
    seekUntil(fp, '"');
    strcpy(w.surName, readUntil(fp, '"'));

    seekUntil(fp, ':');
    w.salary = atof(readUntil(fp, ','));

    seekUntil(fp, ':');
    seekUntil(fp, '"');
    strcpy(w.area, readUntil(fp, '"'));

    return w;
}

