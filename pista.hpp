#ifndef PISTA_H_
#define PISTA_H_
#include <iostream>
#include <math.h>
#include <GL/gl.h>

using namespace std;

class Pista{
private:
  GLfloat x,y,xo,yo;
  GLint texture;

public:
  Pista(GLfloat x, GLfloat y, GLfloat xo, GLfloat yo, GLint texture);

  void desenhaPista();
};


#endif
