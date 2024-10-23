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
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            jeu->plateau[i][j] = NULL;
        }
    }
    for (int i = 0; i < TAILLE; i++){
        for( int j = i + 2; j < TAILLE; j++){
        //N
            jeu->plateau[i][j] = (Pion *)malloc(sizeof(Pion));
            jeu->plateau[i][j]->type = CHEVALIER;
            jeu->plateau[i][j]->couleur = NOIR;
        }
    }
    for (int i = 2; i < TAILLE; i++) {
        for (int j = 0; j < i-1; j++) {
            jeu->plateau[i][j] = (Pion *)malloc(sizeof(Pion));
            jeu->plateau[i][j]->type = CHEVALIER;
            jeu->plateau[i][j]->couleur = BLANC;
        }
    }
    jeu->plateau[0][TAILLE-1] = NULL;
    jeu->plateau[TAILLE-1][0] = NULL;
}
  //jeu->plateau[0][TAILLE-1]->type = CHATEAU;
  //jeu->plateau[TAILLE-1][0]->type = CHATEAU;


void afficherPlateau(Jeu *jeu){
    
    for(int i = 0; i < TAILLE ; i++){
        printf("-----------\n");
        for(int j = 0; j < TAILLE ; j++){
            if(jeu->plateau[i][j] == NULL){
                printf("| ");
            }
            else if(jeu->plateau[i][j]->couleur == BLANC){
                printf("|b");
            }
            else if(jeu->plateau[i][j]->couleur == NOIR){
                printf("|n");
            }
            
        }
        printf("|\n");
    }
    printf("-----------\n");
    
    
}

void recup_deplacement(Mouvement * mvt, Jeu * jeu, Pion pion){
    char c;
    int game = 1;
    do {
        if (pion.couleur == BLANC){
            printf("Joueur blanc, voulez vous faire un déplacement ou une interrogation? ('d' ou 'i')");
            scanf(" %c", &c);
            if (c == 'd'){
                printf("Quel dépplacement joueur blanc ?\n Saisie sous la forme (a,b) --> (c,d)");
                scanf("%d %d %d %d", &(mvt->depart.x), &(mvt->depart.y), &(mvt->arrivee.x), &(mvt->arrivee.y));
            } else if (c == 'i') {
                Case interroge, questionne;
                printf("Quel pion blanc est l'interrogateur?\n Saisie sous la forme (a,b)");
                scanf("%d %d", &(interroge.x), &(interroge.y));
                printf("Quel pion est quesitonné ?\n Saisie sous la forme (a,b)");
                scanf("%d %d", &(questionne.x), &(questionne.y));
            }
        }
        if (pion.couleur == NOIR){
            printf("Joueur noir, voulez vous faire un déplacement ou une interrogation? ('d' ou 'i')");
            scanf(" %c", &c);
            if (c == 'd'){
                printf("Quel dépplacement joueur noir ?\n Saisie sous la forme (a,b) --> (c,d)");
                scanf("%d %d %d %d", &(mvt->depart.x), &(mvt->depart.y), &(mvt->arrivee.x), &(mvt->arrivee.y));
            } else if (c == 'i') {
                Case interroge, questionne;
                printf("Quel pion noir est l'interrogateur?\n Saisie sous la forme (a,b)");
                scanf("%d %d", &(interroge.x), &(interroge.y));
                printf("Quel pion est quesitonné ?\n Saisie sous la forme (a,b)");
                scanf("%d %d", &(questionne.x), &(questionne.y));
            }
        }
     } while(game);
}



void Deplacements(Mouvement * mvt, Jeu * jeu){
    if (position_valide){
        if (jeu->plateau[mvt->depart.x][mvt->depart.y] != NULL && (jeu->plateau[mvt->depart.x][mvt->depart.y]->couleur == BLANC || jeu->plateau[mvt->depart.x][mvt->depart.y]->couleur == NOIR)) {
            jeu->plateau[mvt->arrivee.x][mvt->arrivee.y] = jeu->plateau[mvt->depart.x][mvt->depart.y];
            jeu->plateau[mvt->depart.x][mvt->depart.y] = NULL;
        }
    }
}

int main (){
     Jeu jeu;
     Mouvement mvt;

    // Initialisation du plateau
    printf("Initialisation du plateau...\n");
    initPlateau(&jeu);
    
    // Affichage du plateau
    printf("Affichage du plateau :\n");
    afficherPlateau(&jeu);
    
    return 1;
}
