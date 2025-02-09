#include <gtkmm.h>
#include <gtkmm/application.h>
#include "VueG.hpp"
#include "Demineur.hpp"
#include "Controleur.hpp"

int main(int argc,char * argv []) {
    auto app =Gtk::Application::create(argc,argv);

    Demineur *m=new Demineur();
    VueG *vg = new VueG();
    Controleur *c = new Controleur(m,vg);
    m->ajouterObservateur(vg);
      
    vg->set_default_size(300,200);
    vg->set_title("Jeu du Demineur");
    return app->run(*vg);
}
