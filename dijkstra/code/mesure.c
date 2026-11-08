/*
 * Itératif ou récursif, mesuré.
 *   mesure chaine_iter N / chaine_rec N
 *       un graphe en ligne de N sommets (0 -> 1 -> 2 -> ... -> N-1), puis un parcours en profondeur depuis 0,
 *       avec la pile (profondeur_iter) ou en récursif (profondeur_rec) : un appel par sommet, empilés.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphe.h"

int main(int argc, char **argv) {
    if (argc != 3 || (strcmp(argv[1], "chaine_iter") != 0 && strcmp(argv[1], "chaine_rec") != 0)) {
        fprintf(stderr, "usage : %s chaine_iter|chaine_rec N (2 <= N <= 20000000)\n", argv[0]);
        return 2;
    }
    char *fin;
    long n = strtol(argv[2], &fin, 10);
    if (*fin != '\0' || n < 2 || n > 20000000) { /* au-delà, le graphe et les tableaux dépassent ~1 Go */
        fprintf(stderr, "N doit etre un entier entre 2 et 20000000\n");
        return 2;
    }
    Graphe *g = graphe_creer((int)n, true);
    /* Chaque liste n'a qu'un arc : l'insertion triée de ajouter_arc ne parcourt rien, la construction est en O(N). */
    for (int u = 0; u + 1 < (int)n; u++)
        ajouter_arc(g, u, u + 1, 1);
    int *ordre = malloc((size_t)n * sizeof(int));
    if (ordre == NULL) {
        perror("malloc");
        return 1;
    }
    clock_t debut = clock();
    int nb = strcmp(argv[1], "chaine_iter") == 0 ? profondeur_iter(g, 0, ordre) : profondeur_rec(g, 0, ordre);
    printf("%s : %d sommets visites en %.3f s\n", argv[1], nb, (double)(clock() - debut) / CLOCKS_PER_SEC);
    free(ordre);
    graphe_detruire(g);
    return 0;
}
