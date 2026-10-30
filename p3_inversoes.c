/*
 * Problema 3 - Contagem de inversoes com combinacao quadratica
 *
 * Uma inversao e um par (i, j), i < j, com v[i] > v[j].
 * Divide-se o vetor em duas metades, contam-se as inversoes de cada metade
 * recursivamente e, na combinacao, comparam-se TODOS os pares (i na esquerda,
 * j na direita):
 *
 *   T(n) = 2 T(n/2) + Theta(n^2)                   (Caso 3 do Teorema Mestre)
 *
 * Resultado esperado: Theta(n^2).
 * "operacoes" = comparacoes v[i] > v[j] realizadas.
 *
 * Uso: ./p3_inversoes [arquivo.csv] [k_max]   (n = 2^1, 2^2, ..., 2^k_max)
 */
#include "util.h"

static unsigned long long ops;

static long long conta(const int *v, int n) {
    if (n <= 1) return 0;                 /* caso base: T(1) = 0 */
    int h = n / 2;
    /* a = 2 chamadas recursivas, tamanho n/2 (b = 2) */
    long long inv = conta(v, h) + conta(v + h, n - h);
    /* combinacao: h * (n-h) = n^2/4 comparacoes -> f(n) = Theta(n^2) */
    for (int i = 0; i < h; i++)
        for (int j = h; j < n; j++) {
            ops++;
            if (v[i] > v[j]) inv++;
        }
    return inv;
}

/* Referencia por forca bruta (validacao). */
static long long bruta(const int *v, int n) {
    long long inv = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (v[i] > v[j]) inv++;
    return inv;
}

int main(int argc, char **argv) {
    const char *arq = argc > 1 ? argv[1] : "resultados.csv";
    int kmax = argc > 2 ? atoi(argv[2]) : 16;
    srand(12345);

    for (int k = 1; k <= kmax; k++) {
        long n = 1L << k;
        int *v = malloc(n * sizeof(int));
        for (long i = 0; i < n; i++) v[i] = rand();
        long long res = 0;
        int reps = 0; double total = 0;
        do {
            ops = 0;
            double t0 = agora_ms();
            res = conta(v, (int)n);
            total += agora_ms() - t0;
            reps++;
        } while (total < TEMPO_MIN_MS);
        if (n <= 4096 && res != bruta(v, (int)n)) {
            fprintf(stderr, "ERRO n=%ld\n", n); return 1;
        }
        registrar_csv(arq, "P3", "inversoes_quadratico", n, ops, total / reps, reps);
        printf("P3 n=%ld ops=%llu inv=%lld tempo=%.4f ms (reps=%d)\n",
               n, ops, res, total / reps, reps);
        free(v);
    }
    return 0;
}
