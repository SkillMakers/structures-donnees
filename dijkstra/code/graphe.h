/*
 * Graphe pondéré par listes d'adjacence.
 * Les sommets sont numérotés de 0 à n - 1 et s'affichent A, B, C...
 * Chaque sommet a une liste chaînée de ses arcs (comme dans la vidéo sur les listes chaînées),
 * triée par sommet d'arrivée : les parcours visitent les voisins dans l'ordre alphabétique.
 * Un graphe non orienté range chaque arête deux fois, une fois dans chaque sens.
 */
#ifndef GRAPHE_H
#define GRAPHE_H

#include <limits.h>
#include <stdbool.h>

#define INFINI INT_MAX /* distance d'un sommet qu'on ne peut pas atteindre */
#define AUCUN (-1)     /* prédécesseur du départ, ou d'un sommet qu'on ne peut pas atteindre */

typedef struct Arc {
    int vers;
    int poids;
    struct Arc *suivant;
} Arc;

typedef struct {
    int n;
    bool oriente;
    int nb_arcs; /* nombre de maillons dans toutes les listes (une arête non orientée en compte deux) */
    Arc **adj;   /* adj[u] : la liste des arcs qui partent de u */
} Graphe;

Graphe *graphe_creer(int n, bool oriente);
void graphe_detruire(Graphe *g);
/* Ajoute l'arc u -> v de poids p (et v -> u si le graphe n'est pas orienté). */
void ajouter_arc(Graphe *g, int u, int v, int poids);

/* Les distances sont des int : la somme des poids d'un chemin doit rester sous INT_MAX (INFINI).
   Pour des poids énormes, il faudrait des long long et un test de dépassement. */

/* Dijkstra, version tableau (O(n²)) : poids positifs ou nuls seulement.
   Remplit dist[] (INFINI si inaccessible) et pred[] (AUCUN pour le départ).
   Si trace est vrai, affiche une ligne du tableau à chaque sommet fixé. */
void dijkstra(const Graphe *g, int depart, int dist[], int pred[], bool trace);

/* Bellman-Ford : accepte les poids négatifs (sans cycle négatif). Sert à vérifier le piège. */
void bellman_ford(const Graphe *g, int depart, int dist[]);

/* Affiche le chemin depart -> arrivee à partir de pred[], par exemple « A C B D », ou « inaccessible ».
   depart peut être n'importe quel sommet du chemin remonté, pas seulement la source du calcul.
   Les prédécesseurs donnent le chemin à l'envers : la boucle utilise une pile, la récursion la pile d'appels. */
void chemin_iter(const int pred[], int depart, int arrivee);
void chemin_rec(const int pred[], int depart, int arrivee);

/* Parcours en largeur depuis depart : remplit ordre[] (les sommets dans l'ordre de visite),
   etapes[] (le nombre d'arcs du chemin le plus court en étapes, -1 si inaccessible) et
   pred[] (le sommet d'où l'on arrive ; AUCUN pour le départ et les inaccessibles), à donner à chemin_iter / chemin_rec.
   Renvoie le nombre de sommets visités. Utilise une file. */
int largeur(const Graphe *g, int depart, int ordre[], int etapes[], int pred[]);

/* Parcours en profondeur depuis depart : remplit ordre[], renvoie le nombre de sommets visités.
   Les deux versions visitent les sommets dans le même ordre. */
int profondeur_rec(const Graphe *g, int depart, int ordre[]);
int profondeur_iter(const Graphe *g, int depart, int ordre[]);

/* La lettre d'un sommet : 0 -> 'A'. */
char nom(int s);

#endif
