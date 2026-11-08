/*
 * Le piège de la vidéo, mesuré.
 *   mesure trie N       insère 1, 2, ..., N dans l'ordre (inserer_iter), puis recherche chaque valeur
 *   mesure melange N    les mêmes valeurs dans un ordre mélangé (graine fixe : résultat reproductible)
 *   mesure peigne_iter N / peigne_rec N
 *                       l'arbre de « trie N » (chaque nœud n'a qu'un fils droit), construit directement en O(N),
 *                       puis sa hauteur, avec une boucle (hauteur_iter) ou en récursif (hauteur_rec).
 *                       hauteur_rec fait un calcul après l'appel récursif : gcc ne peut pas le changer en boucle,
 *                       même en -O2 (au contraire de rechercher_rec).
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "abr.h"

static int usage(const char *prog) {
    fprintf(stderr, "usage : %s trie|melange N (N <= 100000) | peigne_iter|peigne_rec N (N <= 100000000)\n", prog);
    return 2;
}

static double secondes(clock_t debut) {
    return (double)(clock() - debut) / CLOCKS_PER_SEC;
}

/* Générateur pseudo-aléatoire écrit ici plutôt que rand() : rand() change d'une bibliothèque C à l'autre,
   et le résultat (la hauteur de l'arbre mélangé) doit être le même sur toutes les machines. */
static unsigned long long graine = 42;
static unsigned long hasard(void) {
    graine = graine * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned long)(graine >> 33);
}

/* Mélange de Fisher-Yates. */
static void melanger(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = (int)(hasard() % (unsigned long)(i + 1));
        int tmp = t[i];
        t[i] = t[j];
        t[j] = tmp;
    }
}

int main(int argc, char **argv) {
    if (argc != 3)
        return usage(argv[0]);
    const char *mode = argv[1];
    char *fin;
    long n = strtol(argv[2], &fin, 10);
    if (*fin != '\0' || n <= 0 || n > 100000000)
        return usage(argv[0]);

    if (strcmp(mode, "trie") == 0 || strcmp(mode, "melange") == 0) {
        if (n > 100000)   /* trié : environ N² / 2 comparaisons, des heures au-delà */
            return usage(argv[0]);
        int *valeurs = malloc(n * sizeof(int));
        if (valeurs == NULL)
            return 1;
        for (int i = 0; i < n; i++)
            valeurs[i] = i + 1;
        if (strcmp(mode, "melange") == 0)
            melanger(valeurs, n);

        clock_t debut = clock();
        Noeud *racine = NULL;
        for (int i = 0; i < n; i++)
            racine = inserer_iter(racine, valeurs[i]);
        double t_inserer = secondes(debut);

        debut = clock();
        int trouves = 0;
        for (int i = 1; i <= n; i++)
            trouves += rechercher_iter(racine, i) != NULL;
        double t_chercher = secondes(debut);

        printf("%s, N=%ld : hauteur %d ; inserer %.3f s ; chercher les %ld valeurs %.3f s (%d trouvees)\n",
               mode, n, hauteur_iter(racine), t_inserer, n, t_chercher, trouves);
        liberer(racine);
        free(valeurs);
        return 0;
    }

    bool peigne_iter = strcmp(mode, "peigne_iter") == 0, peigne_rec = strcmp(mode, "peigne_rec") == 0;
    if (!peigne_iter && !peigne_rec)
        return usage(argv[0]);

    /* Le même arbre qu'en insérant 1..N dans l'ordre, mais construit en gardant un pointeur sur le dernier nœud. */
    Noeud *racine = NULL, *dernier = NULL;
    for (int i = 1; i <= n; i++) {
        Noeud *x = malloc(sizeof(Noeud));
        if (x == NULL)
            return 1;
        x->valeur = i;
        x->gauche = x->droit = NULL;
        if (dernier == NULL)
            racine = x;
        else
            dernier->droit = x;
        dernier = x;
    }
    int h = peigne_iter ? hauteur_iter(racine) : hauteur_rec(racine);
    printf("%s, N=%ld : hauteur %d\n", mode, n, h);
    while (racine != NULL) {   /* une boucle : liberer() est récursive, et l'arbre a N niveaux */
        Noeud *suivant = racine->droit;
        free(racine);
        racine = suivant;
    }
    return 0;
}
