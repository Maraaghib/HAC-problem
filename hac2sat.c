#include <stdio.h>
#include <stdlib.h>
// #include "all.h"

void usage(char* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

// BEGIN TEST
int orderG(){
    return 20;
}

int sizeG(){
    return 10;
}

int are_adjacent(int u, int v){
    if(0<= u && 0<=v && u<orderG() && v<orderG()){
        return (u+v==19);
    }
    else return 0;
}
// END TEST


int main(int argc, char const *argv[]) {

    int n = orderG(); // Nombre de sommets du graphe
    int m = sizeG(); // Nombre d'arêtes du graphe
    int k = atoi(argv[1]); // La profondeur du graphe
    int nbvar = n * k;
    int nbclauses = 0;

    // Création/Ouverture du fichier
    FILE* fd = NULL;
    fd = fopen("formule.cnf", "w");
    if(fd == NULL){
        usage("Impossible to open the file !");
    }


    // On alloue un tableau de dimension n*k pour stocker les sommets et les profondeurs
    int **X = malloc(n * sizeof(int*));
    if(X == NULL){
        usage("Not enough memory !");
    }

    for (int i = 0; i < n; i++) {
        X[i] = malloc(k * sizeof(int));
        if(X[i] == NULL){
            usage("Not enough memory !");
        }
    }

    // Pour chaque variable, on lui affecte un indice qui lui est unique
    for (int v = 0; v < n; v++) {
        for (int h = 0; h < k; h++) {
            X[v][h] = (k * v) + 1 + h; // Indice de la variable
        }
    }

    fprintf(fd, "c\nc Première contrainte\nc\np cnf %d 000\n", nbvar);


    /* 1) Pour chaque sommet v ∈ V , il y a un unique entier h t.q. Xv,h est vrai */

    // Il existe un sommet de profondeur h
    for (int v = 0; v < n; v++) {
        for (int h = 0; h < k; h++) {
            fprintf(fd, "%d ", X[v][h]); // Xv,0 V Xv,1 V Xv,2 V ... V Xv,k
        }
        fprintf(fd, "0\n");
        nbclauses++;
    }
    for (int v = 0; v < n; v++) {
        fprintf(fd, "c Le sommet (%d) ne peut pas avoir deux profondeur différents\n", v);
        // Un sommet ne peut pas avoir deux profondeurs différents
        for (int l = 0; l < k; l++) {
            for (int h = l; h < k-1; h++) {
                fprintf(fd, "-%d -%d ", X[v][l], X[v][h+1]); // ~Xv,l /\ ~Xv,h+1
                fprintf(fd, "0\n");
                nbclauses++;
            }
        }
    }

    /* 2) Il y a un unique sommet v t.q. d(v) = 0 (“v est la racine”) */
    for (int v = 0; v < n; v++) {
        fprintf(fd, "%d ", X[v][0]);
        fprintf(fd, "0\n");
        nbclauses++;
    }

    rewind(fd);
    fprintf(fd, "c\nc Première contrainte\nc\np cnf %d %d\n", nbvar, nbclauses);
    printf("%d\n", nbclauses);
    // Libération des tableaux
    for (int i = 0; i < n; i++) {
        free(X[i]);
    }
    free(X);

    fclose(fd);

    return 0;
}
