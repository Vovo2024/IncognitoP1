#include <stdio.h>
#include <stdlib.h>




#define TAILLE 5



typedef enum _couleur{BLANC, NOIR} Couleur ; 
typedef enum _type{CHEVALIER, ESPION} Type ;

// Définition de la taille du plateau
typedef struct _pion {
    Type type;
    Couleur couleur ; 
} Pion;

typedef struct _jeu {
    Pion *plateau[TAILLE][TAILLE]; 
    Couleur joueur ;
} Jeu;


typedef struct { 
    int x, y;
} Case , Direction ;


typedef struct _mouvement {
    Case depart;
    Case arrivee ; 
}Mouvement;

//----------Prototypes des fonctions----------//

//Fonction pour tracer la grille du jeu 
void traceGrille();

//Fonction pour 


void initPlateau(Jeu  *jeu){
    srand(time(NULL));

    int i_espion_noir = rand() % TAILLE;
    int i_espion_blanc = rand() % (TAILLE - 2) + 2;

    int j_espion_noir = rand() % TAILLE;
    int j_espion_blanc = rand() % (TAILLE - 2) + 2;

    //Fonction pour initialiser le plateau avec les positions initiales des pions
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
         //initialise le plateau avec des NULL dans toutes les cases
            jeu->plateau[i][j] = NULL;
        }
    }
    for (int i = 0; i < TAILLE; i++){
        for( int j = i + 2; j < TAILLE; j++){
        //positions initiales des pions noirs
            jeu->plateau[i][j] = (Pion *)malloc(sizeof(Pion));
            jeu->plateau[i][j]->type = CHEVALIER;
            jeu->plateau[i][j]->couleur = NOIR;
        }
    }
    for (int i = 2; i < TAILLE; i++) {
        for (int j = 0; j < i-1; j++) {
        //positions initiales des pions blancs
            jeu->plateau[i][j] = (Pion *)malloc(sizeof(Pion));
            jeu->plateau[i][j]->type = CHEVALIER;
            jeu->plateau[i][j]->couleur = BLANC;
        }
    }
    
    jeu->plateau[i_espion_blanc][j_espion_blanc]->type = ESPION;
    jeu->plateau[i_espion_blanc][j_espion_blanc]->couleur = BLANC;
    jeu->plateau[i_espion_noir][j_espion_noir]->type = ESPION;
    jeu->plateau[i_espion_noir][j_espion_noir]->couleur = NOIR;

    // init Des chateaux dans les coins
    if (jeu->plateau[0][TAILLE - 1] == NULL) {
        jeu->plateau[0][TAILLE - 1] = (Pion*)malloc(sizeof(Pion));
    }
    jeu->plateau[0][TAILLE - 1]->type = CHATEAU;

    if (jeu->plateau[TAILLE - 1][0] == NULL) {
        jeu->plateau[TAILLE - 1][0] = (Pion*)malloc(sizeof(Pion));
    }
    jeu->plateau[0][TAILLE-1] = NULL;
    jeu->plateau[TAILLE-1][0] = NULL;
}

void afficherPlateau(Jeu *jeu){
    //Fonction qui affiche le plateau dans le terminal
    for(int i = 0; i < TAILLE+1 ; i++){
        printf("-----------\n");
        for(int j = 0; j < TAILLE ; j++){
            if(jeu->plateau[i][j] == NULL){
                printf("| ");
            }
            else if(jeu->plateau[i][j]->type == CHATEAU) {
                printf("|*");
            } else if(jeu->plateau[i][j]->couleur == BLANC){
                printf("|b");
            }
            else if(jeu->plateau[i][j]->couleur == NOIR){
                printf("|n");
            }
            
        }
        printf("|\n");
    }
}

