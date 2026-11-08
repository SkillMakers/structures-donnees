#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"

static void *allouer(size_t taille) {
    void *m = malloc(taille);
    if (m == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return m;
}

static void *allouer_zero(size_t nombre, size_t taille) {
    void *m = calloc(nombre, taille);
    if (m == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    return m;
}

char nom(int s) {
    return (char)('A' + s);
}

Graphe *graphe_creer(int n, bool oriente) {
    Graphe *g = allouer(sizeof(Graphe));
    g->n = n;
    g->oriente = oriente;
    g->nb_arcs = 0;
    g->adj = allouer((size_t)n * sizeof(Arc *));
    for (int u = 0; u < n; u++)
        g->adj[u] = NULL;
    return g;
}

void graphe_detruire(Graphe *g) {
    for (int u = 0; u < g->n; u++) {
        Arc *a = g->adj[u];
        while (a != NULL) {
            Arc *suivant = a->suivant; /* lu avant free : après, a n'existe plus */
            free(a);
            a = suivant;
        }
    }
    free(g->adj);
    free(g);
}

/* Insertion triée dans la liste de u (le pointeur sur le lien, comme dans la vidéo sur les arbres). */
static void inserer_arc(Graphe *g, int u, int v, int poids) {
    Arc **lien = &g->adj[u];
    while (*lien != NULL && (*lien)->vers < v)
        lien = &(*lien)->suivant;
    Arc *a = allouer(sizeof(Arc));
    a->vers = v;
    a->poids = poids;
    a->suivant = *lien;
    *lien = a;
    g->nb_arcs++;
}

void ajouter_arc(Graphe *g, int u, int v, int poids) {
    inserer_arc(g, u, v, poids);
    if (!g->oriente)
        inserer_arc(g, v, u, poids);
}

static void afficher_ligne(const Graphe *g, int fixe, const int dist[], const bool fige[]) {
    printf("on fixe %c |", nom(fixe));
    for (int v = 0; v < g->n; v++) {
        if (fige[v] && v != fixe)
            printf("    .");          /* déjà fixé avant cette étape */
        else if (dist[v] == INFINI)
            printf("  inf");
        else
            printf(" %4d", dist[v]);
    }
    printf("\n");
}

void dijkstra(const Graphe *g, int depart, int dist[], int pred[], bool trace) {
    bool *fige = allouer((size_t)g->n * sizeof(bool));
    for (int v = 0; v < g->n; v++) {
        dist[v] = INFINI;
        pred[v] = AUCUN;
        fige[v] = false;
    }
    dist[depart] = 0;
    if (trace) {
        printf("          |");
        for (int v = 0; v < g->n; v++)
            printf("    %c", nom(v));
        printf("\n");
    }
    for (int etape = 0; etape < g->n; etape++) {
        /* Le sommet non fixé le plus proche du départ. */
        int u = AUCUN;
        for (int v = 0; v < g->n; v++)
            if (!fige[v] && dist[v] != INFINI && (u == AUCUN || dist[v] < dist[u]))
                u = v;
        if (u == AUCUN)
            break; /* les sommets restants sont inaccessibles */
        fige[u] = true;
        /* On relâche les arcs qui partent de u, vers les sommets pas encore fixés. */
        for (const Arc *a = g->adj[u]; a != NULL; a = a->suivant)
            if (!fige[a->vers] && dist[u] + a->poids < dist[a->vers]) {
                dist[a->vers] = dist[u] + a->poids;
                pred[a->vers] = u;
            }
        if (trace)
            afficher_ligne(g, u, dist, fige);
    }
    free(fige);
}

void bellman_ford(const Graphe *g, int depart, int dist[]) {
    for (int v = 0; v < g->n; v++)
        dist[v] = INFINI;
    dist[depart] = 0;
    /* n - 1 tours suffisent : un plus court chemin a au plus n - 1 arcs. */
    for (int tour = 1; tour < g->n; tour++)
        for (int u = 0; u < g->n; u++) {
            if (dist[u] == INFINI)
                continue;
            for (const Arc *a = g->adj[u]; a != NULL; a = a->suivant)
                if (dist[u] + a->poids < dist[a->vers])
                    dist[a->vers] = dist[u] + a->poids;
        }
}

void chemin_iter(const int pred[], int depart, int arrivee) {
    /* On remonte les prédécesseurs (le chemin à l'envers) en empilant, puis on dépile. */
    /* On s'arrête au départ demandé, comme la version récursive (pas forcément à la source de pred[]). */
    int longueur = 1, debut = arrivee;
    while (debut != depart && pred[debut] != AUCUN) {
        debut = pred[debut];
        longueur++;
    }
    if (debut != depart) { /* la remontée s'arrête avant le départ */
        printf("inaccessible\n");
        return;
    }
    int *pile = allouer((size_t)longueur * sizeof(int));
    int sommet = 0;
    for (int s = arrivee; sommet < longueur; s = pred[s])
        pile[sommet++] = s;
    while (sommet > 0)
        printf("%c ", nom(pile[--sommet]));
    printf("\n");
    free(pile);
}

static bool chemin_rec_aux(const int pred[], int depart, int s) {
    if (s == depart) { /* cas de base : on est revenu au départ */
        printf("%c ", nom(s));
        return true;
    }
    if (pred[s] == AUCUN) /* cas de base : la remontée s'arrête avant le départ */
        return false;
    if (!chemin_rec_aux(pred, depart, pred[s]))
        return false;
    printf("%c ", nom(s)); /* affiché au retour de l'appel : le chemin sort à l'endroit */
    return true;
}

void chemin_rec(const int pred[], int depart, int arrivee) {
    if (!chemin_rec_aux(pred, depart, arrivee))
        printf("inaccessible");
    printf("\n");
}

int largeur(const Graphe *g, int depart, int ordre[], int etapes[], int pred[]) {
    int *file = allouer((size_t)g->n * sizeof(int)); /* chaque sommet entre une seule fois dans la file */
    int tete = 0, queue = 0, nb = 0;
    for (int v = 0; v < g->n; v++) {
        etapes[v] = -1;
        pred[v] = AUCUN;
    }
    etapes[depart] = 0;
    file[queue++] = depart;
    while (tete < queue) {
        int u = file[tete++];
        ordre[nb++] = u;
        for (const Arc *a = g->adj[u]; a != NULL; a = a->suivant)
            if (etapes[a->vers] == -1) { /* marqué dès qu'il entre dans la file, pas quand il en sort */
                etapes[a->vers] = etapes[u] + 1;
                pred[a->vers] = u;
                file[queue++] = a->vers;
            }
    }
    free(file);
    return nb;
}

static void profondeur_rec_aux(const Graphe *g, int u, bool vu[], int ordre[], int *nb) {
    vu[u] = true;
    ordre[(*nb)++] = u;
    for (const Arc *a = g->adj[u]; a != NULL; a = a->suivant)
        if (!vu[a->vers])
            profondeur_rec_aux(g, a->vers, vu, ordre, nb);
}

int profondeur_rec(const Graphe *g, int depart, int ordre[]) {
    bool *vu = allouer_zero((size_t)g->n, sizeof(bool));
    int nb = 0;
    profondeur_rec_aux(g, depart, vu, ordre, &nb);
    free(vu);
    return nb;
}

int profondeur_iter(const Graphe *g, int depart, int ordre[]) {
    /* Un sommet peut être empilé plusieurs fois (une fois par arc qui y mène) : la pile a la taille du nombre
       d'arcs, plus le départ. On le marque quand on le dépile, comme la récursion quand elle y entre.
       voisins[] reçoit la liste d'un sommet : elle peut avoir plus de n arcs (arcs en double), d'où nb_arcs. */
    int *pile = allouer((size_t)(g->nb_arcs + 1) * sizeof(int));
    int *voisins = allouer((size_t)(g->nb_arcs + 1) * sizeof(int));
    bool *vu = allouer_zero((size_t)g->n, sizeof(bool));
    int sommet = 0, nb = 0;
    pile[sommet++] = depart;
    while (sommet > 0) {
        int u = pile[--sommet];
        if (vu[u])
            continue;
        vu[u] = true;
        ordre[nb++] = u;
        /* Les voisins sont empilés à l'envers : le premier de la liste sort en premier,
           comme dans la version récursive. Sans ça, l'ordre de visite change. */
        int k = 0;
        for (const Arc *a = g->adj[u]; a != NULL; a = a->suivant)
            voisins[k++] = a->vers;
        while (k > 0) {
            int v = voisins[--k];
            if (!vu[v])
                pile[sommet++] = v;
        }
    }
    free(vu);
    free(voisins);
    free(pile);
    return nb;
}
