#include <iomanip>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "VueG.hpp"
#include "Controleur.hpp"

VueG::VueG():
  itemMenu("Menu"),
  itemAide("Aide"),
  itemParametres("Parametres"),
  subItemFichierScores("Scores"),
  subItemQuitter(Gtk::Stock::CLOSE),
  subItemRegles(Gtk::Stock::HELP),
  subItemMode("Mode"),
  boiteMenu(false),
  boiteJeu(false),
  Grille(true),
  boiteJoueur(false),
  Jeu(false),
  numCouche("Couche: 1"),
  difficulteJeu("Difficulte: Facile"),
  modeJeu("Mode: Normal"),
  avatarJoueur(images[0]),
  bDrapeaux("Drapeaux"),
  bAvatar("Changer Avatar"),
  bRestart("Nouvelle Partie"),
  bPlus("+"),
  bMoins("-"),
  it(images.begin()+1)
{
  auto undo = new Gtk::Image("./Icons/undo.png");
  auto redo = new Gtk::Image("./Icons/redo.png");
  bUndo.set_image(*undo);
  bRedo.set_image(*redo);
  /*Creation des differents menus*/
  /*Barre de menu*/
  barreMenu.append(itemMenu);
  barreMenu.append(itemAide);
  barreMenu.append(itemParametres);
  itemMenu.set_submenu(Menu);
  itemAide.set_submenu(Aide);
  itemParametres.set_submenu(Parametres);
  /*Sous items du menu deroulant Menu*/
  Menu.append(subItemFichierScores);
  Menu.append(separateur);
  Menu.append(subItemQuitter);
  /*Sous items du menu deroulant Aide*/
  Aide.append(subItemRegles);
  /*Sous items du menu deroulant Parametres*/
  Parametres.append(subItemMode);

  /*Gestion des emplacements graphiques*/
  bDrapeaux.set_active(false);
  boiteMenu.pack_start(barreMenu,Gtk::PACK_SHRINK);
  boiteMenu.pack_start(bUndo,Gtk::PACK_SHRINK);
  boiteMenu.pack_start(bRedo,Gtk::PACK_SHRINK);
  boiteMenu.pack_start(bPlus,Gtk::PACK_SHRINK);
  boiteMenu.pack_start(bMoins,Gtk::PACK_SHRINK);
  boiteJoueur.pack_start(avatarJoueur,Gtk::PACK_SHRINK);
  boiteJoueur.pack_start(pseudoJoueur,Gtk::PACK_SHRINK);
  boiteJoueur.pack_start(bAvatar,Gtk::PACK_SHRINK);
  boiteInfo.pack_start(difficulteJeu,Gtk::PACK_SHRINK);
  boiteInfo.pack_start(modeJeu,Gtk::PACK_SHRINK);
  boiteInfo.pack_start(numCouche,Gtk::PACK_SHRINK);
  boiteJoueur.pack_end(boiteInfo,Gtk::PACK_SHRINK);
  Jeu.pack_start(boiteMenu,Gtk::PACK_SHRINK);
  Grille.pack_start(boiteJoueur);//,Gtk::PACK_SHRINK);
  Grille.pack_start(GrilleJeu);//,Gtk::PACK_SHRINK);
  //boiteJeu.pack_start(boiteJoueur,Gtk::PACK_SHRINK);
  boiteMenu.pack_end(bDrapeaux,Gtk::PACK_SHRINK);
  // Jeu.pack_start(boiteJeu,Gtk::PACK_SHRINK);
  Jeu.pack_start(Grille, Gtk::PACK_SHRINK);
  Jeu.pack_end(bRestart,Gtk::PACK_SHRINK);
  add(Jeu);

  
  /*Gestion des connexions*/
  subItemQuitter.signal_activate().connect(sigc::mem_fun(*this,&VueG::close));
  subItemRegles.signal_activate().connect(sigc::mem_fun(*this,&VueG::afficherInstructions));
  subItemFichierScores.signal_activate().connect(sigc::mem_fun(*this,&VueG::afficherFichierScores));
  bAvatar.signal_clicked().connect(sigc::mem_fun(*this,&VueG::changerAvatar));
  afficherPremierePage();
  afficherDialogue();
  afficherDifficulte();

  this->start = chrono::steady_clock::now();
  show_all_children();
}

