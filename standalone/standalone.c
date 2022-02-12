#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "avalam.h"
#include "topologie.h"
#include "standalone.h"

int main(int argc, char *argv[]) {
    // Declaration et initialisation des variables du jeu
    T_Position plateau = getPositionInitiale();
    T_Score score;
    T_ListeCoups coupsLegaux = getCoupsLegaux(plateau);
    T_Coup coup;

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
    printf("Emplacement du fichier js : '%s' \n", file_location);
    printf("DEBUG : %s", ISDEBUG());
    printf("\n");
    printf("Placement des bonus/malus :\n");
    
    saisirEvolution(&plateau);
    getchar();
    score = evaluerScore(plateau); // Maintenant que les pions évolution sont placé on peut initialisé le score
    printf0("Fin de saisie des bonus, début de partie");
	
    afficherPosition(plateau); // A retirer
    ecraserJson(plateau, score, file_location); // Mise à jour du fichier json
    
    while(coupsLegaux.nb){
        //system("clear"); // Nettoyage du shell

        afficherScore(score);
		printf("Trait aux %ss \n",COLNAME(plateau.trait));
	
		printf("\tQuel est la position du pion à déplacer :");
		scanf("%d",&coup.origine);
		printf1("Le pion a déplacé est le numéro %d",&coup.origine);
	
        printf("\tQuel est la position du pion d'arrivée :");   
        scanf("%d",&coup.destination);
		printf1("Le pion sur lequel va etre placé le pion est %d",&coup.destination);
        
		printf2("\tLe coup à jouer est : %d -> %d\n",coup.origine,coup.destination);
        plateau = jouerCoup(plateau,coup.origine,coup.destination);
		
		coupsLegaux = getCoupsLegaux(plateau);
        printf("\tIl y a %d coups possibles\n", coupsLegaux.nb);
        printf("\n\n\n\n");
		
		score=evaluerScore(plateau);
		ecraserJson(plateau, score, file_location);
    }
	 // Affichage des scores (à tester)

    if (score.nbJ > score.nbR)
    	printf("Le joueur Jaune a gagné !");

    if (score.nbR > score.nbJ)
    	printf("Le joueur Rouge a gagné !");

    if (score.nbR == score.nbJ)
    {
        if(score.nbJ5 > score.nbR5)
        printf("Le joueur Jaune a gagné !");

        if(score.nbR5 > score.nbJ5)
        printf("Le joueur Rouge a gagné !");

        if(score.nbJ5 == score.nbR5)
        printf("Egalité !");
    }
	printf0("Fin de partie !");
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

void saisirEvolution(T_Position *p) {
	int pbonus; // position des bonus saisie par l'utilisateur
	// Placement des bonus/malus avec vérification des saisies
    do{ // Placement bonus Jaune
		printf("\tbonusJ :");
		scanf("%d",&pbonus);
		p->evolution.bonusJ = pbonus;
		printf1("la valeur donnée est : %d\n",p->evolution.bonusJ);
    }while(p->cols[pbonus].couleur != JAU);
    
    printf("la valeur donnée est : %d\n",p->evolution.bonusJ);
    
    do{ // Placement bonus Rouge
		printf("\tbonusR :");
		scanf("%d",&pbonus);
		p->evolution.bonusR = pbonus;
		printf1("la valeur donnée est : %d\n",p->evolution.bonusR);
    }while(p->cols[pbonus].couleur != ROU );
    
    printf("la valeur donnée est : %d\n",p->evolution.bonusR);

    do{ // Placement malus Rouge
		printf("\tmalusR :");
		scanf("%d",&pbonus);
		p->evolution.malusR = pbonus;
		printf1("la valeur donnée est : %d\n",p->evolution.malusR);
    }while((p->evolution.malusR == p->evolution.bonusR) || (p->cols[pbonus].couleur != ROU));
    printf("la valeur donnée est : %d\n",p->evolution.malusR);
    
    do{ // Placement malus Jaune
    	printf("\tmalusJ :");
    	scanf("%d",&pbonus);
    	p->evolution.malusJ = pbonus;
		printf1("la valeur donnée est : %d\n",p->evolution.malusJ);
    }while((p->evolution.malusJ == p->evolution.bonusJ ) || (p->cols[pbonus].couleur != JAU));
    printf("la valeur donnée est : %d\n",p->evolution.malusJ);
    
    return;
}