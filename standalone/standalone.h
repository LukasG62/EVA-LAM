#define DEFAULT_LOCATION "../web/data/refresh-data.js"
#define MAX_FILELOCATION 100 // Taille max du chemin spécifié par ligne de commande

#ifdef __DEBUG__
	#define DEBUGVALUE 1
#else
	#define DEBUGVALUE 0
#endif

#define ISDEBUG() (DEBUGVALUE ? "ON" : "OFF")

int ecraserJson(T_Position p, T_Score s, char *flocation);
void saisirEvolution(T_Position *p);
