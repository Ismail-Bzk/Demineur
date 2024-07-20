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
  int premier_coup = 1; //indicateur pour savoir si le joueur a d�j� jou�

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
   * @Role : Lie le clique sur un bouton de la grille � la d�couverte de la case
   *
   * @param : l,c : indices de ligne et de colonne de la case cliqu�e
   * @retval: none
*/
  void on_gridBox_button(int l, int c){
    if (premier_coup == 1){ //s'il s'agit de la premi�re case d�couverte
      v->setDrapeauxActif(); //le joueur peut d�sormais placer des drapeaux
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
   * @Role : Lie le clique sur le bouton restart � la r�initialisation du jeu
   *
   * @param : none
   * @retval: none
*/
  void on_restart_button(){
    /*On restart le modele et la vue*/
    m->restart();
    v->resetGrille();
    
    premier_coup=1; //le prochain coup sera le premier

    /*On remet les param�tres initiaux*/
    m->no_death_mode=0;
    v->setModeJeu("normal");
    v->enregistrer_score=1;
    v->addGridListener(this);
    v->profondeur=1;
  }


   /**
   * @Role : Lie le clique sur le bouton undo � l'action de d�faire la derni�re commande
   *
   * @param : none
   * @retval: none
*/
  void on_undo_button(){
    m->undo();
    if(!premier_coup){ //on affichage l'�tat de la grille si une commande a �t� d�faite 
      m->affichage(v->profondeur);
    }
  }


   /**
   * @Role : Lie le clique sur le bouton redo � l'action de refaire la derni�re
   *
   * @param : none
   * @retval: none
*/
  void on_redo_button(){
    m->redo();
    if(!premier_coup){ //on affiche l'�tat de la grille si une commande a �t� refaite
      m->affichage(v->profondeur);
    }
  }


   /**
   * @Role : Lie le clique sur l'item mode � l'affichage de la fen�tre de s�lection du no death mode
   *
   * @param : none
   * @retval: none
*/
  void on_item_mode(){
    m->no_death_mode=v->afficherChoixMode();
  }


   /**
   * @Role : Lie le clique sur le bouton - au passage dans la couche inf�rieure de la grille 3D
   *
   * @param : none
   * @retval: none
*/
  void on_moins_button(){
    if (v->profondeur>1){
      v->profondeur--;//si on est pas � la premi�re couche, on d�cr�mente
      v->setNumCouche(v->profondeur);
      if(!premier_coup){ //si le joueur a d�j� jou� un coup on peut afficher
      m->affichage(v->profondeur);
      }
    }
  }


   /**
   * @Role : Lie le clique sur le bouton + au passage dans la couche sup�rieure de la grille 3D
   *
   * @param : none
   * @retval: none
*/
  void on_plus_button(){
    if (v->profondeur<5){
      v->profondeur++;//si on est pas � la derni�re couche, on incr�mente
      v->setNumCouche(v->profondeur);
      if(!premier_coup){ //si le joueur a d�j� jou� un coup on peut afficher
	m->affichage(v->profondeur);
      }
    }
  }
  
};
