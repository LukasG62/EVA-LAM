#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "avalam.h"
#include "topologie.h"

#define DEFAULT_NAME "diag.js"
#define DEFAULT_NOTE ""
#define MAXCOMM 500
#define MAX_FEN 100
#define MAX_FENNBVIDE 4

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
    char fen_nbVide[MAX_FENNBVIDE] = ""; // Nombre de case vide indiqué dans le fen
    int diag_id; // numéro du diagramme
    octet fenpos= 0; // compteur pour parcourir le fen
    octet col = 0; // compteur pour parcourir le plateau
    octet i;
    char ans;
    char *comm[MAXCOMM+1];
    
    // Initialisation des pions évolutions
	plateau.evolution.bonusJ = UNKNOWN;
	plateau.evolution.bonusR = UNKNOWN;
	plateau.evolution.malusJ = UNKNOWN;
	plateau.evolution.malusR = UNKNOWN;

    
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
    while(fen[fenpos]) { //on continue tant que l'on a pas croiser la section dédiée au trait (gestion après la boucle while)
        // TODO reflechir à comment faire les vérifications
        switch(fen[fenpos]){
            // cas pour les pions jaunes
            case 'u':
                placer_colonne(&plateau, col, U, JAU);
                col++; 
                break;
            
            case 'd':
                placer_colonne(&plateau, col, D, JAU);
                col++;
                break;
                
            case 't':
                placer_colonne(&plateau, col, T, JAU);
                col++;
                break;
            
            case 'q':
                placer_colonne(&plateau, col, Q, JAU);
                col++;
                break;
            
            case 'c':
                placer_colonne(&plateau, col, C, JAU);
                col++;
                break;
                
            case 'b':
                placer_evolution(&plateau, col-1, BONUS, JAU);
                break;
                
            case 'm':
                placer_evolution(&plateau, col-1, MALUS, JAU);
                break;
            
            // cas pour les pion Rouge
            case 'U':
                placer_colonne(&plateau, col, U, ROU);
                col++;
                break;
            
            case 'D':
                placer_colonne(&plateau, col, D, ROU);
                col++;
                break;
            
            case 'T':
                placer_colonne(&plateau, col, T, ROU);
                col++;
                break;    
            
            case 'Q':
                placer_colonne(&plateau, col, Q, ROU);
                col++;
                break; 
            
            case 'C':
                placer_colonne(&plateau, col, C, ROU);
                col++;
                break;
            
            case 'B':
                 placer_evolution(&plateau, col-1, BONUS, ROU);
                 break;
            
            case 'M':
                 placer_evolution(&plateau, col-1, MALUS, ROU);
                 break;
            
            case '0' ... '9': // Une implémentation possible (pas ouf je trouve)
                strncat(fen_nbVide, &fen[fenpos], 1);
                printf("%s \n ", fen_nbVide);
                
                // Si le suivant n'est pas un chiffre
                if(!isdigit(fen[fenpos+1])) {
                    for(i = 0; i < atoi(fen_nbVide) && col < NBCASES; i++) {
                        placer_colonne(&plateau, col, VIDE, VIDE);
                        col++;
                    }
                    fen_nbVide[0] = '\0';
                }
                break;
                
                
            case ' ':
                if (fen[fenpos+1] != 'j'  && fen[fenpos+1]!= 'r') 
			break; // si l'espace n'est pas celui du trait
                
                if (fen[fenpos+1] == 'r') // si le trait est au rouge
                    plateau.trait = ROU ;
                else // il est forcément jaune (cas d'erreur mettra le trait a jaune car pas de vérif avec j)
                    plateau.trait = JAU ;
                break;
            
            //TODO Caractère inconnu ou gestion du nombre de case vide ??
            default:
                //printf1("Charactère %d non pris en charge, suite de l'analyse du FEN \n",fenpos); // message d'erreur (possibilité de le mettre que dans le mode DEBUG)
                break;
            
        }
        fenpos++;
    }
    for(col; col < NBCASES; col++) placer_colonne(&plateau, col, VIDE, VIDE);
    afficherPosition(plateau); // à retirer
    printf1("Trait aux : %s \n", COLNAME(plateau.trait));
    do
    {
    	printf("Souhaitez-vous ajouter un commentaire au fichier diag ?\n(yes -> y / no -> n");
   	scanf("%c",ans);
    }while (ans!= "y" || ans!= "n" );
    if ( ans ==  "y")
	    printf("Rentrez les commentaires, attention max %d charactères",MAXCOMM);
	    while (read(0,comm,MAXCOMM))
		    printf("%c",comm);
	    
}

void placer_colonne(T_Position *p,octet col, octet nb, octet c){
    // tu lui donnes le plateau, la colonne, sa valeur, et la couleur et elle te place le pion
    // passage par adresse donc p est modifié
    if(col < NBCASES) {
        p->cols[col].nb = nb;
        p->cols[col].couleur = c;
        printf3("Placement [%d | [%s : %d]\n", col, COLNAME(c), nb); // printf pour la version debug
    }
}

void placer_evolution(T_Position *p, octet col, octet type, octet c) {
    // tu lui donnes le plateau, la colonne, et le type (BONUS ou MALUS) et elle te place le pion évolution
    // passage par adresse donc p est modifié
    if ( col < 0) return;
    if(type == MALUS){
        //On place notre pion evolution dans le plateau selon sa couleur
        if(c == JAU){
            if(p->evolution.malusJ != UNKNOWN) return; // on fait rien si la valeur à déjà été changé
            
            p->evolution.malusJ = col;
            printf3("Placement d'un %s %s à la colonne n° %s\n", type, COLNAME(c), col); // printf pour la version debug
        }
        else {
            if(c == ROU) {
                if(p->evolution.malusR != UNKNOWN) return; // on fait rien si la valeur à déjà été changé
                
                p->evolution.malusR = col;
                printf3("Placement d'un %s %s à la colonne n° %s\n", type, COLNAME(c), col); // printf pour la version debug
            }
        }
    }
    // Sinon c'est un bonus
    else {
        //On place notre pion evolution dans le plateau selon sa couleur
        if(c == JAU) {
            if(p->evolution.bonusJ != UNKNOWN) return; // on fait rien si la valeur à déjà été changé
            
            p->evolution.bonusJ = col;
            printf3("Placement d'un %s %s à la colonne n° %s\n", type, COLNAME(c), col); // printf pour la version debug
        }
        else {
            if(c == ROU) {
                if(p->evolution.bonusR != UNKNOWN) return; // on fait rien si la valeur à déjà été changé
                
                p->evolution.bonusR = col;
                printf3("Placement d'un %s %s à la colonne n° %s\n", type, COLNAME(c), col); // printf pour la version debug
            }
        }
    }
}

