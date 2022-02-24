#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "avalam.h"
#include "topologie.h"
#include "diag.h"

int main(int argc, char *argv[]){
    T_Position plateau;
    plateau.trait = 0;
    
    char fen[MAX_FEN]; // code fen
    char fen_nbVide[MAX_FENNB] = ""; // nombre de case vide dans fen (chaine)
	char comm[MAXCOMM+1]; // commentaire de la situation
	char filename[MAX_NAME]; // nom du fichier json généré 
    int diag_id; // numéro du diagramme
    
    octet fenpos= 0; // compteur pour parcourir le fen
    octet col = 0; // compteur pour parcourir le plateau
    octet i;
    
    // initialisation des pions évolutions
	initEvolution(&(plateau.evolution));
    
    // vérification du nombre d'arguments
    if(argc != 3) {
        return printf("Argument invalide ! \n %s <Numero diagramme> <code fen> \n", argv[0]);
        return 1;
    }
    // récupération des arguments
    else {
        diag_id = atoi(argv[1]); //conversion de la chaine en entier
        strcpy(fen, argv[2]);
    }
    // parcours du fen jusqu'au trait ou fin de chaine
    while(fen[fenpos] && (fen[fenpos] != 'r' && fen[fenpos] != 'j')) {
    
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
            
            case '0' ... '9':
                if(strlen(fen_nbVide) < MAX_FENNB-1)
					strncat(fen_nbVide, &fen[fenpos], 1); // Concatenation de chaques chiffres trouvés
					
                printf1("Nombre en cours : %s \n", fen_nbVide);
                
				//Fin lecture nombre
                if(!isdigit(fen[fenpos+1])) {
                	// placement des colonnes vides
                    for(i = 0; i < atoi(fen_nbVide); i++) {
                        placer_colonne(&plateau, col, VIDE, VIDE);
                        col++;
                    }
                    fen_nbVide[0] = '\0';
                }
                break;
                
                
            case ' ':
                if (fen[fenpos+1] != 'j'  && fen[fenpos+1]!= 'r') {
                	fprintf(stderr, "[ERREUR] Espace en trop ! \n");
                	break;
                }
                
                if (fen[fenpos+1] == 'r')
                    plateau.trait = ROU ;
                else
                    plateau.trait = JAU ;
                break;
            
            default:
                fprintf(stderr,"[ERREUR] Caractère [%d]:%c non pris en charge, suite de l'analyse du FEN \n",fenpos, fen[fenpos]);
                break;
            
        }
        fenpos++;
    }
    //Remplissages des colonnes restantes
	for(col; col < NBCASES; col++) placer_colonne(&plateau, col, VIDE, VIDE); 
	
	afficherPositionDebug(plateau);
    printf1("Trait aux : %s \n", COLNAME(plateau.trait));

	printf("Nom du fichier js de la situation [%s] : ", DEFAULT_NAME);
   	fgets(filename, MAX_NAME, stdin);
	filename[strlen(filename)-1] = '\0';

	if (!filename[0] || strlen(filename) >=MAX_NAME){
		strcpy(filename, DEFAULT_NAME);
	}
	printf1("Nom fichier saisie : %s \n", filename);

	// Saisie du commentaire de la situation
	printf("Rentrez les commentaires, attention max %d charactères : \n",MAXCOMM);
		
	fread(comm, sizeof(comm), 1, stdin); // lecture du stdin
	printf1("\n%s\n", comm); // Affichage debug de la note
	

	if(!generer_json(plateau, diag_id, fen, comm, filename)) fprintf(stderr, "[ERREUR] Impossible d'ouvrir le fichier !");
	    
}

void placer_colonne(T_Position *p,octet col, octet nb, octet c){
    if(col < NBCASES) {
        p->cols[col].nb = nb;
        p->cols[col].couleur = c;
        printf3("Placement [%d | [%s : %d]\n", col, COLNAME(c), nb);
    }
    else fprintf(stderr, "[ERREUR] Dépassement du nombre de colonne ! \n");
}

void placer_evolution(T_Position *p, octet col, octet type, octet c) {
    if ( col < 0) return; 
    
    if(type == MALUS){
		
        if(c == JAU){
            if(p->evolution.malusJ != UNKNOWN) return;
            p->evolution.malusJ = col;
        }
        else {
            if(c == ROU) {
                if(p->evolution.malusR != UNKNOWN) return;
                
                p->evolution.malusR = col;
            }
        }
    }
    // Sinon c'est un bonus
    else {
		
        if(c == JAU) {
            if(p->evolution.bonusJ != UNKNOWN) return; 
            p->evolution.bonusJ = col;
        }
        else {
            if(c == ROU) {
                if(p->evolution.bonusR != UNKNOWN) return;
                
                p->evolution.bonusR = col;
            }
        }
    }
    printf3("Placement d'un %s %s à la colonne n°%d\n", EVOLNAME(type), COLNAME(c), col); 
}

int generer_json(T_Position p, int diag_id,char *fen,char *comm, char *flocation) {
	int i=0;
	FILE *fic=NULL; // Pointeur de notre fichier
	fic=fopen(flocation,"w");
	
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
		while(comm[i]){ // lecture du commentaire
			if(comm[i] == '\n') { // transformation \n en balise
				fprintf(fic, "<br />");
				printf1("Ajout du chr : %c \n", comm[i]);
			}
			else fprintf(fic, "%c", comm[i]);
			
			i++;
		}
		fprintf(fic, "\"});");
		fclose(fic);
		return 1;
	}
	
}

void initEvolution(T_Evolution *ptrE){
	ptrE->bonusJ = UNKNOWN;
	ptrE->bonusR = UNKNOWN;
	ptrE->malusJ = UNKNOWN;
	ptrE->malusR = UNKNOWN;

}
