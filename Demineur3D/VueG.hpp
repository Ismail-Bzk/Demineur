#pragma once

#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/alignment.h>
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
#include "Mine.hpp"
#include "score_file.h"

class Controleur;

class VueG : public Gtk::Window, public Observateur<std::vector<std::string>,int> {

public:
  int profondeur=1;
  int Difficulte, Bombes;
  int classic_game_mode = 1;
  std::string pseudo;
  std::chrono::_V2::steady_clock::time_point start;
  score_file score;
  std::vector<std::string> images= {"./Avatars/yoshi.png","./Avatars/toad.png","./Avatars/luigi.png","./Avatars/mario.png","./Avatars/maskass.png","./Avatars/boo.png","./Avatars/birdo.png","./Avatars/peach.png"};
  
private:
  Gtk::MenuBar barreMenu;
  Gtk::MenuItem itemMenu,itemAide,itemParametres;
  Gtk::Menu Menu,Aide,Parametres;
  Gtk::ImageMenuItem subItemFichierScores,subItemQuitter,subItemRegles,subItemMode,subItemDimensions;
  Gtk::SeparatorMenuItem separateur;
  Gtk::HBox boiteMenu,boiteJeu,Grille;
  Gtk::VBox boiteJoueur,Jeu,boiteInfo;
  Gtk::Label pseudoJoueur,numCouche,difficulteJeu,modeJeu;
  Gtk::Image avatarJoueur;
  Gtk::Grid GrilleJeu;
  Gtk::CheckButton bDrapeaux;
  Gtk::Button bAvatar,bRestart,bUndo,bRedo,bPlus,bMoins;
  std::vector<Mine*> casesGrille;
  std::vector<std::string>::iterator it;
  
  
  
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
  std::pair<int,int> afficherChoixDimensions();
  void initialiserGrille(const int M);
  void addGridListener(Controleur* c);
  void addBDrapeauxListener(Controleur* c);
  void addBRestartListener(Controleur* c);
  void addBUndoListener(Controleur* c);
  void addBRedoListener(Controleur* c);
  void addItemModeListener(Controleur* c);
  void resetGrille();
  std::vector<Mine*> get_casesGrille();
  void changerAvatar();
  
};


