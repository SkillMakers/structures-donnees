/*
 * File d'entiers (FIFO : le premier enfilé est le premier défilé).
 * Implémentée par un tableau circulaire : on retire à l'indice tete, on ajoute à (tete + nb) % capacite.
 * Le compteur nb distingue la file vide (nb == 0) de la file pleine (nb == capacite).
 */
#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

typedef struct {
    int *cases;
    int capacite;
    int tete;   /* indice du premier élément */
    int nb;     /* nombre d'éléments */
} File;

/* Crée une file vide de capacite cases (arrête le programme si la mémoire manque). */
File *file_creer(int capacite);
void file_detruire(File *f);

bool file_est_vide(const File *f);
bool file_est_pleine(const File *f);
/* La file ne doit pas être pleine. */
void enfiler(File *f, int valeur);
/* La file ne doit pas être vide. */
int defiler(File *f);

/* Affiche « tete -> 4 9 2 7 <- queue » (lit la structure interne : réservé aux tests). */
void file_afficher(const File *f);

#endif
