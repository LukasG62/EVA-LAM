#define DEFAULT_NAME "diag.js"
#define MAXCOMM 500
#define MAX_FEN 100
#define MAX_FENNB 4
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

void placer_colonne(T_Position *p, octet col, octet nb, octet c);
void placer_evolution(T_Position *p, octet col, octet type, octet c);
int generer_json(T_Position p, int diag_id,char *fen,char *comm, char *flocation);