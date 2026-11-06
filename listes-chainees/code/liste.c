#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

Maillon *creer_maillon(int valeur) {
    Maillon *m = malloc(sizeof(Maillon));
    if (m == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    m->valeur = valeur;
    m->suivant = NULL;
    return m;
}

Maillon *inserer_tete(Maillon *tete, int valeur) {
    Maillon *m = creer_maillon(valeur);
    m->suivant = tete;
    return m;
}

/* ---------- insertion en queue ---------- */

Maillon *inserer_queue_iter(Maillon *tete, int valeur) {
    Maillon *m = creer_maillon(valeur);
    if (tete == NULL)
        return m;
    Maillon *p = tete;
    while (p->suivant != NULL)
        p = p->suivant;
    p->suivant = m;
    return tete;
}

Maillon *inserer_queue_rec(Maillon *tete, int valeur) {
    if (tete == NULL)
        return creer_maillon(valeur);
    tete->suivant = inserer_queue_rec(tete->suivant, valeur);
    return tete;
}

/* ---------- affichage ---------- */

void afficher_iter(const Maillon *tete) {
    for (const Maillon *p = tete; p != NULL; p = p->suivant)
        printf("%d -> ", p->valeur);
    printf("NULL\n");
}

static void afficher_rec_aux(const Maillon *tete) {
    if (tete == NULL) {
        printf("NULL");
        return;
    }
    printf("%d -> ", tete->valeur);
    afficher_rec_aux(tete->suivant);
}

void afficher_rec(const Maillon *tete) {
    afficher_rec_aux(tete);
    printf("\n");
}

/* Sans pointeur vers le précédent, il faut mémoriser les valeurs : un tableau sert de pile. */
void afficher_envers_iter(const Maillon *tete) {
    int n = longueur_iter(tete);
    int *pile = malloc(n * sizeof(int));
    if (pile == NULL && n > 0) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int sommet = 0;
    for (const Maillon *p = tete; p != NULL; p = p->suivant)
        pile[sommet++] = p->valeur;
    while (sommet > 0)
        printf("%d ", pile[--sommet]);
    printf("\n");
    free(pile);
}

/* On affiche APRES l'appel : la pile d'appels rend les valeurs dans l'ordre inverse. */
static void afficher_envers_rec_aux(const Maillon *tete) {
    if (tete == NULL)
        return;
    afficher_envers_rec_aux(tete->suivant);
    printf("%d ", tete->valeur);
}

void afficher_envers_rec(const Maillon *tete) {
    afficher_envers_rec_aux(tete);
    printf("\n");
}

/* ---------- longueur, somme, maximum ---------- */

int longueur_iter(const Maillon *tete) {
    int n = 0;
    for (const Maillon *p = tete; p != NULL; p = p->suivant)
        n++;
    return n;
}

int longueur_rec(const Maillon *tete) {
    if (tete == NULL)
        return 0;
    return 1 + longueur_rec(tete->suivant);
}

int somme_iter(const Maillon *tete) {
    int s = 0;
    for (const Maillon *p = tete; p != NULL; p = p->suivant)
        s += p->valeur;
    return s;
}

int somme_rec(const Maillon *tete) {
    if (tete == NULL)
        return 0;
    return tete->valeur + somme_rec(tete->suivant);
}

int maximum_iter(const Maillon *tete) {
    assert(tete != NULL);   /* précondition : liste non vide */
    int max = tete->valeur;
    for (const Maillon *p = tete->suivant; p != NULL; p = p->suivant)
        if (p->valeur > max)
            max = p->valeur;
    return max;
}

/* Cas de base : un seul maillon (le maximum d'une liste vide n'existe pas). */
int maximum_rec(const Maillon *tete) {
    assert(tete != NULL);
    if (tete->suivant == NULL)
        return tete->valeur;
    int max_reste = maximum_rec(tete->suivant);
    return tete->valeur > max_reste ? tete->valeur : max_reste;
}

/* ---------- recherche ---------- */

Maillon *rechercher_iter(Maillon *tete, int valeur) {
    Maillon *p = tete;
    while (p != NULL && p->valeur != valeur)
        p = p->suivant;
    return p;
}

Maillon *rechercher_rec(Maillon *tete, int valeur) {
    if (tete == NULL || tete->valeur == valeur)
        return tete;
    return rechercher_rec(tete->suivant, valeur);
}

/* ---------- suppression ---------- */

Maillon *supprimer_iter(Maillon *tete, int valeur) {
    if (tete == NULL)
        return NULL;
    if (tete->valeur == valeur) {
        Maillon *nouvelle_tete = tete->suivant;
        free(tete);
        return nouvelle_tete;
    }
    Maillon *p = tete;
    while (p->suivant != NULL && p->suivant->valeur != valeur)
        p = p->suivant;
    if (p->suivant != NULL) {
        Maillon *a_supprimer = p->suivant;
        p->suivant = a_supprimer->suivant;
        free(a_supprimer);
    }
    return tete;
}

Maillon *supprimer_rec(Maillon *tete, int valeur) {
    if (tete == NULL)
        return NULL;
    if (tete->valeur == valeur) {
        Maillon *reste = tete->suivant;
        free(tete);
        return reste;
    }
    tete->suivant = supprimer_rec(tete->suivant, valeur);
    return tete;
}

/* ---------- libération ---------- */

void liberer_iter(Maillon *tete) {
    while (tete != NULL) {
        /* lire le suivant AVANT free : après, le maillon n'existe plus */
        Maillon *suivant = tete->suivant;
        free(tete);
        tete = suivant;
    }
}

void liberer_rec(Maillon *tete) {
    if (tete == NULL)
        return;
    liberer_rec(tete->suivant);
    free(tete);
}
