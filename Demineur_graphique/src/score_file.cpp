#include "score_file.h"

#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

// constructor prototype
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
        file.close();
    }

    else
        cout<<"Could not open the file\n";
}

score_file::~score_file(){};

void score_file::print_scores(const int difficulte_choisie){
    system("cls");
    cout<<"\x1B[2J\x1B[H";
    cout << "------------------ SCORE FILE :";
    if (classic_game_mode_ == 1){
        switch (difficulte_choisie){
            case 10 :
                cout << " MODE FACILE ------------------" << endl<<endl;
                for (int e = 0 ; e < nb_of_player_scored_easy ; ++e){
                    cout << "TOP " << setw(2)<< e+1 << " | joueur : " <<setw(15)<< list_of_easy_player[e].name_ << " | temps : " << setw(5) <<list_of_easy_player[e].chrono_ << " secondes"<< endl ;
                }
            break;
            case 20:
                cout << " MODE MOYEN ------------------" << endl<<endl;
                for (int e = 0 ; e < nb_of_player_scored_medium ; ++e){
                    cout << "TOP " << setw(2) <<e+1 << " | joueur : " <<setw(15)<< list_of_easy_player[e].name_ << " | temps : " << setw(5) <<list_of_easy_player[e].chrono_ << " secondes"<< endl ;
                }
            break;
            case 30 :
                cout << " MODE DIFFICILE ------------------" << endl<<endl;
                for (int e = 0 ; e < nb_of_player_scored_hard ; ++e){
                    cout <<  "TOP " <<setw(2) << e+1 << " | joueur : " << setw(15)<<list_of_easy_player[e].name_ << " | temps : " << setw(5) <<list_of_easy_player[e].chrono_ << " secondes"<< endl ;
                }
            break;
        }
    }
    else{
        cout << " MODE CUSTOM ------------------" << endl<<endl;
        for (int e = 0 ; e < nb_of_player_scored_custom ; ++e){
            cout << "joueur : " << setw(15)<<list_of_custom_player[e].name_ << " | temps : " << setw(5) <<list_of_custom_player[e].chrono_ << " secondes"<< " | grille de : "<< list_of_custom_player[e].difficulte_ << " cases "<< " avec  "<< list_of_custom_player[e].nb_bombes_ << " bombes "<< endl ;
        }
    }

    cout << endl<<"------------------ END OF SCORE FILE ------------------" << endl << endl ;

    if (New_High_Score == true)
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!! Well done !!! New High Score !!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
}


// write out and save high score file
void score_file::add_score(const int score, const double tps,const int nb_bombes){

    New_High_Score = false;

	type_player test;
	test.chrono_=tps;
	test.difficulte_=score;
	test.name_=player_name_;
	test.nb_bombes_=nb_bombes;

    if (classic_game_mode_ == 1){
        switch (score){
            case 10 :
                ++nb_of_player_scored_easy;
                list_of_easy_player.push_back(test);
                if ((test.chrono_<list_of_easy_player[0].chrono_) | (nb_of_player_scored_easy ==1))
                    New_High_Score =true;
                sort(list_of_easy_player.begin(), list_of_easy_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});
                if (nb_of_player_scored_easy >10)
                    nb_of_player_scored_easy=10;
            break;
            case 20:
                ++nb_of_player_scored_medium;
                list_of_medium_player.push_back(test);
                if ((test.chrono_<list_of_medium_player[0].chrono_) | (nb_of_player_scored_medium ==1))
                    New_High_Score =true;
                sort(list_of_medium_player.begin(), list_of_medium_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});
                if (nb_of_player_scored_medium >10)
                    nb_of_player_scored_medium=10;
            break;
            case 30 :
                ++nb_of_player_scored_hard;
                list_of_hard_player.push_back(test);
                if (test.chrono_<list_of_hard_player[0].chrono_ or nb_of_player_scored_hard==1)
                    New_High_Score =true;
                sort(list_of_hard_player.begin(), list_of_hard_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_< b.chrono_;});
                if (nb_of_player_scored_hard >10)
                    nb_of_player_scored_hard=10;
            break;
        }
    }
    else{
        ++nb_of_player_scored_custom;
        list_of_custom_player.push_back(test);
        sort(list_of_custom_player.begin(), list_of_custom_player.end(),[](const type_player &a, const type_player &b) {return a.chrono_ > b.chrono_;});

        if (nb_of_player_scored_hard >10)
            nb_of_player_scored_hard=10;
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
    file.close();
	}
}

