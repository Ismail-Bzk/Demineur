#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <iostream>

using namespace std;

typedef enum {DECOUVERTE, NON_DECOUVERTE, DRAPEAU}type_etat;


/**
	* @Type  : permet de caracteriser un elements de l'historique
	* @param : position permet de connaitre l'emplacement sur la grille du changement.
	* @param : profondeur_ permet de connaitre la grille sur laquelle le changement a eu lieu.
	*	@param : etat permet de note le changement : la case est devenue decouverte par exemple
	*/
typedef struct{
  pair<int,int> position;
  int position_profondeur_;
  type_etat etat_ ;
} Type_history;


class History{
public:
  History();
  virtual ~History();
  void show_all_modif();
  void add_modif(Type_history history_last_modif );
  void reset();

  //int tour_actuelle = 0;
  //std::vector<std::vector<Type_history>> history_;
  //int tour_total = 0 ;
  //int Undo_Mode =0 ;

private:
	int tour_total = 0 ;  //int tour_actuelle = 0;
  std::vector<std::vector<Type_history>> history_;
	int tour_total = 0 ;
  int Undo_Mode =0 ;
};

#endif // HISTORY_H
