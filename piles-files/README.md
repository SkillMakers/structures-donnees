# Piles et files en C

Code de la vidéo [**Piles et files en C : cours et exercices corrigés**](https://youtu.be/PTLaJJ202OY).

## Les 6 exercices

Avec les seules opérations de la pile (empiler, dépiler, sommet, est vide) et de la file (enfiler, défiler,
est vide), en boucle et en récursif quand ça a un sens :

1. Écrire empiler et dépiler ; empiler 4, 9, 2, 7 ; lire le sommet ; dépiler deux fois.
2. File dans 4 cases : enfiler 4, 9, 2, 7 ; défiler deux fois ; enfiler 5 et 8.
3. Rechercher une valeur dans la pile, sans la modifier.
4. Compter les éléments d'une pile, puis la copier, sans la modifier.
5. Inverser une pile, puis inverser une file.
6. Les parenthèses, crochets et accolades d'un texte sont-ils équilibrés ?

## Les fichiers

| Fichier | Contenu |
|---|---|
| `code/pile.h`, `code/pile.c` | la pile : une liste chaînée utilisée par la tête |
| `code/file.h`, `code/file.c` | la file : un tableau circulaire (tête, nombre d'éléments, modulo) |
| `code/exercices.h`, `code/exercices.c` | le corrigé des exercices 3 à 6, en `_iter` et en `_rec` |
| `code/demo.c` | les 6 exercices exécutés ; sa sortie attendue est dans `code/attendu-demo.txt` |
| `code/pieges.c` | la recherche qui dépile sans rempiler, et la file circulaire sans compteur |
| `code/piege_vide.c` | dépiler une pile vide : sans test, puis avec l'`assert` de `pile.c` |
| `code/mesure.c` | recherche et inversion sur N éléments : boucle contre récursion |
| `code/executer.sh` | compile tout, exécute, et écrit les sorties dans `preuves/` |

## Compiler et lancer

Sous Linux ou WSL :

```bash
cd code
gcc -std=c11 -Wall -Wextra -Werror -o demo demo.c pile.c file.c exercices.c
./demo
```

Tout refaire, preuves comprises, dans un conteneur Debian (gcc 12, valgrind), depuis ce dossier :

```bash
docker run --rm -v "$PWD:/w" -w /w debian:bookworm-slim sh -c \
  "apt-get update -qq && apt-get install -y -qq gcc libc6-dev valgrind && sh code/executer.sh"
```

## Ce que montrent les preuves

- `demo.txt` : les 6 exercices ; boucle et récursion donnent les mêmes résultats (pour l'inversion, la
  version récursive, appliquée au résultat, remet l'ordre de départ).
- `valgrind-demo.txt` : 119 allocations, 119 libérations, aucune erreur.
- `pieges.txt` : la recherche qui dépile répond « trouve », et la pile ne contient plus que 4 ;
  sans compteur, une file de 4 cases pleine se dit vide.
- `piege_vide.txt` : dépiler une pile vide sans test plante (segmentation fault) ; avec l'`assert`,
  le programme s'arrête sur un message clair.
- `mesure-recherche.txt` : avec la pile d'appels de 8 Mo, la recherche récursive plante vers
  175 000 éléments ; la boucle passe 10 millions.
- `mesure-inversion.txt` : inverser 20 000 éléments prend 0,006 s en boucle (trois transvasements)
  et 23,4 s en récursif (insérer au fond) : doubler N multiplie le temps récursif par environ 4.