int coup_valide(Jeu *jeu, Mouvement *mvt) {
    Case depart = mvt->depart;
    Case arrivee = mvt->arrivee;

    // Vérifier si la case de départ contient un pion
    if (jeu->plateau[depart.x][depart.y] == NULL) {
        return 0;  // Pas de pion à déplacer
    }

    // Vérifier que la destination est dans les limites du plateau
    if (arrivee.x < 0 || arrivee.x >= TAILLE || arrivee.y < 0 || arrivee.y >= TAILLE) {
        return 0;  // La destination est en dehors du plateau
    }

    // Vérifier si la destination est différente de la case de départ
    if (depart.x == arrivee.x && depart.y == arrivee.y) {
        return 0;  // Pas de déplacement
    }

    // Vérifier si la case d'arrivée est occupée par une pièce du même camp
    Pion *pion_depart = jeu->plateau[depart.x][depart.y];
    Pion *pion_arrivee = jeu->plateau[arrivee.x][arrivee.y];

    if (pion_arrivee != NULL && pion_arrivee->couleur == pion_depart->couleur) {
        return 0;  // 
    }

    // direction du mouvement
    int dx = arrivee.x - depart.x;
    int dy = arrivee.y - depart.y;

    // Vérifier que le déplacement est bien dans une direction droite ou diagonale
    if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) {
        return 0;  // Le mouvement n'est ni droit ni diagonal
    }

    // 6. Vérifier si le chemin est libre (sans pièces entre la case de départ et d'arrivée)
    int pas_x = (dx != 0) ? dx / abs(dx) : 0;  // Pas pour avancer en x (0 si pas de mouvement horizontal)
    int pas_y = (dy != 0) ? dy / abs(dy) : 0;  // Pas pour avancer en y (0 si pas de mouvement vertical)

    int x = depart.x + pas_x;
    int y = depart.y + pas_y;

    // Parcourir les cases intermédiaires
    while (x != arrivee.x || y != arrivee.y) {
        if (jeu->plateau[x][y] != NULL) {  // Il y a une pièce sur le chemin
            return 0;  // Le chemin est bloqué
        }
        x += pas_x;
        y += pas_y;
    }

    // Si toutes les conditions sont satisfaites, le coup est valide

    return 1;
}

// Fonction pour tester le type de chaque pion dans le plateau
void testerPlateau(Jeu *jeu) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            Pion *pion = jeu->plateau[i][j];
            if (pion == NULL) {
                printf("Position (%d, %d) : Case vide\n", i, j);
            } else {
                printf("Position (%d, %d) : ", i, j);
                // Tester le type du pion
                switch (pion->type) {
                    case CHEVALIER:
                        printf("Chevalier ");
                        break;
                    case ESPION:
                        printf("Espion ");
                        break;
                    default:
                        printf("Inconnu ");
                        break;
                }
                // Tester la couleur du pion
                switch (pion->couleur) {
                    case BLANC:
                        printf("blanc\n");
                        break;
                    case NOIR:
                        printf("noir\n");
                        break;
                    default:
                        printf("couleur inconnue\n");
                        break;
                }
            }
        }
    }
}

void afficherDetailsPions(Jeu* jeu) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            printf("Case (%d, %d) : ", i, j);
            if (jeu->plateau[i][j] == NULL) {
                printf("Vide\n");
            } else {
                if (jeu->plateau[i][j]->type == CHEVALIER) {
                    printf("Type = CHEVALIER, ");
                } else if (jeu->plateau[i][j]->type == ESPION) {
                    printf("Type = ESPION, ");
                }

                if (jeu->plateau[i][j]->couleur == BLANC) {
                    printf("Couleur = BLANC\n");
                } else if (jeu->plateau[i][j]->couleur == NOIR) {
                    printf("Couleur = NOIR\n");
                }
            }
        }
    }

void recup_deplacement(Mouvement * mvt, Jeu * jeu, Pion pion){ //REVOIR LE NOM DE LA FCT
    //Fonction qui récupère les saisies de l'utilisateur pour déplacer un pion ou l'interroger
    //besoin de verif que les positions de depart et d'arrivee sont valides ??
    char c;
    int game = 1;
    do {
            printf("Joueur %s, voulez vous faire un déplacement ou une interrogation? ('d' ou 'i')",  (pion.couleur == BLANC) ? "blanc" : "noir");
            scanf(" %c", &c);
            if (c == 'd'){
                //récupère les données de case de départ et d'arrivée
                printf("Quel dépplacement joueur %s ?\n Saisie sous la forme (a,b) --> (c,d)",  (pion.couleur == BLANC) ? "blanc" : "noir");
                scanf("%d %d %d %d", &(mvt->depart.x), &(mvt->depart.y), &(mvt->arrivee.x), &(mvt->arrivee.y));
            } 
            else if (c == 'i'){
                interroge(mvt, jeu, pion);
            }
    } while(coup_valide);
}

