/*
 * Problema 1 - Multiplicacao recursiva de matrizes (divisao e conquista)
 *
 * Recorrencia:  T(n) = 8 T(n/2) + Theta(n^2)      (Caso 1 do Teorema Mestre)
 *   - 8 submultiplicacoes de matrizes (n/2 x n/2);
 *   - combinacao: soma de matrizes n x n (Theta(n^2)).
 * Resultado esperado: Theta(n^3).
 *
 * "operacoes" = multiplicacoes escalares + somas escalares realizadas.
 *
 * Uso: ./p1_matmul [arquivo.csv] [n_max]     (n = 2, 4, 8, ..., n_max)
 */
#include <math.h>
#include "util.h"

static unsigned long long ops;           /* contador de operacoes aritmeticas */

/*
 * C = A * B para submatrizes n x n (n potencia de 2).
 * la, lb, lc: "leading dimension" (passo entre linhas) de A, B e C.
 */
static void mul(const double *A, const double *B, double *C,
                int n, int la, int lb, int lc) {
    if (n == 1) {                        /* caso base: T(1) = 1 */
        C[0] = A[0] * B[0];
        ops++;
        return;
    }
    int h = n / 2;
    int lt = n;
    double *T = malloc((size_t)n * n * sizeof(double));   /* produtos auxiliares */
    if (!T) { fprintf(stderr, "sem memoria\n"); exit(1); }

    const double *A11 = A,           *A12 = A + h;
    const double *A21 = A + h * la,  *A22 = A + h * la + h;
    const double *B11 = B,           *B12 = B + h;
    const double *B21 = B + h * lb,  *B22 = B + h * lb + h;
    double *C11 = C,           *C12 = C + h;
    double *C21 = C + h * lc,  *C22 = C + h * lc + h;
    double *T11 = T,           *T12 = T + h;
    double *T21 = T + h * lt,  *T22 = T + h * lt + h;

    /* 8 chamadas recursivas: a = 8, subproblemas de tamanho n/2 (b = 2) */
    mul(A11, B11, C11, h, la, lb, lc);  mul(A12, B21, T11, h, la, lb, lt);
    mul(A11, B12, C12, h, la, lb, lc);  mul(A12, B22, T12, h, la, lb, lt);
    mul(A21, B11, C21, h, la, lb, lc);  mul(A22, B21, T21, h, la, lb, lt);
    mul(A21, B12, C22, h, la, lb, lc);  mul(A22, B22, T22, h, la, lb, lt);

    /* combinacao: C += T  ->  n^2 somas, f(n) = Theta(n^2) */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            C[i * lc + j] += T[i * lt + j];
            ops++;
        }
    free(T);
}

/* Multiplicacao iterativa (so para validar a correcao). */
static void mul_ref(const double *A, const double *B, double *C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double s = 0;
            for (int k = 0; k < n; k++) s += A[i * n + k] * B[k * n + j];
            C[i * n + j] = s;
        }
}

int main(int argc, char **argv) {
    const char *arq = argc > 1 ? argv[1] : "resultados.csv";
    int nmax = argc > 2 ? atoi(argv[2]) : 512;
    srand(12345);

    for (int n = 2; n <= nmax; n *= 2) {
        double *A = malloc((size_t)n * n * sizeof(double));
        double *B = malloc((size_t)n * n * sizeof(double));
        double *C = malloc((size_t)n * n * sizeof(double));
        for (int i = 0; i < n * n; i++) { A[i] = rand() / (double)RAND_MAX;
                                          B[i] = rand() / (double)RAND_MAX; }
        /* validacao (n pequeno) */
        if (n <= 64) {
            double *R = malloc((size_t)n * n * sizeof(double));
            mul(A, B, C, n, n, n, n);
            mul_ref(A, B, R, n);
            double erro = 0;
            for (int i = 0; i < n * n; i++) erro = fmax(erro, fabs(C[i] - R[i]));
            if (erro > 1e-9) { fprintf(stderr, "ERRO n=%d (%g)\n", n, erro); return 1; }
            free(R);
        }
        int reps = 0; double total = 0;
        do {
            ops = 0;
            double t0 = agora_ms();
            mul(A, B, C, n, n, n, n);
            total += agora_ms() - t0;
            reps++;
        } while (total < TEMPO_MIN_MS);
        registrar_csv(arq, "P1", "matmul_recursiva", n, ops, total / reps, reps);
        printf("P1 n=%d ops=%llu tempo=%.4f ms (reps=%d)\n", n, ops, total / reps, reps);
        free(A); free(B); free(C);
    }
    return 0;
}
