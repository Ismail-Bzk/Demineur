#pragma once

#include "Observateur.hpp"
#include <list>

template<typename T, typename R>
class Observable {
private:
  std::list<Observateur<T,R>*> list_observateurs;

public:
  void notifierObservateurs(T info, R res) {
    for (auto obs : this->list_observateurs) obs->update(info,res);
  }

  void ajouterObservateur(Observateur<T,R> * observateur) {
    this->list_observateurs.push_back(observateur);
  }
};

