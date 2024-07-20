#include "score_file.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;


score_file::score_file(){}


/**
	* @Role  : Constructeur, permet d'initialiser tous les elements liee a la gestion, l'affichage et l'enregistrement des scores.
	*          Il permet de recuperer toutes les donnees du fichier des scores et de les stocker dans des vecteurs.
	*/
score_file::score_file(string file_name, string player_name, const int classic_game_mode) : file_name_(file_name),player_name_(player_name),classic_game_mode_(classic_game_mode){
  string line,word;
  type_player player_score;
  ifstream file (file_name_, ios::in);
  if(file.is_open()){
		/* Recupere le nb de joueur FACILE */
    getline(file,line);

    stringstream str(line);
    getline(str, word, ':');
    getline(str, word, '\n');
    nb_of_player_scored_easy=stoi(word.c_str());
    /* On Recupere le nom des joueur/score/chrono FACILE */
    for (int i=0; i< nb_of_player_scored_easy;++i){
      getline(file,line);
      str.clear();
      str.str("");
      str<<line;
      getline(str, word,':');
      player_score.name_ = word.c_str();
      getline(str, word, ':');
      player_score.difficulte_ = stoi(word.c_str());
      getline(str, word, '\n');
      player_score.chrono_ = stod(word.c_str());
      list_of_easy_player.push_back(player_score);
    }

    /* Recupere le nb de joueur MOYEN */
    getline(file,line);
    str.clear();
    str.str("");
    str<<line;
    getline(str, word, ':');
    getline(str, word, '\n');
    nb_of_player_scored_medium=stoi(word.c_str());

    /* On Recupere le nom des joueur/score/chrono MOYEN */
    for (int i=0; i< nb_of_player_scored_medium;++i){
      getline(file,line);
      str.clear();
      str.str("");
      str<<line;
      getline(str, word, ':');
      player_score.name_ = word.c_str();
      getline(str, word, ':');
      player_score.difficulte_ = stoi(word.c_str());
      getline(str, word, '\n');
      player_score.chrono_ = stod(word.c_str());
      list_of_medium_player.push_back(player_score);
    }

    /* Recupere le nb de joueur DIFFICILE */
    getline(file,line);
    str.clear();
    str.str("");
    str<<line;
    getline(str, word, ':');
    getline(str, word, '\n');
    nb_of_player_scored_hard=stoi(word.c_str());
    /* On Recupere le nom des joueur/score/chrono DIFFICILE */
    for (int i=0; i< nb_of_player_scored_hard;++i){
      getline(file,line);
      str.clear();
      str.str("");
      str<<line;
      getline(str, word, ':');
      player_score.name_ = word.c_str();
      getline(str, word, ':');
      player_score.difficulte_ = stoi(word.c_str());
      getline(str, word, '\n');
      player_score.chrono_ = stod(word.c_str());
      list_of_hard_player.push_back(player_score);
    }

    /* Recupere le nb de joueur CUSTOM */
    getline(file,line);
    str.clear();
    str.str("");
    str<<line;
    getline(str, word, ':');
    getline(str, word, '\n');
    nb_of_player_scored_custom=stoi(word.c_str());

    /* On Recupere le nom des joueur/score/chrono CUSTOM */
    for (int i=0; i< nb_of_player_scored_custom;++i){
      getline(file,line);
      str.clear();
      str.str("");
      str<<line;
      getline(str, word, ':');
      player_score.name_ = word.c_str();
      getline(str, word, ':');
      player_score.difficulte_ = stoi(word.c_str());
      getline(str, word, ':');
      player_score.nb_bombes_=stoi(word.c_str());
      getline(str, word, '\n');
      player_score.chrono_ = stod(word.c_str());
      list_of_custom_player.push_back(player_score);
    }

    /* Recupere le nb de joueur 3D */
    getline(file,line);
    str.clear();
    str.str("");
    str<<line;
    getline(str, word, ':');
    getline(str, word, '\n');
    nb_of_player_scored_3D=stoi(word.c_str());

    /* On Recupere le nom des joueur/score/chrono 3D */
    for (int i=0; i< nb_of_player_scored_3D;++i){
      getline(file,line);
      str.clear();
      str.str("");
      str<<line;
      getline(str, word, ':');
      player_score.name_ = word.c_str();
      getline(str, word, ':');
      player_score.difficulte_ = stoi(word.c_str());
      getline(str, word, ':');
      player_score.nb_bombes_=stoi(word.c_str());
      getline(str, word, '\n');
      player_score.chrono_ = stod(word.c_str());
      list_of_3D_player.push_back(player_score);
    }
    file.close();
  }
  else
    cout<<"Could not open the file\n";
}


