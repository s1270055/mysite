#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>


enum {TRANS, ROT, SCALE};
int g_mode = TRANS;

enum {PERSP, ORTHO};
int g_proj_mode = PERSP;


// window dimension
int g_width = 800;
int g_height = 800;


// angle (in degree) to rotate around x, y, z
GLfloat g_angle[3] = {0.0f, 0.0f, 0.0f};

// amount to trnaslate along x, y, z
GLfloat g_trans[3] = {0.0f, 0.0f, 0.0f};

// scaling factor along x, y, z
GLfloat g_scale[3] = {1.0f, 1.0f, 1.0f};


void 
drawQuad(GLfloat* p0, GLfloat* c0,
	 GLfloat* p1, GLfloat* c1,
	 GLfloat* p2, GLfloat* c2,
	 GLfloat* p3, GLfloat* c3) 
{
  glBegin(GL_QUADS);
  glColor3fv(c0); 
  glVertex3fv(p0);
  glColor3fv(c1); 
  glVertex3fv(p1);
  glColor3fv(c2); 
  glVertex3fv(p2);
  glColor3fv(c3); 
  glVertex3fv(p3);
  glEnd();
}


void drawCube(void) {
  GLfloat vertex[8][3] = {{-1.0f,-1.0f,-1.0f}, { 1.0f,-1.0f,-1.0f},
			  { 1.0f, 1.0f,-1.0f}, {-1.0f, 1.0f,-1.0f}, 
			  {-1.0f,-1.0f, 1.0f}, { 1.0f,-1.0f, 1.0f},
			  { 1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f} };   
  
  GLfloat color[8][3] = {{0.0f,0.0f,0.0f}, {1.0f,0.0f,0.0f}, 
			 {1.0f,1.0f,0.0f}, {0.0f,1.0f,0.0f}, 
			 {0.0f,0.0f,1.0f}, {1.0f,0.0f,1.0f},
			 {1.0f,1.0f,1.0f}, {0.0f,1.0f,1.0f} };

  int face[6][4] = {
    {0, 4, 7, 3}, {1, 5, 6, 2}, {0, 1, 5, 4}, 
    {3, 7, 6, 2}, {0, 3, 2, 1}, {4, 5, 6, 7} };
  
  int i;
  for (i=0; i<6; ++i) {
    drawQuad(vertex[face[i][0]], color[face[i][0]],
             vertex[face[i][1]], color[face[i][1]],
             vertex[face[i][2]], color[face[i][2]],
             vertex[face[i][3]], color[face[i][3]]);
  }
}


void setProjection() {
  // Projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Replace gluPerspective below with a conditional:
  // if g_proj_mode is PERSP, then use a perspective projection
  // otherwise use an orthogonal projection
  if(g_proj_mode == PERSP)
  {
    gluPerspective(45.0, (GLdouble)g_width/(GLdouble)g_height, 0.1, 20);
  } else
  {
    glOrtho(-10.0f,10.0f,-10.0f,10.0f,-20.0f,20.0f);
  }
  
  
  // Modelview transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setProjection();

  glPushMatrix();

  // Complete
  // Apply the model-view transformation 
  // given the parameters values (g_trans, g_angle, g_scale)
  // and the GL functions glTranslate, glRotate, glScale
  glTranslatef(g_trans[0], g_trans[1], g_trans[2]);

  glRotatef(g_angle[0],1.0f, 0.0f, 0.0f);
  glRotatef(g_angle[1],0.0f,1.0f,0.0f);
  glRotatef(g_angle[2],0.0f,0.0f,1.0f);

  glScalef(g_scale[0], g_scale[1], g_scale[2]);
  
  drawCube();

  glPopMatrix();

  glutSwapBuffers();
}


void reshape(int w, int h) {
  glViewport(0, 0, w, h);

  g_width = w;
  g_height = h;
}


// Increase the rotation angle by amt around ax
void rotate(int ax, GLfloat amt) {
  g_angle[ax] += amt;
}


// Increase the translation by amt along ax
void translate(int ax, GLfloat amt) {
  g_trans[ax] += amt;
}


// Multiply the scaling factor by amt along ax
void scale(int ax, GLfloat amt) {
  g_scale[ax] *= amt;
}


void keyboard(unsigned char k, int x, int y) 
{
  switch (k) {
  case 27:
    exit(EXIT_SUCCESS);
    break;

  case 'p':
    g_proj_mode = (1 - g_proj_mode);
    break;

  case 't':
  case 'T':
    g_mode = TRANS;
    break;

  case 'r':
  case 'R':
    g_mode = ROT;
    break;

  case 's':
  case 'S':
    g_mode = SCALE;
    break;

  case 'x':
    if (g_mode == TRANS) translate(0, -0.5f);
    if (g_mode == ROT) rotate(0, -5.0f);
    if (g_mode == SCALE) scale(0, 0.9f);
    break;

  case 'X':
    if (g_mode == TRANS) translate(0, 0.5f);
    if (g_mode == ROT) rotate(0, 5.0f);
    if (g_mode == SCALE) scale(0, 1.1f);
    break;

  case 'y':
    if (g_mode == TRANS) translate(1, -0.5f);
    if (g_mode == ROT) rotate(1, -5.0f);
    if (g_mode == SCALE) scale(1, 0.9f);
    break;

  case 'Y':
    if (g_mode == TRANS) translate(1, 0.5f);
    if (g_mode == ROT) rotate(1, 5.0f);
    if (g_mode == SCALE) scale(1, 1.1f);
    break;

  case 'z':
    if (g_mode == TRANS) translate(2, -0.5f);
    if (g_mode == ROT) rotate(2, -5.0f);
    if (g_mode == SCALE) scale(2, 0.9f);
    break;

  case 'Z':
    if (g_mode == TRANS) translate(2, 0.5f);
    if (g_mode == ROT) rotate(2, 5.0f);
    if (g_mode == SCALE) scale(2, 1.1f);
    break;

  default:
    break;
  }

  glutPostRedisplay();
}


void init(void) {
  glClearColor(0.5, 0.5, 0.5, 1.0);
  glEnable(GL_DEPTH_TEST);
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Cube");

  init();
  
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}