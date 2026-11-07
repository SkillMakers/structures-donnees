/*
 * Dépiler une pile vide.
 *   ./piege_vide sans_test : la version qui ne teste rien lit le sommet NULL -> segmentation fault
 *   ./piege_vide assert    : depiler de pile.c, avec son assert -> message clair, puis arrêt
 */
#include <stdio.h>
#include <string.h>
#include "pile.h"

static int depiler_sans_test(Pile *p) {
    Maillon *m = p->sommet;
    int valeur = m->valeur;   /* sommet NULL : lecture à l'adresse 0 */
    p->sommet = m->suivant;
    return valeur;
}

int main(int argc, char **argv) {
    if (argc < 2 || (strcmp(argv[1], "sans_test") != 0 && strcmp(argv[1], "assert") != 0)) {
        fprintf(stderr, "usage: %s sans_test|assert\n", argv[0]);
        return 2;
    }
    Pile *p = pile_creer();
    empiler(p, 4);
    printf("depiler : %d\n", depiler(p));
    fflush(stdout);
    int v = strcmp(argv[1], "sans_test") == 0 ? depiler_sans_test(p) : depiler(p);
    printf("depiler encore : %d\n", v);
    pile_detruire(p);
    return 0;
}
