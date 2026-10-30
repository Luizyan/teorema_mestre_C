/*
 * Problema 2 - Merge sort com divisao em TRES partes
 *
 * Recorrencia:  T(n) = 3 T(n/3) + Theta(n)         (Caso 2 do Teorema Mestre)
 *   - 3 subvetores de tamanho n/3 ordenados recursivamente (a = b = 3);
 *   - intercalacao dos 3 subvetores ordenados em tempo linear.
 * Resultado esperado: Theta(n log n).
 *
 * "operacoes" = iteracoes do laco de intercalacao (um elemento escrito por iteracao).
 *
 * Uso: ./p2_mergesort3 [arquivo.csv] [k_max]   (n = 3^1, 3^2, ..., 3^k_max)
 */
#include "util.h"

static unsigned long long ops;

/* Intercala v[l,m1), v[m1,m2) e v[m2,r) (cada um ja ordenado). */
static void merge3(int *v, int *aux, int l, int m1, int m2, int r) {
    int i = l, j = m1, k = m2;
    for (int o = l; o < r; o++) {
        int pi = (i < m1), pj = (j < m2), pk = (k < r);
        int escolhe;                      /* 0, 1 ou 2: de qual parte sai o menor */
        if (pi && (!pj || v[i] <= v[j]) && (!pk || v[i] <= v[k])) escolhe = 0;
        else if (pj && (!pk || v[j] <= v[k]))                      escolhe = 1;
        else                                                       escolhe = 2;
        aux[o] = (escolhe == 0) ? v[i++] : (escolhe == 1) ? v[j++] : v[k++];
        ops++;                            /* f(n) = Theta(n) */
    }
    memcpy(v + l, aux + l, (size_t)(r - l) * sizeof(int));
}

/* Ordena v[l, r). */
static void msort3(int *v, int *aux, int l, int r) {
    int n = r - l;
    if (n <= 1) return;                   /* caso base: T(1) = 0 */
    int t = (n + 2) / 3;                  /* = n/3 quando n e potencia de 3 */
    int m1 = l + t, m2 = l + 2 * t;
    if (m2 > r) m2 = r;
    msort3(v, aux, l, m1);                /* 3 chamadas recursivas: a = 3, b = 3 */
    msort3(v, aux, m1, m2);
    msort3(v, aux, m2, r);
    merge3(v, aux, l, m1, m2, r);
}

int main(int argc, char **argv) {
    const char *arq = argc > 1 ? argv[1] : "resultados.csv";
    int kmax = argc > 2 ? atoi(argv[2]) : 14;
    srand(12345);

    long n = 1;
    for (int k = 1; k <= kmax; k++) {
        n *= 3;
        int *orig = malloc(n * sizeof(int)), *v = malloc(n * sizeof(int)),
            *aux = malloc(n * sizeof(int));
        for (long i = 0; i < n; i++) orig[i] = rand();

        int reps = 0; double total = 0;
        do {
            memcpy(v, orig, n * sizeof(int));
            ops = 0;
            double t0 = agora_ms();
            msort3(v, aux, 0, (int)n);
            total += agora_ms() - t0;
            reps++;
        } while (total < TEMPO_MIN_MS);

        for (long i = 1; i < n; i++)      /* validacao: vetor ordenado */
            if (v[i - 1] > v[i]) { fprintf(stderr, "ERRO n=%ld\n", n); return 1; }
        registrar_csv(arq, "P2", "mergesort_3_vias", n, ops, total / reps, reps);
        printf("P2 n=%ld ops=%llu tempo=%.4f ms (reps=%d)\n", n, ops, total / reps, reps);
        free(orig); free(v); free(aux);
    }
    return 0;
}
