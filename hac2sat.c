#include <stdio.h>
#include <stdlib.h>
#include "all.h"

void usage(char* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {

    int n = orderG(); // Nombre de sommets du graphe
    int m = sizeG(); // Nombre d'arêtes du graphe
    int k = atoi(argv[1]); // La profondeur du graphe

    // Création/Ouverture du fichier
    FILE* fd = NULL;
    fd = fopen("formule.cnf", "w");
    if(fd == NULL)
        usage("Impossible to open the file !");


    // On alloue un tableau de dimension n*k pour stocker les sommets et les profondeurs
    int **X = malloc(n * sizeof(int*));
    if(X == NULL)
        usage("Not enough memory !");

    for (int i = 0; i < n; i++) {
        X[i] = malloc(k * sizeof(int));
        if(X[i] == NULL)
            usage("Not enough memory !");
    }

    // Pour chaque variable, on lui affecte un indice qui lui est unique
    for (int v = 0; v < n; v++) {
        for (int h = 0; h < k; h++) {
            X[v][h] = (k * v) + 1 + h; // Indice de la variable
        }
    }

    /* 1) Pour chaque sommet v ∈ V , il y a un unique entier h t.q. Xv,h est vrai */

    for (int v = 0; v < n; v++) {
        for (int h = 0; h < k; h++) {
            fprintf(fd, "%d ", X[v][h]);
            fprintf(fd, "0\n");
        }

        for (int h = 0; h < k-1; h++) {
            fprintf(fd, "-%d -%d ", X[v][h], X[v][h+1]);
            fprintf(fd, "0\n");
        }
    }


    // Libération des tableaux
    for (int i = 0; i < n; i++) {
        free(X[i]);
    }
    free(X);

    return 0;
}
