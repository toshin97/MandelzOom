/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/
/*  Rubber Bands XOR Demo
/* Prepared By: Tahsin Oshin                             */
/*--------------------------------------------------------*/

#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <iostream>  //Text IO for debugging. 
#include <list>	     //A linked list class.
#include <GL/glut.h> //Glut

using namespace std;

// Defining default values for window size, shape and location.
#define INITIAL_WIN_W 800
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 150
#define INITIAL_WIN_Y 50


// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight, windowWidth;

//variables that define a rectangle in a complex plane
double x_1, x_2, y_1, y_2;

struct Rectangle { //rectangle struct to hold the 4 coordinates
	double xmin;
	double ymin;
	double xmax;
	double ymax;

	Rectangle(double xmn, double ymn, double xmx, double ymx)
		: xmin(xmn), ymin(ymn), xmax(xmx), ymax(ymx)
	{}
};

list<Rectangle*>  rectList; //declaring a list of pointers to rectangle
list<Rectangle*>::iterator  rectIter; //iterate to iterate throught the list




void displayPicture()
{
	int h = windowHeight;
	int w = windowWidth;

	glBegin(GL_POINTS);
	for (int u = 0; u < windowWidth; u++) { //for each pixel u iterating to right along WindowWidth
		for (int v = 0; v < windowHeight; v++) { ////for each pixel v iterating down along WindowHeight

			double comX = x_1 + u * ((x_2 - x_1) / (w - 1));
			double comY = y_1 + v * ((y_2 - y_1) / (h - 1)); // to complex no. s
			
			double zX = 0.0;
			double zY = 0.0; // initial base complex z0 = 0 + 0i

			int limit; 
			double gradient = 0.0;

			for (int i = 1; i <= 1000; i++) { // loop of intervals returning max int for which com does not 
												//belong to Madelbrot set 
				double zsqX = (zX * zX) - (zY * zY);
				double zsqY = 2.0 * zX * zY; //squares base 

				zX = zsqX + comX; // adds base to s (complex no. calculated from (u,v) coordinate)
				zY = zsqY + comY;

				double mag = zX * zX + zY * zY;
				if (mag > 4.0) { //if z NOT a member of M.set, 
					limit = i;
					gradient = (double)limit / 1000.0;
					break;
				}


				else if (i == 1000) {
					limit = 1000;
					
				}
			}

			if (limit == 1000) {

				glColor3f(0.0, 0.0, 0.0); //set pixel to black if mag <= 4
				glVertex2i(u, v);
			}
			 
			else {
				glColor3f(0.5, 0.0, 3.0); //set pixel to black if mag <= 4
				glVertex2i(u, v);
			} 

			
						
		}
	}


	glEnd();
	glFlush();
}

	

void drawLine(int xOld, int yOld, int xNew, int yNew)
// Draw a line from (xOld,yOld) to (xNew,yNew).
{
	glBegin(GL_LINE);
	glVertex2i(xOld, yOld);
	glVertex2i(xNew, yNew);
	glEnd();
	glFlush();
}



void drawRubberBand(int xA, int yA, int xS, int yS)
{
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);
	glBegin(GL_LINE_LOOP);
	glVertex2i(xA, yA);
	glVertex2i(xS, yA);
	glVertex2i(xS, yS);
	glVertex2i(xA, yS);
	glEnd();
	glDisable(GL_COLOR_LOGIC_OP);
	glFlush();
}

void rubberBand(int x, int y)
// Callback for processing mouse motion.
{
	if (rubberBanding)
	{
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		y = windowHeight - y;
		xStretch = x;
		yStretch = y;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		glFlush();
	}
}

 void push() { //zoom in
	 rectIter++;
	 if (rectIter == rectList.end()) {
		 rectIter--;
		 return;
	 }
	


	 Rectangle* rectPointer = *rectIter;
	 x_1 = rectPointer->xmin;
	 y_1 = rectPointer->ymin;
	 x_2 = rectPointer->xmax;
	 y_2 = rectPointer->ymax;

	 glutPostRedisplay();
	 glFlush();
	
} 

void pop() { // zoom-out 
	
	if (rectIter == rectList.begin())
		return; 
	rectIter--;
	Rectangle* rectPointer = *rectIter;
	x_1 = rectPointer->xmin;
	y_1 = rectPointer->ymin;
	x_2 = rectPointer->xmax;
	y_2 = rectPointer->ymax;

		glutPostRedisplay();
		glFlush();		
}


