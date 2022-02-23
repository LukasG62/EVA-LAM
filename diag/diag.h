#define DEFAULT_NAME "diag.js"
#define MAXCOMM 500
#define MAX_FEN 100
#define MAX_FENNB 3
#define MAX_NAME 20

#ifdef __DEBUG__
	#define afficherPositionDebug(p) afficherPosition(p)
#else
	#define afficherPositionDebug(p)
#endif

#define U 1
#define D 2
#define T 3
#define Q 4
#define C 5
#define BONUS 0
#define MALUS 1
#define EVOLNAME(type) (type ? "malus" : "bonus")

//##################################################################################################
//Fonction : placer_colonne                                                                        #
// Description : Procédure qui permet de placer une colonne                                        #                   
// Entrees : *p:Pointeur d'un T_Position, col: numéro de la colonne, nb: Nombre de pions           #
//		     c: Couleur du pion                                                                    #
// Sorties : la struture p est modifié                                                             #
// Retourne : (/)                                                                                  #
//##################################################################################################
void placer_colonne(T_Position *p, octet col, octet nb, octet c);

//##################################################################################################
//Fonction : placer_evolution                                                                      #
// Description : Procédure qui permet de placer un pions évolution dans une colonne                #                   
// Entrees : *p:Pointeur d'un T_Position, col: numéro de la colonne, type: 0=BONUS 1=MALUS         #
//		     c: Couleur du pion                                                                    #
// Sorties : la struture p est modifié                                                             #
// Retourne : (/)                                                                                  #
//##################################################################################################
void placer_evolution(T_Position *p, octet col, octet type, octet c);

//##################################################################################################
//Fonction : generer_json                                                                          #
// Description : Fonction qui permet de génerer un fichier javascript utilisable par diag.html     #                   
// Entrees : *p:Pointeur d'un T_Position, diag_id: numéro du diagramme, fen: le code fen           #
//		     comm: commentaire du diagramme, flocation: Chemin du fichier                          #
// Sorties : (/)                                                                                   #
// Retourne : 0:échec de la génération 1:succès                                                    #
//##################################################################################################
int generer_json(T_Position p, int diag_id,char *fen,char *comm, char *flocation);

//##################################################################################################
//Fonction : placer_evolution                                                                      #
// Description : Fonction pour initialiser la struture T_evolution                                 #                   
// Entrees : *ptrE:Pointeur d'un T_Evolution                                                       #
// Sorties : la struture pointé                                                                    #
// Retourne : (/)                                                                                  #
//##################################################################################################
void initEvolution(T_Evolution *ptrE);
