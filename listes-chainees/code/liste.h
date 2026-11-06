/*
 * Liste simplement chaînée d'entiers.
 * Chaque opération existe en deux versions : _iter (une boucle) et _rec (un appel récursif).
 * Une liste est un pointeur vers son premier maillon (la tête) ; la liste vide est NULL.
 */
#ifndef LISTE_H
#define LISTE_H

typedef struct Maillon {
    int valeur;
    struct Maillon *suivant;
} Maillon;

/* Alloue un maillon (arrête le programme si la mémoire manque). */
Maillon *creer_maillon(int valeur);
/* Ajoute valeur au début et renvoie la nouvelle tête. */
Maillon *inserer_tete(Maillon *tete, int valeur);

/* Ajoute valeur à la fin et renvoie la tête (nouvelle si la liste était vide). */
Maillon *inserer_queue_iter(Maillon *tete, int valeur);
Maillon *inserer_queue_rec(Maillon *tete, int valeur);

/* Affiche « 12 -> 5 -> NULL » ; les versions _envers affichent « 5 12 ». */
void afficher_iter(const Maillon *tete);
void afficher_rec(const Maillon *tete);
void afficher_envers_iter(const Maillon *tete);
void afficher_envers_rec(const Maillon *tete);

int longueur_iter(const Maillon *tete);
int longueur_rec(const Maillon *tete);
int somme_iter(const Maillon *tete);
int somme_rec(const Maillon *tete);
/* La liste ne doit pas être vide (le maximum d'une liste vide n'existe pas). */
int maximum_iter(const Maillon *tete);
int maximum_rec(const Maillon *tete);

/* Renvoie le premier maillon qui contient valeur, ou NULL si elle est absente. */
Maillon *rechercher_iter(Maillon *tete, int valeur);
Maillon *rechercher_rec(Maillon *tete, int valeur);

/* Supprime la première occurrence de valeur (libère son maillon) et renvoie la nouvelle tête.
   Si valeur est absente, la liste est inchangée. */
Maillon *supprimer_iter(Maillon *tete, int valeur);
Maillon *supprimer_rec(Maillon *tete, int valeur);

/* Libère tous les maillons. Après l'appel, tete ne doit plus être utilisé. */
void liberer_iter(Maillon *tete);
void liberer_rec(Maillon *tete);

#endif
