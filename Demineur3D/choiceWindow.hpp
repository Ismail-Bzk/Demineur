#include <string>

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/dialog.h>
#include <gtkmm/separator.h>

class choiceWindow : public Gtk::Dialog{
private :
  Gtk::Box* boiteV;
  Gtk::Box m_box;
  Gtk::Label commentaire_;
  Gtk::RadioButtonGroup groupe_;
  Gtk::RadioButton option1_, option2_, option3_,option4_,option5_;
  Gtk::Separator separateur;
  int optionChoisie_=1;

public :
  choiceWindow(Gtk::Window* parent, std::string titre, std::string commentaire, std::string option1, std::string option2, std::string option3,std::string option4,std::string option5):
    Gtk::Dialog(titre, *parent),
    boiteV(get_vbox()),
    commentaire_(commentaire),
    option1_(groupe_,option1),
    option2_(groupe_,option2),
    option3_(groupe_,option3),
    option4_(groupe_,option4),
    option5_(groupe_,option5)
  {
    boiteV->pack_start(commentaire_);
    m_box.pack_start(option1_);
    m_box.pack_start(option2_);
    m_box.pack_start(option3_);
    m_box.pack_start(option4_);
    m_box.pack_start(option5_);
    boiteV->pack_start(m_box);
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    option1_.signal_toggled().connect(sigc::mem_fun(*this, &choiceWindow::setOption1));
    option2_.signal_toggled().connect(sigc::mem_fun(*this, &choiceWindow::setOption2));
    option3_.signal_toggled().connect(sigc::mem_fun(*this, &choiceWindow::setOption3));
    option4_.signal_toggled().connect(sigc::mem_fun(*this, &choiceWindow::setOption4));
    option5_.signal_toggled().connect(sigc::mem_fun(*this, &choiceWindow::setOption5));
    show_all_children();
    
  }

  int getOption(){
    return optionChoisie_;
  }

  void setOption1(){
    this->optionChoisie_=1;
  }
  void setOption2(){
    this->optionChoisie_=2;
  }
  void setOption3(){
    this->optionChoisie_=3;
  }
  void setOption4(){
    this->optionChoisie_=4;
   }
  void setOption5(){
    this->optionChoisie_=5;
  }
};
