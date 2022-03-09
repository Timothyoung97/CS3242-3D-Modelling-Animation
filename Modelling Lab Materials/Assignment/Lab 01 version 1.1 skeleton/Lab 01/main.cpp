// CS3241Lab1.cpp : Defines the entry point for the console application.
#ifdef _WIN32
#include <Windows.h>
#include "GL\glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif
#include <iostream>
#include "mesh.h"
#define NO_OBJECT 10;

using std::cout;
using std::endl;

//#define M_PI 3.141592654

myObjType myObj; // object to be drawn

// global variable
bool toggleSmooth = FALSE; 				// flag for smooth/flat shading
bool toggleEdge = FALSE;				// flag for edge visibility
bool toggleHighlight = FALSE;			// flag for highlighting
bool toggleColor = FALSE;				// flag for color
int switchSubdivLoopBetaVersion = 1; 	// flag for switching between beta and alpha version of subdivision loop

GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;

int current_object = 0;

using namespace std;

void setupLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
    GLfloat	ambientProperties[]  = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat	diffuseProperties[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPosition[] = {-100.0f,100.0f,100.0f,1.0f};
	
    glClearDepth( 1.0 );

	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition);
	
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
    glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}



void display(void)
{
	float mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_ambient_color[] = { 0.8f, 0.8f, 0.2f, 1.0f };
	float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float shininess = 20;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
		glRotatef(angle2, 1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);
		glScalef(zoom, zoom, zoom);
		myObj.draw(toggleSmooth, toggleEdge, toggleColor);
	glPopMatrix();
	glutSwapBuffers();
}




void keyboard (unsigned char key, int x, int y)
{
	char filename[256];
	switch (key) {
	case 'e':
	case 'E':
		toggleEdge = !toggleEdge;
		break;
	case 'c':
	case 'C':
		toggleColor = !toggleColor;
		break;
	case 'l':
	case 'L':
		myObj.loopSubdivide(switchSubdivLoopBetaVersion);
		break;
	case '1':
		cout << "Loop subdivison beta changed to Warran" << endl;
		switchSubdivLoopBetaVersion = 1;
		break;
	case '2':
		cout << "Loop subdivison beta changed to Loop[16]" << endl;
		switchSubdivLoopBetaVersion = 2;
		break;
	case 'b':
	case 'B':
		myObj.barycentricSubdivide();
		break;
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 's':
	case 'S':
		toggleSmooth = !toggleSmooth;
		break;
	case 'h':
	case 'H':
		toggleHighlight = !toggleHighlight;
		break;
	case 'o':
	case 'O':
	{
		cout << "Enter the filename you want to write:";
		std::cin >> filename;
		myObj.writeFile(filename);
		break;
	}
	case 'Q':
	case 'q':
		exit(0);
	case 'r':
	case 'R':
	{
		std::string filename;
		cout << "Enter the filename of the file you want to open (including file extension):";
		std::cin >> filename;
		myObj.readFile(filename);
		break;
	}
	default:
	break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
	mouseButton = button;
    moving = 1;
    startx = x;
    starty = y;
  }
  if (state == GLUT_UP) {
	mouseButton = button;
    moving = 0;
  }
}

void motion(int x, int y)
{
  if (moving) {
	if(mouseButton==GLUT_LEFT_BUTTON)
	{
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
	}
	else zoom += ((y-starty)*0.001);
    startx = x;
    starty = y;
	glutPostRedisplay();
  }
  
}

int main(int argc, char **argv)
{
	char filename[255];
	cout<<"CS3242 "<< endl<< endl;

	cout << "Enter the filename of the file you want to open (including file extension):";
	cin >> filename;
	myObj.readFile(filename);

	cout << "S: Toggle Smooth Shading" << endl;
	cout << "H: Toggle Highlight" << endl;
	cout << "L: Loop Subdivision. Switching beta-version by pressing key 1 (Warran) or 2 (Loop[16])" << endl;
	cout << "C: Color Components" << endl;
	cout << "R: Read specific .obj or .off file" << endl;
	cout << "B: Barycentric Subdivision" << endl;
	cout << "E: Draw Edges" << endl;
	cout << "W: Draw Wireframe" << endl;
	cout << "P: Draw Polygon" << endl;
	cout << "O: Write object to file" << endl;
	cout << "V: Draw Vertices" << endl;
	cout << "Q: Quit" << endl << endl;

	cout << "Left mouse click and drag: rotate the object" << endl;
	cout << "Right mouse click and drag: zooming" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 3");
	glClearColor (1.0,1.0,1.0, 1.0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); 
	glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, 
		/* Z far */ 80.0);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();

	return 0;
}