score_file::~score_file(){};


/**
	* @Role  : permet d'afficher les scores
	* @param : difficulte_choisie permet de connaitre le mode de jeu afin d'afficher uniquement les scores de ce mode de jeu.
	* @retval: un ostringstream qui correspond au texte afficher dans la fenetre des scores.
	*/
ostringstream score_file::print_scores(const int difficulte_choisie){
  ostringstream text;
  text  << "------------------ SCORE FILE :";
  if (classic_game_mode_ == 1){
    switch (difficulte_choisie){
    case 8 :
			text  << " MODE FACILE ------------------" << endl<<endl;
      for (int e = 0 ; e < nb_of_player_scored_easy ; ++e){
	text  << "TOP " << setw(2)<< e+1 << " | joueur : " <<setw(15)<< list_of_easy_player[e].name_ << " | temps : " << setw(5) <<list_of_easy_player[e].chrono_ << " secondes"<< endl ;
      }
      break;
    case 14:
      text  << " MODE MOYEN ------------------" << endl<<endl;
      for (int e = 0 ; e < nb_of_player_scored_medium ; ++e){
	text  << "TOP " << setw(2) <<e+1 << " | joueur : " <<setw(15)<< list_of_easy_player[e].name_ << " | temps : " << setw(5) <<list_of_easy_player[e].chrono_ << " secondes"<< endl ;
      }
      break;
    case 20 :
      text << " MODE DIFFICILE ------------------" << endl<<endl;
      for (int e = 0 ; e < nb_of_player_scored_hard ; ++e){
	text <<  "TOP " <<setw(2) << e+1 << " | joueur : " << setw(15)<<list_of_easy_player[e].name_ << " | temps : " << setw(5) <<list_of_easy_player[e].chrono_ << " secondes"<< endl ;
      }
      break;
    }
  }
  else if(classic_game_mode_ == 0){
    text << " MODE CUSTOM ------------------" << endl<<endl;
    for (int e = 0 ; e < nb_of_player_scored_custom ; ++e){
      text << "joueur : " << setw(15)<<list_of_custom_player[e].name_ << " | temps : " << setw(5) <<list_of_custom_player[e].chrono_ << " secondes"<< " | grille de : "<< list_of_custom_player[e].difficulte_ << " cases "<< " avec  "<< list_of_custom_player[e].nb_bombes_ << " bombes "<< endl ;
    }
  }
  else{
    text << " MODE 3D ------------------" << endl<<endl;
    for (int e = 0 ; e < nb_of_player_scored_3D ; ++e){
      text <<  "TOP " <<setw(2) << e+1 << " | joueur : " << setw(15)<<list_of_3D_player[e].name_ << " | temps : " << setw(5) <<list_of_3D_player[e].chrono_ << " secondes"<< endl ;
        }
  }
  text << endl<<"------------------ END OF SCORE FILE ------------------" << endl << endl ;
  if (New_High_Score == true)
    text << "!!!!!!!!!!!!!!!!!!!!!!!!!! Well done !!! New High Score !!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
  return text;
}


/**
	* @Role  : permet de trier les meilleurs joueurs et de modifier le fichier des scores.
	* @param : score permet de connaitre la difficulte et le mode de jeu choisit
	* @param : tps est le temps mis par le joueur pour finir la partie
	* @param : nb_bombes permet de connaitre et d'enregistrer le nombre de bombe pour le mode de jeu personnalise.
	* @retval: none.
	*/
