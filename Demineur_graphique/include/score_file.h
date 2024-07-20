#ifndef SCORE_FILE_H
#define SCORE_FILE_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    string name_;
    int difficulte_;
    int nb_bombes_ = 0;
    double chrono_;
} type_player;


class score_file{
    public:
        score_file(string file_name, string player_name, const int classic_game_mode);
        virtual ~score_file();
        void print_scores(const int difficulte_choisie);
        void add_score(const int score,const double tps,const int nb_bombes);

        bool New_High_Score = false;

    private:
        string file_name_;
        string player_name_;
        int classic_game_mode_ = 1;                                       //on verifie que le joeur ne selectionne pas CUSTOM avec une grille de 10/20/30 de cote

        int nb_of_player_scored_easy=0;
        vector <type_player> list_of_easy_player;
        int nb_of_player_scored_medium=0;
        vector <type_player> list_of_medium_player;
        int nb_of_player_scored_hard=0;
        vector <type_player> list_of_hard_player;
        int nb_of_player_scored_custom=0;
        vector <type_player> list_of_custom_player;

};

#endif // SCORE_FILE_H
