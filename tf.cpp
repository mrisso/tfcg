#include "arena.hpp"
#include "nave.hpp"
#include "base.hpp"
#include "pista.hpp"
#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "tinyxml2.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

using namespace std;
using namespace tinyxml2;

//Camera controls
double camDist=50;
double camXYAngle=0;
double camXZAngle=0;
int toggleCam = 0;
int camAngle = 60;
int lastX = 0;
int lastY = 0;
int buttonDown=0;

GLuint textureArena;
GLuint texturePlayer;
GLuint textureInimigo;
GLuint textureBase;
GLuint texturePista;

Arena *arena;
Nave *player;
Pista *pista;
vector<Nave*> enemies;
vector<Base*> bases;

string name;

int readXml2(char* filename)
{
  string aux;

  XMLDocument doc;
  XMLError err = doc.LoadFile(strcat(filename, "config.xml"));

  XMLCheckResult(err);

  XMLNode* pRoot = doc.FirstChild();
  XMLElement* pElement = pRoot->FirstChildElement("arquivoDaArena");

  pElement = pRoot->FirstChildElement("arquivoDaArena");
  pElement = pElement->FirstChildElement("nome");
  name = pElement->GetText();

  pElement = pRoot->FirstChildElement("arquivoDaArena");
  pElement = pElement->FirstChildElement("tipo");
  aux = pElement->GetText();

  name = name + "." + aux;

  pElement = pRoot->FirstChildElement("arquivoDaArena");
  pElement = pElement->FirstChildElement("caminho");
  aux = pElement->GetText();

  name = aux + name;
  cout << name;

  float k, kTiro, freqTiro, kE, kTiroE;

  pElement = pRoot->FirstChildElement("jogador");
  pElement->QueryFloatAttribute("velTiro", &kTiro);
  pElement->QueryFloatAttribute("vel", &k);

  pElement = pRoot->FirstChildElement("inimigo");
  pElement->QueryFloatAttribute("freqTiro", &freqTiro);
  pElement->QueryFloatAttribute("vel", &kE);
  pElement->QueryFloatAttribute("velTiro", &kTiroE);

  XMLDocument svgDoc;

  err = svgDoc.LoadFile(name.c_str());

  XMLCheckResult(err);

  XMLElement* svg = svgDoc.FirstChildElement("svg");
  XMLElement* circle = svg->FirstChildElement("circle");
  XMLElement* line = svg->FirstChildElement("line");

  int id;
  float x, y, radius;
  string fill;

  while(circle)
  {
    x = circle->FloatAttribute("cx");
    y = circle->FloatAttribute("cy");
    radius = circle->FloatAttribute("r");
    id = circle->FloatAttribute("id");
    fill = circle->Attribute("fill");

    //Arena
    if(fill == "blue")
      arena = new Arena(x,y,radius,textureArena);

    //Player
    else if(fill == "green")
      player = new Nave(x,y,radius,texturePlayer,k,kTiro);

    //Flying Obstacles
    else if(fill == "red")
    {
      Nave *n = new Nave(x,y,radius,textureInimigo,kE,kTiroE,freqTiro);
      enemies.push_back(n);
    }

    //Ground Obstacles
    else if(fill == "orange")
    {
      Base *b = new Base(x,y,radius,textureBase);
      bases.push_back(b);
    }

    circle = circle->NextSiblingElement("circle");
  }
  float xo,yo;

  x = line->FloatAttribute("x1");
  y = line->FloatAttribute("y1");
  xo = line->FloatAttribute("x2");
  yo = line->FloatAttribute("y2");

  pista = new Pista(x,y,xo,yo,texturePista);

  return 0;
}

GLuint LoadTextureRAW( const char * filename )
{

  GLuint texture;
  Image* image = loadBMP(filename);

  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
               0,                            //0 for now
               GL_RGB,                       //Format OpenGL uses for image
               image->width, image->height,  //Width and height
               0,                            //The border of the image
               GL_RGB, //GL_RGB, because pixels are stored in RGB format
               GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
               //as unsigned numbers
               image->pixels);               //The actual pixel data
  delete image;

  return texture;
}

