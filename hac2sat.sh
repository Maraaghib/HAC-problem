#!/bin/bash

while [[ -z $reponse || $reponse == 'O' ]]; do
    # Afficher tous les graphes
    ls graphs

    # Récupérer le graphes choisi
    echo "Parmi ces graphes ci-dessus, lequel voulez-vous réduire en SAT (sans le .c) ?"
    read graph

    # Tester si le fichier .c du graphe existe
    if [[ ! -e graphs/$graph.c ]]; then
        echo "Ce graphe n'existe pas"
        return
    fi

    # Récupérer la profondeur du graphe
    echo -e "Donnez une profondeur du graphe: \c"
    read k

    # Tester si le caractère saisi est un nombre ou non
    if [[ ! $k =~ ^[0-9]+$ ]]; then
        echo "La profondeur doit être un entier"
        return
    fi

    # Copier le graphe sur lequel on fait le test dans le répertoire courant
    cp graphs/$graph.c ./

    make clean # Nettoyer l'espace de travail
    make # Compiler le programme hac2sat
    rm -f $graph.c # Supprimer la copie du graphe sur lequel nous travaillions
    ./hac2sat $k # Exécuter le programme pour générer le fichier de format DIMACS
    ./glucose-syrup/simp/glucose formule.cnf # Lancer le solveur

    reponse=''
    while [[ $reponse != 'N' && $reponse != 'O' ]]; do
        read -p "Voulez-vous continuer ? (O/N) " reponse
    done
done
