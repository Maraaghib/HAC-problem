%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *strdup(const char *s);

int arr[10000], length = 0, c, bytesread;
char str[10000];
char* str1;


%}


%%

^v(.)+$ {/*printf("%s", yytext); */strcpy(str, yytext);}
"v"/(.|\n) {}

.|\n {}
%%

/* Translates a Latex character into an Arabic character */


int yywrap() {return 1;}
int main(int argc, char **argv){
    yylex();

    int k = atoi(argv[1]) + 1;
    int n = 0;

    str1 = str+2;
    while (sscanf(str1, "%d%n", &c, &bytesread) > 0) {
        arr[length++] = c;
        str1 += bytesread;
    }
    if(length <= 0)
        exit(EXIT_FAILURE);

    int *vars = malloc((length-1) * sizeof(int));
    int *vertices = malloc((length-1) * sizeof(int));
    int *depths = malloc((length-1) * sizeof(int));
    int *exists = malloc((length-1) * sizeof(int));
    if(vars == NULL || depths == NULL || vertices == NULL || exists == NULL){
        fprintf(stderr, "Not enough memory !\n");
        exit(EXIT_FAILURE);
    }


    for(int i = 0; i < length-1; i++) {
        int v = (arr[i] % k == 0) ? arr[i]/k : arr[i]/k + 1;
        int h = (arr[i] % k == 0) ? (arr[i]/v) - 1: (arr[i] % k) - 1;
        if(arr[i] > 0){
            vars[n] = arr[i];
            vertices[n] = v;
            depths[n] = h;
            n++;
            /**/
        }
    }


    /* Déterminer le nombre d'arêtes du graphe*/
    int size = 0;
    int l = 0;
    bool doublon = false;
    for(int i = n-1; i >= 0; i--) {
        doublon = false;
        for(int t = 0; t < n; t++) {
            if(depths[i] == exists[t])
                doublon = true;
        }
        if(doublon)
            continue;
        for(int j = 0; j < n; j++) {
            if(depths[j] == depths[i] - 1)
                size++;
        }
        exists[l++] = depths[i];
    }

    /* ******************************************** */

    printf("int orderG(){\n\treturn %d;\n}\n\n" ,n);

    printf("int sizeG(){\n\treturn %d;\n}\n\n" ,size);

    printf("int are_adjacent(int u, int v){\n");
    printf("\tif(0 <= u && 0 <= v && u < orderG() && v < orderG()){\n");
    printf("\t\tint depths[%d] = {", n);
    for(int i = 0; i < n-1; i++) {
        printf("%d, ",depths[i]);
    }
    printf("%d};\n", depths[n-1]);
    printf("\t\treturn ((depths[u] - depths[v]) == 1 || (depths[u] - depths[v]) == -1);\n");
    printf("\t}\n");
    printf("\telse\n\t\treturn 0;\n");

    printf("}\n");

    /* ******************************************** */


    free(vars);
    free(vertices);
    free(depths);

    return 0;
}
