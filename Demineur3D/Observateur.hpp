#pragma once

template<typename T, typename R>
class Observateur {
public:
  virtual void update(T &info, R &res ) = 0;
};
