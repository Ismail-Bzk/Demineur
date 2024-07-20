#pragma once

#include "gtkmm.h"
#include "gtkmm/button.h"
#include <vector>

class Mine: public Gtk::Button{
private:
    int ligne;
    int colonne;
  //MinesweeperGame *game;
  //std::vector<Mine*> v;

public:
  void set_ligne(int l){
      this->ligne = l;
    }

  void set_colonne(int c){
    this-> colonne=c;
  }

  int get_ligne(){
     return this->ligne;
    }

  int get_colonne(){
    return this->colonne;
  }
 };
 
