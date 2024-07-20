#ifndef DEMINEUR_H
#define DEMINEUR_H

#include "History.hpp"

#include <vector>
#include <ctime> // Obligatoire
#include <cstdlib> // Obligatoire
#include <string>
#include <iostream>
#include "Observable.hpp"

using namespace std;

#define je_suis_une_bombe   9

#define Trois_D             100

/**
	* @Type  : permet de caracteriser chacun des elements de la grille
	* @param : type_etat permet de connaitre l'etat de la case : Decouvert ou Non_Decouverte ou Drapeau
	* @param : nb_bombe permet de connaitre le nombre de bombe voisine a la case.
	*					 Elle permet aussi de definir une bombe si nb_bombe vaut je_suis_une_bombe soit 9
	*/
typedef struct{
  type_etat etat_ = NON_DECOUVERTE;
  int nb_bombe_ = 0;
} Type_ma_case;


class Demineur : public Observable<std::vector<std::string>,int> {
public :
  int difficulte_ ;												/** difficulte choisit par le joueur **/
  int profondeur_ ;												/** profondeur_ permet de se reperer sur le 3 eme axe **/
  int nb_bombes_ ;												/** nombres de bombes choisit par je joueur **/
  int nb_case_decouvert_ = 0;							/** permet de connaitre le nombre de case decouverte et de declarer le joueur gagnant **/
  History game_history_;									/** historique de la partie **/
  int partie = 1;													/** permet de connaitre la validite et la consequence de la derniere action du joueur **/
  int drapeaux=0;													/** permet d'activer le mode drapeau **/
  int no_death_mode=0;										/** permet d'activer le mode sans mort **/

private:
  std::vector<std::vector<std::vector<Type_ma_case>>> Mon_jeu_;							/** permet de representer le jeu **/

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
