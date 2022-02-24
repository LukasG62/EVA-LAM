#define DEFAULT_LOCATION "../web/data/refresh-data.js"
#define MAX_FILELOCATION 100 // Taille max du chemin spécifié par ligne de commande

#ifdef __DEBUG__
	#define DEBUGVALUE 1
#else
	#define DEBUGVALUE 0
#endif

#define ISDEBUG() (DEBUGVALUE ? "ON" : "OFF")

//##################################################################################################
//Fonction : ecraserJson                                                                           #
// Description : Permet de mettre à jour le fichier json                                           #                   
// Entrees : p:Le plateau de jeu, s: le score, flocation: le chemin du fichier                     #
// Sorties : (/)                                                                                   #
// Retourne : 0:échec, 1:succés                                                                    #
//##################################################################################################
int ecraserJson(T_Position p, T_Score s, char *flocation);


//##################################################################################################
//Fonction : saisirEvolution                                                                       #
// Description : Permet de saisir les 4 pions                                                      #                   
// Entrees : *p: pointeur du plateau de jeu                                                        #
// Sorties : la struture pointée est modifiée                                                      #
// Retourne : (/)                                                                                  #
//##################################################################################################
void saisirEvolution(T_Position *p);


//##################################################################################################
//Fonction : initScore                                                                             #
// Description : Fonction pour initialiser la struture T_Score                                     #                   
// Entrees : *ptrE:Pointeur d'un T_Score                                                           #
// Sorties : la struture pointée est modifiée                                                      #
// Retourne : (/)                                                                                  #
//##################################################################################################
void initScore(T_Score *ptrS);

//##################################################################################################
//Fonction : initEvolution                                                                         #
// Description : Fonction pour initialiser la struture T_evolution                                 #                   
// Entrees : *ptrE:Pointeur d'un T_Evolution                                                       #
// Sorties : la struture pointée est modifiée                                                      #
// Retourne : (/)                                                                                  #
//##################################################################################################
void initEvolution(T_Evolution *ptrE);
