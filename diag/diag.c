#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avalam.h"
#include "topologie.h"

#define DEFAULT_NAME "diag.js"
#define DEFAULT_NOTE ""
#define MAX_FEN 100

#define U 1
#define D 2
#define T 3
#define Q 4
#define C 5
#define BONUS 0
#define MALUS 1

void placer_colonne(T_Position *p, octet col, octet nb, octet c);
void placer_evolution(T_Position *p, octet col, octet type, octet c);
int main(int argc, char *argv[]){
    T_Position plateau;
    
    char fen[MAX_FEN];
    int diag_id; // numéro du diagramme
    octet j= 0; // compteur pour parcourir le fen
    octet i = 0; // compteur pour parcourir le plateau
    octet nb_evolution; // pour compter le nombre de bonus 
    
    // vérification du nombre d'arguments
    if(argc != 3) {
        return printf("Argument invalide ! \n %s <Numero diagramme> <code fen> \n", argv[0]);
        return 1;
    }
    // récupération des arguments
    else {
        diag_id = atoi(argv[1]);
        strcpy(fen, argv[2]);
    }
    // parcours du fen
    while(fen[j]) {
        // TODO reflechir à comment faire les vérifications
        switch(fen[j]){
            // cas pour les pions jaunes
            case 'u':
                placer_colonne(&plateau, i, U, JAU);
                i++; 
                break;
            
            case 'd':
                placer_colonne(&plateau, i, D, JAU);
                i++;
                break;
                
            case 't':
                placer_colonne(&plateau, i, T, JAU);
                i++;
                break;
            
            case 'q':
                placer_colonne(&plateau, i, Q, JAU);
                i++;
                break;
            
            case 'c':
                placer_colonne(&plateau, i, C, JAU);
                i++;
                break;
                
            case 'b':
                placer_evolution(&plateau, i-1, BONUS, JAU);
                nb_evolution++;
                break;
                
            case 'm':
                placer_evolution(&plateau, i-1, MALUS, JAU);
                nb_evolution++;
                break;
            
            // cas pour les pion Rouge
            case 'U':
                placer_colonne(&plateau, i, U, ROU);
                i++;
                break;
            
            case 'D':
                placer_colonne(&plateau, i, D, ROU);
                i++;
                break;
            
            case 'T':
                placer_colonne(&plateau, i, T, ROU);
                i++;
                break;    
            
            case 'Q':
                placer_colonne(&plateau, i, Q, ROU);
                i++;
                break; 
            
            case 'C':
                placer_colonne(&plateau, i, C, ROU);
                i++;
                break;
            
            case 'B':
                 placer_evolution(&plateau, i-1, BONUS, ROU);
                 nb_evolution++;
                 break;
            
            case 'M':
                 placer_evolution(&plateau, i-1, MALUS, ROU);
                 nb_evolution++;
                 break;
            
            //TODO cas pour le trait (caractère après l'espace)
            case ' ':
                break;
            
            //TODO Caractère inconnu ou gestion du nombre de case vide ??
            default:
                break;
            
        }
        j++;
    }
    for(i; i < NBCASES; i++) placer_colonne(&plateau, i, VIDE, VIDE);
    afficherPosition(plateau); // à retirer
}

void placer_colonne(T_Position *p,octet col, octet nb, octet c){
    // tu lui donnes le plateau, la colonne, sa valeur, et la couleur et elle te place le pion
    // passage par adresse donc p est modifié
    p->cols[col].nb = nb;
    p->cols[col].couleur = c;
    printf3("Placement [%d | [%s : %d]\n", col, COLNAME(c), nb); // printf pour la version debug
}

void placer_evolution(T_Position *p, octet col, octet type, octet c) {
    // tu lui donnes le plateau, la colonne, et le type (BONUS ou MALUS) et elle te place le pion évolution
    // passage par adresse donc p est modifié
    if(type == MALUS){
        //On place notre pion evolution dans le plateau selon sa couleur
        if(c == JAU) p->evolution.malusJ = col;
        else if(c == ROU) p->evolution.malusR = col;
    }
    // Sinon c'est un bonus
    else {
        //On place notre pion evolution dans le plateau selon sa couleur
        if(c == JAU) p->evolution.bonusJ = col;
        else if(c == ROU) p->evolution.bonusR = col;
    }
}
