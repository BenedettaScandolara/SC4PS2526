#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Uso: %s N a x y valore_atteso\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    double a = atof(argv[2]);
    double xValue = atof(argv[3]);
    double yValue = atof(argv[4]);

    double ValoreAtteso = atof(argv[5]);

    double *x = malloc(N * sizeof(double));
    double *y = malloc(N * sizeof(double));
    double *d = malloc(N * sizeof(double));

    if (x == NULL || y == NULL || d == NULL)
    {
        printf("Errore: memoria insufficiente per N = %d\n", N);
        free(x);
        free(y);
        free(d);
        return 1;
    }

    int i;
    for (i = 0; i < N; i++)
    {
        x[i] = xValue;
        y[i] = yValue;
        d[i] = a * x[i] + y[i];
    }

    // confronto esatto (==) e confronto con tolleranza
    double tolleranza = 1e-9;
    int sbagliatiEsatto = 0;
    int sbagliatiTolleranza = 0;

    for (i = 0; i < N; i++)
    {
        if (d[i] != ValoreAtteso)
        {
            sbagliatiEsatto++;
        }
        if (fabs(d[i] - ValoreAtteso) > tolleranza)
        {
            sbagliatiTolleranza++;
        }
    }

    printf("N = %d, a = %g, x = %g, y = %g\n", N, a, xValue, yValue);
    printf("Valore atteso : %.20f\n", ValoreAtteso);
    printf("d[0]          : %.20f\n", d[0]);
    printf("Confronto esatto (==)          : %d sbagliati su %d\n", sbagliatiEsatto, N);
    printf("Confronto con tolleranza 1e-9  : %d sbagliati su %d\n", sbagliatiTolleranza, N);

    free(x);
    free(y);
    free(d);

    return 0;
}
