#include "exercices.h"

/* Dépile tout source dans destination : l'ordre est inversé. */
static void transvaser(Pile *source, Pile *destination) {
    while (!pile_est_vide(source))
        empiler(destination, depiler(source));
}

/* ---------- rechercher sans détruire ---------- */

/* Chaque élément dépilé est gardé dans une pile auxiliaire, puis tout est remis en place. */
bool pile_rechercher_iter(Pile *p, int valeur) {
    Pile *aux = pile_creer();
    bool trouve = false;
    while (!pile_est_vide(p) && !trouve) {
        int v = depiler(p);
        empiler(aux, v);
        trouve = (v == valeur);
    }
    transvaser(aux, p);
    pile_detruire(aux);
    return trouve;
}

/* La pile d'appels joue le rôle de la pile auxiliaire : chaque appel garde v et le rempile au retour. */
bool pile_rechercher_rec(Pile *p, int valeur) {
    if (pile_est_vide(p))
        return false;
    int v = depiler(p);
    bool trouve = (v == valeur) || pile_rechercher_rec(p, valeur);
    empiler(p, v);
    return trouve;
}

/* ---------- taille ---------- */

int pile_taille_iter(Pile *p) {
    Pile *aux = pile_creer();
    int n = 0;
    while (!pile_est_vide(p)) {
        empiler(aux, depiler(p));
        n++;
    }
    transvaser(aux, p);
    pile_detruire(aux);
    return n;
}

int pile_taille_rec(Pile *p) {
    if (pile_est_vide(p))
        return 0;
    int v = depiler(p);
    int n = 1 + pile_taille_rec(p);
    empiler(p, v);
    return n;
}

/* ---------- copier ---------- */

/* Tout passe dans aux (ordre inversé), puis chaque élément repart à la fois dans p et dans la copie. */
Pile *pile_copier_iter(Pile *p) {
    Pile *aux = pile_creer();
    Pile *copie = pile_creer();
    transvaser(p, aux);
    while (!pile_est_vide(aux)) {
        int v = depiler(aux);
        empiler(p, v);
        empiler(copie, v);
    }
    pile_detruire(aux);
    return copie;
}

/* On copie le reste d'abord : au retour, le sommet est posé sur la copie et rempilé sur p. */
Pile *pile_copier_rec(Pile *p) {
    if (pile_est_vide(p))
        return pile_creer();
    int v = depiler(p);
    Pile *copie = pile_copier_rec(p);
    empiler(copie, v);
    empiler(p, v);
    return copie;
}

/* ---------- inverser une pile ---------- */

/* Un transvasement inverse l'ordre. Trois transvasements : p est inversée. */
void pile_inverser_iter(Pile *p) {
    Pile *a = pile_creer();
    Pile *b = pile_creer();
    transvaser(p, a);
    transvaser(a, b);
    transvaser(b, p);
    pile_detruire(a);
    pile_detruire(b);
}

/* Pose valeur tout au fond : on dépile tout, on empile valeur, on rempile au retour. */
static void inserer_au_fond(Pile *p, int valeur) {
    if (pile_est_vide(p)) {
        empiler(p, valeur);
        return;
    }
    int v = depiler(p);
    inserer_au_fond(p, valeur);
    empiler(p, v);
}

/* On retire le sommet, on inverse le reste, et l'ancien sommet va au fond. */
void pile_inverser_rec(Pile *p) {
    if (pile_est_vide(p))
        return;
    int v = depiler(p);
    pile_inverser_rec(p);
    inserer_au_fond(p, v);
}

/* ---------- inverser une file ---------- */

/* Tout passe par une pile : ce qui entre en premier dans la pile en sort en dernier. */
void file_inverser_iter(File *f) {
    Pile *p = pile_creer();
    while (!file_est_vide(f))
        empiler(p, defiler(f));
    while (!pile_est_vide(p))
        enfiler(f, depiler(p));
    pile_detruire(p);
}

/* On retire la tête, on inverse le reste, et l'ancienne tête repart en queue. */
void file_inverser_rec(File *f) {
    if (file_est_vide(f))
        return;
    int v = defiler(f);
    file_inverser_rec(f);
    enfiler(f, v);
}

/* ---------- parenthèses équilibrées ---------- */

static char ouvrante(char fermante) {
    return fermante == ')' ? '(' : fermante == ']' ? '[' : '{';
}

/* Chaque ouvrante est empilée ; chaque fermante doit correspondre au sommet. */
bool parentheses_equilibrees(const char *texte) {
    Pile *p = pile_creer();
    bool ok = true;
    for (const char *c = texte; *c != '\0' && ok; c++) {
        if (*c == '(' || *c == '[' || *c == '{')
            empiler(p, *c);
        else if (*c == ')' || *c == ']' || *c == '}')
            ok = !pile_est_vide(p) && depiler(p) == ouvrante(*c);
    }
    ok = ok && pile_est_vide(p);   /* une ouvrante restée seule : déséquilibré */
    pile_detruire(p);
    return ok;
}
