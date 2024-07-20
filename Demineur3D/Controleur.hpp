#pragma once

#include "Modele.hpp"
#include "Demineur.h"
#include "VueG.hpp"

class Controleur {
private:
  Demineur *m;
  VueG *v;
  int premier_coup = 1;
  
public:
//etablit la connexion entre le modele et la vue 
//gestionnaire de signaux pour les boutons 

  Controleur(Demineur *mm, VueG *vv) : m(mm), v(vv) {
    v->addGridListener(this);
    v->addBDrapeauxListener(this);
    v->addBRestartListener(this);
    v->addBUndoListener(this);
    v->addBRedoListener(this);
    v->addItemModeListener(this);
  }

  void on_gridBox_button(int l, int c,int p){
    //fonction qui lie le clique sur les boutons à la découverte d'une case
    if (premier_coup == 1){
      m->configDemineur(v->Difficulte,v->Bombes);
      m->poser_bombe(l,c,p);
      premier_coup=0;
    }
    m->update(l,c,p);
  }

  void on_drapeaux_button(){
    //fonction qui lie le clique sur le bouton Drapeaux au mode drapeau
    m->drapeaux=!(m->drapeaux);
  }

  void on_restart_button(){
    m->restart();
    v->resetGrille();
    premier_coup=1;
    v->addGridListener(this);
  }

  void on_undo_button(){
    m->undo();
    if(!premier_coup){
      m->affichage(v->profondeur);
    }
  }
  
  void on_redo_button(){
    m->redo();
    if(!premier_coup){
      m->affichage(v->profondeur);
    }
  }

  void on_item_mode(){
    m->no_death_mode=v->afficherChoixMode();
  }  
  
};
