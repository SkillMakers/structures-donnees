/*
 * Les exercices de la vidéo, sur le même arbre du début à la fin :
 * on insère 50, 30, 70, 20, 40, 60, 80, 35, 65 dans cet ordre.
 *
 *              50
 *          /        \
 *        30          70
 *       /  \        /  \
 *     20    40    60    80
 *          /        \
 *        35          65
 */
#include <stdio.h>
#include "abr.h"

static const int VALEURS[] = {50, 30, 70, 20, 40, 60, 80, 35, 65};
#define NB_VALEURS (int)(sizeof VALEURS / sizeof VALEURS[0])

static Noeud *arbre_iter(void) {
    Noeud *racine = NULL;
    for (int i = 0; i < NB_VALEURS; i++)
        racine = inserer_iter(racine, VALEURS[i]);
    return racine;
}

static Noeud *arbre_rec(void) {
    Noeud *racine = NULL;
    for (int i = 0; i < NB_VALEURS; i++)
        racine = inserer_rec(racine, VALEURS[i]);
    return racine;
}

static const char *trouve(const Noeud *n) {
    return n != NULL ? "trouve" : "absent (NULL)";
}

/* Supprime valeur avec les deux versions, chacune sur un arbre neuf, et affiche le résultat. */
static void essai_suppression(int valeur, const char *cas) {
    printf("--- supprimer %d (%s)\n", valeur, cas);
    Noeud *a = supprimer_iter(arbre_iter(), valeur);
    Noeud *b = supprimer_rec(arbre_iter(), valeur);
    printf("iter, largeur : ");
    largeur(a);
    printf("rec,  largeur : ");
    largeur(b);
    printf("iter, infixe  : ");
    infixe_iter(a);
    printf("rec,  infixe  : ");
    infixe_rec(b);
    liberer(a);
    liberer(b);
}

int main(void) {
    printf("=== 1. Inserer 50, 30, 70, 20, 40, 60, 80, 35, 65 ===\n");
    Noeud *a = arbre_iter();
    Noeud *b = arbre_rec();
    printf("iter, largeur : ");
    largeur(a);
    printf("rec,  largeur : ");
    largeur(b);
    printf("inserer 40 une 2e fois, largeur : ");
    a = inserer_iter(a, 40);
    largeur(a);
    liberer(b);

    printf("\n=== 2. Rechercher ===\n");
    printf("iter 35 : %s\n", trouve(rechercher_iter(a, 35)));
    printf("rec  35 : %s\n", trouve(rechercher_rec(a, 35)));
    printf("iter 55 : %s\n", trouve(rechercher_iter(a, 55)));
    printf("rec  55 : %s\n", trouve(rechercher_rec(a, 55)));

    printf("\n=== 3. Minimum et maximum ===\n");
    printf("minimum : iter %d, rec %d\n", minimum_iter(a), minimum_rec(a));
    printf("maximum : iter %d, rec %d\n", maximum_iter(a), maximum_rec(a));

    printf("\n=== 4. Hauteur, noeuds, feuilles ===\n");
    printf("hauteur     : iter %d, rec %d\n", hauteur_iter(a), hauteur_rec(a));
    printf("noeuds      : iter %d, rec %d\n", nb_noeuds_iter(a), nb_noeuds_rec(a));
    printf("feuilles    : iter %d, rec %d\n", nb_feuilles_iter(a), nb_feuilles_rec(a));

    printf("\n=== 5. Parcours en profondeur ===\n");
    printf("prefixe rec  : ");
    prefixe_rec(a);
    printf("prefixe iter : ");
    prefixe_iter(a);
    printf("infixe  rec  : ");
    infixe_rec(a);
    printf("infixe  iter : ");
    infixe_iter(a);
    printf("suffixe rec  : ");
    suffixe_rec(a);
    printf("suffixe iter : ");
    suffixe_iter(a);

    printf("\n=== 6. Parcours en largeur ===\n");
    printf("largeur : ");
    largeur(a);
    liberer(a);

    printf("\n=== 7. Supprimer ===\n");
    essai_suppression(20, "une feuille");
    essai_suppression(60, "un seul fils");
    essai_suppression(30, "deux fils : remplace par son successeur 35");
    essai_suppression(50, "la racine, deux fils : remplacee par son successeur 60, qui a un fils droit");
    essai_suppression(70, "deux fils : son successeur 80 est son fils droit");
    essai_suppression(55, "absent");
    return 0;
}
