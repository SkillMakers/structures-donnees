/*
 * Le maximum récursif mal écrit : l'appel récursif apparaît deux fois dans la même expression.
 * Le second appel n'a lieu que si le maximum est dans le reste : on compte les appels sur une liste
 * croissante (pire cas) et sur la même liste à l'envers.
 */
#include <stdio.h>
#include "liste.h"

static long appels = 0;

static int maximum_mauvais(const Maillon *t) {
    appels++;
    if (t->suivant == NULL)
        return t->valeur;
    return t->valeur > maximum_mauvais(t->suivant)
               ? t->valeur
               : maximum_mauvais(t->suivant);
}

int main(void) {
    for (int croissante = 1; croissante >= 0; croissante--) {
        Maillon *l = NULL;
        for (int i = 0; i < 30; i++)
            l = inserer_queue_iter(l, croissante ? i + 1 : 30 - i);
        appels = 0;
        int m = maximum_mauvais(l);
        printf("%s, 30 maillons : maximum %d, %ld appels\n",
               croissante ? "croissante  " : "decroissante", m, appels);
        liberer_iter(l);
    }
    return 0;
}
