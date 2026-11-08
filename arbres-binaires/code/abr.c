#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "abr.h"

static void *allouer(size_t taille) {
    void *m = malloc(taille);
    if (m == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return m;
}

static Noeud *creer_noeud(int valeur) {
    Noeud *n = allouer(sizeof(Noeud));
    n->valeur = valeur;
    n->gauche = NULL;
    n->droit = NULL;
    return n;
}

/* ---------- une pile et une file de pointeurs de nœuds, pour les versions avec une boucle ---------- */

typedef struct {
    const Noeud **cases;
    int debut, nb, capacite;   /* debut ne sert qu'à la file */
} Suite;

static Suite suite_creer(void) {
    Suite s = {allouer(16 * sizeof(Noeud *)), 0, 0, 16};
    return s;
}

/* Ajoute à la fin. Arrivée au bout du tableau, la file est ramenée au début ; le tableau ne double que s'il est
   au moins à moitié plein (sinon une file qui avance sans grossir réclamerait de la mémoire sans fin). */
static void ajouter(Suite *s, const Noeud *n) {
    if (s->debut + s->nb == s->capacite) {
        int capacite = s->nb >= s->capacite / 2 ? 2 * s->capacite : s->capacite;
        const Noeud **cases = allouer(capacite * sizeof(Noeud *));
        for (int i = 0; i < s->nb; i++)
            cases[i] = s->cases[s->debut + i];
        free(s->cases);
        s->cases = cases;
        s->debut = 0;
        s->capacite = capacite;
    }
    s->cases[s->debut + s->nb++] = n;
}

static const Noeud *retirer_fin(Suite *s) {     /* pile : le dernier ajouté */
    return s->cases[s->debut + --s->nb];
}

static const Noeud *retirer_debut(Suite *s) {   /* file : le premier ajouté */
    s->nb--;
    return s->cases[s->debut++];
}

/* ---------- rechercher ---------- */

Noeud *rechercher_iter(Noeud *racine, int valeur) {
    Noeud *n = racine;
    while (n != NULL && n->valeur != valeur)
        n = valeur < n->valeur ? n->gauche : n->droit;
    return n;
}

Noeud *rechercher_rec(Noeud *racine, int valeur) {
    if (racine == NULL || racine->valeur == valeur)
        return racine;
    if (valeur < racine->valeur)
        return rechercher_rec(racine->gauche, valeur);
    return rechercher_rec(racine->droit, valeur);
}

/* ---------- insérer ---------- */

/* On descend avec un pointeur sur le lien à remplir : la racine, ou le fils gauche ou droit d'un nœud. */
Noeud *inserer_iter(Noeud *racine, int valeur) {
    Noeud **lien = &racine;
    while (*lien != NULL) {
        if (valeur == (*lien)->valeur)
            return racine;
        lien = valeur < (*lien)->valeur ? &(*lien)->gauche : &(*lien)->droit;
    }
    *lien = creer_noeud(valeur);
    return racine;
}

/* Au retour, chaque nœud du chemin se raccroche au sous-arbre renvoyé (le même, sauf tout en bas). */
Noeud *inserer_rec(Noeud *racine, int valeur) {
    if (racine == NULL)
        return creer_noeud(valeur);
    if (valeur < racine->valeur)
        racine->gauche = inserer_rec(racine->gauche, valeur);
    else if (valeur > racine->valeur)
        racine->droit = inserer_rec(racine->droit, valeur);
    return racine;
}

/* ---------- minimum, maximum ---------- */

int minimum_iter(const Noeud *racine) {
    assert(racine != NULL);
    while (racine->gauche != NULL)
        racine = racine->gauche;
    return racine->valeur;
}

int minimum_rec(const Noeud *racine) {
    assert(racine != NULL);
    if (racine->gauche == NULL)
        return racine->valeur;
    return minimum_rec(racine->gauche);
}

int maximum_iter(const Noeud *racine) {
    assert(racine != NULL);
    while (racine->droit != NULL)
        racine = racine->droit;
    return racine->valeur;
}

int maximum_rec(const Noeud *racine) {
    assert(racine != NULL);
    if (racine->droit == NULL)
        return racine->valeur;
    return maximum_rec(racine->droit);
}

/* ---------- hauteur, nombre de nœuds, nombre de feuilles ---------- */

int hauteur_rec(const Noeud *racine) {
    if (racine == NULL)
        return 0;
    int g = hauteur_rec(racine->gauche);
    int d = hauteur_rec(racine->droit);
    return 1 + (g > d ? g : d);
}

/* Parcours en largeur, un niveau à la fois : la hauteur est le nombre de niveaux. */
int hauteur_iter(const Noeud *racine) {
    if (racine == NULL)
        return 0;
    Suite file = suite_creer();
    ajouter(&file, racine);
    int niveaux = 0;
    while (file.nb > 0) {
        niveaux++;
        for (int n = file.nb; n > 0; n--) {   /* les nœuds de ce niveau */
            const Noeud *x = retirer_debut(&file);
            if (x->gauche) ajouter(&file, x->gauche);
            if (x->droit) ajouter(&file, x->droit);
        }
    }
    free(file.cases);
    return niveaux;
}

int nb_noeuds_rec(const Noeud *racine) {
    if (racine == NULL)
        return 0;
    return 1 + nb_noeuds_rec(racine->gauche) + nb_noeuds_rec(racine->droit);
}

int nb_noeuds_iter(const Noeud *racine) {
    if (racine == NULL)
        return 0;
    Suite file = suite_creer();
    ajouter(&file, racine);
    int total = 0;
    while (file.nb > 0) {
        const Noeud *x = retirer_debut(&file);
        total++;
        if (x->gauche) ajouter(&file, x->gauche);
        if (x->droit) ajouter(&file, x->droit);
    }
    free(file.cases);
    return total;
}

int nb_feuilles_rec(const Noeud *racine) {
    if (racine == NULL)
        return 0;
    if (racine->gauche == NULL && racine->droit == NULL)
        return 1;
    return nb_feuilles_rec(racine->gauche) + nb_feuilles_rec(racine->droit);
}

int nb_feuilles_iter(const Noeud *racine) {
    if (racine == NULL)
        return 0;
    Suite file = suite_creer();
    ajouter(&file, racine);
    int feuilles = 0;
    while (file.nb > 0) {
        const Noeud *x = retirer_debut(&file);
        if (x->gauche == NULL && x->droit == NULL)
            feuilles++;
        if (x->gauche) ajouter(&file, x->gauche);
        if (x->droit) ajouter(&file, x->droit);
    }
    free(file.cases);
    return feuilles;
}

/* ---------- parcours en profondeur ---------- */

static void prefixe_aux(const Noeud *n) {
    if (n == NULL)
        return;
    printf("%d ", n->valeur);
    prefixe_aux(n->gauche);
    prefixe_aux(n->droit);
}

void prefixe_rec(const Noeud *racine) {
    prefixe_aux(racine);
    printf("\n");
}

/* Le droit est empilé avant le gauche, pour que le gauche sorte en premier. */
void prefixe_iter(const Noeud *racine) {
    Suite pile = suite_creer();
    if (racine) ajouter(&pile, racine);
    while (pile.nb > 0) {
        const Noeud *x = retirer_fin(&pile);
        printf("%d ", x->valeur);
        if (x->droit) ajouter(&pile, x->droit);
        if (x->gauche) ajouter(&pile, x->gauche);
    }
    printf("\n");
    free(pile.cases);
}

static void infixe_aux(const Noeud *n) {
    if (n == NULL)
        return;
    infixe_aux(n->gauche);
    printf("%d ", n->valeur);
    infixe_aux(n->droit);
}

void infixe_rec(const Noeud *racine) {
    infixe_aux(racine);
    printf("\n");
}

/* On descend à gauche en empilant le chemin ; on affiche en remontant, puis on passe au sous-arbre droit. */
void infixe_iter(const Noeud *racine) {
    Suite pile = suite_creer();
    const Noeud *x = racine;
    while (x != NULL || pile.nb > 0) {
        while (x != NULL) {
            ajouter(&pile, x);
            x = x->gauche;
        }
        x = retirer_fin(&pile);
        printf("%d ", x->valeur);
        x = x->droit;
    }
    printf("\n");
    free(pile.cases);
}

static void suffixe_aux(const Noeud *n) {
    if (n == NULL)
        return;
    suffixe_aux(n->gauche);
    suffixe_aux(n->droit);
    printf("%d ", n->valeur);
}

void suffixe_rec(const Noeud *racine) {
    suffixe_aux(racine);
    printf("\n");
}

/* Deux piles : la 1re produit racine, droit, gauche ; la 2e rend cet ordre à l'envers : gauche, droit, racine. */
void suffixe_iter(const Noeud *racine) {
    Suite p1 = suite_creer(), p2 = suite_creer();
    if (racine) ajouter(&p1, racine);
    while (p1.nb > 0) {
        const Noeud *x = retirer_fin(&p1);
        ajouter(&p2, x);
        if (x->gauche) ajouter(&p1, x->gauche);
        if (x->droit) ajouter(&p1, x->droit);
    }
    while (p2.nb > 0)
        printf("%d ", retirer_fin(&p2)->valeur);
    printf("\n");
    free(p1.cases);
    free(p2.cases);
}

/* ---------- parcours en largeur ---------- */

void largeur(const Noeud *racine) {
    Suite file = suite_creer();
    if (racine) ajouter(&file, racine);
    while (file.nb > 0) {
        const Noeud *x = retirer_debut(&file);
        printf("%d ", x->valeur);
        if (x->gauche) ajouter(&file, x->gauche);
        if (x->droit) ajouter(&file, x->droit);
    }
    printf("\n");
    free(file.cases);
}

/* ---------- supprimer ---------- */

/* Deux fils : on recopie le successeur (le minimum du sous-arbre droit), puis on supprime le successeur,
   qui n'a jamais de fils gauche. */
Noeud *supprimer_rec(Noeud *racine, int valeur) {
    if (racine == NULL)
        return NULL;
    if (valeur < racine->valeur) {
        racine->gauche = supprimer_rec(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droit = supprimer_rec(racine->droit, valeur);
    } else if (racine->gauche == NULL || racine->droit == NULL) {   /* feuille ou un seul fils */
        Noeud *fils = racine->gauche != NULL ? racine->gauche : racine->droit;
        free(racine);
        return fils;
    } else {
        racine->valeur = minimum_rec(racine->droit);
        racine->droit = supprimer_rec(racine->droit, racine->valeur);
    }
    return racine;
}

Noeud *supprimer_iter(Noeud *racine, int valeur) {
    Noeud **lien = &racine;   /* le lien qui pointe vers le nœud courant */
    while (*lien != NULL && (*lien)->valeur != valeur)
        lien = valeur < (*lien)->valeur ? &(*lien)->gauche : &(*lien)->droit;
    if (*lien == NULL)
        return racine;
    Noeud *cible = *lien;
    if (cible->gauche != NULL && cible->droit != NULL) {
        /* deux fils : le successeur est tout à gauche du sous-arbre droit */
        Noeud **lien_succ = &cible->droit;
        while ((*lien_succ)->gauche != NULL)
            lien_succ = &(*lien_succ)->gauche;
        cible->valeur = (*lien_succ)->valeur;
        lien = lien_succ;   /* il reste à supprimer le successeur, qui a au plus un fils (à droite) */
        cible = *lien;
    }
    *lien = cible->gauche != NULL ? cible->gauche : cible->droit;
    free(cible);
    return racine;
}

/* ---------- libérer ---------- */

/* En suffixe : les deux sous-arbres d'abord, le nœud en dernier (après free, ses fils seraient perdus). */
void liberer(Noeud *racine) {
    if (racine == NULL)
        return;
    liberer(racine->gauche);
    liberer(racine->droit);
    free(racine);
}
