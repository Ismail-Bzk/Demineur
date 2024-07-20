#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <iostream>

using namespace std;

typedef enum {DECOUVERTE, NON_DECOUVERTE, DRAPEAU}type_etat;

typedef struct{
  pair<int,int> position;
  type_etat etat_ ;
} Type_history;


class History{
public:
  History();
  virtual ~History();
  void show_all_modif();
  void add_modif(Type_history history_last_modif );
  void reset();

  int tour_actuelle = 0;
  std::vector<std::vector<Type_history>> history_;
  int tour_total = 0 ;
  int Undo_Mode =0 ;

private:
  //std::vector<std::vector<Type_history>> history_; 
        //int tour_total = 0 ;
};

#endif // HISTORY_H
