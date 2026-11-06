# Listes chaînées en C

Code de la vidéo [**Listes chaînées en C : cours et exercices corrigés**](https://youtu.be/UAFC9QbYD4E).

## Les 6 exercices

Sur la liste 12 → 5 → 31 → 8, écris chaque fonction en deux versions, avec une boucle puis en récursif :

1. Construire la liste par insertion en queue.
2. Afficher la liste, puis l'afficher à l'envers.
3. Calculer la longueur, la somme et le maximum (liste non vide).
4. Rechercher une valeur : renvoyer le maillon qui la contient, ou `NULL`.
5. Supprimer la première occurrence d'une valeur (teste la tête, le milieu, une valeur absente) et
   renvoyer la nouvelle tête.
6. Libérer toute la liste, puis vérifier avec valgrind qu'il ne reste aucune fuite.

## Les fichiers

| Fichier | Contenu |
|---|---|
| `code/liste.h`, `code/liste.c` | toutes les fonctions, en `_iter` et en `_rec` (le corrigé) |
| `code/exercices.c` | les 6 exercices sur 12 → 5 → 31 → 8, les deux versions côte à côte |
| `code/piege_tete.c` | le piège de la tête : le pointeur passé par valeur, et ses deux corrections |
| `code/mesure_pile.c` | longueur, recherche et libération sur N maillons : où la récursion plante |
| `code/max_mauvais.c` | le maximum récursif mal écrit (appel en double) : appels comptés |
| `code/executer.sh` | compile tout, exécute, et écrit les sorties dans `preuves/` |

## Compiler et lancer

Un seul programme, sous Linux ou WSL :

```bash
cd code
gcc -std=c11 -Wall -Wextra -Werror -o exercices exercices.c liste.c
./exercices
```

Tout refaire, preuves comprises, dans un conteneur Debian (gcc 12, valgrind), depuis ce dossier :

```bash
docker run --rm -v "$PWD:/w" -w /w debian:bookworm-slim sh -c \
  "apt-get update -qq && apt-get install -y -qq gcc libc6-dev valgrind && sh code/executer.sh"
```

## Ce que montrent les preuves

- `exercices.txt` : les deux versions donnent les mêmes résultats sur les 6 exercices.
- `valgrind-exercices.txt` : 18 allocations, 18 libérations, aucune fuite.
- `valgrind-piege_tete.txt` : la version fausse de l'insertion en tête perd 16 octets.
- `mesure_pile.txt` : avec la pile de 8 Mo par défaut, la longueur récursive compilée en `-O0` plante
  (segmentation fault) vers 262 456 maillons ; la boucle compte 10 millions de maillons.
- `recursion-O0-O2.txt` : en `-O2`, gcc transforme `longueur_rec`, `somme_rec` et `rechercher_rec` en
  boucles, mais `liberer_rec` reste récursive et plante toujours sur un million de maillons.
- `max_mauvais.txt` : le maximum mal écrit fait 1 073 741 823 appels sur une liste croissante de
  30 maillons, contre 30 sur la même liste à l'envers.
