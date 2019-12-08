#include "arena.hpp"

Arena::Arena(GLfloat raio, GLint texture)
{
  this->raio = raio;
  this->texture = texture;
}

void Arena::desenhaArena()
{
  GLfloat materialEmission[] = { 1.00, 1.00, 0.00, 1};
  GLfloat materialColor[] = { 1.0, 1.0, 0.0, 1};
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
  GLfloat mat_shininess[] = { 50.0 };
  glColor3f(1,1,0);

  glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );

  glBindTexture (GL_TEXTURE_2D, texture);

    const double PI = 3.14159;

    /* top triangle */
    double i, resolution  = 0.001;
    double height = 4*20;

    glPushMatrix();
    glTranslatef(0, -0.5, -50);

    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, height, 0);  /* center */
        for (i = 2 * PI; i >= 0; i -= resolution)

        {
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(this->raio * cos(i), height, this->raio * sin(i));
        }
        /* close the loop back to 0 degrees */
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(this->raio, height, 0);
    glEnd();

    /* bottom triangle: note: for is in reverse order */
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, 0, 0);  /* center */
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(this->raio * cos(i), 0, this->raio * sin(i));
        }
    glEnd();

    /* middle tube */
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            const float tc = ( i / (float)( 2 * PI ) );
            glTexCoord2f( tc, 0.0 );
            glVertex3f(this->raio * cos(i), 0, this->raio * sin(i));
            glTexCoord2f( tc, 1.0 );
            glVertex3f(this->raio * cos(i), height, this->raio * sin(i));
        }
        /* close the loop back to zero degrees */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f(this->raio, 0, 0);
        glTexCoord2f( 0.0, 1.0 );
        glVertex3f(this->raio, height, 0);
    glEnd();

    glPopMatrix();
}
