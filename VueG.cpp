#include <iomanip>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "VueG.hpp"
#include "Controleur.hpp"

//constructeur
VueG::VueG():
  /*Instanciation des éléments graphiques*/
  itemMenu("Menu"),
  itemAide("Aide"),
  itemParametres("Parametres"),
  subItemFichierScores("Scores"),
  subItemQuitter(Gtk::Stock::CLOSE),
  subItemRegles(Gtk::Stock::HELP),
  subItemMode("Mode"),
  boiteMenu(false),
  boiteJeu(false),
  Grille(false),
  boiteJoueur(false),
  Jeu(false),
  boiteGrille(false),
  avatarJoueur(images[0]),
  bDrapeaux("Drapeaux"),
  bAvatar("Changer Avatar"),
  bRestart("Nouvelle Partie"),
  bPlus("+"),
  bMoins("-"),
  it(images.begin()+1)
{
  /*Association des images aux boutons undo/redo*/
  auto undo = new Gtk::Image("./Icons/undo.png");
  auto redo = new Gtk::Image("./Icons/redo.png");
  bUndo.set_image(*undo);
  bRedo.set_image(*redo);
  
  /*Affichage des informations par défaut sur la partie */
  numCouche.set_markup("<big><b><span color='red'> Etage: 1</span></b></big>");
  difficulteJeu.set_markup("<b><span color='blue'>Difficulte: facile</span></b>");
  modeJeu.set_markup("<b><span color='blue'>Mode: normal</span></b>");
  
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
  boiteJoueur.pack_end(boiteInfo,Gtk::PACK_SHRINK);
  Jeu.pack_start(boiteMenu,Gtk::PACK_SHRINK);
  boiteGrille.pack_start(grilleJeu);
  boiteGrille.pack_start(numCouche);
  Grille.pack_start(boiteJoueur);
  Grille.pack_start(boiteGrille);
  boiteMenu.pack_end(bDrapeaux,Gtk::PACK_SHRINK);
  Jeu.pack_start(Grille, Gtk::PACK_SHRINK);
  Jeu.pack_end(bRestart,Gtk::PACK_SHRINK);
  add(Jeu);

  /*Gestion des signaux internes à la classe */
  subItemQuitter.signal_activate().connect(sigc::mem_fun(*this,&VueG::close));
  subItemRegles.signal_activate().connect(sigc::mem_fun(*this,&VueG::afficherInstructions));
  subItemFichierScores.signal_activate().connect(sigc::mem_fun(*this,&VueG::afficherFichierScores));
  bAvatar.signal_clicked().connect(sigc::mem_fun(*this,&VueG::changerAvatar));

  /*Affichage des éléments graphiques*/
  show_all_children();

  /*Elements devant être invisibles au début (dépendant d'un mode de jeu) */
  bMoins.hide();
  bPlus.hide();
  numCouche.hide();
  bDrapeaux.set_sensitive(false); //on ne peut pas poser un drapeau tant qu'on a pas joué 1 fois

  /*Affichage des premières fenêtres*/
  afficherPremierePage(); //présentation du jeu
  afficherDialogue();    //saisie du pseudo
  afficherDifficulte(); //sélection de la difficulté
  
  /*Début du chrono pour le score*/
  this->start = chrono::steady_clock::now(); 
}

/*Destructeur*/
VueG::~VueG(){
  for(auto cases: casesGrille)
    delete[] cases;
}

