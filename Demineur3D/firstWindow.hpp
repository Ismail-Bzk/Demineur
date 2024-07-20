#pragma once

#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/dialog.h>

class firstWindow : public Gtk::Dialog{

private:
  Gtk::Box* box1;
  Gtk::HBox box2;
  Gtk::HButtonBox box3;
  Gtk::Label zoneTexte;
  
public:
  firstWindow(Gtk::Window* parent):
    Gtk::Dialog("Bienvenue", *parent),
    box1(get_content_area()),
    box2(false,1),
    zoneTexte("Bienvenue dans cette version numerique du Demineur!\nSi vous souhaitez lire les regles du jeu avant de commencer,\ncliquer sur le bouton 'Instructions', sinon cliquer sur 'Suivant'\n\nVous pourrez avoir acces aux regles en cours de partie\nen cliquant sur le menu 'Aide'")

  {
    zoneTexte.set_justify(Gtk::JUSTIFY_FILL);
    zoneTexte.set_alignment(Gtk::ALIGN_CENTER);
    box1->pack_start(zoneTexte,Gtk::PACK_SHRINK);
    add_button("Instructions", Gtk::RESPONSE_HELP);
    add_button("Suivant", Gtk::RESPONSE_OK);
    show_all_children ();

}

  ~firstWindow(){}

};
