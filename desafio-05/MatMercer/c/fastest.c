#include <search.h>
#include <stdio.h>
#include <string.h>
#include "fasticionarios.h"

void assertReadUntil();
void assertReadSalary();
void assertSeekArea();
void assertSeekUntil();
void assertReadWorker();
void assertGlobalAverage();

void assertStringEqual(char* msg, char* expected, char* yielded);
void assertIntEqual(char* msg, int expected, int yielded);
void assertFloatEqual(char* msg, float expected, float yielded);
void pass(char* msg);

int main() {
    assertSeekUntil();
    assertReadUntil();

    assertSeekArea();
    assertReadSalary();
    assertReadWorker();
    assertGlobalAverage();

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

void assertReadSalary() {
    const char* f = "test_readSalary_0.txt";

    FILE *fp;

    fp = fopen(f, "r");

    int salary = readSalary(fp);

    assertIntEqual("read the salary corretly", 123456, salary);
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
    assertFloatEqual("worker: maps worker salary", 2404875, w.salary);
    assertStringEqual("worker: maps worker area", "A2", w.area);
}

void assertGlobalAverage() {
    struct Worker w1;
    struct Worker w2;

    int accumulator = 0;

    w1.salary = 5;
    w2.salary = 1;

    globalAverage(w1, &accumulator);
    globalAverage(w2, &accumulator);

    assertFloatEqual("calculates global average correctly", 0.03, getGlobalAverage(&accumulator));
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
        fprintf(stderr, "ERROR: '%s', expected '%f' but got '%f', [%u != %u]\n", msg, expected, yielded, *(unsigned int*)&expected, *(unsigned int*)&yielded);
    } else {
        pass(msg);
    }
}

void pass(char* msg) {
    fprintf(stderr, "PASS: %s\n", msg);
}
