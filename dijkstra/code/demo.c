/*
 * Les exercices de la vidéo, sur les graphes dessinés à l'écran.
 * Graphe 1 (non orienté, 6 villes) : A-B 4, A-C 2, B-C 1, B-D 5, C-D 8, C-E 10, D-E 2, D-F 6, E-F 3.
 * Graphe 2 (orienté) : A->B 3, A->C 7, B->C 2, B->D 6, C->D 1, C->E 8, D->E 2, E->B 1.
 * Graphe 3 (orienté, le piège) : A->B 2, A->C 1, B->C -2, C->D 1.
 */
#include <stdio.h>
#include "graphe.h"

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5

static void afficher_dist(const Graphe *g, const int dist[]) {
    for (int v = 0; v < g->n; v++) {
        if (dist[v] == INFINI)
            printf("%c=inf ", nom(v));
        else
            printf("%c=%d ", nom(v), dist[v]);
    }
    printf("\n");
}

static void afficher_ordre(const int ordre[], int nb) {
    for (int i = 0; i < nb; i++)
        printf("%c ", nom(ordre[i]));
    printf("\n");
}

int main(void) {
    int dist[6], pred[6], ordre[6], etapes[6];

    Graphe *g1 = graphe_creer(6, false);
    ajouter_arc(g1, A, B, 4);
    ajouter_arc(g1, A, C, 2);
    ajouter_arc(g1, B, C, 1);
    ajouter_arc(g1, B, D, 5);
    ajouter_arc(g1, C, D, 8);
    ajouter_arc(g1, C, E, 10);
    ajouter_arc(g1, D, E, 2);
    ajouter_arc(g1, D, F, 6);
    ajouter_arc(g1, E, F, 3);

    printf("=== 1. Dijkstra depuis A, graphe 1 (le tableau) ===\n");
    dijkstra(g1, A, dist, pred, true);
    printf("distances : ");
    afficher_dist(g1, dist);

    printf("\n=== 2. Le chemin le plus court de A a F ===\n");
    printf("iter : ");
    chemin_iter(pred, A, F);
    printf("rec  : ");
    chemin_rec(pred, A, F);
    printf("longueur : %d\n", dist[F]);

    printf("\n=== 3. Graphe 2 (oriente) : Dijkstra depuis A ===\n");
    Graphe *g2 = graphe_creer(5, true);
    ajouter_arc(g2, A, B, 3);
    ajouter_arc(g2, A, C, 7);
    ajouter_arc(g2, B, C, 2);
    ajouter_arc(g2, B, D, 6);
    ajouter_arc(g2, C, D, 1);
    ajouter_arc(g2, C, E, 8);
    ajouter_arc(g2, D, E, 2);
    ajouter_arc(g2, E, B, 1);
    dijkstra(g2, A, dist, pred, true);
    printf("distances : ");
    afficher_dist(g2, dist);
    printf("chemin A -> E, iter : ");
    chemin_iter(pred, A, E);
    printf("chemin A -> E, rec  : ");
    chemin_rec(pred, A, E);
    printf("depuis E : ");
    dijkstra(g2, E, dist, pred, false);
    afficher_dist(g2, dist);
    printf("chemin E -> A, iter : ");
    chemin_iter(pred, E, A);
    printf("chemin E -> A, rec  : ");
    chemin_rec(pred, E, A);

    printf("\n=== 4. Le moins d'etapes de A a F : parcours en largeur, graphe 1 ===\n");
    int nb = largeur(g1, A, ordre, etapes, pred);
    printf("ordre de visite : ");
    afficher_ordre(ordre, nb);
    printf("etapes : ");
    for (int v = 0; v < g1->n; v++)
        printf("%c=%d ", nom(v), etapes[v]);
    printf("\n");
    printf("chemin A -> F : ");
    chemin_iter(pred, A, F);

    printf("\n=== 5. Parcours en profondeur depuis F, graphe 1 ===\n");
    nb = profondeur_iter(g1, F, ordre);
    printf("profondeur, iter : ");
    afficher_ordre(ordre, nb);
    nb = profondeur_rec(g1, F, ordre);
    printf("profondeur, rec  : ");
    afficher_ordre(ordre, nb);
    nb = largeur(g1, F, ordre, etapes, pred);
    printf("largeur          : ");
    afficher_ordre(ordre, nb);

    printf("\n=== 6. Le piege : un poids negatif (graphe 3, oriente) ===\n");
    Graphe *g3 = graphe_creer(4, true);
    ajouter_arc(g3, A, B, 2);
    ajouter_arc(g3, A, C, 1);
    ajouter_arc(g3, B, C, -2);
    ajouter_arc(g3, C, D, 1);
    dijkstra(g3, A, dist, pred, true);
    printf("dijkstra     : ");
    afficher_dist(g3, dist);
    bellman_ford(g3, A, dist);
    printf("bellman-ford : ");
    afficher_dist(g3, dist);

    graphe_detruire(g1);
    graphe_detruire(g2);
    graphe_detruire(g3);
    return 0;
}
