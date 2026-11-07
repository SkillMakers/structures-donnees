/*
 * Les exercices de la vidéo, sur la même pile du début à la fin : on empile 4, 9, 2, 7 (7 au sommet),
 * et la même file : on enfile 4, 9, 2, 7 (4 en tête).
 */
#include <stdio.h>
#include "exercices.h"

static Pile *pile_exemple(void) {
    Pile *p = pile_creer();
    int valeurs[] = {4, 9, 2, 7};
    for (int i = 0; i < 4; i++)
        empiler(p, valeurs[i]);
    return p;
}

static File *file_exemple(int capacite) {
    File *f = file_creer(capacite);
    int valeurs[] = {4, 9, 2, 7};
    for (int i = 0; i < 4; i++)
        enfiler(f, valeurs[i]);
    return f;
}

int main(void) {
    printf("=== 1. Pile : empiler 4, 9, 2, 7 ===\n");
    Pile *p = pile_exemple();
    pile_afficher(p);
    printf("sommet : %d\n", sommet(p));
    printf("depiler : %d\n", depiler(p));
    printf("depiler : %d\n", depiler(p));
    pile_afficher(p);
    pile_detruire(p);

    printf("\n=== 2. File : enfiler 4, 9, 2, 7 (capacite 4) ===\n");
    File *f = file_exemple(4);
    file_afficher(f);
    printf("pleine : %s\n", file_est_pleine(f) ? "oui" : "non");
    printf("defiler : %d\n", defiler(f));
    printf("defiler : %d\n", defiler(f));
    printf("enfiler 5, puis 8\n");
    enfiler(f, 5);
    enfiler(f, 8);
    file_afficher(f);
    printf("cases du tableau : [%d, %d, %d, %d], tete = indice %d\n",
           f->cases[0], f->cases[1], f->cases[2], f->cases[3], f->tete);
    file_detruire(f);

    printf("\n=== 3. Rechercher sans detruire ===\n");
    p = pile_exemple();
    printf("iter 9 : %s ; ", pile_rechercher_iter(p, 9) ? "trouve" : "absent");
    pile_afficher(p);
    printf("rec  9 : %s ; ", pile_rechercher_rec(p, 9) ? "trouve" : "absent");
    pile_afficher(p);
    printf("iter 5 : %s ; ", pile_rechercher_iter(p, 5) ? "trouve" : "absent");
    pile_afficher(p);
    printf("rec  5 : %s ; ", pile_rechercher_rec(p, 5) ? "trouve" : "absent");
    pile_afficher(p);

    printf("\n=== 4. Taille et copie ===\n");
    printf("iter : %d ; ", pile_taille_iter(p));
    pile_afficher(p);
    printf("rec  : %d ; ", pile_taille_rec(p));
    pile_afficher(p);
    Pile *c = pile_copier_iter(p);
    printf("copie iter : ");
    pile_afficher(c);
    pile_detruire(c);
    c = pile_copier_rec(p);
    printf("copie rec  : ");
    pile_afficher(c);
    pile_detruire(c);
    printf("original   : ");
    pile_afficher(p);

    printf("\n=== 5. Inverser la pile ===\n");
    pile_inverser_iter(p);
    printf("iter : ");
    pile_afficher(p);
    pile_inverser_rec(p);
    printf("rec, sur le resultat : ");
    pile_afficher(p);
    pile_detruire(p);

    printf("\n=== 6. Inverser la file ===\n");
    f = file_exemple(4);
    file_inverser_iter(f);
    printf("iter : ");
    file_afficher(f);
    file_inverser_rec(f);
    printf("rec, sur le resultat : ");
    file_afficher(f);
    file_detruire(f);

    printf("\n=== 7. Parentheses equilibrees ===\n");
    const char *textes[] = {"({[]})", "f(t[i]) + {x}", "([)]", "((a)", "())"};
    for (int i = 0; i < 5; i++)
        printf("%-15s : %s\n", textes[i], parentheses_equilibrees(textes[i]) ? "equilibre" : "NON equilibre");
    return 0;
}
