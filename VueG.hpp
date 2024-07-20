#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/textview.h>
#include <gtkmm/table.h>
#include <gtkmm/checkbutton.h>
#include <vector>
#include <cstring>
#include <utility>
#include <sstream>
#include <chrono>

#include "Observateur.hpp"
#include "firstWindow.hpp"
#include "choiceWindow.hpp"
#include "dimWindow.hpp"
#include "Dialogue.hpp"
#include "Case.hpp"
#include "score_file.hpp"

class Controleur;

class VueG : public Gtk::Window, public Observateur<std::vector<std::string>,int> {

public:
  int profondeur=1;  //couche active du demineur 3D
  int Difficulte, Bombes; 
  int classic_game_mode = 1; //mode de jeu 1=normal, 0=custom, 2=3D
  int enregistrer_score=1;  // =1 si on peut enregistrer le score dans le fichier des scores
  std::string pseudo;
  std::chrono::_V2::steady_clock::time_point start;
  score_file score;
  std::vector<std::string> images= {"./Avatars/yoshi.png","./Avatars/toad.png","./Avatars/luigi.png","./Avatars/mario.png","./Avatars/maskass.png","./Avatars/boo.png","./Avatars/birdo.png","./Avatars/peach.png"};
  
private:
  /*Elements graphiques*/
  Gtk::MenuBar barreMenu;
  Gtk::MenuItem itemMenu,itemAide,itemParametres;
  Gtk::Menu Menu,Aide,Parametres;
  Gtk::ImageMenuItem subItemFichierScores,subItemQuitter,subItemRegles,subItemMode,subItemDimensions;
  Gtk::SeparatorMenuItem separateur;
  Gtk::HBox boiteMenu,boiteJeu,Grille;
  Gtk::VBox boiteJoueur,Jeu,boiteInfo,boiteGrille;
  Gtk::Label pseudoJoueur,numCouche,difficulteJeu,modeJeu;
  Gtk::Image avatarJoueur;
  Gtk::Grid grilleJeu;
  Gtk::CheckButton bDrapeaux;
  Gtk::Button bAvatar,bRestart,bUndo,bRedo,bPlus,bMoins;
  
  /*Elements non graphiques*/
  std::vector<Case*> casesGrille;  //vector qui stocke toutes les cases de la grille
  std::vector<std::string>::iterator it; //iterateur utilisé sur le vecteur images
  
  
public:
  VueG();
  virtual ~VueG();
  void close();
  void update(std::vector<std::string> &info, int& res);
  void afficherDialogue();
  void afficherPremierePage();
  void afficherInstructions();
  void afficherDifficulte();
  int afficherChoixMode();
  void afficherFichierScores();
  void message(std::string message, std::string titre, std::string sous_message);
  std::pair<int,int> afficherChoixDimensions();
  void initialiserGrille(const int M);
  void addGridListener(Controleur* c);
  void addBDrapeauxListener(Controleur* c);
  void addBRestartListener(Controleur* c);
  void addBUndoListener(Controleur* c);
  void addBRedoListener(Controleur* c);
  void addItemModeListener(Controleur* c);
  void addBMoinsListener(Controleur* c);
  void addBPlusListener(Controleur* c);
  void resetGrille();
  void changerAvatar();
  void setNumCouche(int c);
  void setDifficulteJeu(std::string d);
  void setModeJeu(std::string m);
  void setDrapeauxActif();
  
};