/**
   * @Role : Actualise l'interface en fonction de l'état du modele 
   *
   * @param : info : vecteur contenant l'état de chaque case de la grille
   * @param : res : entier indiquant l'état du jeu
   *                -1 gagné | 0 perdu | 1 toujours en cours | 2 perdu en no death mode | 3 gagné en no death mode 
   * @retval: none
*/
void VueG::update(std::vector<std::string> &info, int& res){
  auto grille=this->casesGrille;
  for(auto i=0; i<(int)info.size(); i++){
    auto img = new Gtk::Image("./Icons/"+info[i]+".png"); //on update l'affichage de chaque case
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
    /*Les boutons ne sont plus accessibles*/
    bUndo.set_sensitive(false);
    bRedo.set_sensitive(false);
    bDrapeaux.set_sensitive(false);
    bMoins.set_sensitive(false);
    bPlus.set_sensitive(false);
    if(res == 0){ //si le jeu est perdu
      message("Vous avez perdu","Fin du jeu","Aie attention aux bombes! Vous gagnerez surement la prochaine fois. Pour rejouer cliquez sur Nouvelle partie"); //fenêtre adaptée
    }
    else if(res==-1){ //si le jeu est gagné
      if(enregistrer_score){ //si le joueur n'a pas sélectionné no_death_mode durant la partie
	auto duration = ((int) chrono::duration<double, std::milli> (chrono::steady_clock::now() - start).count()/10) / 100.0; //on récupère son chrono
	std::ostringstream temps;
	temps << duration;
	message("Vous avez Gagne","Fin du jeu","FELICITATIONS !!! Vous avez mis: " + temps.str() + " secondes.N hesitez pas a rejouer en augmentant la difficulte.\nVotre score a été pris en compte"); //fenêtre adpatée
	this->score.add_score(this->Difficulte,duration,this->Bombes); //on enregistre le score
	afficherFichierScores(); //on affiche les 10 meilleurs scores de cette difficulté
      }
      else{ //le joueur est passé en no_death_mode durant la partie, il ne peut pas enregistrer son score
	message("Vous avez Gagne","Fin du jeu","FELICITATIONS !!! N hesitez pas a rejouer en augmentant la difficulte.\n"); //fenêtre adaptée
      }
    }
  }
  else{//le jeu est fini en no_death_mode
    if(res==2){ //perdu en no_death_mode
      message("Bombe","No death mode","Vous avez selectionne une bombe, votre derniere action a ete annulee");
    }
    else if(res==3){ //gagné en no_death_mode
      bUndo.set_sensitive(false);
      bRedo.set_sensitive(false);
      bDrapeaux.set_sensitive(false);
      message("Vous avez Gagne","Fin du jeu","FELICITATIONS !!! N hesitez pas a rejouer en augmentant la difficulte.\n");
      
    }
  }
}

/**
   * @Role : Ferme la fenêtre active 
   *
   * @param : none 
   * @retval: none
*/
void VueG::close(){
  Gtk::Window::close();
}


/**
   * @Role : Affiche une fenêtre de type firstWindow de présentation du jeu     
   *
   * @param : none 
   * @retval: none
*/
void VueG::afficherPremierePage() {
  //Création de la fenêtre
  firstWindow introWindow(this);
  introWindow.resize(400,50);
  int reponse = introWindow.run();
  if(reponse==Gtk::RESPONSE_HELP){ //si le joueur clique sur le bouton "Instruction"
    afficherInstructions();//affichage des règles du Demineur dans une fenêtre qui s'ouvre
   
  }
}


/**
   * @Role : Affiche une fenêtre de type dialogue de saisie du pseudo     
   *
   * @param : none 
   * @retval: none
*/
void VueG::afficherDialogue() {
  //Création de la boîte de dialogue
  Dialogue dialogue("Nom du joueur", this, "Veuillez entrer votre pseudo :");
  int reponse = dialogue.run();
  if(reponse == Gtk::RESPONSE_OK) { //Si l'utilisateur clique sur Valider.
    this->pseudo=dialogue.get_texte(); //on enregistre son pseudo
    pseudoJoueur.set_text(this->pseudo); //on l'affiche sous l'avatar
  }
}


