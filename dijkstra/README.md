# Algorithme de Dijkstra en C

Code de la vidéo [**Algorithme de Dijkstra : cours et exercices corrigés (graphes)**](https://youtu.be/pOVjK5UV1L0).

## Les graphes

- Graphe 1, non orienté (six villes) : A-B 4, A-C 2, B-C 1, B-D 5, C-D 8, C-E 10, D-E 2, D-F 6, E-F 3
- Graphe 2, orienté : A→B 3, A→C 7, B→C 2, B→D 6, C→D 1, C→E 8, D→E 2, E→B 1
- Graphe 3, orienté : A→B 2, A→C 1, B→C -2, C→D 1

## Les 5 exercices

1. Graphe 1 : appliquer Dijkstra depuis A, à la main, et donner la distance de A à chaque sommet.
2. Graphe 1 : donner le chemin le plus court de A à F, avec une boucle, puis en récursif.
3. Graphe 2 : les distances depuis A et le chemin de A à E. Puis le chemin de E à A.
4. Graphe 1 : le chemin de A à F avec le moins de routes (parcours en largeur).
5. Graphe 1 : le parcours en profondeur depuis F, en récursif puis en boucle ; le comparer au parcours en largeur.

Le piège : Dijkstra depuis A sur le graphe 3, comparé à Bellman-Ford.

## Les fichiers

| Fichier | Contenu |
|---|---|
| `code/graphe.h`, `code/graphe.c` | le graphe en listes d'adjacence, Dijkstra (version tableau), Bellman-Ford, le chemin en `_iter` et en `_rec`, les parcours en largeur et en profondeur (`_iter` et `_rec`) |
| `code/demo.c` | les 5 exercices et le piège ; sa sortie attendue est dans `code/attendu-demo.txt` |
| `code/mesure.c` | profondeur sur un graphe en ligne : boucle contre récursion |
| `code/executer.sh` | compile avec `-Werror`, exécute, compare `demo` à la sortie attendue, valgrind, mesures |

## Compiler et lancer

Sous Linux ou WSL :

```bash
cd code
gcc -std=c11 -Wall -Wextra -Werror -o demo demo.c graphe.c
./demo
```

Tout refaire, preuves comprises, dans un conteneur Debian (gcc 12, valgrind), depuis ce dossier :

```bash
docker run --rm -v "$PWD:/w" -w /w debian:bookworm-slim sh -c \
  "apt-get update -qq && apt-get install -y -qq gcc libc6-dev valgrind && sh code/executer.sh"
```

## Ce que montrent les preuves

- `demo.txt` : le tableau de Dijkstra depuis A (A 0, B 3, C 2, D 8, E 10, F 13), le chemin A C B D E F en boucle
  et en récursif, le graphe orienté (E vers A inaccessible), le parcours en largeur (A B D F, 3 étapes), la
  profondeur depuis F, et le piège : Dijkstra trouve C = 1 et D = 2, Bellman-Ford C = 0 et D = 1.
- `valgrind-demo.txt` : 50 allocations, 50 libérations, aucune erreur.
- `mesure-chaine.txt` : avec la pile d'appels de 8 Mo, la profondeur récursive plante vers 105 000 sommets
  en `-O0`, et à 1 million même en `-O2` ; la boucle parcourt 10 millions de sommets en 0,435 s.
