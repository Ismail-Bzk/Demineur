#include <string>

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>

class Dialogue : public Gtk::Dialog{
private :
  Gtk::Box* boiteV;
  Gtk::Label Commentaire; //message de la boite de dialogue
  Gtk::Entry pseudo; //zone de saisie du pseudo du joueur

public :
  Dialogue(std::string titre, Gtk::Window* parent, std::string message):
    Gtk::Dialog(titre, *parent),
    boiteV(get_vbox()),
    Commentaire(message)
  {
    pseudo.set_text("Anonyme");  //pseudo par défaut
    pseudo.set_max_length(15);   //max 15 caractères pour le pseudo

    //Ajout du message et de la zone de saisie du pseudo
    boiteV->pack_start(Commentaire);
    boiteV->pack_start(pseudo);
    //Ajout d'un bouton
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

    show_all();
  }

  ~Dialogue(){}


/**
   * @Role : Renvoie le pseudo saisie par le joueur
   *
   * @param : none
   * @retval: none
*/
std::string get_texte() {
    return pseudo.get_text();
}
    
};
