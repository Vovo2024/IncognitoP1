#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE 5




typedef enum _couleur{BLANC, NOIR} Couleur ; 
typedef enum _type{CHEVALIER, ESPION, CHATEAU} Type ;

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

typedef struct {//structure pour les couple (i,j)=> position des espions
    int i;
    int j;
} PosEspion;

PosEspion positions_valides[] = {//Declaration de la liste des posValides des espions 
    {1, 2},
    {3, 0},
    {3, 1},
    {4, 1},
    {4, 2}
};

// Fonction pour obtenir une position valide aléatoire pour un espion
PosEspion obtenir_position_valide() {
    int i = rand() % 5;
    return positions_valides[i];
}


//Fonction pour initialiser le plateau avec les positions initiales des pions
void initChevalier(Jeu  *jeu){
    srand(time(NULL));
    printf("TEST 1 OK\n");

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
    printf("OK");
}
void initEspion(Jeu *jeu){
    PosEspion pos_Espion_b = obtenir_position_valide();
    PosEspion pos_Espion_n = {pos_Espion_b.j, pos_Espion_b.i};

    if(jeu->plateau[pos_Espion_b.i][pos_Espion_b.j] == NULL){
        jeu->plateau[pos_Espion_b.i][pos_Espion_b.j] = (Pion *)malloc(sizeof(Pion)); 
    }
    if(jeu->plateau[pos_Espion_n.i][pos_Espion_n.j] == NULL){
        jeu->plateau[pos_Espion_n.i][pos_Espion_n.j] = (Pion *)malloc(sizeof(Pion)); 
    }
    jeu->plateau[pos_Espion_b.i][pos_Espion_b.j]->type = ESPION;
    jeu->plateau[pos_Espion_b.i][pos_Espion_b.j]->couleur = BLANC;

    jeu->plateau[pos_Espion_n.i][pos_Espion_n.j]->type = ESPION;
    jeu->plateau[pos_Espion_n.i][pos_Espion_n.j]->couleur = NOIR;


}
void initChateau(Jeu *jeu){

    if(jeu->plateau[0][TAILLE - 1] != NULL){//INIT CHATEAU BLANC
        jeu->plateau[TAILLE - 1][0] = (Pion *)malloc(sizeof(Pion));
        jeu->plateau[TAILLE - 1][0]-> type = CHATEAU;
        jeu->plateau[TAILLE - 1][0]-> couleur = BLANC;
    }

    if(jeu->plateau[0][TAILLE - 1] != NULL){//INIT CHATEAU NOIR
        jeu->plateau[0][TAILLE - 1] = (Pion *)malloc(sizeof(Pion));
        jeu->plateau[0][TAILLE - 1]-> type = CHATEAU;
        jeu->plateau[0][TAILLE - 1]-> couleur = NOIR;
    }
}


void initPlateau(Jeu *jeu){
    initChevalier(jeu);  // Pas besoin de &jeu
    initEspion(jeu);     // Pas besoin de &jeu
    initChateau(jeu);    // Pas besoin de &jeu
}

void afficherPlateau(Jeu *jeu){
    //Fonction qui affiche le plateau dans le terminal
    for(int i = 0; i < TAILLE ; i++){
        printf("-----------\n");
        for(int j = 0; j < TAILLE ; j++){

            if(jeu->plateau[i][j] == NULL){//case vide
                printf("| ");
            }
            else if(jeu->plateau[i][j]->type == CHATEAU) {//case chateau
                printf("|C");
            }else if(jeu->plateau[i][j]->couleur == BLANC &&  jeu->plateau[i][j]->type == CHEVALIER){//case pion blanc
                printf("|b");
            }else if(jeu->plateau[i][j]->couleur == NOIR && jeu->plateau[i][j]->type == CHEVALIER){//case pion noir
                printf("|n");
            }else if(jeu->plateau[i][j]->type == ESPION) {//case espion
                printf("|e");
            }
            
        }
        printf("|\n");
    }
}


int coup_valide(Jeu * jeu, Mouvement * mvt) {
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
        printf("Joueur %s tu as déjà un pion à cet endroit, choisis une autre position.", (pion_depart->couleur == BLANC) ? "blanc" : "noir");
        return 0;  // 
    }

    // direction du mouvement
    int dx = arrivee.x - depart.x;
    int dy = arrivee.y - depart.y;

    // Vérifier que le déplacement est bien dans une direction droite ou diagonale
    if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) {
        return 0;  // Le mouvement n'est ni droit ni diagonal
    }

    // Vérifier si le chemin est libre (sans pièces entre la case de départ et d'arrivée)
    int pas_x = (dx != 0) ? dx / abs(dx) : 0;  // Pas pour avancer en x (0 si pas de mouvement horizontal)
    int pas_y = (dy != 0) ? dy / abs(dy) : 0;  // Pas pour avancer en y (0 si pas de mouvement vertical)

    // Parcourir les cases intermédiaires
    for (int x = depart.x + pas_x, y = depart.y + pas_y; x != arrivee.x || y != arrivee.y;  x += pas_x, y += pas_y) {
        if (jeu->plateau[x][y] != NULL) {  // Il y a une pièce sur le chemin
            return 0;  // Le chemin est bloqué
        }
    }

    // Si toutes les conditions sont satisfaites, le coup est valide

    return 1;
}

