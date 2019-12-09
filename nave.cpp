#include "nave.hpp"

Nave::Nave(GLfloat cx, GLfloat cy, GLfloat raio, GLint texture, GLfloat k, GLfloat kTiro, GLfloat freqTiro)
{
  this->cx = cx;
  this->cy = cy;
  this->raio = raio;
  this->texture = texture;
  this->k = k;
  this->kTiro = kTiro;
  this->freqTiro = freqTiro;
}

void Nave::desenhaNave()
{
}
