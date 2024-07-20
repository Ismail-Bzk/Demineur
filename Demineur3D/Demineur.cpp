#include "Demineur.h"
using namespace std;
#include <iomanip>

Type_history test;


Demineur::Demineur(){}

/* configDemineur  -> instancie le vecteur qui compose la grille
    -> parametre : la difficulte choisit par le joueur, et le nombre de bombe s'il decide de choisir le nombre de case sur sa grille
*/
void Demineur::configDemineur(const int difficulte,const int mon_nb_bombe){
    this->difficulte_ = difficulte;
    this->nb_bombes_ = mon_nb_bombe;
    if (this->nb_bombes_ == 0 ){
        this->nb_bombes_ = difficulte_;
    }

    this->profondeur_ = 1;
    if (this->difficulte_ == Trois_D ){
        this -> profondeur_ = 5;
        this -> nb_bombes_ = 5;
        this -> difficulte_ = 5;
    }

    this->Mon_jeu_.resize(profondeur_+2);

    for ( int i = 0; i < profondeur_ +2; ++i ){
        this->Mon_jeu_[i].resize(difficulte_+2);
        for ( int j = 0; j < difficulte_+2; ++j ){
            this->Mon_jeu_[i][j].resize(difficulte_+2);
        }
    }
}


Demineur::~Demineur()
{
    //dtor
}

/* poser_bombe  -> choisit aleatoirement l'emplacement des bombes dans la grille
    -> parametre : la position x, y, z du premier coup du joueur afin de ne pas commencer la partie sur une bombe
*/
void Demineur::poser_bombe(const int x, const int y, const int z){

    for(int p=0 ; p<profondeur_+2 ; ++p){
        for(int i=0 ; i<difficulte_+2 ; ++i){
            for(int j=0 ; j<difficulte_+2 ; ++j){
                    Mon_jeu_[p][i][j].nb_bombe_=0;
            }
        }
    }

    int bomb_x, bomb_y, bomb_z =0;
    int nb_bombes = nb_bombes_;

    for (int i=1 ; i <= nb_bombes; ++i){                                         //Choix des coordonnées des bombes

        srand(time(NULL)+nb_bombes+i);                                          //en plus de time(NULL) on lui donne i et nb_bombes car ils evoluent plus rapidement que time(NULL)

        bomb_x = ((rand() % difficulte_)+1);
        bomb_y = ((rand() % difficulte_)+1);
        if (profondeur_ == 1){
            bomb_z = 1 - i ;
        }

        //cout <<  "z : " << bomb_z + i << " | x : " << bomb_x << " | y : "<< bomb_y <<endl;

        if((Mon_jeu_[bomb_z+i][bomb_x][bomb_y].nb_bombe_ < je_suis_une_bombe) & !((bomb_x == (x+1)) & (bomb_y == (y+1)) & (i == z+1 ))){

          Mon_jeu_[bomb_z+i][bomb_x][bomb_y].nb_bombe_ = je_suis_une_bombe;

          for(int p = bomb_z+i-1; p<=bomb_z+i+1;++p){
              for(int l=bomb_x-1; l<=bomb_x+1;++l){
                for(int c=bomb_y-1; c<=bomb_y+1;++c){
                    ++Mon_jeu_[p][l][c].nb_bombe_;
                }
              }
          }
        }
        else{    // si la case est deja une bombe, on annule l'iteration
            ++nb_bombes;
        }
    }
}


void Demineur::affichage(const int profondeur) {
  std::vector<std::string> valeur;
    for (int ligne=1 ; ligne <= difficulte_ ; ++ligne) {
        for (int colonne=1 ; colonne <= difficulte_ ; ++colonne) {
            if (Mon_jeu_[profondeur][ligne][colonne].etat_ == NON_DECOUVERTE ) {
          valeur.push_back("n");
            }
            else if (Mon_jeu_[profondeur][ligne][colonne].etat_ == DRAPEAU ) {
          valeur.push_back("flag");
            }
            else if (Mon_jeu_[profondeur][ligne][colonne].etat_== DECOUVERTE) {
                if (Mon_jeu_[profondeur][ligne][colonne].nb_bombe_ >= je_suis_une_bombe) {
          valeur.push_back("mine");
                    }
                else{
          valeur.push_back(to_string(Mon_jeu_[profondeur][ligne][colonne].nb_bombe_));
        }
            }
        }
    }
    //si le joueur ne peut pas mourir et qu'il a cliqué sur une bombe
    if((!partie) && (no_death_mode)){
      notifierObservateurs(valeur,2);
    }
    //si le mode de jeu est normal
    else{
      notifierObservateurs(valeur,1);
    }
}


