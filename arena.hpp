#ifndef ARENA_H_
#define ARENA_H_
#include <iostream>
#include <math.h>
#include <GL/gl.h>

using namespace std;

class Arena{
private:
  GLfloat raio;
  GLint texture;

public:
  Arena(GLfloat raio, GLint texture);

  void desenhaArena();
};


#endif
