#include <stdio.h>
#include <stdlib.h>
// #include "all.h"

void usage(char* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

// BEGIN TEST
int orderG(){
return 20;}

int sizeG(){
return 10;}

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
    int nbvar = n * (k+1);
    int nbclauses = 0;

    // Création/Ouverture du fichier
    FILE* fd = NULL;
    fd = fopen("formule.cnf", "w");
    if(fd == NULL){
        usage("Impossible to open the file !");
    }

    // On alloue un tableau de dimension n*k pour stocker les sommets et les profondeurs
    int **X = malloc((n+1) * sizeof(int*)); // [1...n]
    if(X == NULL){
        usage("Not enough memory !");
    }

    for (int i = 1; i <= n; i++) {
        X[i] = malloc((k+1) * sizeof(int)); // [0...k]
        if(X[i] == NULL){
            usage("Not enough memory !");
        }
    }

    // Pour chaque variable, on lui affecte un indice qui lui est unique
    for (int v = 1; v <= n; v++) {
        for (int h = 0; h <= k; h++) {
            X[v][h] = ((k+1) * (v-1)) + 1 + h; // Indice de la variable
        }
    }

    /* 1) Pour chaque sommet v ∈ V , il y a un unique entier h t.q. Xv,h est vrai */
    fprintf(fd, "c\nc PREMIÈRE CONTRAINTE\nc\np cnf %d 000\n", nbvar);
    // Il existe un sommet de profondeur h
    fprintf(fd, "c\nc Il existe au moins un sommet de profondeur h\nc\n");
    for (int v = 1; v <= n; v++) {
        for (int h = 0; h <= k; h++) {
            fprintf(fd, "%d ", X[v][h]); // Xv,0 V Xv,1 V Xv,2 V ... V Xv,k
        }
        fprintf(fd, "0\n");
        nbclauses++;
    }
    for (int v = 1; v <= n; v++) {
        fprintf(fd, "c\nc Le sommet (%d) ne peut pas avoir deux profondeur différents\nc\n", v);
        // Un sommet ne peut pas avoir deux profondeurs différents
        for (int l = 0; l <= k; l++) {
            for (int h = l; h <= k-1; h++) {
                fprintf(fd, "-%d -%d ", X[v][l], X[v][h+1]); // ~Xv,l /\ ~Xv,h+1
                fprintf(fd, "0\n");
                nbclauses++;
            }
        }
    }

    /* 2) Il y a un unique sommet v t.q. d(v) = 0 (“v est la racine”) */
    fprintf(fd, "c\nc DEUXIÈME CONTRAINTE\nc\n");
    fprintf(fd, "c\nc Il y a au moins un sommet de profondeur 0\nc\n");
    for (int v = 1; v <= n; v++) {
        fprintf(fd, "%d ", X[v][0]);
    }
    fprintf(fd, "0\n");
    nbclauses++;

    fprintf(fd, "c\nc Deux sommets différents ne peuvent pas avoir une profondeur 0\nc\n");
    for (int v = 1; v <= n; v++) {
        for (int l = v; l <= n-1; l++) {
            fprintf(fd, "-%d -%d 0\n", X[v][0], X[l+1][0]);
            nbclauses++;
        }
    }

    /* 3) Il y a au moins un sommet v t.q. d(v) = k. */
    fprintf(fd, "c\nc TROISIÈME CONTRAINTE\nc\n");
    fprintf(fd, "c\nc Il y a au moins un sommet de profondeur k\nc\n");
    for (int v = 1; v <= n; v++) {
        fprintf(fd, "%d ", X[v][k]);
    }
    fprintf(fd, "0\n");
    nbclauses++;

    /*
        4) Pour chaque sommet v, si d(v) > 0, alors il existe un sommet u tel que
         uv ∈ E et d(u) = d(v) − 1 (“le sommet u est un parent potentiel de v dans l’arbre”)
    */
    fprintf(fd, "c\nc QUATRIÈME CONTRAINTE\nc\n");
    // for (int v = 1; v <= n; v++) {
    //     for (int h = 1; h <= k; h++) {
    //         fprintf(fd, "%d\n", X[v][h]);
    //     }
    // }

    for (int v = 1; v <= n; v++) { // Pour chaque sommet v
        for (int h = 1; h <= k; h++) { // Si d(v) > 0
            for (int u = 1; u <= n; u++) { // Il existe un sommet u tel que
                if ((v != u) && are_adjacent((v-1), (u-1)) == 1) { // uv ∈ E
                        fprintf(fd, "%d ", X[u][h-1]);
                }
            }
        }
        fprintf(fd, "0\n");
        nbclauses++;
    }


    // Se positionner au début du fichier pour mettre à jour le nombre de clauses
    rewind(fd);
    fprintf(fd, "c\nc PREMIÈRE CONTRAINTE\nc\np cnf %d %d\n", nbvar, nbclauses);
    printf("%d\n", nbclauses);
    // Libération des tableaux
    for (int i = 0; i < n; i++) {
        free(X[i]);
    }
    free(X);

    fclose(fd);

    return 0;
}
