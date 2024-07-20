#ifndef DEMINEUR_H
#define DEMINEUR_H

#include "History.h"

#include <vector>
#include <ctime> // Obligatoire
#include <cstdlib> // Obligatoire
#include <string>
#include <iostream>


using namespace std;

#define je_suis_une_bombe 9
#define FACILE 10
#define MOYEN 20
#define DIFFICILE 30




typedef struct{
  type_etat etat_ = NON_DECOUVERTE;
  int nb_bombe_ = 0;
} Type_ma_case;



class Demineur {
public :
  int difficulte_ ;
  int nb_bombes_ ;
  int nb_case_decouvert_ = 0;
  History game_history_;
  int partie = 1;
  int victoire = 1;
  int drapeaux=0;
  int no_death_mode=0;

private:
  std::vector<std::vector<Type_ma_case>> Mon_jeu_;

public:
  Demineur();
  virtual ~Demineur();

  void configDemineur(const int difficulte, const int mon_nb_bombe=0);
  void poser_bombe(const int x, const int y);
  void affichage();
  void affichage_final();
  int jouer(const int x, const int y);
  int drapeau(const int x, const int y);
  void decouverte (const int x, const int y);
  void update(const int x, const int y);
  void restart();

  void undo();
  void redo();
};

#endif // DEMINEUR_H
