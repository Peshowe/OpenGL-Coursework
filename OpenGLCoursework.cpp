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
#include <cstdio>
#include <TextureLoader.h> //For loading an image for the texture mapping
#include "objloader.hpp"
#include <array>
#include <vector>

// Global variable for current rendering mode.
char rendermode;
 
//Camera variables
int cameraX = 5;
int cameraY = 5;
int cameraZ = 10;
int centerZ = 0;
int centerX = 0;
int centerY = 0;
int oldX = 0;
int oldY = 0;

//Cube rotation variables
float rotqubeX = 0;
float rotqubeY = 0;
float rotqubeZ = 0;

//Specifying cube material variables
GLfloat material_Ka[] = { 0.11, 0.06, 0.11, 1.00 };
GLfloat material_Kd[] = { 0.43, 0.47, 0.54, 1.00 };
GLfloat material_Ks[] = { 0.33, 0.33, 0.52, 1.00 };
GLfloat material_Ke[] = { 0.00, 0.00, 0.00, 0.00 };
GLfloat material_Se[] = { 10 };

//Texture mapping variables
GLuint g_textureID[1];
unsigned char* image = NULL;
int iheight, iwidth;

//For loading the meshes
std::vector<std::array<float, 3>> vertices;
std::vector<std::array<int, 3>> vertexIndices;

/*
	Scalling the vertices of the imported meshes to fit in the cube
*/
void normaliseVectors() {
	int i;
	float maxX = vertices[0][0];
	float maxY = vertices[0][1];
	float maxZ = vertices[0][2];
	float minX = vertices[0][0];
	float minY = vertices[0][1];
	float minZ = vertices[0][2];
	
	for (i = 0; i < vertices.size(); i++) {
 		if (maxX < vertices[i][0]) maxX = vertices[i][0];
		if (maxY < vertices[i][1]) maxY = vertices[i][1];
		if (maxZ < vertices[i][2]) maxZ = vertices[i][2];
		if (minX > vertices[i][0]) minX = vertices[i][0];
		if (minY > vertices[i][1]) minY = vertices[i][1];
		if (minZ > vertices[i][2]) minZ = vertices[i][2];
	}

	float min = min(min(minX, minZ), minY);
	float range = max(max(maxX-minX, maxZ-minZ), maxY-minY);
	
	//Normalise to [0,1]
	for (i = 0; i < vertices.size(); i++) {
		vertices[i][0] = (vertices[i][0] - minX) / (range);
		vertices[i][1] = (vertices[i][1] - minY) / (range);
		vertices[i][2] = (vertices[i][2] - minZ) / (range);
	}

	//Scale to [-1,1]
	for (i = 0; i < vertices.size(); i++) {
		vertices[i][0] = (vertices[i][0] * 2) - 1;
		vertices[i][1] = (vertices[i][1] * 2) - 1;
		vertices[i][2] = (vertices[i][2] * 2) - 1;
	}

}