void VueG::update(std::vector<std::string> &info, int& res){
  auto grille=this->get_casesGrille();
  for(auto i=0; i<info.size(); i++){
    auto img = new Gtk::Image("./Icons/"+info[i]+".png");
    grille[i]->set_image(*img);
    if(info[i]!="n" and info[i]!="flag"){
      //si une case est découvete et n'est pas un drapeau
      grille[i]->set_sensitive(false);//elle n'est plus cliquable
	}
    else if(info[i]=="n" and !grille[i]->get_sensitive()){
      //si une case est non découverte mais incliquable -> undo réalisé
      grille[i]->set_sensitive(true); //elle redevient cliquable
    }
    
  }
  if(res<2 and res!=1){ //le jeu est fini et pas de no_death_mode
    bUndo.set_sensitive(false);
    bRedo.set_sensitive(false);
    bDrapeaux.set_sensitive(false);
    if(res == 0){ //perdu
      Gtk::MessageDialog winInstruction(*this, "Vous avez perdu", false,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK);
      winInstruction.set_title("Fin du jeu");
      winInstruction.set_secondary_text("Aie attention aux bombes! Vous gagnerez surement la prochaine fois. Pour rejouer cliquez sur Nouvelle partie");
      winInstruction.run();
    }
    else if(res==-1){ //gagné
      auto duration = ((int) chrono::duration<double, std::milli> (chrono::steady_clock::now() - start).count()/10) / 100.0;
      std::ostringstream temps;
      temps << duration;
      Gtk::MessageDialog winInstruction(*this, "Vous avez Gagne", false,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_YES_NO);
      winInstruction.set_title("Fin du jeu");
      winInstruction.set_secondary_text("FELICITATIONS !!! Vous avez mis: " + temps.str() + " secondes. N hesitez pas a rejouer en augmentant la difficulte.\n\n Souhaitez-vous enregistrer votre score?");
      int reponse = winInstruction.run();
      if(reponse == Gtk::RESPONSE_YES) {
	this->score.add_score(this->Difficulte,duration,this->Bombes);
	afficherFichierScores();
      }
    }
  }
  else{//no_death_mode
    if(res==2){ //perdu en no_death_mode
      Gtk::MessageDialog winInstruction(*this, "Bombe", false,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK);
      winInstruction.set_title("No death mode");
      winInstruction.set_secondary_text("Vous avez selectionne une bombe, votre derniere action a ete annulee");
      winInstruction.run();
    }
    else if(res==3){ //gagné en no_death_mode
      bUndo.set_sensitive(false);
      bRedo.set_sensitive(false);
      bDrapeaux.set_sensitive(false);
      Gtk::MessageDialog winInstruction(*this, "Vous avez Gagne", false,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK);
      winInstruction.set_title("Fin du jeu");
      winInstruction.set_secondary_text("FELICITATIONS !!! N hesitez pas a rejouer en augmentant la difficulte.\n");
      winInstruction.run();
    }
  }
}


void VueG::close(){
  Gtk::Window::close();
}

VueG::~VueG(){
  for(auto cases: casesGrille)
    delete[] cases;
}

void VueG::afficherPremierePage() {
  //Création de la boîte de dialogue personnalisée.
  firstWindow introWindow(this);
  introWindow.resize(400,50);
  int reponse = introWindow.run(); //Lancer la boîte de dialogue.
  if(reponse==Gtk::RESPONSE_HELP){
    afficherInstructions();
  }
}

void VueG::afficherDialogue() {
  //Création de la boîte de dialogue personnalisée.
  Dialogue dialogue("Nom du joueur", this, "Veuillez entrer votre pseudo :");
  int reponse = dialogue.run(); //Lancer la boîte de dialogue.
    
    if(reponse == Gtk::RESPONSE_OK) { //Si l'utilisateur clique sur Valider.
      this->pseudo=dialogue.get_texte();
      pseudoJoueur.set_text(this->pseudo);
    }
}


void VueG::afficherInstructions(){
  Gtk::MessageDialog winInstruction(*this, "<big><b>Regles du Demineur</b></big>", true);
  winInstruction.set_title("Regles du jeu");
  winInstruction.set_secondary_text("Le champ de mines du Démineur est représenté par une grille, dont la taille varie avec la difficulte. Chaque case de la grille peut soit cacher une mine, soit etre vide. Le but du jeu est de decouvrir toutes les cases libres sans faire exploser les mines, c est-à-dire sans cliquer sur les cases qui les dissimulent.\n\nLorsque vous cliquez sur une case libre comportant au moins une mine dans l une de ses cases avoisinantes, un chiffre apparaît, indiquant ce nombre de mines.Si en revanche toutes les cases adjacentes sont vides, une case vide est affichee et la meme opération est repetee sur ces cases, et ce jusqu a ce que la zone vide soit entierement delimitee par des chiffres.\n\nVous pouvez signaler les cases contenant des mines presumees par un drapeau en cliquant sur le bouton — mais ce n est aucunement obligatoire. Il faut faire attention a ne pas signaler une case saine par un drapeau, car cela peut vous induire en erreur");
  winInstruction.run();
  }