/**
   * @Role : Affiche la fenêtre de type MessageDialog présentant les règles du jeu     
   *
   * @param : none 
   * @retval: none
*/
void VueG::afficherInstructions(){
   message("Regles du Demineur","Regles du jeu","Le champ de mines du Démineur est représenté par une grille, dont la taille varie avec la difficulte. Chaque case de la grille peut soit cacher une mine, soit etre vide. Le but du jeu est de decouvrir toutes les cases libres sans faire exploser les mines, c est-à-dire sans cliquer sur les cases qui les dissimulent.\n\nLorsque vous cliquez sur une case libre comportant au moins une mine dans l une de ses cases avoisinantes, un chiffre apparaît, indiquant ce nombre de mines.Si en revanche toutes les cases adjacentes sont vides, une case vide est affichee et la meme opération est repetee sur ces cases, et ce jusqu a ce que la zone vide soit entierement delimitee par des chiffres.\n\nVous pouvez signaler les cases contenant des mines presumees par un drapeau en cliquant sur le bouton — mais ce n est aucunement obligatoire. Il faut faire attention a ne pas signaler une case saine par un drapeau, car cela peut vous induire en erreur");
}


/**
   * @Role : Affiche une fenêtre de type choiceWindow pour sélectionner la difficulté     
   *
   * @param : none 
   * @retval: none
*/
void VueG::afficherDifficulte(){
  //Création de la fenêtre de choix
  choiceWindow choix(this, "Difficulte","Choisissez la difficulte","Facile","Moyen","Difficile","Choisir","3D");
  choix.run();
  /*Par défaut les éléments graphiques relatant à la difficulté 3D sont cachés*/
  bMoins.hide();
  bPlus.hide();
  numCouche.hide();
  
  int difficulte, bombes;
  //on récupère le choix du joueur
  switch(choix.getOption()){
  case 1: //le joueur a sélectionné Facile
    difficulte=8; //grille de 8x8
    bombes=8;
    this->classic_game_mode=1; //mode normal
    this->setDifficulteJeu("facile"); //on affiche la difficulté choisie
    initialiserGrille(difficulte); //on initialise la grille aux bonnes dimensions
    break;
  case 2: //le joueur a sélectionné Moyenne
    difficulte=14; //grille de 14x14
    bombes=25;
    this->classic_game_mode=1;
    this->setDifficulteJeu("moyenne");
    initialiserGrille(difficulte);
    break;
  case 3: //le joueur a sélectionné Difficile
    difficulte=20; //grille de 20x20
    bombes=50;
    this->classic_game_mode=1;
    this->setDifficulteJeu("difficile");
    initialiserGrille(difficulte);
    break;
  case 4:{ //le joueur a sélectionné Choisir
    auto diff=afficherChoixDimensions(); //on affiche une fenetre qui permet de choisir le nombre de bombes et la dimension
    /*On enregistre les paramètres*/
    difficulte=diff.first;
    bombes=diff.second;
    this->classic_game_mode=0; //mode custom
    this->setDifficulteJeu("custom");
    initialiserGrille(difficulte);
    break;}
  case 5: //le joueur a sélectionné 3D
    difficulte=100; //par défaut, la difficulté est mise à 100 bien que la grille soit de dimensions 5x5
    bombes=1; //une seule bombe max par couche
    this->classic_game_mode=2; //mode 3D
    this->setDifficulteJeu("3D");
    this->setNumCouche(profondeur); //on affiche la couche actuelle
    initialiserGrille(5);
    /*Affichage des éléments graphiques relatifs à la 3D*/
    bMoins.show();
    bPlus.show();
    bMoins.set_sensitive(true);
    bPlus.set_sensitive(true);
    numCouche.show();
    message("Explications 3D","3D","Chaque case a desormais 26 voisins! 8 sur le meme etage et 9 sur l'etage du dessus et du dessous. Deplacez-vous entre les étages de la grille en utilisant les boutons + et -. Derniere information importante, il n'y a qu'une seule bombe par etage !\n");
    break;
  }
  /*On enregistre les paramètres choisis par le joueur*/
  this->Difficulte=difficulte;
  this->Bombes=bombes;
  /*On initialise le fichier des scores avec les infos de la partie*/
  this->score = score_file("score.txt", this->pseudo,this->classic_game_mode);
}


