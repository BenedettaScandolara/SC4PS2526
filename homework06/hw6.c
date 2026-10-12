#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <hdf5.h>
#include "config-parser/parser.h"

int main(int argc, char *argv[])
{
    char *filein = (argc > 1) ? argv[1] : "input.txt";

    // valori letti dal file di input
    int N = 0;
    int chunk_size = 0;
    double a = 0.0, xValue = 0.0, yValue = 0.0;
    char fileout[CONFIG_ARG_MAX_BYTES] = "output/chunks.h5";

    config_option_t co = read_config_file(filein);
    if (co == NULL)
    {
        printf("Errore: non riesco a leggere %s\n", filein);
        return 1;
    }
    while (co != NULL)
    {
        if (strcmp(co->key, "N") == 0) N = atoi(co->value);
        else if (strcmp(co->key, "chunk_size") == 0) chunk_size = atoi(co->value);
        else if (strcmp(co->key, "a") == 0) a = atof(co->value);
        else if (strcmp(co->key, "x") == 0) xValue = atof(co->value);
        else if (strcmp(co->key, "y") == 0) yValue = atof(co->value);
        else if (strcmp(co->key, "output_file") == 0) strcpy(fileout, co->value);
        co = co->prev;
    }

    if (N <= 0 || chunk_size <= 0)
    {
        printf("Errore: N e chunk_size devono essere > 0\n");
        return 1;
    }

    double *x = malloc(N * sizeof(double));
    double *y = malloc(N * sizeof(double));
    double *d = malloc(N * sizeof(double));        // codice originale
    double *dChunk = malloc(N * sizeof(double));   // codice a chunk
    if (x == NULL || y == NULL || d == NULL || dChunk == NULL)
    {
        printf("Errore: memoria insufficiente per N = %d\n", N);
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        x[i] = xValue;
        y[i] = yValue;
    }

    // 0) codice originale: un solo for
    double sommaOriginale = 0.0;
    for (int i = 0; i < N; i++)
    {
        d[i] = a * x[i] + y[i];
        sommaOriginale += d[i];
    }

    // 1) codice a chunk
    int numero_chunk = (N + chunk_size - 1) / chunk_size;   // divisione arrotondata per eccesso
    double *partial_chunk_sum = malloc(numero_chunk * sizeof(double));

    for (int c = 0; c < numero_chunk; c++)
    {
        int inizio = c * chunk_size;
        int fine = inizio + chunk_size;    // escluso
        if (fine > N)
            fine = N;

        partial_chunk_sum[c] = 0.0;
        for (int i = inizio; i < fine; i++)
        {
            dChunk[i] = a * x[i] + y[i];
            partial_chunk_sum[c] += dChunk[i];
        }
    }

    // 2) somma delle somme parziali
    double sommaChunk = 0.0;
    for (int c = 0; c < numero_chunk; c++)
        sommaChunk += partial_chunk_sum[c];

    // controlli
    int diversi = 0;
    for (int i = 0; i < N; i++)
        if (dChunk[i] != d[i])
            diversi++;

    printf("N = %d, chunk_size = %d, numero di chunk = %d\n", N, chunk_size, numero_chunk);
    printf("Controllo 1: elementi di d diversi dal codice originale: %d su %d\n", diversi, N);
    printf("Controllo 2: somma originale = %.17g\n", sommaOriginale);
    printf("             somma dei chunk = %.17g\n", sommaChunk);
    printf("             uguali con ==: %s\n", sommaChunk == sommaOriginale ? "SI" : "NO");
    printf("             differenza relativa = %.3g\n", fabs(sommaChunk - sommaOriginale) / fabs(sommaOriginale));

    // 4) salvataggio in HDF5: un dataset per ogni chunk + le somme parziali
    hid_t file = H5Fcreate(fileout, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file < 0)
    {
        printf("Errore: non riesco a creare %s (esiste la cartella?)\n", fileout);
        return 1;
    }
    hid_t gruppo = H5Gcreate(file, "chunks", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    for (int c = 0; c < numero_chunk; c++)
    {
        int inizio = c * chunk_size;
        int fine = (inizio + chunk_size > N) ? N : inizio + chunk_size;
        hsize_t dim = fine - inizio;
        char nome[50];
        sprintf(nome, "chunk_%03d", c);

        hid_t spazio = H5Screate_simple(1, &dim, NULL);
        hid_t dataset = H5Dcreate(gruppo, nome, H5T_NATIVE_DOUBLE, spazio, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &dChunk[inizio]);
        H5Dclose(dataset);
        H5Sclose(spazio);
    }

    hsize_t dimSomme = numero_chunk;
    hid_t spazio = H5Screate_simple(1, &dimSomme, NULL);
    hid_t dataset = H5Dcreate(file, "partial_chunk_sum", H5T_NATIVE_DOUBLE, spazio, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, partial_chunk_sum);
    H5Dclose(dataset);
    H5Sclose(spazio);

    H5Gclose(gruppo);
    H5Fclose(file);
    printf("Chunk e somme parziali salvati in %s\n", fileout);

    free(x);
    free(y);
    free(d);
    free(dChunk);
    free(partial_chunk_sum);
    return 0;
}