#ifdef terminal
void Demineur::affichage(const int profondeur) {
    std::string valeur;
    cout << "---------------------- couche "<<profondeur<<"-----------------"<<endl;
    cout <<"    ";
    for (int i=0 ; i < difficulte_ ; ++i) {
        cout <<" "<< setw(3) << i << "  ";
    }
    cout<< endl;
    cout << "     ";
    for (int i=0 ; i < difficulte_ ; i++){
        cout << "------";
    }
    cout <<endl;
    for (int ligne=1 ; ligne <= difficulte_ ; ++ligne) {
        cout << setw(2)<<ligne-1<<"  |";
        for (int colonne=1 ; colonne <= difficulte_ ; ++colonne) {
            if (Mon_jeu_[profondeur][ligne][colonne].etat_ == NON_DECOUVERTE ) {
                valeur = "#";
            }
            else if (Mon_jeu_[profondeur][ligne][colonne].etat_ == DRAPEAU ) {
                valeur = "F";
            }
            else if (Mon_jeu_[profondeur][ligne][colonne].etat_== DECOUVERTE) {
                if (Mon_jeu_[profondeur][ligne][colonne].nb_bombe_ >= je_suis_une_bombe) {
                        valeur = "-";
                    }
                else{
                    valeur = to_string(Mon_jeu_[profondeur][ligne][colonne].nb_bombe_);
                    if (valeur == "0")
                      valeur=" ";
                }
            }
            cout << setw(3) << valeur << "  |";
        }
        cout << endl;
        cout << "     ";
        for (int i=0 ; i < difficulte_ ; i++){
            cout << "------";
        }
        cout <<endl;
    }
}
#endif


void Demineur::affichage_final(const int profondeur) {
  game_history_.reset();
  std::vector<std::string> valeur;

    for (int ligne=1 ; ligne <= difficulte_ ; ++ligne) {
        for (int colonne=1 ; colonne <= difficulte_ ; ++colonne) {
            if (Mon_jeu_[profondeur][ligne][colonne].nb_bombe_ >= je_suis_une_bombe) {
	      valeur.push_back("mine");
                }
            else if ((Mon_jeu_[profondeur][ligne][colonne].nb_bombe_ < je_suis_une_bombe) & (Mon_jeu_[profondeur][ligne][colonne].nb_bombe_ >= 0)) {
	      valeur.push_back(to_string(Mon_jeu_[profondeur][ligne][colonne].nb_bombe_));
            }
        }
    }

    if(partie==0){  //renvoie status perdant
        notifierObservateurs(valeur,0);
    }
    else{
        if(no_death_mode){
            notifierObservateurs(valeur,3);
        }
        else{  //renvoie status gagnant
            notifierObservateurs(valeur,-1);
        }
    }
}




int Demineur::jouer(const int x, const int y,const int z){           //ATTENTION lA PROFONDEUR VA DE 1 A 5 donc z doit appartenir a cette interval
    if (Mon_jeu_[z][x+1][y+1].nb_bombe_ >= je_suis_une_bombe){
        return 0;
    }
     else if (Mon_jeu_[z][x+1][y+1].nb_bombe_ != 0) {
        Mon_jeu_[z][x+1][y+1].etat_ = DECOUVERTE;


        test.position = make_pair(x+1,y+1);
        test.position_profondeur_ = z;
        test.etat_=DECOUVERTE;
        game_history_.add_modif(test);

        ++nb_case_decouvert_;


    }
    else{
        decouverte(x+1,y+1,z);
    }

    ++game_history_.tour_actuelle;
    ++game_history_.tour_total;


    if(nb_case_decouvert_ < difficulte_*difficulte_*profondeur_ - nb_bombes_ )
        return 1;
    else
        return -1;
}