// Scene initialisation.
void InitGL(GLvoid)
{
	glShadeModel(GL_FLAT);                 // Enable flat shading.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background.
	glClearDepth(1.0f);                    // Depth buffer setup.
	glEnable(GL_DEPTH_TEST);               // Enables depth testing.
	glDepthFunc(GL_LEQUAL);                // The type of depth testing to do.
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Read an image.
	image = glmReadPPM("mandrill.ppm", &iwidth, &iheight);
	// Create a texture object with an unused texture ID.
	glGenTextures(1, &g_textureID[0]);
	// Set g_textureID as the current 2D texture object.
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);
	// Set the loaded image as the current texture image.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// Specify what to do when s, t are outside range [0, 1].
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Specify how to interpolate texture colour values
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	load_obj("screwdriver.obj", vertices, vertexIndices);
	normaliseVectors();
	
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
	
	//gluPerspective(45.0f, aspect, 0.1f, 100.0f);
	gluLookAt(cameraX, cameraY, cameraZ,
		centerX, centerY, centerZ,
		0.0f, 1.0f, 0.0f);

	//Enable texturing
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

	//Turn on the lights
	GLfloat pos[4] = { 0.00, 1.00, 3.00, 0.00 };
	GLfloat pos2[4] = { 1.50, 1.00, 1.00, 0.00 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	

	//Cartesian coordinate system as lines.
	glBegin(GL_LINES);
	glColor3f(0.4f, 0.5f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 3.0f);
	glVertex3f(-1.0f, -1.0f, 4.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 3.0f);
	glVertex3f(-1.0f, 0.0f, 3.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 3.0f);
	glVertex3f(0.0f, -1.0f, 3.0f);
	glEnd();

	glRotatef(rotqubeX, 1.0f, 0.0f, 0.0f);	// Rotate The cube around the X axis
	glRotatef(rotqubeY, 0.0f, 1.0f, 0.0f);	// Rotate The cube around the Y axis
	glRotatef(rotqubeZ, 0.0f, 0.0f, 1.0f);  // Rotate The cube around the Z axis

	// Different render modes.
	switch (rendermode) {

	case 'f': // to display faces
	{
		glBegin(GL_QUADS);

		//Set materials
		glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
		glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
		glMaterialfv(GL_FRONT, GL_SHININESS, material_Se);

		//front
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0, 0);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//back
		glNormal3f(0.0f, 0.0f, -1.00f);
		glTexCoord2f(0.1, 0.5);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.1, 0.0);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.4, 0.0);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.4, 0.6);
		glVertex3f(1.0f, -1.0f, -1.0f);

		//top
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.3, 0.9);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.3, 0.2);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.4, 0.2);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.4, 0.9);
		glVertex3f(1.0f, 1.0f, -1.0f);

		//bottom
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.6, 0.7);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.6, 0.5);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0, 0.5);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0, 0.7);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		//right
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.7, 0.3);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.6, 0.1);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0, 0.1);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0, 0.3);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//left
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.5, 1.0);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0, 0.5);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1);
		glVertex3f(-1.0f, -1.0f, 1.0f);


		glEnd();
		break;
	}
	case 'b':
	{
		glBegin(GL_TRIANGLES);

		//Specify materials
		glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
		glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
		glMaterialfv(GL_FRONT, GL_SHININESS, material_Se);

		int i;
		int size = vertexIndices.size();
		for (i = 0; i < size; i++) {

			//Get the vertex indices for each point of each triangle
			int one = vertexIndices[i][0] - 1;
			int two = vertexIndices[i][1] - 1;
			int three = vertexIndices[i][2] - 1;

			//Calculate the surface normal for each triangle by finding the cross product
			float v[] = { vertices[two][0] - vertices[one][0], vertices[two][1] - vertices[one][1], vertices[two][2] - vertices[one][2] };
			float w[] = { vertices[three][0] - vertices[one][0], vertices[three][1] - vertices[one][1], vertices[three][2] - vertices[one][2] };
			
			float nx = (v[1] * w[2]) - (v[2] * w[1]);
			float ny = (v[2] * w[0]) - (v[0] * w[2]);
			float nz = (v[0] * w[1]) - (v[1] * w[0]);

			float ax = nx / (abs(nx) + abs(ny) + abs(nz));
			float ay = ny / (abs(nx) + abs(ny) + abs(nz));
			float az = nz / (abs(nx) + abs(ny) + abs(nz));

			glNormal3f(ax, ay, az);
			//Draw each triangle
			glVertex3f(vertices[one][0], vertices[one][1], vertices[one][2]);
			glVertex3f(vertices[two][0], vertices[two][1], vertices[two][2]);
			glVertex3f(vertices[three][0], vertices[three][1], vertices[three][2]);
		}


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

			
			glBegin(GL_POINTS);

			//Display the points of the loaded mesh
			int i;
			for (i = 0; i < vertices.size(); i++) {
				glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
			}

			glEnd();

			glPointSize(5);
			break;
		}

		case 'e': // to display edges
		{
			
			glBegin(GL_LINES);
			glColor3f(0.4f, 0.5f, 1.0f);

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

			//Display the edges of the loaded mesh
			int i;
			int size = vertexIndices.size();
			for (i = 0; i < size; i++) {

				int one = vertexIndices[i][0] - 1;
				int two = vertexIndices[i][1] - 1;
				int three = vertexIndices[i][2] - 1;

				glVertex3f(vertices[one][0], vertices[one][1], vertices[one][2]);
				glVertex3f(vertices[two][0], vertices[two][1], vertices[two][2]);
				
				glVertex3f(vertices[two][0], vertices[two][1], vertices[two][2]);
				glVertex3f(vertices[three][0], vertices[three][1], vertices[three][2]);

				glVertex3f(vertices[one][0], vertices[one][1], vertices[one][2]);
				glVertex3f(vertices[three][0], vertices[three][1], vertices[three][2]);
			}
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
		case 'b': rendermode = 'b'; break;  // meshes faces
		case 'w': cameraZ--; break; //camera translation + rotation
		case 's': cameraZ++; break; //camera translation + rotation
		case 'a': cameraX--; centerX--; break; //camera translation (modifies the variables such that it doesn't rotate the view)
		case 'd': cameraX++; centerX++; break; //camera translation (modifies the variables such that it doesn't rotate the view)
		case 'm': cameraY++; centerY++; break; //camera translation (modifies the variables such that it doesn't rotate the view)
		case 'n': cameraY--; centerY--; break; //camera translation (modifies the variables such that it doesn't rotate the view)
		case 'x': rotqubeX += 1.0f; if (rotqubeX == 360.00) rotqubeX = 0.00; break;  // rotate cube around X axis
		case 'y': rotqubeY += 1.0f; if (rotqubeY == 360.00) rotqubeY = 0.00; break;  // rotate cube around Y axis
		case 'z': rotqubeZ += 1.0f; if (rotqubeZ == 360.00) rotqubeZ = 0.00; break;  // rotate cube around Z axis
		case 'r': rotqubeX = 0; rotqubeY = 0; rotqubeZ = 0; break; // reset the position of the cube

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
	oldX = x;
	oldY = y;
}


// Handling mouse move events.
void mouseMove(int x, int y)
{
	//rotating camera
	int newX = centerX - (oldX - x);
	int newY = centerY + (oldY - y);
	if (abs(newX) <= 40) {
		centerX = newX;
	}
	if (abs(newY) <= 40) {
		centerY = newY;
	}
		printf("x: %d y: %d \n", centerX, centerY);
		oldX = x;
		oldY = y;
}


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