void loadTextures()
{
  textureArena = LoadTextureRAW( "stars1.bmp" );
  // texturePlayer = LoadTextureRAW( "stars1.bmp" );
  // textureInimigo = LoadTextureRAW( "stars1.bmp" );
  // textureBase = LoadTextureRAW( "stars1.bmp" );
  // texturePista = LoadTextureRAW( "stars1.bmp" );
}

void init (void)
{
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_TEXTURE_2D );
  glEnable(GL_LIGHTING);
  //    glShadeModel (GL_FLAT);
  glShadeModel (GL_SMOOTH);

  glDepthFunc(GL_LEQUAL);

  glEnable(GL_LIGHT0);
}

void DrawAxes()
{
    GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        //x axis
        glPushMatrix();
            glColor3fv(mat_ambient_r);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        //y axis
        glPushMatrix();
            glColor3fv(mat_ambient_g);
            glRotatef(90,0,0,1);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        //z axis
        glPushMatrix();
            glColor3fv(mat_ambient_b);
            glRotatef(-90,0,1,0);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();
    glPopAttrib();
}


void display(void)
{
  glClearColor (0.0,0.0,0.0,1.0);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0,0,-camDist);
  glRotatef(camXZAngle,1,0,0);
  glRotatef(camXYAngle,0,1,0);

  DrawAxes();

  GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  arena->desenhaArena();

  glutSwapBuffers();
}

void idle(void)
{
  glutPostRedisplay();
}

void mouse_callback(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    lastX = x;
    lastY = y;
    buttonDown = 1;
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    buttonDown = 0;
  }
}

void mouse_motion(int x, int y)
{
  if (!buttonDown)
    return;
  camXYAngle += x - lastX;
  camXZAngle += y - lastY;
  camXYAngle = (int)camXYAngle % 360;
  camXZAngle = (int)camXZAngle % 360;

  lastX = x;
  lastY = y;
}

void changeCamera(int angle, int w, int h)
{
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  gluPerspective (angle,
                  (GLfloat)w / (GLfloat)h, 1, 150.0);

  glMatrixMode (GL_MODELVIEW);
}

void reshape (int w, int h) {

  glViewport (0, 0, (GLsizei)w, (GLsizei)h);

  changeCamera(camAngle, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
  static bool textureEnebled = true;
  static bool lightingEnebled = true;
  switch (key) {
  case '0':
    toggleCam = 0;
    break;
  case '1':
    toggleCam = 1;
    break;
  case '2':
    toggleCam = 2;
    break;
  case 't':
    if ( textureEnebled ){
      glDisable( GL_TEXTURE_2D );
    }else{
      glEnable( GL_TEXTURE_2D );
    }
    textureEnebled = !textureEnebled;
    break;
  case 'l':
    if ( lightingEnebled ){
      glDisable( GL_LIGHTING );
    }else{
      glEnable( GL_LIGHTING );
    }
    lightingEnebled = !lightingEnebled;
    break;
  case '+':
    {
      int inc = camAngle >= 180 ? 0 : 1;
      camAngle += inc;
      changeCamera(camAngle,
                   glutGet(GLUT_WINDOW_WIDTH),
                   glutGet(GLUT_WINDOW_HEIGHT));
      break;
    }
  case '-':
    {
      int inc = camAngle <= 5 ? 0 : 1;
      camAngle -= inc;
      changeCamera(camAngle, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
      break;
    }
  case 'w':
    {
      camDist -= 1;
      break;
    }
  case 's':
    {
      camDist+=1;
      break;
    }
  case 27:
    exit(0);
    break;
  }
}

int main(int argc, char**argv)
{
  if(argc != 2)
  {
      printf("Uso: %s <arquivo-xml-conf>\n", argv[0]);
      return 1;
  }

  //Carregar Texturas
  loadTextures();

  //Ler xml
  if(readXml2(argv[1]))
  {
      cout << "Erro!\n";
      return 1;
  }

  glutInit (&argc, argv);

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize (500, 500);

  glutInitWindowPosition (100, 100);

  glutCreateWindow ("A basic OpenGL Window");

  init();

  glutDisplayFunc (display);

  glutIdleFunc (idle);

  glutReshapeFunc (reshape);

  glutKeyboardFunc(keyboard);

  glutMotionFunc(mouse_motion);
  glutMouseFunc(mouse_callback);

  glutMainLoop ();

  return 0;
}
