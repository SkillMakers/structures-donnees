/* Les exercices de la vidéo, sur la même liste : 12 -> 5 -> 31 -> 8. */
#include <stdio.h>
#include "liste.h"

static void titre(const char *t) { printf("\n=== %s ===\n", t); }

/* Construit 12 -> 5 -> 31 -> 8 avec la fonction d'insertion en queue donnée (iter ou rec). */
static Maillon *construire(Maillon *(*inserer)(Maillon *, int)) {
    static const int valeurs[] = {12, 5, 31, 8};
    Maillon *l = NULL;
    for (int i = 0; i < 4; i++)
        l = inserer(l, valeurs[i]);
    return l;
}

int main(void) {
    titre("1. Construire (insertion en queue)");
    Maillon *a = construire(inserer_queue_iter);
    Maillon *b = construire(inserer_queue_rec);
    printf("iter : "); afficher_iter(a);
    printf("rec  : "); afficher_rec(b);
    liberer_iter(b);

    titre("2. Afficher a l'envers");
    printf("iter : "); afficher_envers_iter(a);
    printf("rec  : "); afficher_envers_rec(a);

    titre("3. Longueur, somme, maximum");
    printf("longueur : iter %d, rec %d\n", longueur_iter(a), longueur_rec(a));
    printf("somme    : iter %d, rec %d\n", somme_iter(a), somme_rec(a));
    printf("maximum  : iter %d, rec %d\n", maximum_iter(a), maximum_rec(a));

    titre("4. Rechercher");
    Maillon *m;
    m = rechercher_iter(a, 31);
    printf("iter 31 : %s\n", m ? "trouve" : "absent (NULL)");
    m = rechercher_rec(a, 31);
    printf("rec  31 : %s\n", m ? "trouve" : "absent (NULL)");
    m = rechercher_iter(a, 7);
    printf("iter 7  : %s\n", m ? "trouve" : "absent (NULL)");
    m = rechercher_rec(a, 7);
    printf("rec  7  : %s\n", m ? "trouve" : "absent (NULL)");
    m = rechercher_rec(a, 31);
    if (m != NULL && m->suivant != NULL)
        printf("le maillon de 31 : valeur %d, suivant %d\n", m->valeur, m->suivant->valeur);

    titre("5. Supprimer");
    Maillon *c = construire(inserer_queue_iter);
    c = supprimer_iter(c, 12);
    printf("iter, supprimer 12 (la tete) : "); afficher_iter(c);
    c = supprimer_iter(c, 31);
    printf("iter, supprimer 31 (milieu)  : "); afficher_iter(c);
    c = supprimer_iter(c, 7);
    printf("iter, supprimer 7 (absent)   : "); afficher_iter(c);
    liberer_iter(c);
    Maillon *d = construire(inserer_queue_rec);
    d = supprimer_rec(d, 12);
    printf("rec,  supprimer 12 (la tete) : "); afficher_rec(d);
    d = supprimer_rec(d, 31);
    printf("rec,  supprimer 31 (milieu)  : "); afficher_rec(d);
    d = supprimer_rec(d, 7);
    printf("rec,  supprimer 7 (absent)   : "); afficher_rec(d);
    liberer_rec(d);

    titre("6. Liberer");
    liberer_rec(a);
    printf("liste liberee\n");
    return 0;
}
