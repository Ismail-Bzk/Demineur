#pragma once

#include "Demineur.hpp"
#include "VueG.hpp"
#include <iostream>
#include <cstdlib>

//etablit la connexion entre le modele et la vue, gestionnaire de signaux pour les boutons

class Controleur {
private:
  Demineur *m; //modele
  VueG *v; //vue
  int premier_coup = 1; //indicateur pour savoir si le joueur a déjà joué

public:
  //constructeur
   Controleur(Demineur *mm, VueG *vv) : m(mm), v(vv) {
    v->addGridListener(this);
    v->addBDrapeauxListener(this);
    v->addBRestartListener(this);
    v->addBUndoListener(this);
    v->addBRedoListener(this);
    v->addItemModeListener(this);
    v->addBMoinsListener(this);
    v->addBPlusListener(this);
  }


 /**
   * @Role : Lie le clique sur un bouton de la grille à la découverte de la case
   *
   * @param : l,c : indices de ligne et de colonne de la case cliquée
   * @retval: none
*/
  void on_gridBox_button(int l, int c){
    if (premier_coup == 1){ //s'il s'agit de la première case découverte
      v->setDrapeauxActif(); //le joueur peut désormais placer des drapeaux
      m->configDemineur(v->Difficulte,v->Bombes); //on configure le jeu en fonction dees choix du joueur
      m->poser_bombe(l,c,v->profondeur);
      premier_coup=0;
    }
    m->update(l,c,v->profondeur); //on actualise le jeu
  }

  
 /**
   * @Role : Lie le clique sur le bouton drapeau au placement d'un drapeau dans la grille
   *
   * @param : none
   * @retval: none
*/
  void on_drapeaux_button(){
    m->drapeaux=!(m->drapeaux); //on donne la bonne valeur au mode drapeau du modele
  }


   /**
   * @Role : Lie le clique sur le bouton restart à la réinitialisation du jeu
   *
   * @param : none
   * @retval: none
*/
  void on_restart_button(){
    /*On restart le modele et la vue*/
    m->restart();
    v->resetGrille();
    
    premier_coup=1; //le prochain coup sera le premier

    /*On remet les paramètres initiaux*/
    m->no_death_mode=0;
    v->setModeJeu("normal");
    v->enregistrer_score=1;
    v->addGridListener(this);
    v->profondeur=1;
  }


   /**
   * @Role : Lie le clique sur le bouton undo à l'action de défaire la dernière commande
   *
   * @param : none
   * @retval: none
*/
  void on_undo_button(){
    m->undo();
    if(!premier_coup){ //on affichage l'état de la grille si une commande a été défaite 
      m->affichage(v->profondeur);
    }
  }


   /**
   * @Role : Lie le clique sur le bouton redo à l'action de refaire la dernière
   *
   * @param : none
   * @retval: none
*/
  void on_redo_button(){
    m->redo();
    if(!premier_coup){ //on affiche l'état de la grille si une commande a été refaite
      m->affichage(v->profondeur);
    }
  }


   /**
   * @Role : Lie le clique sur l'item mode à l'affichage de la fenêtre de sélection du no death mode
   *
   * @param : none
   * @retval: none
*/
  void on_item_mode(){
    m->no_death_mode=v->afficherChoixMode();
  }


   /**
   * @Role : Lie le clique sur le bouton - au passage dans la couche inférieure de la grille 3D
   *
   * @param : none
   * @retval: none
*/
  void on_moins_button(){
    if (v->profondeur>1){
      v->profondeur--;//si on est pas à la première couche, on décrémente
      v->setNumCouche(v->profondeur);
      if(!premier_coup){ //si le joueur a déjà joué un coup on peut afficher
      m->affichage(v->profondeur);
      }
    }
  }


   /**
   * @Role : Lie le clique sur le bouton + au passage dans la couche supérieure de la grille 3D
   *
   * @param : none
   * @retval: none
*/
  void on_plus_button(){
    if (v->profondeur<5){
      v->profondeur++;//si on est pas à la dernière couche, on incrémente
      v->setNumCouche(v->profondeur);
      if(!premier_coup){ //si le joueur a déjà joué un coup on peut afficher
	m->affichage(v->profondeur);
      }
    }
  }
  
};
