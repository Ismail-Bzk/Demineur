#include "History.h"
#include <iostream>

using namespace std;
History::History(){

}

History::~History()
{
    //dtor
}

void History::show_all_modif(){
    int tour = 0;
    for (auto a : history_){
        cout << "modification au tours : "<<tour;
        for (auto b : a){
            cout << " || ("  << b.position_profondeur_ << ","<< b.position.first-1 << "," << b.position.second-1 << ")-->" << b.etat_ ;
        }
    cout<< endl;
    ++tour;
    }
    cout << endl;
};

void History::add_modif(Type_history history_last_modif ){
    //cout<<"test modif" << tour_actuelle<< "|"<< tour_total<<endl;
    if ((int) history_.size()!= tour_actuelle+1) {
        history_.resize(tour_actuelle+1);
    }
    if ((tour_actuelle+1 <= tour_total) & (Undo_Mode==1)){
        //cout<<"---------------------------------test erase---------------------------------"<<endl;
        while ( tour_total != tour_actuelle) {
            --tour_total;
            history_[tour_total].clear();
        };
        Undo_Mode=0;
    }
    history_[tour_actuelle].push_back(history_last_modif);
};

void History::reset(){
  history_.clear();
  tour_actuelle=0;
  tour_total=0;
};

