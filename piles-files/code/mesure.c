/*
 * Itératif ou récursif : mesure, sur une pile de N éléments (0 au fond, N - 1 au sommet).
 * Usage : ./mesure MODE N
 *   rech_iter, rech_rec : recherche d'une valeur absente (parcours complet), puis taille vérifiée
 *   inv_iter, inv_rec   : inversion de la pile, durée mesurée, puis sommet vérifié (il doit valoir 0)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exercices.h"

static int usage(const char *prog) {
    fprintf(stderr, "usage: %s rech_iter|rech_rec|inv_iter|inv_rec N (N > 0)\n", prog);
    return 2;
}

int main(int argc, char **argv) {
    if (argc < 3)
        return usage(argv[0]);
    const char *mode = argv[1];
    /* un mode mal tapé ne doit pas lancer une autre version en silence */
    bool rech_iter = strcmp(mode, "rech_iter") == 0, rech_rec = strcmp(mode, "rech_rec") == 0;
    bool inv_iter = strcmp(mode, "inv_iter") == 0, inv_rec = strcmp(mode, "inv_rec") == 0;
    if (!rech_iter && !rech_rec && !inv_iter && !inv_rec)
        return usage(argv[0]);

    char *fin;
    long n = strtol(argv[2], &fin, 10);
    if (*fin != '\0' || n <= 0)
        return usage(argv[0]);   /* N doit être un entier strictement positif */
    Pile *p = pile_creer();
    for (long i = 0; i < n; i++)
        empiler(p, (int)i);

    if (rech_iter || rech_rec) {
        bool trouve = rech_iter ? pile_rechercher_iter(p, -1) : pile_rechercher_rec(p, -1);
        printf("%s, N=%ld : %s, taille ensuite %d\n", mode, n, trouve ? "trouve" : "absent",
               pile_taille_iter(p));
    } else {
        clock_t debut = clock();
        if (inv_iter)
            pile_inverser_iter(p);
        else
            pile_inverser_rec(p);
        double s = (double)(clock() - debut) / CLOCKS_PER_SEC;
        printf("%s, N=%ld : %.3f s, sommet ensuite %d\n", mode, n, s, sommet(p));
    }
    pile_detruire(p);
    return 0;
}
