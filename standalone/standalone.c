#include <stdio.h>
#include <string.h>
#include "avalam.h"
#include "topologie.h"

#define DEFAULT_LOCATION "web/data/refresh-data.js"
#define MAX_FILELOCATION 100 // Taille max du chemin spécifié par ligne de commande

#ifdef __DEGUG__
	#define DEBUGVALUE 1
#else
	#define DEBUGVALUE 0
#endif

#define ISDEBUG() (DEBUGVALUE ? "ON" : "OFF")

int ecraserJson(T_Position p, T_Score s, char *flocation);

int main(int argc, char *argv[]) {
    // Declaration et initialisation des variables du jeu
    T_Position plateau = getPositionInitiale();
    T_Score score;
    T_ListeCoups coupsLegaux = getCoupsLegaux(plateau);
    T_Coup coup;
    octet pbonus; // position des bonus saisie par l'utilisateur

    // Variable contenant le chemin du fichier json
    char file_location[MAX_FILELOCATION] = DEFAULT_LOCATION;
    
    // Si il n'y pas au moins un argument alors on l'indique et on prend le chemin par défaut
    if(argc < 2) printf("[Warning] Chemin non spécifié \n\n");
    else{
	// Si il y a au moins un argument alors on vérifie sa taille
	if(strlen(argv[1]) > MAX_FILELOCATION){
	    // Dans le cas d'un paramètre non valide on termine le programme
	    printf("Chemin spécifié trop long ! Fin du programme");
	    return 1;
	}
	strcpy(file_location, argv[1]);
    }
	
    printf("\t\t EVA-LAM PROTOTYPE 01\n");
    printf("\t\t --------------------\n");
    printf("Emplacement du fichier js : '%s' \n", file_location);
    printf("DEBUG : %s", ISDEBUG());
    printf("\n");
    printf("Placement des bonus/malus :\n");
    
    // Placement des bonus/malus avec vérification des saisies
    do{ // Placement bonus Jaune
	printf("\tbonusJ :");
	scanf("%d",&pbonus);
	plateau.evolution.bonusJ = pbonus;
	printf("la valeur donnée est : %d\n",plateau.evolution.bonusJ);
    }while(plateau.cols[pbonus].couleur != JAU);
    
    do{ // Placement bonus Rouge
	printf("\tbonusR :");
	scanf("%d",&pbonus);
	plateau.evolution.bonusR = pbonus;
    }while(plateau.cols[pbonus].couleur != ROU );
    
    printf("la valeur donnée est : %d\n",plateau.evolution.bonusR);

    do{ // Placement malus Rouge
	printf("\tmalusR :");
	scanf("%d",&pbonus);
	plateau.evolution.malusR = pbonus;
    }while((plateau.evolution.malusR == plateau.evolution.bonusR) || (plateau.cols[pbonus].couleur != ROU));
    printf("la valeur donnée est : %d\n",plateau.evolution.malusR);
    
    do{ // Placement malus Jaune
    	printf("\tmalusJ :");
    	scanf("%d",&pbonus);
    	plateau.evolution.malusJ = pbonus;
    }while((plateau.evolution.malusJ == plateau.evolution.bonusJ ) || (plateau.cols[pbonus].couleur != JAU));
    printf("la valeur donnée est : %d\n",plateau.evolution.malusJ);
    
    score = evaluerScore(plateau); // Maintenant que les pions évolution sont placé on peut initialisé le score
	
    //afficherPosition(plateau); // A retirer
    ecraserJson(plateau, score, file_location); // Mise à jour du fichier json
    
    while(coupsLegaux.nb){
		// On affiche le score
        afficherScore(score);
	printf("Trait aux %ss \n",COLNAME(plateau.trait));
	
	printf("\tQuel est la position du pion à déplacer :");
	scanf("%d",&coup.origine);
	
        printf("\tQuel est la position du pion d'arrivée :");   
        scanf("%d",&coup.destination);
        
	printf("\tLe coup à jouer est : %d -> %d\n",coup.origine,coup.destination);
        plateau = jouerCoup(plateau,coup.origine,coup.destination);
		
	coupsLegaux = getCoupsLegaux(plateau);
        printf("\tIl y a %d coups possibles\n", coupsLegaux.nb);
        printf("\n\n\n\n");
	score=evaluerScore(plateau);
	ecraserJson(plateau, score, file_location);
		
    }
	//TODO Affichage du vainqueur
	return 0;
}

int ecraserJson(T_Position p, T_Score s, char *flocation){ //update du fichier Jason en fonction de l'avancement de la partie
	octet i;
	FILE *fic=NULL; // Pointeur de notre fichier
	fic=fopen(flocation,"w"); // Ouverture du fichier en mode écriture avec écrasement
	
	if (fic==NULL) return 0;
	
	else {
		fprintf(fic,"traiterJson({\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:%d,\n %s:[",
																    STR_TURN,p.trait,
																    STR_SCORE_J,s.nbJ,
																    STR_SCORE_J5,s.nbJ5,
																    STR_SCORE_R,s.nbR,
																    STR_SCORE_R5,s.nbR5,
																    STR_BONUS_J,p.evolution.bonusJ,
																    STR_BONUS_R,p.evolution.bonusR,
																    STR_MALUS_R,p.evolution.malusR,
																    STR_MALUS_J,p.evolution.malusJ,
																    STR_COLS
		);

		fprintf(fic,"{%s:%d, %s:%d,}",STR_NB,p.cols[0].nb,STR_COULEUR,p.cols[0].couleur);
		for (i=1;i<NBCASES;i++) fprintf(fic,",\n\t{%s:%d, %s:%d,}",STR_NB,p.cols[i].nb,STR_COULEUR,p.cols[i].couleur);
		
		fprintf(fic,"]});");
		fclose(fic);
		return 1;
	}
	
}
