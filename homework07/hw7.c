#include <stdio.h>
#include <math.h>
#include <gsl/gsl_rng.h>

#define N_SAMPLES 100000

int main(void)
{
    // due stream indipendenti (stesso generatore, seed diversi)
    gsl_rng *rng1 = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng *rng2 = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(rng1, 12345);
    gsl_rng_set(rng2, 67890);

    // 1) lancio di monete
    FILE *f = fopen("output/coin.csv", "w");
    if (f == NULL)
    {
        printf("Errore: non riesco a scrivere in output/ (esiste la cartella?)\n");
        return 1;
    }
    fprintf(f, "n,fraction_heads\n");
    int teste = 0;
    printf("1) Lancio di monete\n");
    for (int n = 1; n <= N_SAMPLES; n++)
    {
        if (gsl_rng_uniform(rng1) < 0.5)
            teste++;
        double frazione = (double)teste / n;
        fprintf(f, "%d,%.6f\n", n, frazione);
        if (n == 10 || n == 100 || n == 1000 || n == 10000 || n == 100000)
            printf("   n = %6d   frazione di teste = %.5f\n", n, frazione);
    }
    fclose(f);

    // 2) stima di pi: x dal primo stream, y dal secondo
    f = fopen("output/pi.csv", "w");
    fprintf(f, "N,pi_estimate,abs_error\n");
    printf("2) Stima di pi\n");
    for (long N = 10; N <= 10000000; N *= 10)
    {
        long dentro = 0;
        for (long i = 0; i < N; i++)
        {
            double x = gsl_rng_uniform(rng1);
            double y = gsl_rng_uniform(rng2);
            if (x*x + y*y <= 1.0)
                dentro++;
        }
        double stima = 4.0 * dentro / N;
        fprintf(f, "%ld,%.10f,%.10f\n", N, stima, fabs(stima - M_PI));
        printf("   N = %8ld   pi = %.6f   errore = %.2e\n", N, stima, fabs(stima - M_PI));
    }
    fclose(f);

    // 3) Y = U^2   e   4) esponenziale con Y = -ln(1-U)/lambda
    double lambda = 1.5;
    f = fopen("output/samples.csv", "w");
    fprintf(f, "u_squared,exponential\n");
    for (int i = 0; i < N_SAMPLES; i++)
    {
        double u = gsl_rng_uniform(rng1);
        double y = u * u;
        double e = -log(1.0 - gsl_rng_uniform(rng1)) / lambda;
        fprintf(f, "%.10f,%.10f\n", y, e);
    }
    fclose(f);
    printf("3-4) %d campioni di U^2 e dell'esponenziale salvati in output/samples.csv\n", N_SAMPLES);

    gsl_rng_free(rng1);
    gsl_rng_free(rng2);
    return 0;
}
