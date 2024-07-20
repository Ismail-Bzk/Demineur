#include "Demineur.h"
#include "score_file.h"

#include <chrono>
using namespace std;




int main(){

    string player_name, choix;
    int difficulte_choisie;
    int no_death_mode=1;
    int nb_bombes =0;
    int classic_game_mode = 1;
    cout << "Bonjour tres cher joueur !" << endl << "Pour commencer veuillez inscrire votre pseudo sans espace : ";
    //cin >> player_name;
    cout << "Choisissez votre difficulte : FACILE : 0 // MOYEN : 1 // DIFFICILE : 2 // PERSONNALISE : 3 --> ";
    //cin >> difficulte_choisie;
    switch (difficulte_choisie){
        case 0 :
            difficulte_choisie = FACILE;
        break;

        case 1 :
            difficulte_choisie = MOYEN;
        break;

        case 2 :
            difficulte_choisie = DIFFICILE;
        break;

        case 3 :
            classic_game_mode=0;
            cout << "De quel taille voulez vous la grille : ";
            cin >> difficulte_choisie;
            cout << "Combien de bombe voulez vous : ";
            cin >> nb_bombes;
            while(difficulte_choisie*difficulte_choisie < nb_bombes){
                cout << "Vous avez selectionner trop de bombe. Combien de bombe voulez vous : ";
                cin >> nb_bombes;
            };
        break;

        default :
            difficulte_choisie = FACILE;
            nb_bombes = FACILE;
        break;
    }

    score_file score = score_file("score.txt", player_name,classic_game_mode);

    cout << "Voulez vous voir le tableau des scores ? Y or N --> ";
    //cin >> choix;
    if (choix == "Y"){
        score.print_scores(difficulte_choisie);
        cout << endl << "Pour commencer la partie, appuyer sur s puis entree";
        cin >> choix;

    }


    Demineur Mon_jeu=Demineur();
    Mon_jeu.configDemineur(difficulte_choisie,nb_bombes);


    cout << endl;
    int partie = 1;
    int premier_coup = 1;
    int x;
    int y;
    char choix2;

    system("cls");
    cout<<"\x1B[2J\x1B[H";

    Mon_jeu.affichage();
    cout << endl;

    cout << "Voulez vous jouer en \"no death mode\", cela signifie que vous ne pourrez pas mourir et vous ne pourrez pas sauvegarder votre score. Tapez 1 si oui, 0 si non: ";
    cin >> no_death_mode;

    auto start = chrono::steady_clock::now();

    while ((partie or no_death_mode ) and (partie !=-1)){    //inverser partie ie defaite = 1 meme chose avec victoire
        cout <<endl;
        switch (partie){
            case 0 :
                cout << "Vous avez selectionne une bombe, votre derniere action a ete annule"<<endl;
            break;
            case 2:
                cout << "Veuillez selectionner une case ou vous n'avez pas encore jouer" << endl;
            break;
            case 3:
                cout << "Veuillez selectionner une case ou vous n'avez pas depose de drapeau" << endl;
            break;
            case 4:
                cout << "Veuillez selectionner une case non decouverte" << endl;
            break;
        }
        cout <<endl;
        cout << "What do you want to do ? PLay:'p' / Undo:'u' / Redo:'r' / Flag:'f' --> ";
        cin >> choix2;

        switch(choix2){

            case 'p' :
                cout << "x : ";
                cin >> x;
                cout << "y : ";
                cin >> y;

                if (premier_coup == 1){
                    Mon_jeu.poser_bombe(x,y);
                    premier_coup=0;
                }

                partie = Mon_jeu.jouer(x,y);

                break;

            case 'f' :
                cout << "x : ";
                cin >> x;
                cout << "y : ";
                cin >> y;

                partie = Mon_jeu.drapeau(x,y);
            break;

            case 'u' :
                Mon_jeu.undo();
                break;
            case 'r' :
                Mon_jeu.redo();
                break;
        }
        system("cls");
        cout<<"\x1B[2J\x1B[H";
        Mon_jeu.affichage();
        cout << endl;
        //cout << Mon_jeu.nb_case_decouvert_ <<endl;
        Mon_jeu.game_history_.show_all_modif();
        cout << "tour actuelle "<<Mon_jeu.game_history_.tour_actuelle<<endl;
        cout << "tour total "<<Mon_jeu.game_history_.tour_total<<endl;
        //victoire = (Mon_jeu.nb_case_decouvert_ < Mon_jeu.difficulte_*Mon_jeu.difficulte_ - Mon_jeu.nb_bombes_ );
/*test score  victoire = 0;*/

    }
    if (partie != -1){
        system("cls");
        cout<<"\x1B[2J\x1B[H";
        cout << " YOU LOOSE " <<endl;
        cout << endl;
        Mon_jeu.affichage_final();
    }
    else {
        system("cls");
        cout<<"\x1B[2J\x1B[H";
        Mon_jeu.affichage_final();
        auto duration = ((int) chrono::duration<double, std::milli> (chrono::steady_clock::now() - start).count()/10) / 100.0;
        cout << " YOU WIN, CONGRAT" << endl << "Vous avez mis : " << duration << " secondes "<< endl;

        if (no_death_mode ==0){
            cout <<"Voulez vous enregistrer votre scores ? Y or N --> ";
            cin >> choix;
            if (choix == "Y"){
                score.add_score(Mon_jeu.difficulte_,duration,nb_bombes);
                score.print_scores(difficulte_choisie);
            }
        }

    }

    return 0;
}
