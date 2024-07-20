#ifndef DEMINEUR_H
#define DEMINEUR_H

#include "History.h"

#include <vector>
#include <ctime> // Obligatoire
#include <cstdlib> // Obligatoire
#include <string>
#include <iostream>
#include "Observable.hpp"

using namespace std;

#define je_suis_une_bombe   9

#define Trois_D             100


typedef struct{
  type_etat etat_ = NON_DECOUVERTE;
  int nb_bombe_ = 0;
} Type_ma_case;


class Demineur : public Observable<std::vector<std::string>,int> {
public :
  int difficulte_ ;
  int profondeur_ ;
  int nb_bombes_ ;
  int nb_case_decouvert_ = 0;
  History game_history_;
  int partie = 1;
  int victoire = 1;
  int drapeaux=0;
  int no_death_mode=0;

private:

  std::vector<std::vector<std::vector<Type_ma_case>>> Mon_jeu_;

public:
  Demineur();
  virtual ~Demineur();

  void configDemineur(const int difficulte, const int mon_nb_bombe=0);
  void poser_bombe(const int x, const int y, const int z=0);
  void affichage(const int profondeur);
  void affichage_final(const int profondeur);
  int jouer(const int x, const int y,const int z);
  int drapeau(const int x, const int y,const int z);
  void decouverte (const int x, const int y, const int z);
  void update(const int x, const int y,const int z);
  void restart();

  void undo();
  void redo();
};

#endif // DEMINEUR_H
