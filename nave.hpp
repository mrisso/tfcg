#ifndef NAVE_H_
#define NAVE_H_
#include <iostream>
#include <math.h>
#include <GL/gl.h>

using namespace std;

class Nave{
private:
  GLfloat raio;
  GLfloat cx, cy; // Centro x e y
  GLint texture;
  GLfloat kTiro;
  GLfloat k;
  GLfloat freqTiro;

public:
  Nave(GLfloat cx, GLfloat cy, GLfloat raio, GLint texture, GLfloat k, GLfloat kTiro, GLfloat freqTiro = 0);

  void desenhaNave();
};


#endif
