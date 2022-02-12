#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "avalam.h"
#include "topologie.h"
#include "diag.h"

int main(int argc, char *argv[]){
    T_Position plateau;
    
    char fen[MAX_FEN]; // code fen
    char fen_nbVide[MAX_FENNB] = ""; // Nombre de case vide indiqué dans le fen
	char comm[MAXCOMM+1]; // commentaire de la situation
	char filename[MAX_NAME]; // Nom du fichier json généré 
    int diag_id; // numéro du diagramme
    octet fenpos= 0; // compteur pour parcourir le fen
    octet col = 0; // compteur pour parcourir le plateau
    octet i;
    
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
    
        switch(fen[fenpos]){
            // cas pour les pions jaunes
            case 'u':
                placer_colonne(&plateau, col, U, JAU);
                col++; 
	        	printf1("Colonne de %d placée",U);
                break;
            
            case 'd':
                placer_colonne(&plateau, col, D, JAU);
                col++;
				printf1("Colonne de %d placée",D);
                break;
                
            case 't':
                placer_colonne(&plateau, col, T, JAU);
                col++;
				printf1("Colonne de %d placée",T);
                break;
            
            case 'q':
                placer_colonne(&plateau, col, Q, JAU);
                col++;
				printf1("Colonne de %d placée",Q);
                break;
            
            case 'c':
                placer_colonne(&plateau, col, C, JAU);
                col++;
				printf1("Colonne de %d placée",C);
                break;
                
            case 'b':
                placer_evolution(&plateau, col-1, BONUS, JAU);
				printf("Bonus placé");
                break;
                
            case 'm':
                placer_evolution(&plateau, col-1, MALUS, JAU);
				printf("Malus placé");
                break;
            
            // cas pour les pion Rouge
            case 'U':
                placer_colonne(&plateau, col, U, ROU);
                col++;
				printf1("Colonne de %d placée",U);
                break;
            
            case 'D':
                placer_colonne(&plateau, col, D, ROU);
                col++;
				printf1("Colonne de %d placée",D);
                break;
            
            case 'T':
                placer_colonne(&plateau, col, T, ROU);
                col++;
				printf1("Colonne de %d placée",T);
                break;    
            
            case 'Q':
                placer_colonne(&plateau, col, Q, ROU);
                col++;
				printf1("Colonne de %d placée",Q);
                break; 
            
            case 'C':
                placer_colonne(&plateau, col, C, ROU);
                col++;
				printf1("Colonne de %d placée",C);
                break;
            
            case 'B':
                placer_evolution(&plateau, col-1, BONUS, ROU);
				printf("Bonus placé");
                break;
            
            case 'M':
				placer_evolution(&plateau, col-1, MALUS, ROU);
				printf("Malus placé");
				break;
            
            case '0' ... '9':
                strncat(fen_nbVide, &fen[fenpos], 1); // Concatenation de chaque chiffre trouvé
                printf1("Nombre en cours : %s \n ", fen_nbVide);
                
                // Si le suivant n'est pas un chiffre
                if(!isdigit(fen[fenpos+1])) {
                	// Alors on place le nombre de colonne indiqué
                    for(i = 0; i < atoi(fen_nbVide) && col < NBCASES; i++) {
                        placer_colonne(&plateau, col, VIDE, VIDE);
                        col++;
                    }
                    fen_nbVide[0] = '\0'; // reset du nombre
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
            
            default:
                printf1("Charactère %d non pris en charge, suite de l'analyse du FEN \n",fenpos); // message d'erreur (possibilité de le mettre que dans le mode DEBUG)
                break;
            
        }
        fenpos++;
    }
    
	for(col; col < NBCASES; col++) placer_colonne(&plateau, col, VIDE, VIDE); // remplissage des autres colonnes s'ils en restent
	afficherPosition(plateau); // à retirer
    printf1("Trait aux : %s \n", COLNAME(plateau.trait));

	printf("Nom du fichier js de la situation [%s] : ", DEFAULT_NAME);
   	fgets(filename, MAX_NAME, stdin);
	filename[strlen(filename)-1] = '\0';

	if (!filename[0] || strlen(filename) >=MAX_NAME){ // Si la saisie est vide ou trop grande alors on prends le nom par défaut
		strcpy(filename, DEFAULT_NAME);
	}
	printf1("Nom fichier saisie : %s \n", filename);

	// Saisie du commentaire de la situation
	printf("Rentrez les commentaires, attention max %d charactères : \n",MAXCOMM);
		
	fread(comm, sizeof(comm), 1, stdin); // lecture du stdin
	printf1("%s", comm); // Affichage debug de la note
	

	generer_json(plateau, diag_id, fen, comm, filename);
	    
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

int generer_json(T_Position p, int diag_id,char *fen,char *comm, char *flocation) {
	int i=0;
	FILE *fic=NULL; // Pointeur de notre fichier
	fic=fopen(flocation,"w"); // Ouverture du fichier en mode écriture avec écrasement
	
	if (fic==NULL) return 0;
	
	else {
		fprintf(fic,"traiterJson({\n %s:%d,\n %s:%d,\n %s:\"%s\",\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:[",
																    STR_TURN,p.trait,
																    STR_NUMDIAG,diag_id, 
																    STR_FEN,fen,
																    STR_BONUS_J,p.evolution.bonusJ,
																    STR_BONUS_R,p.evolution.bonusR,
																    STR_MALUS_R,p.evolution.malusR,
																    STR_MALUS_J,p.evolution.malusJ,
																    STR_COLS
		);

		fprintf(fic,"{%s:%d, %s:%d,}",STR_NB,p.cols[0].nb,STR_COULEUR,p.cols[0].couleur);
		for (i=1;i<NBCASES;i++) 
			fprintf(fic,",\n\t{%s:%d, %s:%d}",STR_NB,p.cols[i].nb,STR_COULEUR,p.cols[i].couleur);
		
		fprintf(fic,"], \n %s:\"", STR_NOTES);
		i = 0;
		while(comm[i]){ // lecture caractère par caractère
			if(comm[i] == '\n') { // Si il y a un retour à la ligne
				fprintf(fic, "<br />"); // on place une balise <br>
				printf1("Ajout du chr : %c \n", comm[i]);
			}
			else fprintf(fic, "%c", comm[i]); // on place le caractère dans tous les cas
			
			i++;
		}
		fprintf(fic, "\"});");
		fclose(fic);
		return 1;
	}
	
}
