#!/bin/bash

while [[ -z $reponse || $reponse == 'O' ]]; do
    # Afficher tous les graphes
    ls graphs

    # Récupérer le graphes choisi
    echo "Parmi ces graphes ci-dessus, lequel voulez-vous réduire en SAT (sans le .c) ?"
    read graph

    # Tester si le fichier .c du graphe existe
    while [[ ! -e graphs/$graph.c ]]; do
        echo "Ce graphe n'existe pas. veuillez réessayer SVP"
        read graph
    done

    # Récupérer la profondeur du graphe
    echo -e "Donnez une profondeur du graphe: \c"
    read k

    # Tester si le caractère saisi est un nombre ou non
    while [[ ! $k =~ ^[0-9]+$ ]]; do
        echo "La profondeur doit être un entier"
        read k
    done

    # Copier le graphe sur lequel on fait le test dans le répertoire courant
    cp graphs/$graph.c ./

    make clean # Nettoyer l'espace de travail
    make # Compiler le programme hac2sat
    rm -f $graph.c # Supprimer la copie du graphe sur lequel nous travaillions
    #./hac2sat $k | tail -n 1 | grep -q 'E' && echo "Y a un problème" # Exécuter le programme pour générer le fichier de format DIMACS
    TEST_OUT=$(./hac2sat $k 2>&1)
    echo $TEST_OUT
    echo $TEST_OUT | grep -v 'Usage' && ./glucose-syrup/simp/glucose formule.cnf # Lancer le solveur s'il n'y a pas erreur d'entrée


    reponse=''
    while [[ $reponse != 'N' && $reponse != 'O' ]]; do
        read -p "Voulez-vous continuer ? (O/N) " reponse
    done
done
