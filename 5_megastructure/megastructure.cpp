#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>

#define pi 2*acos(0)

#define BASESCALE 50
#define INNERBASESCALE 47
#define PINK_WALK_WAY 44
#define WATER_BORDER 38

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
		}glEnd();
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);{
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			/*glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);*/
		}glEnd();
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);{
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
		    cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
		    cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){
                drawgrid = 1 - drawgrid;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void DRAW_BASEMENT()
{
    double zindex = 0.0;
    for(int i=0; i<20; i++)
    {
        glPushMatrix();{
            glTranslatef(0,0,zindex);
            glPushMatrix();{
                 glColor3ub(0,128+(i*3),i);
                 glScalef(BASESCALE-(i*0.7), BASESCALE-(i*0.7),0.0);
                 glutSolidTetrahedron();
            }glPopMatrix();
            glPushMatrix();{
                glRotatef(45,0,0,1);
                glColor3ub(0,128+(i*3),i);
                glScalef(BASESCALE-(i*0.7), BASESCALE-(i*0.7),0.0);
                glutSolidTetrahedron();
            }glPopMatrix();
        }glPopMatrix();
        zindex += 0.1;
    }
}

void DRAW_WALKWAY()
{
    double zindex = 0.0;
    double pinkwalk = 0.0;
    for(int i=0; i<20; i++)
    {
        zindex += 0.1;
        pinkwalk = BASESCALE-(i*0.7);
    }

    glPushMatrix();{
        glTranslatef(0,0,zindex);
        glColor3ub(238,162,173);
        glScalef(pinkwalk, pinkwalk,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
    glPushMatrix();{
        glTranslatef(0,0,zindex);
        glRotatef(45,0,0,1);
        glColor3ub(238,162,173);
        glScalef(pinkwalk, pinkwalk,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();

    glPushMatrix();{
        glTranslatef(0,0,zindex + 0.05);
        glColor3ub(0,205,0);
        glScalef(pinkwalk-2, pinkwalk-2,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
    glPushMatrix();{
        glTranslatef(0,0,zindex + 0.05);
        glRotatef(45,0,0,1);
        glColor3ub(0,205,0);
        glScalef(pinkwalk-2, pinkwalk-2,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
}

void DRAW_WATERBODY()
{
    glPushMatrix();{
        glTranslatef(0,0,1.8);
        glColor3ub(238,162,173);
        glScalef(WATER_BORDER, WATER_BORDER,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
    glPushMatrix();{
        glTranslatef(0,0,1.8);
        glRotatef(45,0,0,1);
        glColor3ub(238,162,173);
        glScalef(WATER_BORDER, WATER_BORDER,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
    glPushMatrix();{
        glTranslatef(0,0,1.9);
        glColor3ub(0,197,205);
        glScalef(WATER_BORDER-1, WATER_BORDER-1,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
    glPushMatrix();{
        glTranslatef(0,0,1.9);
        glRotatef(45,0,0,1);
        glColor3ub(0,197,205);
        glScalef(WATER_BORDER-1, WATER_BORDER-1,0.0);
        glutSolidTetrahedron();
    }glPopMatrix();
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


    DRAW_BASEMENT();
    DRAW_WALKWAY();
    //DRAW_WATERBODY();

	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
    drawgrid=0;
	drawaxes=1;
	cameraHeight=120.0;
	cameraAngle=1.0;
	angle=0;
	//codes for initialization
	//clear the screen
	glClearColor(0,0,0,0);
	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(850, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Lotus Temple");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