void Deplacements(Mouvement * mvt, Jeu * jeu){
    //Fonction qui déplace les pions
    int depart_x = mvt->depart.x;
    int depart_y = mvt->depart.y;
    int arrivee_x = mvt->arrivee.x;
    int arrivee_y = mvt->arrivee.y
    if (coup_valide){
        if (jeu->plateau[depart_x][depart_y] != NULL && (jeu->plateau[depart_x][depart_y]->couleur == BLANC || jeu->plateau[depart_x][depart_y]->couleur == NOIR)) {
            //la valeur d'arrivée devient celle de départ -> le pion est déplacé, et la valeur de départ est réinitialisée à NULL
            jeu->plateau[arrivee_x][arrivee_y] = jeu->plateau[depart_x][depart_y];
            jeu->plateau[depart_x][depart_y] = NULL;
        }
        else if (jeu->plateau[depart_x][depart_y]->type == ESPION && //condition à ajt : ESPION ENTRE DANS CHATEAU ADVERSE!! 
                (arrivee_x == 0 || arrivee_x == 4) && 
                (arrivee_y == 0 || arrivee_y == 4) && 
                (arrivee_x != arrivee_y)) {
                //si un espion entre dans le chateau adverse, la partie est gagnée
                    gagne(pion.couleur);
        }
    }
}

//Fonction qui questionne le pion designé
void interroge(Mouvement * mvt, Jeu * jeu, Pion pion){ //verif aussi s'il y a un pion à la position désignée ?
    Case interroge, questionne;
    //demande du pion interrogateur
    printf("Quel pion %s est l'interrogateur?\n Saisie sous la forme (a,b)", (pion.couleur == BLANC) ? "blanc" : "noir");
    scanf("%d %d", &(interroge.x), &(interroge.y));
    //choix du pion à questionner 
    printf("Quel pion est quesitonné ?\n Saisie sous la forme (a,b)");
    scanf("%d %d", &(questionne.x), &(questionne.y));
    Pion interrogeP = *jeu->plateau[interroge.x][interroge.y]; //récupère le type et la couleur du pion qui interroge
    Pion questionneP = *jeu->plateau[questionne.x][questionne.y]; //récupère le type et la couleur du pion questionné
    //vérifie si le pion qui interroge est l'espion
    if (interrogeP.type == ESPION){
        //vérifie si le pion questionné n'est pas l'espion
        if (questionneP.type != ESPION){
            printf("Joueur %s, vous avez interrogé un chevalier avec votre espion...\n", (pion.couleur == BLANC) ? "blanc" : "noir");
            gagne(pion.couleur, questionneP);
        }
    //vérifie si le pion questionné est l'espion
    } else if (questionneP.type == ESPION){
        printf("Joueur %s, vous avez démasqué l'espion adverse.", (pion.couleur == BLANC) ? "blanc" : "noir"); 
        gagne(pion.couleur, interrogeP); 
    }   
}

void gagne(Couleur couleur, Pion pion){
    //Fonction qui donne la phrase lorsqu'un joueur gagne
    printf("Le gagnant est le joueur %s : BRAVO !", (pion.couleur == BLANC) ? "blanc" : "noir");
}

int main() {
     Jeu jeu;
     Mouvement mvt;
     Pion pion_noir = {CHEVALIER, NOIR};
     Pion pion_blanc = {CHEVALIER, BLANC};
    // Initialisation du plateau
    printf("Initialisation du plateau...\n");
    initPlateau(&jeu);
    
    // Affichage du plateau
    printf("Affichage du plateau :\n");
    afficherPlateau(&jeu);
    
    return 1;
}