void reshape(int w, int h)
// Callback for processing reshape events.
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}


void escExit(GLubyte key, int, int)
// Callback for processing keyboard events.
{
	if (key == 27 /* ESC */) std::exit(0);
}


void mainMenu(int item)
// Callback for processing main menu.
{
	switch (item)
	{
		case 1: push(); break;
		case 2: pop(); break;
		case 3: {
			//rectList.erase();
			std::exit(0);
	    }
	}
}



void setMenus()
// Function for creating menus.
{
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Push", 1);
	glutAddMenuEntry("Pop", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}


void processLeftDown(int x, int y)
// Function for processing mouse left botton down events.
{
	if (!rubberBanding)
	{
		int xNew = x;
		int yNew = windowHeight - y;
		xAnchor = xNew;
		yAnchor = yNew;
		xStretch = xNew;
		yStretch = yNew;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		rubberBanding = true;
	}
}



void processLeftUp(int x, int y)
// Function for processing mouse left botton up events.
{
	if (rubberBanding)
	{   
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		rubberBanding = false;
		

		xStretch = x;
		yStretch = windowHeight - y;
		
		double comXAnchor = x_1 + xAnchor * ((x_2 - x_1) / (windowWidth - 1));
		double comYAnchor = y_1 + yAnchor * ((y_2 - y_1) / (windowHeight - 1)); 
		double comXStretch = x_1 + xStretch * ((x_2 - x_1) / (windowWidth - 1));   //converting to
		double comYStretch = y_1 + yStretch * ((y_2 - y_1) / (windowHeight - 1)); // complex nos.

		x_1 = comXAnchor;
		y_1 = comYStretch;
		x_2 = comXStretch;
		y_2 = comYAnchor; //re-initializing the initial coordinates

		rectIter++;
		rectList.erase(rectIter, rectList.end());
		rectIter = rectList.end();
		rectIter--;

		/*if ((rectIter != rectList.begin) && (rectIter != rectList.end)) {
		
			rectList.erase(rectIter, rectList.end());
		}*/

		Rectangle *newRect = new Rectangle(x_1, y_1, x_2, y_2);
		 //assisigning complex nos. to create new rect struct 
		rectList.push_back(newRect);
		rectIter++;

		glutPostRedisplay(); //redisplay with new complex nos.
		cout << "(" << x_1 << "," << y_1 << "," << x_2 << "," << y_2 << ")" << endl;
								//display zoom coordinates
		glFlush();
	}
}


void mouse(int button, int state, int x, int y)
// Function for processing mouse events.
{
	if (button == GLUT_LEFT_BUTTON)
		switch (state)
		{
		case GLUT_DOWN: processLeftDown(x, y); break;
		case GLUT_UP: processLeftUp(x, y); break;
		}
}



int main(int argc, char * argv[])
{
	// Mask floating point exceptions.
	_control87(MCW_EM, MCW_EM);

	// Initialize glut with command line parameters.
	glutInit(&argc, argv);

	// Choose RGB display mode for normal screen window.
	glutInitDisplayMode(GLUT_RGB);

	// Set initial window size, position, and title.
	glutInitWindowSize(INITIAL_WIN_W, INITIAL_WIN_H);
	glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
	windowWidth = INITIAL_WIN_W;
	windowHeight = INITIAL_WIN_H;
	glutCreateWindow("Rubber Bands Demo");

	// You don't (yet) want to know what this does.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)INITIAL_WIN_W, 0.0, (double)INITIAL_WIN_H), glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375, 0.375, 0.0);



	// Set the callbacks for the normal screen window.
	glutDisplayFunc(displayPicture);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(escExit);
	glutMotionFunc(rubberBand);
	glutPassiveMotionFunc(rubberBand);

	// Set the drawing color.
	glColor3f(1.0, 1.0, 1.0);

	// Set the color for clearing the window.
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Set up the menus.
	setMenus();

	x_1 = -2.0;
	x_2 = 0.5;
	y_1 = -1.25;
	y_2 = 1.25;

	Rectangle *myRect = new Rectangle(x_1, y_1, x_2, y_2); //initializing base rectangle with the coordinates
	 
	 rectList.push_back(myRect);
	 rectIter = rectList.begin(); //initializing iterator to be at start of rect list

	glutMainLoop();
	return 0;
}


