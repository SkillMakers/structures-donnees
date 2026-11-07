/*
 * Deux pièges, sans plantage :
 *   1. la recherche qui dépile sans rempiler : elle répond juste, mais la pile est détruite ;
 *   2. la file circulaire sans compteur : avec seulement tete et queue, pleine et vide se confondent.
 */
#include <stdbool.h>
#include <stdio.h>
#include "pile.h"

static bool pile_rechercher_faux(Pile *p, int valeur) {
    while (!pile_est_vide(p))
        if (depiler(p) == valeur)
            return true;
    return false;
}

/* File circulaire « naïve » : queue = indice de la prochaine case libre, pas de compteur. */
typedef struct {
    int cases[4];
    int tete;
    int queue;
} FileNaive;

static bool naive_est_vide(const FileNaive *f) {
    return f->tete == f->queue;
}

static void naive_enfiler(FileNaive *f, int valeur) {
    f->cases[f->queue] = valeur;
    f->queue = (f->queue + 1) % 4;
}

int main(void) {
    printf("=== Piege 1 : rechercher en depilant ===\n");
    Pile *p = pile_creer();
    int valeurs[] = {4, 9, 2, 7};
    for (int i = 0; i < 4; i++)
        empiler(p, valeurs[i]);
    pile_afficher(p);
    printf("rechercher 9 : %s\n", pile_rechercher_faux(p, 9) ? "trouve" : "absent");
    printf("apres : ");
    pile_afficher(p);
    pile_detruire(p);

    printf("\n=== Piege 2 : file circulaire sans compteur (4 cases) ===\n");
    FileNaive f = {.tete = 0, .queue = 0};
    printf("file neuve : tete = %d, queue = %d, vide : %s\n", f.tete, f.queue,
           naive_est_vide(&f) ? "oui" : "non");
    for (int i = 0; i < 4; i++)
        naive_enfiler(&f, valeurs[i]);
    printf("apres 4 enfiler : tete = %d, queue = %d, vide : %s\n", f.tete, f.queue,
           naive_est_vide(&f) ? "oui" : "non");
    return 0;
}
