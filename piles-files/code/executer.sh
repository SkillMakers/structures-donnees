#!/bin/sh
# Compile et exécute tout, sorties dans ../preuves/. Lancé dans un conteneur Debian (gcc + valgrind).
# -Werror : un seul avertissement fait échouer la compilation, et set -e arrête alors le script
# (aucune preuve n'est écrite avec d'anciens exécutables).
set -eu
cd "$(dirname "$0")"
P=../preuves
mkdir -p "$P"
OPT="-std=c11 -Wall -Wextra -Werror -O0 -g"
BIN="demo pieges piege_vide mesure"
rm -f $BIN
trap 'rm -f $BIN' EXIT   # les exécutables sont supprimés même si le script échoue

{
  echo "\$ gcc --version | head -1"; gcc --version | head -1
  echo "\$ valgrind --version"; valgrind --version
  echo "\$ ulimit -s"; ulimit -s
  for f in $BIN; do
    echo "\$ gcc $OPT -o $f $f.c pile.c file.c exercices.c"
    gcc $OPT -o $f $f.c pile.c file.c exercices.c
    echo "(compilation reussie avec -Werror : aucun avertissement)"
  done
} > "$P/compilation.txt" 2>&1

./demo > "$P/demo.txt" 2>&1
# Sortie de référence relue à la main : une régression fait échouer le script au lieu de réécrire la preuve.
if ! diff -u attendu-demo.txt "$P/demo.txt"; then
  echo "ECHEC : la sortie de demo differe de attendu-demo.txt" >&2
  exit 1
fi
./pieges > "$P/pieges.txt" 2>&1
# Une erreur mémoire ou une fuite certaine fait échouer le script (code 1).
valgrind --leak-check=full --errors-for-leak-kinds=definite --error-exitcode=1 \
  ./demo > /dev/null 2> "$P/valgrind-demo.txt"

# lance PROG ARGS... : affiche la sortie et le code retour, sans arrêter le script si le programme plante.
lance() {
  code=0
  "$@" 2>&1 || code=$?
  echo "code retour : $code"
}

{
  echo "--- depiler une pile vide, version sans test"
  lance ./piege_vide sans_test
  echo "--- depiler une pile vide, depiler de pile.c (assert)"
  lance ./piege_vide assert
} > "$P/piege_vide.txt" 2>&1

# Itératif ou récursif. Recherche : la pile d'appels (8 Mo) contre une pile auxiliaire dans le tas.
{
  echo "pile d'appels : $(ulimit -s) Ko"
  for n in 1000 100000 1000000 10000000; do
    echo "--- N=$n"
    lance ./mesure rech_iter $n
    lance ./mesure rech_rec $n
  done

  # Seuil de la recherche récursive, par dichotomie ; trois essais (la pile change de place à chaque exécution).
  if ./mesure rech_rec 1000 > /dev/null 2>&1 && ! ./mesure rech_rec 1000000 > /dev/null 2>&1; then
    for essai in 1 2 3; do
      bas=1000; haut=1000000
      while [ $((haut - bas)) -gt 1000 ]; do
        mil=$(( (bas + haut) / 2 ))
        if ./mesure rech_rec $mil > /dev/null 2>&1; then bas=$mil; else haut=$mil; fi
      done
      echo "seuil, essai $essai (a 1000 pres) : passe a N=$bas, plante a N=$haut"
    done
  else
    echo "bornes invalides : 1000 doit passer et 1000000 planter (pile illimitee ?)"
  fi
} > "$P/mesure-recherche.txt" 2>&1

# Inversion : trois transvasements (itératif) contre insérer au fond (récursif), durées.
{
  for n in 1000 5000 10000 20000; do
    echo "--- N=$n"
    lance ./mesure inv_iter $n
    lance ./mesure inv_rec $n
  done
} > "$P/mesure-inversion.txt" 2>&1

echo "termine"
