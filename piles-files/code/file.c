#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

File *file_creer(int capacite) {
    assert(capacite > 0);
    File *f = malloc(sizeof(File));
    int *cases = malloc(capacite * sizeof(int));
    if (f == NULL || cases == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    f->cases = cases;
    f->capacite = capacite;
    f->tete = 0;
    f->nb = 0;
    return f;
}

void file_detruire(File *f) {
    free(f->cases);
    free(f);
}

bool file_est_vide(const File *f) {
    return f->nb == 0;
}

bool file_est_pleine(const File *f) {
    return f->nb == f->capacite;
}

/* La case libre suit le dernier élément ; le modulo fait repartir l'indice à 0 après la dernière case. */
void enfiler(File *f, int valeur) {
    assert(!file_est_pleine(f));
    f->cases[(f->tete + f->nb) % f->capacite] = valeur;
    f->nb++;
}

int defiler(File *f) {
    assert(!file_est_vide(f));
    int valeur = f->cases[f->tete];
    f->tete = (f->tete + 1) % f->capacite;
    f->nb--;
    return valeur;
}

void file_afficher(const File *f) {
    printf("tete ->");
    for (int i = 0; i < f->nb; i++)
        printf(" %d", f->cases[(f->tete + i) % f->capacite]);
    printf(" <- queue\n");
}
