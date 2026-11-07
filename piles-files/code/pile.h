/*
 * Pile d'entiers (LIFO : le dernier empilé est le premier dépilé).
 * Implémentée par une liste chaînée : empiler = insérer en tête, dépiler = supprimer la tête.
 * Les exercices n'utilisent que les fonctions de ce fichier, jamais la structure interne.
 */
#ifndef PILE_H
#define PILE_H

#include <stdbool.h>

typedef struct Maillon {
    int valeur;
    struct Maillon *suivant;
} Maillon;

typedef struct {
    Maillon *sommet;   /* NULL quand la pile est vide */
} Pile;

/* Crée une pile vide (arrête le programme si la mémoire manque). */
Pile *pile_creer(void);
/* Libère les maillons restants et la pile. */
void pile_detruire(Pile *p);

bool pile_est_vide(const Pile *p);
void empiler(Pile *p, int valeur);
/* La pile ne doit pas être vide. */
int depiler(Pile *p);
/* Lit le sommet sans le retirer. La pile ne doit pas être vide. */
int sommet(const Pile *p);

/* Affiche « sommet -> 7 2 9 4 » (lit la structure interne : réservé aux tests). */
void pile_afficher(const Pile *p);

#endif
