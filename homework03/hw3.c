#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// le matrici sono salvate riga per riga: M[i][j] = M[i*N + j]

void azzera(double *C, int N)
{
    for (int i = 0; i < N * N; i++)
        C[i] = 0.0;
}

void matmul_ijk(double *A, double *B, double *C, int N)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}

void matmul_ikj(double *A, double *B, double *C, int N)
{
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++)
            for (int j = 0; j < N; j++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}

void matmul_jik(double *A, double *B, double *C, int N)
{
    for (int j = 0; j < N; j++)
        for (int i = 0; i < N; i++)
            for (int k = 0; k < N; k++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}

void matmul_jki(double *A, double *B, double *C, int N)
{
    for (int j = 0; j < N; j++)
        for (int k = 0; k < N; k++)
            for (int i = 0; i < N; i++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}

void matmul_kij(double *A, double *B, double *C, int N)
{
    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}

void matmul_kji(double *A, double *B, double *C, int N)
{
    for (int k = 0; k < N; k++)
        for (int j = 0; j < N; j++)
            for (int i = 0; i < N; i++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}

// controllo completo: diagonale = a*b, fuori diagonale = 0
int controllo_completo(double *C, int N, double ab)
{
    int sbagliati = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            double atteso = (i == j) ? ab : 0.0;
            if (C[i*N + j] != atteso)
                sbagliati++;
        }
    return sbagliati;
}

// controllo veloce: traccia e somma di tutti gli elementi devono essere N*a*b
int controllo_veloce(double *C, int N, double ab)
{
    double traccia = 0.0;
    double somma = 0.0;
    for (int i = 0; i < N; i++)
        traccia += C[i*N + i];
    for (int i = 0; i < N * N; i++)
        somma += C[i];
    return (traccia == N * ab && somma == N * ab);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Uso: %s a b N fileout\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int N = atoi(argv[3]);
    char *fileout = argv[4];

    double *A = malloc(N * N * sizeof(double));
    double *B = malloc(N * N * sizeof(double));
    double *C = malloc(N * N * sizeof(double));

    if (A == NULL || B == NULL || C == NULL)
    {
        printf("Errore: memoria insufficiente per N = %d\n", N);
        return 1;
    }

    // A = a * identita', B = b * identita'
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            A[i*N + j] = (i == j) ? a : 0.0;
            B[i*N + j] = (i == j) ? b : 0.0;
        }

    void (*metodi[6])(double *, double *, double *, int) =
        {matmul_ijk, matmul_ikj, matmul_jik, matmul_jki, matmul_kij, matmul_kji};
    char *nomi[6] = {"ijk", "ikj", "jik", "jki", "kij", "kji"};

    printf("N = %d, a = %g, b = %g\n", N, a, b);
    printf("ordine   tempo (s)   sbagliati   controllo veloce\n");

    for (int m = 0; m < 6; m++)
    {
        azzera(C, N);

        clock_t inizio = clock();
        metodi[m](A, B, C, N);
        clock_t fine = clock();
        double tempo = (double)(fine - inizio) / CLOCKS_PER_SEC;

        int sbagliati = controllo_completo(C, N, a * b);
        int ok = controllo_veloce(C, N, a * b);

        printf("%s      %8.3f    %8d    %s\n", nomi[m], tempo, sbagliati, ok ? "OK" : "SBAGLIATO");
    }

    FILE *f = fopen(fileout, "w");
    if (f == NULL)
    {
        printf("Errore: non riesco ad aprire %s\n", fileout);
        return 1;
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            fprintf(f, "%g ", C[i*N + j]);
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Matrice C salvata in %s\n", fileout);

    free(A);
    free(B);
    free(C);

    return 0;
}
