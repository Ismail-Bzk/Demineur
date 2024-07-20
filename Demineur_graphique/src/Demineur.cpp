#include "Demineur.h"
using namespace std;
#include <iomanip>

Type_history test;


Demineur::Demineur(){}

void Demineur::configDemineur(const int difficulte,const int mon_nb_bombe){
  this->difficulte_=difficulte;
  this->nb_bombes_=mon_nb_bombe;
  if (this->nb_bombes_ == 0 ){
    this->nb_bombes_ = difficulte_;
  }
  this->Mon_jeu_.resize(difficulte+2);
  for (int i = 0; i < difficulte+2; ++i )
   this-> Mon_jeu_[i].resize(difficulte+2);

}


Demineur::~Demineur()
{
    //dtor
}


void Demineur::poser_bombe(const int x, const int y){
    int bomb_x, bomb_y;
    int nb_bombes = nb_bombes_;
    for (int i=0 ; i < nb_bombes; ++i){                                         //Choix des coordonnées des 10 bombes

        srand(time(NULL)+nb_bombes+i);

        bomb_x = ((rand() % difficulte_)+1);
        bomb_y = ((rand() % difficulte_)+1);
        if((Mon_jeu_[bomb_x][bomb_y].nb_bombe_ < je_suis_une_bombe) & !((bomb_x == (x+1)) & (bomb_y == (y+1)))){
          Mon_jeu_[bomb_x][bomb_y].nb_bombe_ = je_suis_une_bombe;
          for(int i=bomb_x-1; i<=bomb_x+1;++i){
            for(int j=bomb_y-1; j<=bomb_y+1;++j){
                ++Mon_jeu_[i][j].nb_bombe_;
            }
          }
        }
        else{                                                                       // si la case est deja une bombe, on annule l'iteration
            ++nb_bombes;
        }
    }
}

