#!/bin/sh
# Compile et exécute tout, sorties dans ../preuves/. Lancé dans un conteneur Debian (gcc + valgrind).
# -Werror : un seul avertissement fait échouer la compilation, et set -e arrête alors le script
# (aucune preuve n'est écrite avec d'anciens exécutables).
set -eu
cd "$(dirname "$0")"
P=../preuves
mkdir -p "$P"
OPT="-std=c11 -Wall -Wextra -Werror -O0 -g"
rm -f exercices piege_tete mesure_pile mesure_pile_O2 max_mauvais

{
  echo "\$ gcc --version | head -1"; gcc --version | head -1
  echo "\$ valgrind --version"; valgrind --version
  echo "\$ ulimit -s"; ulimit -s
  for f in exercices piege_tete mesure_pile max_mauvais; do
    echo "\$ gcc $OPT -o $f $f.c liste.c"
    gcc $OPT -o $f $f.c liste.c
    echo "(compilation reussie avec -Werror : aucun avertissement)"
  done
  echo "\$ gcc ${OPT%-O0 -g} -O2 -o mesure_pile_O2 mesure_pile.c liste.c"
  gcc ${OPT%-O0 -g} -O2 -o mesure_pile_O2 mesure_pile.c liste.c
  echo "(compilation reussie avec -Werror : aucun avertissement)"
} > "$P/compilation.txt" 2>&1

./exercices > "$P/exercices.txt" 2>&1
./piege_tete > "$P/piege_tete.txt" 2>&1
./max_mauvais > "$P/max_mauvais.txt" 2>&1

valgrind --leak-check=full ./exercices > /dev/null 2> "$P/valgrind-exercices.txt"
valgrind --leak-check=full ./piege_tete > /dev/null 2> "$P/valgrind-piege_tete.txt" || true

# lance PROG ARGS... : affiche la sortie et le code retour, sans arrêter le script si le programme plante.
lance() {
  code=0
  "$@" 2>&1 || code=$?
  echo "code retour : $code"
}

# Mesures sur la pile d'appels (8 Mo par défaut).
{
  echo "pile : $(ulimit -s) Ko"
  for n in 10 1000 100000 1000000 10000000; do
    echo "--- -O0, N=$n"
    lance ./mesure_pile $n iter
    lance ./mesure_pile $n rec
  done

  # Seuil de la longueur récursive, par dichotomie. Les bornes sont vérifiées d'abord ; trois essais,
  # car la position de la pile change à chaque exécution (ASLR).
  if ./mesure_pile 1000 rec > /dev/null 2>&1 && ! ./mesure_pile 1000000 rec > /dev/null 2>&1; then
    for essai in 1 2 3; do
      bas=1000; haut=1000000
      while [ $((haut - bas)) -gt 1000 ]; do
        mil=$(( (bas + haut) / 2 ))
        if ./mesure_pile $mil rec > /dev/null 2>&1; then bas=$mil; else haut=$mil; fi
      done
      echo "seuil, essai $essai (a 1000 pres) : passe a N=$bas, plante a N=$haut"
    done
  else
    echo "bornes invalides : 1000 doit passer et 1000000 planter (pile illimitee ?)"
  fi

  echo "--- recherche recursive (recursion terminale), -O0, N=1000000"
  lance ./mesure_pile 1000000 rech
  echo "--- liberation recursive, -O0, N=1000000"
  lance ./mesure_pile 1000000 lib

  echo "--- meme programme compile avec -O2, N=10000000"
  lance ./mesure_pile_O2 10000000 rech
  lance ./mesure_pile_O2 10000000 rec
  echo "--- liberation recursive, -O2, N=1000000"
  lance ./mesure_pile_O2 1000000 lib
} > "$P/mesure_pile.txt" 2>&1

# Ce que gcc a fait des fonctions récursives : une fonction encore récursive s'appelle elle-même.
{
  # Exécutables liés : les adresses sont résolues (dans un .o, l'appel à free ressemble à un appel à soi-même).
  for prog in mesure_pile mesure_pile_O2; do
    objdump -d --no-show-raw-insn $prog > $prog.asm
    for f in longueur_rec somme_rec rechercher_rec liberer_rec; do
      n=$(awk -v f="$f" '$0 ~ "<"f">:$" {dans=1; next} /^$/ {dans=0} dans' $prog.asm | grep -c "call.*<$f>" || true)
      echo "$prog : $f contient $n appel(s) a elle-meme"
    done
    rm -f $prog.asm
  done
} > "$P/recursion-O0-O2.txt" 2>&1

rm -f exercices piege_tete mesure_pile mesure_pile_O2 max_mauvais
echo fini
