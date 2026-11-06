/*
 * Le piège classique : modifier la tête dans une fonction qui reçoit le pointeur PAR VALEUR.
 * La fonction modifie sa copie ; la liste du main ne change pas, et le maillon créé est perdu.
 */
#include <stdio.h>
#include "liste.h"

static void inserer_tete_faux(Maillon *tete, int valeur) {
    Maillon *m = creer_maillon(valeur);
    m->suivant = tete;
    tete = m;            /* ne change que la copie locale */
}

/* Correction 1 : renvoyer la nouvelle tête (c'est le choix de liste.c). */
/* Correction 2 : passer l'adresse du pointeur. */
static void inserer_tete_adresse(Maillon **tete, int valeur) {
    Maillon *m = creer_maillon(valeur);
    m->suivant = *tete;
    *tete = m;
}

int main(void) {
    Maillon *l = NULL;
    l = inserer_queue_iter(l, 12);
    l = inserer_queue_iter(l, 5);

    inserer_tete_faux(l, 99);
    printf("apres inserer_tete_faux(l, 99)       : "); afficher_iter(l);

    l = inserer_tete(l, 99);
    printf("apres l = inserer_tete(l, 99)        : "); afficher_iter(l);

    inserer_tete_adresse(&l, 42);
    printf("apres inserer_tete_adresse(&l, 42)   : "); afficher_iter(l);

    liberer_iter(l);
    return 0;
}