/*// Fonction pour tester le type de chaque pion dans le plateau
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
}*/

/*void afficherDetailsPions(Jeu* jeu) {
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
}*/


void gagne(Couleur couleur, Pion pion){
    //Fonction qui donne la phrase lorsqu'un joueur gagne
    printf("Le gagnant est le joueur %s : BRAVO !", (pion.couleur == BLANC) ? "blanc" : "noir");
}

//Fonction qui questionne le pion designé
void interroge(Mouvement * mvt, Jeu * jeu, Pion pion){ 
    do {
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
    } while (coup_valide(jeu, mvt));
}

int espion_dans_chateau(Mouvement *mvt, Pion *pion) {
    int arrivee_x = mvt->arrivee.x;
    int arrivee_y = mvt->arrivee.y;

    //vérifiez si le pion est un espion
    if (pion->type == ESPION) {
        //vérifiez si l'espion arrive dans le château adverse
        if ((arrivee_x == 0 && arrivee_y == 4) ||  //château blanc
            (arrivee_x == 4 && arrivee_y == 0)) { //château noir
            return 1;
        }
    }
    return 0;
}

//Fonction qui récupère les saisies de l'utilisateur pour déplacer un pion ou l'interroger
void recup_saisies(Mouvement * mvt, Jeu * jeu, Pion * pion){
    char c;
    do {
            printf("Joueur %s, voulez vous faire un déplacement ou une interrogation? ('d' ou 'i')",  (pion->couleur == BLANC) ? "blanc" : "noir");
            scanf(" %c", &c);
            if (c == 'd'){
                //récupère les données de case de départ et d'arrivée
                printf("Quel dépplacement joueur %s ?\n Saisie sous la forme (a,b) --> (c,d)",  (pion->couleur == BLANC) ? "blanc" : "noir");
                scanf("%d %d %d %d", &(mvt->depart.x), &(mvt->depart.y), &(mvt->arrivee.x), &(mvt->arrivee.y));
            } 
            else if (c == 'i'){
                interroge(mvt, jeu, *pion);
            }
    } while(coup_valide(jeu, mvt));
}


//Fonction qui déplace les pions
void Deplacements(Mouvement * mvt, Jeu * jeu, Pion * pion){
    int depart_x = mvt->depart.x;
    int depart_y = mvt->depart.y;
    int arrivee_x = mvt->arrivee.x;
    int arrivee_y = mvt->arrivee.y;
    if (coup_valide(jeu, mvt)){
        //la valeur d'arrivée devient celle de départ -> le pion est déplacé, et la valeur de départ est réinitialisée à NULL
        if (jeu->plateau[depart_x][depart_y] != NULL && (jeu->plateau[depart_x][depart_y]->couleur == BLANC || jeu->plateau[depart_x][depart_y]->couleur == NOIR)) {
            jeu->plateau[arrivee_x][arrivee_y] = jeu->plateau[depart_x][depart_y];
            jeu->plateau[depart_x][depart_y] = NULL;
        }
        //si un espion entre dans le chateau adverse, la partie est gagnée
        else if (jeu->plateau[depart_x][depart_y]->type == ESPION && //condition à ajt : ESPION ENTRE DANS CHATEAU ADVERSE!! 
                (arrivee_x == 0 || arrivee_x == 4) && 
                (arrivee_y == 0 || arrivee_y == 4) && 
                (arrivee_x != arrivee_y)) {
                    gagne(pion->couleur, *pion);
        }
        else if (espion_dans_chateau(mvt, jeu->plateau[arrivee_x][arrivee_y])) {
            gagne(pion->couleur, *pion);  // Appeler la fonction pour annoncer le gagnant
        }
    }
}


int main() {
    Jeu jeu;
    Mouvement mvt;
    Pion pion;

    // Initialisation du plateau
    initPlateau(&jeu);
    // Affichage du plateau
    printf("Affichage du plateau :\n");
    afficherPlateau(&jeu);
    

    while (1){
        recup_saisies(&mvt, &jeu, &pion);
        if (coup_valide(&jeu, &mvt)){
             Deplacements(&mvt, &jeu, &pion);
             afficherPlateau(&jeu);
             jeu.joueur = (jeu.joueur == BLANC) ? NOIR : BLANC;

        } else {
            printf("Position non licite, veuillez réessayer.");
            continue;
        }
    }
    return 0;
}