void Demineur::affichage() {
    std::string valeur;
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
            if (Mon_jeu_[ligne][colonne].etat_ == NON_DECOUVERTE ) {
                valeur = "#";
            }
            else if (Mon_jeu_[ligne][colonne].etat_ == DRAPEAU ) {
                valeur = "F";
            }
            else if (Mon_jeu_[ligne][colonne].etat_== DECOUVERTE) {
                if (Mon_jeu_[ligne][colonne].nb_bombe_ >= je_suis_une_bombe) {
                        valeur = "-";
                    }
                else{
                    valeur = to_string(Mon_jeu_[ligne][colonne].nb_bombe_);
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

void Demineur::affichage_final() {
    std::string valeur;

    cout <<"  ";
    for (int i=0 ; i < difficulte_ ; ++i) {
        cout << " "<< i ;
    }
    cout<< endl<< endl;
    for (int ligne=1 ; ligne <= difficulte_ ; ++ligne) {
        cout << ligne-1<<"  ";
        for (int colonne=1 ; colonne <= difficulte_ ; ++colonne) {
            valeur=" ";
            if (Mon_jeu_[ligne][colonne].nb_bombe_ >= je_suis_une_bombe) {
                    valeur = "-";
                }
            else if ((Mon_jeu_[ligne][colonne].nb_bombe_ < je_suis_une_bombe) & (Mon_jeu_[ligne][colonne].nb_bombe_ > 0)) {
                    valeur = to_string(Mon_jeu_[ligne][colonne].nb_bombe_);
            }
            cout << valeur << " ";
        }
        cout << endl;
    }
}

int Demineur::jouer(const int x, const int y){
    if (Mon_jeu_[x+1][y+1].nb_bombe_ >= je_suis_une_bombe){
        Mon_jeu_[x+1][y+1].etat_ = DECOUVERTE;
        return 0;
    }
    else if (Mon_jeu_[x+1][y+1].etat_ == DECOUVERTE)
        return 2;

    else if(Mon_jeu_[x+1][y+1].etat_ == DRAPEAU)
        return 3;

    else if (Mon_jeu_[x+1][y+1].nb_bombe_ != 0) {
        Mon_jeu_[x+1][y+1].etat_ = DECOUVERTE;

        test.position = make_pair(x+1,y+1);
        test.etat_=DECOUVERTE;
        game_history_.add_modif(test);

        ++nb_case_decouvert_;


    }
    else{
        decouverte(x+1,y+1);
    }

    ++game_history_.tour_actuelle;
    ++game_history_.tour_total;

    if(nb_case_decouvert_ < difficulte_*difficulte_ - nb_bombes_ )
        return 1;
    else
        return -1;
}

int Demineur::drapeau(const int x, const int y){
    if (Mon_jeu_[x+1][y+1].etat_ == DRAPEAU){
        Mon_jeu_[x+1][y+1].etat_ = NON_DECOUVERTE;
        return 1;
    }
    else if (Mon_jeu_[x+1][y+1].etat_ == DECOUVERTE){
        return 4;
    }
    Mon_jeu_[x+1][y+1].etat_ = DRAPEAU;

    test.position = make_pair(x+1,y+1);
    test.etat_=DRAPEAU;
    game_history_.add_modif(test);

    ++game_history_.tour_actuelle;
    ++game_history_.tour_total;


    return 1;

};


void Demineur::decouverte (const int x, const int y){
   if ((Mon_jeu_[x][y].etat_ == NON_DECOUVERTE)&(Mon_jeu_[x][y].nb_bombe_ < 9 )){
       Mon_jeu_[x][y].etat_ = DECOUVERTE;
       test.position = make_pair(x,y);
       test.etat_=DECOUVERTE;
       game_history_.add_modif(test);
       ++nb_case_decouvert_;
   }

   for(int i = x-1; i <= x+1 ; ++i){
     for(int j = y-1; j <= y+1; ++j){

       if ( !((x == i) & (y == j)) & (Mon_jeu_[i][j].etat_ == NON_DECOUVERTE)){

            if ((Mon_jeu_[i][j].nb_bombe_ == 0) & (i > 0) & (j > 0) & (i < difficulte_ +1) & (j < difficulte_ +1))
             decouverte(i,j);

            else if (Mon_jeu_[i][j].nb_bombe_ <9 ){
             Mon_jeu_[i][j].etat_ = DECOUVERTE;
             if ((i > 0) & (j > 0) & (i < difficulte_ +1) & (j < difficulte_ +1)){
               test.position = make_pair(i,j);
               test.etat_=DECOUVERTE;
               game_history_.add_modif(test);
               ++nb_case_decouvert_;
             }
        }
       }
     }
   }
}


/*
void Demineur::update(const int x, const int y){
  if(!this->drapeaux){  //si le joueur joue
      partie = this->jouer(x,y);
      if((partie!=-1) and (partie!=0)){//si le jouer n'a ni gagné(-1) ni perdu(0)
	this->affichage(); //on update la grille
      }
      else{  //si fin du jeu
	//si le joueur a perdu mais est en mode no_death_mode
	if(no_death_mode && !partie){
	  this->affichage(); //on affiche la grille avant le coup perdant
	}
	//si le joueur a gagné
	else{
	  this->affichage_final();
	}
      }
    }
    else{ //si le joueur place des drapeaux
      partie = this->drapeau(x,y);
      this->affichage();
    }
};
*/

void Demineur::restart(){
  (this->Mon_jeu_).erase(this->Mon_jeu_.begin(),this->Mon_jeu_.end());
  this->nb_case_decouvert_=0;
  //configDemineur(this->difficulte_,this->nb_bombes_);

};

void Demineur::undo(){
    if (game_history_.tour_actuelle > 0){
        game_history_.Undo_Mode=1;
        --game_history_.tour_actuelle;
        for(auto e : game_history_.history_[game_history_.tour_actuelle]){
            --nb_case_decouvert_;
            if (e.etat_ == NON_DECOUVERTE)
                Mon_jeu_[e.position.first][e.position.second].etat_ = DECOUVERTE;
            else
                Mon_jeu_[e.position.first][e.position.second].etat_ = NON_DECOUVERTE;
        }
    }
};

void Demineur::redo(){
    if (game_history_.tour_actuelle < game_history_.tour_total){
        for(auto e : game_history_.history_[game_history_.tour_actuelle]){
            ++nb_case_decouvert_;
            Mon_jeu_[e.position.first][e.position.second].etat_ = e.etat_;
        }
        ++game_history_.tour_actuelle;
    }
};