void Demineur::decouverte (const int x, const int y, const int z){
   if ((Mon_jeu_[z][x][y].etat_ == NON_DECOUVERTE)&(Mon_jeu_[z][x][y].nb_bombe_ < je_suis_une_bombe )){    //Les regles font que meme en 3D il n'y a qu'une seule bombe par etage donc une cellule peut avoir au max 3 bombes en 3D
       Mon_jeu_[z][x][y].etat_ = DECOUVERTE;

       test.position = make_pair(x,y);
       test.position_profondeur_ = z;
       test.etat_=DECOUVERTE;
       game_history_.add_modif(test);

       ++nb_case_decouvert_;
    }
    for(int p=z-1; p<=z+1;++p){
        for(int i = x-1; i <= x+1 ; ++i){
            for(int j = y-1; j <= y+1; ++j){
                if ( !((x == i) & (y == j) & (z == p)) & (Mon_jeu_[p][i][j].etat_ == NON_DECOUVERTE)){
                    if ((Mon_jeu_[p][i][j].nb_bombe_ == 0) & (p > 0) & (i > 0) & (j > 0) & (i < difficulte_ +1) & (j < difficulte_ +1) & (p < profondeur_ +1))
                        decouverte(i,j,p);

                    else if (Mon_jeu_[p][i][j].nb_bombe_ < je_suis_une_bombe ){
                        Mon_jeu_[p][i][j].etat_ = DECOUVERTE;
                        if ((i > 0) & (j > 0) & (p > 0) & (i < difficulte_ +1) & (j < difficulte_ +1) & (p < profondeur_ +1 )){
                            test.position = make_pair(i,j);
                            test.position_profondeur_ = p;
                            test.etat_=DECOUVERTE;
                            game_history_.add_modif(test);

                            ++nb_case_decouvert_;
                        }
                    }
                }
            }
        }
    }
}





int Demineur::drapeau(const int x, const int y, const int z){
    if (Mon_jeu_[z][x+1][y+1].etat_ == DRAPEAU){
        Mon_jeu_[z][x+1][y+1].etat_ = NON_DECOUVERTE;
        return 1;
    }
    else if (Mon_jeu_[z][x+1][y+1].etat_ == DECOUVERTE){
        return 4;
    }
    Mon_jeu_[z][x+1][y+1].etat_ = DRAPEAU;

    test.position = make_pair(x+1,y+1);
    test.position_profondeur_ =z;
    test.etat_=DRAPEAU;
    game_history_.add_modif(test);

    ++game_history_.tour_actuelle;
    ++game_history_.tour_total;

    return 1;

};

void Demineur::undo(){
    if (game_history_.tour_actuelle > 0){
        game_history_.Undo_Mode=1;
        --game_history_.tour_actuelle;
        for(auto e : game_history_.history_[game_history_.tour_actuelle]){
            --nb_case_decouvert_;
            if (e.etat_ == NON_DECOUVERTE)
                Mon_jeu_[e.position_profondeur_][e.position.first][e.position.second].etat_ = DECOUVERTE;
            else
                Mon_jeu_[e.position_profondeur_][e.position.first][e.position.second].etat_ = NON_DECOUVERTE;
        }
    }
};

void Demineur::redo(){
    if (game_history_.tour_actuelle < game_history_.tour_total){
        for(auto e : game_history_.history_[game_history_.tour_actuelle]){
            ++nb_case_decouvert_;
            Mon_jeu_[e.position_profondeur_][e.position.first][e.position.second].etat_ = e.etat_;
        }
        ++game_history_.tour_actuelle;
    }
};



void Demineur::update(const int x, const int y, const int z){
  if(!this->drapeaux){  //si le joueur joue
    if(Mon_jeu_[z][x+1][y+1].etat_ !=DRAPEAU){
      partie = this->jouer(x,y,z);
      if((partie!=-1) and (partie!=0)){//si le jouer n'a ni gagné(-1) ni perdu(0)
	this->affichage(z); //on update la grille
      }
      else{  //si fin du jeu
	//si le joueur a perdu mais est en mode no_death_mode
	if(no_death_mode && !partie){
	  this->affichage(z); //on affiche la grille avant le coup perdant
	}
	//si le joueur a gagné
	else{
	  this->affichage_final(z);
	  partie=5;
	}
      }
    }
  }
    else{ //si le joueur place des drapeaux
      partie = this->drapeau(x,y,z);
      this->affichage(z);
    }
};


void Demineur::restart(){
  (this->Mon_jeu_).erase(this->Mon_jeu_.begin(),this->Mon_jeu_.end());
  this->nb_case_decouvert_=0;
  this->game_history_.reset();

};



