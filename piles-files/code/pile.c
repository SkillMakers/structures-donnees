#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

static void *allouer(size_t taille) {
    void *m = malloc(taille);
    if (m == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return m;
}

Pile *pile_creer(void) {
    Pile *p = allouer(sizeof(Pile));
    p->sommet = NULL;
    return p;
}

void pile_detruire(Pile *p) {
    while (!pile_est_vide(p))
        depiler(p);
    free(p);
}

bool pile_est_vide(const Pile *p) {
    return p->sommet == NULL;
}

/* Insertion en tête de la liste chaînée : O(1), quelle que soit la taille. */
void empiler(Pile *p, int valeur) {
    Maillon *m = allouer(sizeof(Maillon));
    m->valeur = valeur;
    m->suivant = p->sommet;
    p->sommet = m;
}

/* Suppression de la tête : lire la valeur et le suivant AVANT free. */
int depiler(Pile *p) {
    assert(!pile_est_vide(p));
    Maillon *m = p->sommet;
    int valeur = m->valeur;
    p->sommet = m->suivant;
    free(m);
    return valeur;
}

int sommet(const Pile *p) {
    assert(!pile_est_vide(p));
    return p->sommet->valeur;
}

void pile_afficher(const Pile *p) {
    printf("sommet ->");
    for (const Maillon *m = p->sommet; m != NULL; m = m->suivant)
        printf(" %d", m->valeur);
    printf("\n");
}
