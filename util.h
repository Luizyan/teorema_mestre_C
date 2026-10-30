/*
 * util.h - Funcoes auxiliares compartilhadas pelos tres experimentos.
 *  - agora_ms(): relogio monotonico em milissegundos
 *  - registrar_csv(): acrescenta uma linha ao arquivo CSV comum
 *    (cria o cabecalho se o arquivo ainda nao existir).
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEMPO_MIN_MS 50.0   /* cada medida repete ate somar >= 50 ms */

static double agora_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e3 + t.tv_nsec / 1e6;
}

/* Formato: problema,algoritmo,n,operacoes,tempo_ms,repeticoes */
static void registrar_csv(const char *arq, const char *prob, const char *alg,
                          long n, unsigned long long ops, double ms, int reps) {
    FILE *f = fopen(arq, "r");
    int novo = (f == NULL);
    if (f) fclose(f);
    f = fopen(arq, "a");
    if (!f) { perror("fopen"); exit(1); }
    if (novo) fprintf(f, "problema,algoritmo,n,operacoes,tempo_ms,repeticoes\n");
    fprintf(f, "%s,%s,%ld,%llu,%.6f,%d\n", prob, alg, n, ops, ms, reps);
    fclose(f);
}

#endif