void VueG::afficherDifficulte(){
  choiceWindow choix(this, "Difficulte","Choisissez la difficulte","Facile","Moyen","Difficile","Choisir","3D");
  choix.run();
  int difficulte, bombes;
  switch(choix.getOption()){
  case 1:
    difficulte=8;
    bombes=8;
    this->classic_game_mode=1;
    this->difficulteJeu.set_text("Difficulte: Facile");
    break;
  case 2:
    difficulte=14;
    bombes=25;
    this->classic_game_mode=1;
    this->difficulteJeu.set_text("Difficulte: Moyenne");
    break;
  case 3:
    difficulte=20;
    bombes=50;
    this->classic_game_mode=1;
    this->difficulteJeu.set_text("Difficulte: Difficile");
    break;
  case 4:{
    auto diff=afficherChoixDimensions();
    difficulte=diff.first;
    bombes=diff.second;
    this->classic_game_mode=0;
    this->difficulteJeu.set_text("Difficulte: Custom");
    break;}
  case 5:
    difficulte=5;
    bombes=1;
    this->classic_game_mode=2;
    this->difficulteJeu.set_text("Difficulte: 3D");
    break;
  }
  this->Difficulte=difficulte;
  this->Bombes=bombes;
  initialiserGrille(this->Difficulte);
  this->score = score_file("score.txt", this->pseudo,this->classic_game_mode);
}

int VueG::afficherChoixMode(){
  Gtk::MessageDialog fenetreMode(*this, "Voulez vous jouer en \"no death mode\"?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
   fenetreMode.set_title("Mode ?");
   fenetreMode.set_secondary_text("Cela signifie que vous ne pourrez pas mourir et vous ne pourrez pas sauvegarder votre score");
   int resultat = fenetreMode.run();
   if(resultat == Gtk::RESPONSE_YES) { //S'il a cliqué sur Oui.
     this->modeJeu.set_text("Mode: No death");
     return 1;
    }
   this->modeJeu.set_text("Mode: Normal");
   return 0;  
}


void VueG::afficherFichierScores(){
  std::string  fScore = (this->score.print_scores(this->Difficulte)).str();
  Gtk::MessageDialog fenetreScores(*this, "Fichier de Score", false);
  fenetreScores.set_title("Scores");
  fenetreScores.set_secondary_text(fScore);
  fenetreScores.run();    
}

std::pair<int,int> VueG::afficherChoixDimensions(){
  dimWindow fenetre_dimensions(this,"Dimensions","Nombre de bombes",5,2,18,1);
  fenetre_dimensions.run();
  return make_pair(fenetre_dimensions.get_dim(),fenetre_dimensions.get_bombes());  
}

void VueG::initialiserGrille(const int M){
  GrilleJeu.set_row_homogeneous(true);
  GrilleJeu.set_column_homogeneous(true);
  for(int i=0; i<M; i++){
    for(int j=0; j<M; j++){
      auto btn = new Mine();
      btn->set_ligne(i);
      btn->set_colonne(j);
      casesGrille.push_back(btn);
      GrilleJeu.attach(*btn,btn->get_colonne(),btn->get_ligne());
      btn->show();
    }
  }
}

void VueG::addGridListener(Controleur* c){
  for(auto wid: casesGrille){
    (*wid).signal_clicked().connect(sigc::bind(sigc::mem_fun(*c, &Controleur::on_gridBox_button),(*wid).get_ligne(),(*wid).get_colonne(),this->profondeur));
  }  
}

void VueG::addBDrapeauxListener(Controleur* c){
  bDrapeaux.signal_toggled().connect(sigc::mem_fun(*c, &Controleur::on_drapeaux_button));
}

void VueG::addBRestartListener(Controleur* c){
  bRestart.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_restart_button));
}

void VueG::addBUndoListener(Controleur* c){
  bUndo.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_undo_button));
}

void VueG::addBRedoListener(Controleur* c){
  bRedo.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_redo_button));
}

void VueG::addItemModeListener(Controleur* c){
  subItemMode.signal_activate().connect(sigc::mem_fun(*c,&Controleur::on_item_mode));
}

void VueG::resetGrille(){
  for(auto cell: casesGrille){
    GrilleJeu.remove(*cell);
  }
  casesGrille.erase(casesGrille.begin(),casesGrille.end());
  afficherDifficulte();
  this->start = chrono::steady_clock::now();
  bUndo.set_sensitive(true);
  bRedo.set_sensitive(true);
  bDrapeaux.set_sensitive(true);
}

std::vector<Mine*> VueG::get_casesGrille(){
  return this->casesGrille;
}

void VueG::changerAvatar(){
  if(it==images.end()) it=images.begin();
  avatarJoueur.set(*it++);
}
  
