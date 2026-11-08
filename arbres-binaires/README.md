# Arbres binaires de recherche en C

Code de la vidéo [**Arbres binaires de recherche en C : cours et exercices corrigés**](https://youtu.be/0xxmA2Pz5WA).

## Les 6 exercices

Sur l'arbre construit en insérant 50, 30, 70, 20, 40, 60, 80, 35, 65 dans cet ordre, chaque fonction en deux
versions, avec une boucle puis en récursif :

1. Insérer une valeur ; construire l'arbre. Que se passe-t-il avec une valeur déjà présente ?
2. Rechercher 35, puis 55 (renvoyer le nœud, ou `NULL`) ; trouver le minimum et le maximum.
3. Calculer la hauteur, le nombre de nœuds et le nombre de feuilles.
4. Afficher l'arbre en ordre préfixe, infixe et suffixe.
5. Afficher l'arbre niveau par niveau (parcours en largeur).
6. Supprimer 20, puis 60, puis 30, chaque fois dans l'arbre de départ.

## Les fichiers

| Fichier | Contenu |
|---|---|
| `code/abr.h`, `code/abr.c` | toutes les opérations, en `_iter` et en `_rec` (le corrigé) |
| `code/demo.c` | les 6 exercices sur l'arbre de la vidéo ; sa sortie attendue est dans `code/attendu-demo.txt` |
| `code/mesure.c` | le piège : valeurs triées contre mélangées ; hauteur de l'arbre en peigne, boucle contre récursion |
| `code/executer.sh` | compile avec `-Werror`, exécute, compare `demo` à la sortie attendue, valgrind, mesures |

## Compiler et lancer

Sous Linux ou WSL :

```bash
cd code
gcc -std=c11 -Wall -Wextra -Werror -o demo demo.c abr.c
./demo
```

Tout refaire, preuves comprises, dans un conteneur Debian (gcc 12, valgrind), depuis ce dossier :

```bash
docker run --rm -v "$PWD:/w" -w /w debian:bookworm-slim sh -c \
  "apt-get update -qq && apt-get install -y -qq gcc libc6-dev valgrind && sh code/executer.sh"
```

## Ce que montrent les preuves

- `demo.txt` : les 6 exercices ; boucle et récursion donnent les mêmes résultats.
- `valgrind-demo.txt` : 156 allocations, 156 libérations, aucune erreur.
- `mesure-trie.txt` : 40 000 valeurs mélangées font un arbre de 37 niveaux, inséré en 0,061 s ;
  les mêmes valeurs triées font 40 000 niveaux et 8,6 s : l'arbre est devenu une liste chaînée.
- `mesure-peigne.txt` : avec la pile d'appels de 8 Mo, la hauteur récursive de cet arbre plante vers
  175 000 nœuds en `-O0`, et à 1 million même en `-O2` ; la boucle passe 10 millions.
