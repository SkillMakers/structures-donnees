/*
 * Arbre binaire de recherche (ABR) d'entiers.
 * Règle : pour chaque nœud, tout le sous-arbre gauche est plus petit, tout le sous-arbre droit est plus grand.
 * Les valeurs sont distinctes : insérer une valeur déjà présente ne change rien.
 * Un arbre est un pointeur vers sa racine ; l'arbre vide est NULL.
 * Chaque opération existe en deux versions : _iter (une boucle) et _rec (un appel récursif).
 */
#ifndef ABR_H
#define ABR_H

#include <stdbool.h>

typedef struct Noeud {
    int valeur;
    struct Noeud *gauche;
    struct Noeud *droit;
} Noeud;

/* Renvoie le nœud qui contient valeur, ou NULL si elle est absente. */
Noeud *rechercher_iter(Noeud *racine, int valeur);
Noeud *rechercher_rec(Noeud *racine, int valeur);

/* Ajoute valeur à sa place et renvoie la racine (nouvelle si l'arbre était vide). */
Noeud *inserer_iter(Noeud *racine, int valeur);
Noeud *inserer_rec(Noeud *racine, int valeur);

/* L'arbre ne doit pas être vide. */
int minimum_iter(const Noeud *racine);
int minimum_rec(const Noeud *racine);
int maximum_iter(const Noeud *racine);
int maximum_rec(const Noeud *racine);

/* Hauteur = nombre de niveaux : 0 pour l'arbre vide, 1 pour une racine seule. */
int hauteur_rec(const Noeud *racine);
int hauteur_iter(const Noeud *racine);
int nb_noeuds_rec(const Noeud *racine);
int nb_noeuds_iter(const Noeud *racine);
int nb_feuilles_rec(const Noeud *racine);
int nb_feuilles_iter(const Noeud *racine);

/* Parcours en profondeur : affichent les valeurs séparées par un espace, puis un saut de ligne. */
void prefixe_rec(const Noeud *racine);
void prefixe_iter(const Noeud *racine);
void infixe_rec(const Noeud *racine);
void infixe_iter(const Noeud *racine);
void suffixe_rec(const Noeud *racine);
void suffixe_iter(const Noeud *racine);
/* Parcours en largeur : niveau par niveau, de gauche à droite. */
void largeur(const Noeud *racine);

/* Supprime valeur si elle est présente (libère son nœud) et renvoie la nouvelle racine. */
Noeud *supprimer_iter(Noeud *racine, int valeur);
Noeud *supprimer_rec(Noeud *racine, int valeur);

/* Libère tous les nœuds. */
void liberer(Noeud *racine);

#endif
