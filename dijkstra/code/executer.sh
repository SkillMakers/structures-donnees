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
    echo "\$ gcc $OPT -o $f $f.c graphe.c"
    gcc $OPT -o $f $f.c graphe.c
    echo "(compilation reussie avec -Werror : aucun avertissement)"
  done
  # La même mesure compilée avec les optimisations : la récursion plante-t-elle encore ?
  echo "\$ gcc -std=c11 -Wall -Wextra -Werror -O2 -o mesure_o2 mesure.c graphe.c"
  gcc -std=c11 -Wall -Wextra -Werror -O2 -o mesure_o2 mesure.c graphe.c
  echo "(compilation reussie avec -Werror : aucun avertissement)"
} > "$P/compilation.txt" 2>&1

./demo > "$P/demo.txt" 2>&1
# Sortie de référence relue à la main : une régression fait échouer le script au lieu de réécrire la preuve.
if [ -f attendu-demo.txt ]; then
  if ! diff -u attendu-demo.txt "$P/demo.txt"; then
    echo "ECHEC : la sortie de demo differe de attendu-demo.txt" >&2
    exit 1
  fi
else
  echo "ATTENTION : attendu-demo.txt absent (premiere execution : relire preuves/demo.txt et le copier)" >&2
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

# Le parcours en profondeur sur un graphe en ligne : la pile contre la récursion (-O0 puis -O2).
{
  echo "pile d'appels : $(ulimit -s) Ko"
  premier=0; dernier=0
  for n in 1000 100000 1000000 10000000; do
    echo "--- N=$n"
    lance ./mesure chaine_iter $n
    code=0; ./mesure chaine_rec $n 2>&1 || code=$?; echo "code retour : $code"
    [ $n -eq 1000 ] && premier=$code
    [ $n -eq 10000000 ] && dernier=$code
    lance ./mesure_o2 chaine_rec $n
  done
  # Le seuil, par dichotomie entre les deux bornes déjà mesurées (une seule fois : le résultat ne varie pas,
  # la pile d'appels a toujours la même taille).
  if [ $premier -eq 0 ] && [ $dernier -ne 0 ]; then
    bas=1000; haut=10000000
    while [ $((haut - bas)) -gt 1000 ]; do
      mil=$(( (bas + haut) / 2 ))
      if ./mesure chaine_rec $mil > /dev/null 2>&1; then bas=$mil; else haut=$mil; fi
    done
    echo "seuil -O0 (a 1000 pres) : passe a N=$bas, plante a N=$haut"
  else
    echo "bornes invalides : 1000 doit passer et 10000000 planter"
  fi
} > "$P/mesure-chaine.txt" 2>&1

echo "termine"
