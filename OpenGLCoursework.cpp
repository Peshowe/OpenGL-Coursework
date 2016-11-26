#ifdef __APPLE__
#include <OpenGL/gl.h>  // The GL header file.
#include <GLUT/glut.h>  // The GL Utility Toolkit (glut) header.
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>      // The GL header file.
#include "glut.h"       // The GL Utility Toolkit (glut) header (boundled with this program).
#endif

#include <math.h>       // For mathematic operations.

// Global variable for current rendering mode.
char rendermode;

//Camera variables
int cameraX = 5;
int cameraY = 5;
int cameraZ = 10;

//Cube rotation variables
float rotqubeX = 0;
float rotqubeY = 0;
float rotqubeZ = 0;

// Scene initialisation.
void InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH);               // Enable smooth shading.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background.
	glClearDepth(1.0f);                    // Depth buffer setup.
	glEnable(GL_DEPTH_TEST);               // Enables depth testing.
	glDepthFunc(GL_LEQUAL);                // The type of depth testing to do.
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void idle(void)
{
	glutPostRedisplay();  // Trigger display callback.
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Set the camera.
	gluLookAt(cameraX, cameraY, cameraZ,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	GLfloat pos[4] = { 0.00, 0.10, 0.20, 1.00 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);

	//Cartesian coordinate system as lines.
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 3.0f);
	glVertex3f(-1.0f, -1.0f, 4.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 3.0f);
	glVertex3f(-1.0f, 0.0f, 3.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 3.0f);
	glVertex3f(0.0f, -1.0f, 3.0f);
	glEnd();

	glRotatef(rotqubeX, 1.0f, 0.0f, 0.0f);	// Rotate The cube around the X axis
	glRotatef(rotqubeY, 0.0f, 1.0f, 0.0f);	// Rotate The cube around the Y axis
	glRotatef(rotqubeZ, 0.0f, 0.0f, 1.0f);  // Rotate The cube around the Z axis

	// Different render modes.
	switch(rendermode) {

		case 'f': // to display faces
		{
			
			glShadeModel(GL_FLAT);
			glBegin(GL_QUADS);
			glColor3f(1.0f, 5.0f, 0.0f);
			
			glNormal3f(0.3f, 0.4f, 0.87f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);

			glNormal3f(0.1f, 0.2f, 0.47f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			
			glNormal3f(0.6f, 0.1f, 0.1f);
			glColor3f(1.0f, 1.0f, 4.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);
			
			glColor3f(0.0f, 0.0f, 3.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);

			glColor3f(0.0f, 1.0f, 2.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);

			glEnd();
			break;
		}

		case 'v': // to display points
		{
			
			glBegin(GL_POINTS);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);

			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);

			glEnd();

			glPointSize(5);
			break;
		}

		case 'e': // to display edges
		{
			glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 1.0f);

			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);

			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);

			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);

			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);

			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);

			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			
			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);

			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
	
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			
			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);

			glEnd();
			break;
		}
	}

	glutSwapBuffers();
}


// The reshape function sets up the viewport and projection.
void reshape(int width, int height)
{
	// Prevent a divide by zero error by making height equal to 1
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Need to calculate the aspect ratio of the window for gluPerspective.
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	// Return to ModelView mode for future operations.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Callback for standard keyboard presses.
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// Exit the program when escape is pressed
		case 27:
			exit(0);
			break;

			// Switch render mode.
		case 'v': rendermode = 'v'; break;  // vertices
		case 'e': rendermode = 'e'; break;  // edges
		case 'f': rendermode = 'f'; break;  // faces
		case 'w': cameraZ--; break; // zoom in
		case 's': cameraZ++; break; // zoom out
		case 'a': cameraX--; break;
		case 'd': cameraX++; break;
		case 'x': rotqubeX += 1.0f;  break;  // rotate cube around X axis
		case 'y': rotqubeY += 1.0f;  break;  // rotate cube around Y axis
		case 'z': rotqubeZ += 1.0f;  break;  // rotate cube around Z axis

		default:
			break;
	}

	glutPostRedisplay();
}


// Arrow keys need to be handled in a separate function from other keyboard presses.
void arrow_keys(int a_keys, int x, int y)
{
	switch (a_keys)
	{
		case GLUT_KEY_UP:
			glutFullScreen();
			break;

		case GLUT_KEY_DOWN:
			glutReshapeWindow(500, 500);
			break;

		default:
			break;
	}
}


// Handling mouse button event.
void mouseButton(int button, int state, int x, int y)
{

}


// Handling mouse move events.
void mouseMove(int x, int y)
{
	int newX = x - cameraX;
	int newY = y - cameraY;
	if (abs(newX) < 30){
		cameraX = newX;
	}
	if (abs(newY) < 30) {
		cameraY = newY;
	}
	
	
	
}


// Note: You may wish to add interactivity like clicking and dragging to move the camera.
//       In that case, please use the above functions.


// Entry point to the application.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("CM20219 OpenGL Coursework");
	//glutFullScreen();  // Uncomment to start in full screen.
	InitGL();
	rendermode = 'f';

	// Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);  // For special keys
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);

	glutMainLoop();
}
