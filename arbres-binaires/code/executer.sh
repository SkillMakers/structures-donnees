#!/bin/sh
# Compile et exécute tout, sorties dans ../preuves/. Lancé dans un conteneur Debian (gcc + valgrind).
# -Werror : un seul avertissement fait échouer la compilation, et set -e arrête alors le script.
set -eu
cd "$(dirname "$0")"
P=../preuves
mkdir -p "$P"
OPT="-std=c11 -Wall -Wextra -Werror -O0 -g"
BIN="demo mesure mesure_o2"
rm -f $BIN
trap 'rm -f $BIN' EXIT   # les exécutables sont supprimés même si le script échoue

{
  echo "\$ gcc --version | head -1"; gcc --version | head -1
  echo "\$ valgrind --version"; valgrind --version
  echo "\$ ulimit -s"; ulimit -s
  for f in demo mesure; do
    echo "\$ gcc $OPT -o $f $f.c abr.c"
    gcc $OPT -o $f $f.c abr.c
    echo "(compilation reussie avec -Werror : aucun avertissement)"
  done
  # La même mesure compilée avec les optimisations : la récursion plante-t-elle encore ?
  echo "\$ gcc -std=c11 -Wall -Wextra -Werror -O2 -o mesure_o2 mesure.c abr.c"
  gcc -std=c11 -Wall -Wextra -Werror -O2 -o mesure_o2 mesure.c abr.c
  echo "(compilation reussie avec -Werror : aucun avertissement)"
} > "$P/compilation.txt" 2>&1

./demo > "$P/demo.txt" 2>&1
# Sortie de référence relue à la main : une régression fait échouer le script au lieu de réécrire la preuve.
if [ ! -f attendu-demo.txt ]; then
  echo "ECHEC : attendu-demo.txt introuvable (sortie de reference)" >&2
  exit 1
fi
if ! diff -u attendu-demo.txt "$P/demo.txt"; then
  echo "ECHEC : la sortie de demo differe de attendu-demo.txt" >&2
  exit 1
fi
# Une erreur mémoire ou une fuite certaine fait échouer le script (code 1).
valgrind --leak-check=full --errors-for-leak-kinds=definite --error-exitcode=1 \
  ./demo > /dev/null 2> "$P/valgrind-demo.txt"

# lance PROG ARGS... : affiche la sortie et le code retour, sans arrêter le script si le programme plante.
lance() {
  code=0
  "$@" 2>&1 || code=$?
  echo "code retour : $code"
}

# Le piège : les mêmes valeurs, triées ou mélangées.
{
  for n in 1000 10000 20000 40000; do
    echo "--- N=$n"
    lance ./mesure melange $n
    lance ./mesure trie $n
  done
} > "$P/mesure-trie.txt" 2>&1

# L'arbre en peigne (valeurs triées) : sa hauteur, avec une boucle ou en récursif (-O0 puis -O2).
{
  echo "pile d'appels : $(ulimit -s) Ko"
  for n in 1000 100000 1000000 10000000; do
    echo "--- N=$n"
    lance ./mesure peigne_iter $n
    lance ./mesure peigne_rec $n
    lance ./mesure_o2 peigne_rec $n
  done
  if ./mesure peigne_rec 1000 > /dev/null 2>&1 && ! ./mesure peigne_rec 10000000 > /dev/null 2>&1; then
    for essai in 1 2 3; do
      bas=1000; haut=10000000
      while [ $((haut - bas)) -gt 1000 ]; do
        mil=$(( (bas + haut) / 2 ))
        if ./mesure peigne_rec $mil > /dev/null 2>&1; then bas=$mil; else haut=$mil; fi
      done
      echo "seuil, essai $essai (a 1000 pres) : passe a N=$bas, plante a N=$haut"
    done
  else
    echo "bornes invalides : 1000 doit passer et 10000000 planter"
  fi
} > "$P/mesure-peigne.txt" 2>&1

echo "termine"
