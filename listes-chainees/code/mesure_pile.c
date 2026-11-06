/*
 * Itératif ou récursif : mesure. On construit une liste de N maillons, puis on la parcourt avec la
 * version demandée. Chaque appel récursif occupe de la place sur la pile d'appels (8 Mo par défaut
 * sous Linux) : au-delà d'une certaine longueur, une version récursive plante.
 * Usage : ./mesure_pile N iter|rec|rech|lib
 *   iter : longueur_iter      rec : longueur_rec
 *   rech : rechercher_rec d'une valeur absente (parcours complet)
 *   lib  : liberer_rec
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"

static int usage(const char *prog) {
    fprintf(stderr, "usage: %s N iter|rec|rech|lib\n", prog);
    return 2;
}

int main(int argc, char **argv) {
    if (argc < 3)
        return usage(argv[0]);
    const char *mode = argv[2];
    if (strcmp(mode, "iter") != 0 && strcmp(mode, "rec") != 0 &&
        strcmp(mode, "rech") != 0 && strcmp(mode, "lib") != 0)
        return usage(argv[0]);   /* un mode mal tapé ne doit pas lancer une autre version en silence */

    long n = atol(argv[1]);
    Maillon *l = NULL;
    for (long i = 0; i < n; i++)
        l = inserer_tete(l, (int)(i % 100));   /* en tête : O(1), la construction reste rapide */

    if (strcmp(mode, "lib") == 0) {
        liberer_rec(l);
        printf("lib : %ld maillons liberes\n", n);
        return 0;
    }
    if (strcmp(mode, "rech") == 0) {
        Maillon *m = rechercher_rec(l, -1);   /* -1 n'existe pas : parcours complet */
        printf("rech : %s\n", m ? "trouve" : "absent");
    } else {
        int lg = strcmp(mode, "rec") == 0 ? longueur_rec(l) : longueur_iter(l);
        printf("%s : longueur %d\n", mode, lg);
    }
    liberer_iter(l);
    return 0;
}
