#include "arena.hpp"
#include <GL/gl.h>
#include <GL/glut.h>
#include "imageloader.h"

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

Arena *arena;

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

void init (void)
{
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_TEXTURE_2D );
  glEnable(GL_LIGHTING);
  //    glShadeModel (GL_FLAT);
  glShadeModel (GL_SMOOTH);

  glDepthFunc(GL_LEQUAL);

  textureArena = LoadTextureRAW( "stars1.bmp" );

  arena = new Arena(100,textureArena);

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
