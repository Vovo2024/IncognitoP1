#ifndef FONCTION_H
#define FONCTION_H

// Définition de la taille du plateau
#define TAILLE 5
typedef enum _couleur{BLANC, NOIR} Couleur ; 
typedef enum _type{CHEVALIER, ESPION} Type ;


typedef struct _pion {
    Type type;
    Couleur couleur ;
}Pion;

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
#endif 