#include <string>

#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/label.h>
#include <cstring>

class dimWindow : public Gtk::Dialog{
private :
  Gtk::Box* saisieDimensions;
  Gtk::Label saisie1_;
  Gtk::Label saisie2_;
  Glib::RefPtr<Gtk::Adjustment> dimensions_ ;
  Glib::RefPtr<Gtk::Adjustment> bombes_ ;
  Gtk::SpinButton boutonDim_;
  Gtk::SpinButton boutonBombes_;
  int dim_=5;
  int bomb_=5;
  
public :
  dimWindow(Gtk::Window* parent, std::string saisie1, std::string saisie2,  int defaut, int min,int max, int step):
    saisieDimensions(get_vbox()),
    saisie1_(saisie1),
    saisie2_(saisie2),
    dimensions_(Gtk::Adjustment::create(defaut, min, max, step)),
    bombes_(Gtk::Adjustment::create(defaut,1,24,step)),
    boutonDim_(dimensions_),
    boutonBombes_(bombes_)   
  {
    boutonDim_.set_numeric();
    boutonBombes_.set_numeric();
    saisieDimensions->pack_start(saisie1_);
    saisieDimensions->pack_start(boutonDim_);
    saisieDimensions->pack_start(saisie2_);
    saisieDimensions->pack_start(boutonBombes_);
    add_button(Gtk::Stock::OK,  Gtk::RESPONSE_OK);
    
    boutonDim_.signal_value_changed().connect(sigc::mem_fun(*this, &dimWindow::set_dim));
    boutonBombes_.signal_value_changed().connect(sigc::mem_fun(*this, &dimWindow::set_bombes));
    show_all_children();
    
  }
  
  void set_dim(){
    int dim_saisie=boutonDim_.get_value_as_int();
    boutonBombes_.set_range(1,dim_saisie*dim_saisie-1);
    this->dim_= dim_saisie;
  }

  void set_bombes(){
    this->bomb_= boutonBombes_.get_value_as_int();
  }
  
  int get_dim(){
    return this->dim_;
  }

  int get_bombes(){
    return this->bomb_;
  }
  

  

  
};