/**
   * @Role : Affiche une fenêtre de type MessageDialog pour sélectionner le mode No death     
   *
   * @param : none 
   * @retval: 1 si le joueur sélectionne le no death mode
   *          0 sinon
*/
int VueG::afficherChoixMode(){
  //Création de la fenêtre
  Gtk::MessageDialog fenetreMode(*this, "Voulez vous jouer en \"no death mode\"?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
   fenetreMode.set_title("Mode ?");
   fenetreMode.set_secondary_text("Cela signifie que vous ne pourrez pas mourir et vous ne pourrez pas sauvegarder votre score");
   int resultat = fenetreMode.run();
   if(resultat == Gtk::RESPONSE_YES) { //Si le joueur clique sur yes
     enregistrer_score=0; //le joueur ne pourra plus enregistrer son score car il ne peut pas mourir
     this->setModeJeu("no death"); //on affiche le nouveau mode
     return 1;
    }
   setModeJeu("normal"); //si le joueur clique sur no le mode est normal
   return 0;  
}


/**
   * @Role : Affiche une fenêtre de type MessageDialog avec les 10 meilleurs scores enregistrés sur la même difficulté     
   *
   * @param : none 
   * @retval: none
*/
void VueG::afficherFichierScores(){
  std::string  fScore = (this->score.print_scores(this->Difficulte)).str();
  Gtk::MessageDialog fenetreScores(*this, "Fichier de Score", false);
  fenetreScores.set_title("Scores");
  fenetreScores.set_secondary_text(fScore);
  fenetreScores.run();    
}


/**
   * @Role : Affiche une fenêtre de type MessageDialog personnalisable     
   *
   * @param:  message: En-tête du message à afficher
   * @param:  titre: Titre de la fenêtre
   * @param:  sous_message: Corps du texte
   * @retval: none
*/
void VueG::message(std::string message, std::string titre, std::string sous_message){
  Gtk::MessageDialog fenetre(*this, message, false,Gtk::MESSAGE_INFO,Gtk::BUTTONS_OK);
  fenetre.set_title(titre);
  fenetre.set_secondary_text(sous_message);
  fenetre.run();
  
}


/**
   * @Role : Affiche une fenêtre de type dimWindow pour personnaliser la dimension et le nombre de bombes   
   *
   * @param : none 
   * @retval: none
*/
std::pair<int,int> VueG::afficherChoixDimensions(){
  dimWindow fenetre_dimensions(this,"Dimensions","Nombre de bombes",5,2,20,1);
  fenetre_dimensions.run();
  return make_pair(fenetre_dimensions.get_dim(),fenetre_dimensions.get_bombes());  
}


/**
   * @Role : Initialiser la grille de jeu     
   *
   * @param : M: Dimension de la grille 
   * @retval: none
*/
void VueG::initialiserGrille(const int M){
  //toutes les cases ont la même taille
  grilleJeu.set_row_homogeneous(true);
  grilleJeu.set_column_homogeneous(true);
  for(int i=0; i<M; i++){
    for(int j=0; j<M; j++){ //pour chaque emplacement dans la grille
      auto btn = new Case(); //on crée la case
      btn->set_ligne(i);    //on lui attribue un index de ligne  
      btn->set_colonne(j);  //et une colonne
      
      /*Gestion de l'affichage en couleur des cases*/
      Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
      css_provider->load_from_data(     "button {background-image:image(#F4F4F4) ;}\
                                         button:hover {background-image: image(PowderBlue);}\
                                         button:disabled {background-image: image(Gainsboro);}");
      
      btn->get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
      
      casesGrille.push_back(btn);//on ajoute la case au vecteur
      grilleJeu.attach(*btn,btn->get_colonne(),btn->get_ligne()); //on ajoute la case à la grille
      btn->show();
    }
  }
}


/**
   * @Role : Lie le clique sur chaque case de la grille à la fonction de rappel on_gridBox_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addGridListener(Controleur* c){
  for(auto wid: casesGrille){
    (*wid).signal_clicked().connect(sigc::bind(sigc::mem_fun(*c, &Controleur::on_gridBox_button),(*wid).get_ligne(),(*wid).get_colonne())); //on passe en paramètre la ligne et la colonne du bouton cliqué
  }  
}


/**
   * @Role : Lie le clique sur le bouton Drapeau à la fonction de rappel on_drapeaux_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addBDrapeauxListener(Controleur* c){
  bDrapeaux.signal_toggled().connect(sigc::mem_fun(*c, &Controleur::on_drapeaux_button));
}


/**
   * @Role : Lie le clique sur le bouton Restart à la fonction de rappel on_restart_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addBRestartListener(Controleur* c){
  bRestart.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_restart_button));
}


/**
   * @Role : Lie le clique sur le bouton Undo à la fonction de rappel on_undo_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addBUndoListener(Controleur* c){
  bUndo.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_undo_button));
}


/**
   * @Role : Lie le clique sur le bouton Redo à la fonction de rappel on_redo_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addBRedoListener(Controleur* c){
  bRedo.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_redo_button));
}


/**
   * @Role : Lie le clique sur chaque l'item Mode du Menu Paramètre à la fonction de rappel on_item_mode du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addItemModeListener(Controleur* c){
  subItemMode.signal_activate().connect(sigc::mem_fun(*c,&Controleur::on_item_mode));
}


/**
   * @Role : Lie le clique sur le bouton - à la fonction de rappel on_moins_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addBMoinsListener(Controleur* c){
  bMoins.signal_clicked().connect(sigc::mem_fun(*c,&Controleur::on_moins_button));
}


/**
   * @Role : Lie le clique sur le bouton + à la fonction de rappel on_plus_button du controleur
   *
   * @param : c: pointeur sur controleur 
   * @retval: none
*/
void VueG::addBPlusListener(Controleur* c){
  bPlus.signal_clicked().connect(sigc::mem_fun(*c,&Controleur::on_plus_button));
}


/**
   * @Role : Réinitialise le jeu pour une nouvelle partie
   *
   * @param : none
   * @retval: none
*/
void VueG::resetGrille(){
  for(auto cell: casesGrille){
    grilleJeu.remove(*cell); //on supprime chaque case de la grille
  }
  casesGrille.erase(casesGrille.begin(),casesGrille.end()); //on supprime chaque case du vecteur
  afficherDifficulte(); //on demande de nouveau de choisir la difficulté
  this->start = chrono::steady_clock::now(); //on relance le chrono
  
  bUndo.set_sensitive(true); //les boutons undo/redo peuvent de nouveau être utilisés
  bRedo.set_sensitive(true);
  bDrapeaux.set_sensitive(false); //en début de partie le bouton drapeau n'est pas utilisable
}


/**
   * @Role : Change l'image de l'avatar du joueur
   *
   * @param : none 
   * @retval: none
*/
void VueG::changerAvatar(){
  if(it==images.end()) it=images.begin(); //si on est arrivé à la dernière image on revient à celle du début
  avatarJoueur.set(*it++); //on affiche l'image suivante;
}


/**
   * @Role  Affiche la couche courante
   *
   * @param  c: numéro de la couche courante
   * @retval none
   */
void VueG::setNumCouche(int c){
  this->numCouche.set_markup("<big><b><span color='red'>Etage: "+ std::to_string(c)+"</span></b></big>");
}


/**
   * @Role  Affiche la difficulté courante
   *
   * @param  d: difficulté courante
   * @retval none
   */
void VueG::setDifficulteJeu(std::string d){
  this->difficulteJeu.set_markup("<b><span color='blue'>Difficulte: "+d+"</span></b>");
}


/**
   * @Role  Affiche le mode courant
   *
   * @param  m: mode courant
   * @retval none
   */
void VueG::setModeJeu(std::string m){
  this->modeJeu.set_markup("<b><span color='blue'>Mode: "+m+"</span></b>");
}


/**
   * @Role  Rend le bouton Drapeaux cliquable
   *
   * @param  none
   * @retval none
   */
void VueG::setDrapeauxActif(){
  this->bDrapeaux.set_sensitive(true);
}
