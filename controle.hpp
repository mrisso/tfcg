#ifndef CONTROLE_H_
#define CONTROLE_H_
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class Controle{
private:
  bool decolado;
  bool ready;
  bool gameOver;
  bool win;
  bool fire;

public:
  Controle();

  bool getDecolado();
  void setDecolado(bool decolado);

  bool getReady();
  void setReady(bool ready);

  bool getGameOver();
  void setGameOver(bool gameOver);

  bool getWin();
  void setWin(bool win);

  bool getFire();
  void setFire(bool fire);
};


#endif
