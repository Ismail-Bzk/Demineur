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
  Gtk::Label Commentaire;
  Gtk::Entry pseudo;

public :
  Dialogue(std::string titre, Gtk::Window* parent, std::string message):
    Gtk::Dialog(titre, *parent),
    boiteV(get_vbox()),
    Commentaire(message)
  {
    //Récupération de la boîte verticale avec get_vbox().
    boiteV->pack_start(Commentaire);
    boiteV->pack_start(pseudo);

    //Ajout de boutons à la boîte de dialogue.
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

    show_all();
  }

  ~Dialogue(){}

/*
 *Rôle : renvoie le pseudo saisie par le joueur
 */ 
std::string get_texte() {
    return pseudo.get_text();
}
    
};
