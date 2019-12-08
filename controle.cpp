#include "controle.hpp"

Controle::Controle()
{
  this->decolado = false;
  this->ready = false;
  this->gameOver = false;
  this->win = false;
  this->fire = false;
}

bool Controle::getDecolado()
{
  return this->decolado;
}

void Controle::setDecolado(bool decolado)
{
  this->decolado = decolado;
}

bool Controle::getReady()
{
  return this->ready;
}

void Controle::setReady(bool ready)
{
  this->ready = ready;
}

bool Controle::getGameOver()
{
  return this->gameOver;
}

void Controle::setGameOver(bool gameOver)
{
  this->gameOver = gameOver;
}

bool Controle::getWin()
{
  return this->win;
}

void Controle::setWin(bool win)
{
  this->win = win;
}

bool Controle::getFire()
{
  return this->fire;
}

void Controle::setFire(bool fire)
{
  this->fire = fire;
}
