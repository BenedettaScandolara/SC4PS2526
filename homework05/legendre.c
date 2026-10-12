#include <stdio.h>

#define L 50

// forward (Bonnet): P_{l+1} = (2l+1)/(l+1) x P_l - l/(l+1) P_{l-1}
void forward(double x, double *P)
{
    P[0] = 1.0;
    P[1] = x;
    for (int l = 1; l < L; l++)
        P[l+1] = (2.0*l + 1.0) / (l + 1.0) * x * P[l] - l / (l + 1.0) * P[l-1];
}

// backward (Miller): P_{l-1} = (2l+1)/l x P_l - (l+1)/l P_{l+1}
// partenza arbitraria P_{L+1} = 0, P_L = 1, poi riscalo con c = 1/P_0
void backward(double x, double *P)
{
    double Ptilde[L + 2];
    Ptilde[L+1] = 0.0;
    Ptilde[L] = 1.0;
    for (int l = L; l >= 1; l--)
        Ptilde[l-1] = (2.0*l + 1.0) / l * x * Ptilde[l] - (l + 1.0) / l * Ptilde[l+1];

    double c = 1.0 / Ptilde[0];
    for (int l = 0; l <= L; l++)
        P[l] = c * Ptilde[l];
}

int main(void)
{
    double xValori[4] = {0.1, 0.5, 0.9, 0.99};
    double Pforward[L + 1];
    double Pbackward[L + 1];

    for (int n = 0; n < 4; n++)
    {
        double x = xValori[n];
        forward(x, Pforward);
        backward(x, Pbackward);

        char nomefile[100];
        sprintf(nomefile, "output/legendre_x%g.csv", x);
        FILE *f = fopen(nomefile, "w");
        if (f == NULL)
        {
            printf("Errore: non riesco ad aprire %s (esiste la cartella output?)\n", nomefile);
            return 1;
        }

        fprintf(f, "l,forward,backward\n");
        for (int l = 0; l <= L; l++)
            fprintf(f, "%d,%.17e,%.17e\n", l, Pforward[l], Pbackward[l]);
        fclose(f);

        printf("x = %g: scritto %s\n", x, nomefile);
    }

    return 0;
}
