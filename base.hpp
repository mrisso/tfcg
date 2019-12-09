#ifndef BASE_H_
#define BASE_H_
#include <iostream>
#include <math.h>
#include <GL/gl.h>

using namespace std;

class Base{
private:
  GLfloat raio;
  GLfloat cx, cy; // Centro x e y
  GLint texture;

public:
  Base(GLfloat cx, GLfloat cy, GLfloat raio, GLint texture);

  void desenhaBase();
};


#endif
