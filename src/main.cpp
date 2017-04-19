 /*
  * main.cpp
  *
  * Created on: 20 Feb 2017
  * Author    : Noni Hua
  *
  * Brief     : Main function that runs markless augmented reality
  *             Will call function from a finite-state machine and execute
  *             Scheme: Detections -> Tracking -> Displaying
  */

#include <stdio.h>
#include <stdlib.h>
#include "Detections.cpp"

#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

// camera
#include <cmath>
#include <eigen/Eigen/Dense>

// mesh
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>
#include <cmath>
#include <vector>
#include <eigen/Eigen/Dense>

using namespace std;
using namespace opencv_handler;


// Globals.
static int objID = 1; // Object ID.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate objects.
static char *objName; // Pointer to object name.

/* create image processing instance*/
Detections mydetector;

// reference: https://r3dux.org/2012/01/how-to-convert-an-opencv-cvmat-to-an-opengl-texture/
// Function turn a cv::Mat into a texture, and return the texture ID as a GLuint for use
GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
	// Generate a number for our textureID's unique handle
	GLuint textureID;
	glGenTextures(1, &textureID);

	// Bind to our texture handle
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Catch silly-mistake texture interpolation method for magnification
	if (magFilter == GL_LINEAR_MIPMAP_LINEAR  ||
	    magFilter == GL_LINEAR_MIPMAP_NEAREST ||
	    magFilter == GL_NEAREST_MIPMAP_LINEAR ||
	    magFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << endl;
		magFilter = GL_LINEAR;
	}

	// Set texture interpolation methods for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_BGR;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
	}

	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
	             0,                 // Pyramid level (for mip-mapping) - 0 is the top level
	             GL_RGB,            // Internal colour format to convert to
	             mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
	             mat.rows,          // Image height i.e. 480 for Kinect in standard mode
	             0,                 // Border width in pixels (can either be 1 or 0)
	             inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
	             GL_UNSIGNED_BYTE,  // Image data type
	             mat.ptr());        // The actual image data itself

	// If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR  ||
	    minFilter == GL_LINEAR_MIPMAP_NEAREST ||
	    minFilter == GL_NEAREST_MIPMAP_LINEAR ||
	    minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return textureID;
}

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{
   char *c;

   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Routine to write object name.
void writeObjectName()
{
   glPushMatrix();
   glTranslatef(-4.5, 10.0, -20.0);
   glScalef(0.01, 0.01, 0.01);
   writeStrokeString(GLUT_STROKE_ROMAN, objName);
   glPopMatrix();
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // Position the objects for viewing.
   gluLookAt(0.0, 3.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glLineWidth(2.0); // Thicken the wireframes.

   // Commands to turn the objects.
   glPushMatrix();
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Draw objects.
   switch(objID)
   {
      case 1:
         glutSolidSphere(5.0, 40, 40);
		 objName = "Solid Sphere";
         break;
      case 2:
         glutSolidCube(7.0);
		 objName = "Solid Cube";
         break;
      case 3:
         glutSolidCone(3.0, 8.0, 30, 30);
		 objName = "Solid Cone";
         break;
      case 4:
         glutSolidTorus(1.0, 4.0, 30, 30);
		 objName = "Solid Torus";
         break;
      case 5:
		 glScalef(3.0, 3.0, 3.0);
         glutSolidDodecahedron();
		 objName = "Solid Dodecahedron";
         break;
      case 6:
		 glScalef(5.0, 5.0, 5.0);
         glutSolidOctahedron();
		 objName = "Solid Octahecron";
         break;
      case 7:
		 glScalef(6.0, 6.0, 6.0);
         glutSolidTetrahedron();
		 objName = "Solid Tetrahedron";
         break;
      case 8:
		 glScalef(5.0, 5.0, 5.0);
         glutSolidIcosahedron();
		 objName = "Solid Icosahedron";
         break;
      case 9:
         glutSolidTeapot(4.0);
		 objName = "Solid Teapot";
         break;
      default:
		 break;
   }
   glPopMatrix();

  //  // Write label after disabling lighting.
  //  glDisable(GL_LIGHTING);
  //  glColor3f(0.0, 0.0, 0.0);
  //  writeObjectName();
  //  glEnable(GL_LIGHTING);


   // enabling background image capture
  glEnable(GL_TEXTURE_2D);
  float w = 6.4f;
  float h = 4.8f;
  GLuint imageTex = matToTexture(mydetector.img, GL_LINEAR_MIPMAP_LINEAR,   GL_LINEAR, GL_CLAMP);
  GLuint depthTex = matToTexture(mydetector.img, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);

  // Front facing texture
 glBindTexture(GL_TEXTURE_2D, imageTex);
 glBegin(GL_QUADS);
  glTexCoord2f(1, 1);
  glVertex2f(-w/2,  -h/2);
  glTexCoord2f(1, 0);
  glVertex2f(-w/2, h/2);
  glTexCoord2f(0, 0);
  glVertex2f(w/2, h/2);
  glTexCoord2f(0, 1);
  glVertex2f(w/2,-h/2);
 glEnd();

 // // Back facing texture (facing backward because of the reversed the vertex winding)
 // glBindTexture(GL_TEXTURE_2D, depthTex);
 // glBegin(GL_QUADS);
 //   glTexCoord2f(1, 1);
 //   glVertex2f(w/2,  h/2);
 //   glTexCoord2f(1, 0);
 //   glVertex2f(-w/2, h/2);
 //   glTexCoord2f(0, 0);
 //   glVertex2f(-w/2, -h/2);
 //   glTexCoord2f(0, 1);
 //   glVertex2f( w/2,  -h/2);
 // glEnd();

   glDeleteTextures(1, &imageTex);
   glDeleteTextures(1, &depthTex);
   glDisable(GL_TEXTURE_2D);

   glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
  //  // Material property vectors.
  //  float matSpec[] = { 0.0, 1.0, 1.0, 1.0 };
  //  float matShine[] = { 50.0 };
  //  float matAmbAndDif[] = {0.0, 0.1, 1.0, 1.0};
   //
  //  // Light property vectors.
  //  float lightAmb[] = { 0.0, 0.1, 1.0, 1.0 };
  //  float lightDifAndSpec[] = { 0.0, 0.1, 1.0, 1.0 };
  //  float lightPos[] = { 0.0, 7.0, 3.0, 0.0 };
  //  float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
   //
  //  // Material properties of the objects.
  //  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
  //  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
  //  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matAmbAndDif);
   //
  //  // Light0 properties.
  //  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
  //  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
  //  glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
  //  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   //
  //  // Poperties of the ambient light.
  //  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   //
  //  glEnable(GL_LIGHTING); // Enable lighting calculations.
  //  glEnable(GL_LIGHT0); // Enable particular light source.
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   glEnable(GL_NORMALIZE); // Enable automatic normalization of normals.

   glClearColor(1.0, 1.0, 1.0, 1.0);
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27:
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_DOWN)
   {
      if (objID > 1) objID--;
      else objID = 9;
   }
   if(key == GLUT_KEY_LEFT)
   {
      if (objID > 1) objID--;
      else objID = 9;
   }
   if(key == GLUT_KEY_UP)
   {
      if (objID < 9) objID++;
      else objID = 1;
   }
   if(key == GLUT_KEY_RIGHT)
   {
      if (objID < 9) objID++;
      else objID = 1;
   }

   Xangle = Yangle = Zangle = 0.0;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the arrow keys to cycle through the objects." << endl
        << "Press x, X, y, Y, z, Z to turn the objects." << endl;
}


// Main routine.
int main(int argc, char **argv)
{

  if (argc != 2)
  {
    cout << "Usage: node <video input>" << endl;
    return -1;
  }


  mydetector.imgStreamIn(atoi(argv[1]));
  if (mydetector.detected)
  {
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("glutObjects.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
  }


}
