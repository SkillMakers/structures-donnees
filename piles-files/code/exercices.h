/*
 * Les exercices corrigés, chacun en deux versions : _iter (une boucle) et _rec (un appel récursif).
 * Règle de tous les énoncés : on n'utilise que empiler, depiler, sommet, pile_est_vide (et enfiler,
 * defiler, file_est_vide pour la file). Jamais la structure interne.
 */
#ifndef EXERCICES_H
#define EXERCICES_H

#include <stdbool.h>
#include "file.h"
#include "pile.h"

/* Dit si valeur est dans la pile. Au retour, la pile est exactement comme avant. */
bool pile_rechercher_iter(Pile *p, int valeur);
bool pile_rechercher_rec(Pile *p, int valeur);

/* Nombre d'éléments. Au retour, la pile est exactement comme avant. */
int pile_taille_iter(Pile *p);
int pile_taille_rec(Pile *p);

/* Renvoie une nouvelle pile, identique. Au retour, p est exactement comme avant. */
Pile *pile_copier_iter(Pile *p);
Pile *pile_copier_rec(Pile *p);

/* Inverse la pile : le sommet passe au fond. */
void pile_inverser_iter(Pile *p);
void pile_inverser_rec(Pile *p);

/* Inverse la file : la tête passe en queue. */
void file_inverser_iter(File *f);
void file_inverser_rec(File *f);

/* Dit si les parenthèses, crochets et accolades de texte sont bien équilibrés. */
bool parentheses_equilibrees(const char *texte);

#endif