void score_file::add_score(const int score, const double tps,const int nb_bombes){
  New_High_Score = false;

  type_player test;
  test.chrono_=tps;
  test.difficulte_=score;
  test.name_=player_name_;
  test.nb_bombes_=nb_bombes;

  if (classic_game_mode_ == 1){
    switch (score){
    case 8 :
			++nb_of_player_scored_easy;
      list_of_easy_player.push_back(test);
      if ((test.chrono_<list_of_easy_player[0].chrono_) | (nb_of_player_scored_easy ==1))
				New_High_Score =true;
      sort(list_of_easy_player.begin(), list_of_easy_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});       /** On trie le vecteur pour ne garder que les 10 meilleures scores **/
      if (nb_of_player_scored_easy >10)
				nb_of_player_scored_easy=10;
      break;

    case 14:
      ++nb_of_player_scored_medium;
      list_of_medium_player.push_back(test);
      if ((test.chrono_<list_of_medium_player[0].chrono_) | (nb_of_player_scored_medium ==1))
				New_High_Score =true;
      sort(list_of_medium_player.begin(), list_of_medium_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});   /** On trie le vecteur pour ne garder que les 10 meilleures scores **/
      if (nb_of_player_scored_medium >10)
				nb_of_player_scored_medium=10;
      break;

    case 20 :
      ++nb_of_player_scored_hard;
      list_of_hard_player.push_back(test);
      if (test.chrono_<list_of_hard_player[0].chrono_ or nb_of_player_scored_hard==1)
				New_High_Score =true;
      sort(list_of_hard_player.begin(), list_of_hard_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});      /** On trie le vecteur pour ne garder que les 10 meilleures scores **/
      if (nb_of_player_scored_hard >10)
				nb_of_player_scored_hard=10;
      break;
    }
  }
  else if(classic_game_mode_==0){
    ++nb_of_player_scored_custom;
    list_of_custom_player.push_back(test);
    sort(list_of_custom_player.begin(), list_of_custom_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_ > b.chrono_;});   /** On trie le vecteur pour ne garder que les 10 meilleures scores **/
    if (nb_of_player_scored_hard >10)
      nb_of_player_scored_hard=10;
  }

  else {
    ++nb_of_player_scored_3D;
    list_of_3D_player.push_back(test);
    if (test.chrono_<list_of_3D_player[0].chrono_ or nb_of_player_scored_3D==1)
      New_High_Score =true;
    sort(list_of_3D_player.begin(), list_of_3D_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});           /** On trie le vecteur pour ne garder que les 10 meilleures scores **/
    if (nb_of_player_scored_3D >10)
      nb_of_player_scored_3D=10;
  }

  ofstream file;
  //vider le fichier
  file.open(file_name_);
  file <<"";
  file.close();

  ofstream fout(file_name_,  ios::out | ios::app);

  if (fout.is_open()){
    fout << "/FACILE:" << nb_of_player_scored_easy <<endl;
    for (int e = 0 ; e < nb_of_player_scored_easy ; ++e){
      fout << list_of_easy_player[e].name_ << ":" <<list_of_easy_player[e].difficulte_ << ":" << list_of_easy_player[e].chrono_ << endl ;
    }
    fout << "/MOYEN:"<<nb_of_player_scored_medium <<endl;
    for (int e = 0 ; e < nb_of_player_scored_medium ; ++e){
      fout << list_of_medium_player[e].name_ << ":" <<list_of_medium_player[e].difficulte_ << ":" <<list_of_medium_player[e].chrono_ << endl ;
    }
    fout << "/DIFFILE:"<<nb_of_player_scored_hard <<endl;
    for (int e = 0 ; e < nb_of_player_scored_hard ; ++e){
      fout << list_of_hard_player[e].name_ << ":" <<list_of_hard_player[e].difficulte_ << ":" << list_of_hard_player[e].chrono_ << endl ;
    }
    fout << "/CUSTOM:"<<nb_of_player_scored_custom <<endl;
    for (int e = 0 ; e < nb_of_player_scored_custom ; ++e){
      fout << list_of_custom_player[e].name_ << ":" <<list_of_custom_player[e].difficulte_ << ":" <<list_of_custom_player[e].nb_bombes_ << ":"<< list_of_custom_player[e].chrono_ << endl ;
    }
    fout << "/3D:"<<nb_of_player_scored_3D <<endl;
    for (int e = 0 ; e < nb_of_player_scored_3D; ++e){
      fout << list_of_3D_player[e].name_ << ":" <<list_of_3D_player[e].difficulte_ << ":" <<list_of_3D_player[e].nb_bombes_ << ":"<< list_of_3D_player[e].chrono_ << endl ;
    }
    file.close();
  }
}

