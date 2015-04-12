#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/glut.h>

#define g 0.09
#define pi 2*acos(0)

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double hx,hy,hz;
double hangle;
double legangle;
int dir;
bool legmove;

struct point
{
	double x,y,z;
};



void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
    glColor3f(0.0, 1.0, 0.0);	//grey
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


void drawSquare(float a)
{
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-2*a,2);
		glVertex3f(-a,-2*a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawss()
{
	glPushMatrix();{
		glRotatef(angle,0,0,1);
		glTranslatef(75,0,0);
		glRotatef(2*angle,0,0,1);


		glPushMatrix();{
			glRotatef(angle,0,0,1);
			glTranslatef(25,0,0);
			glRotatef(3*angle,0,0,1);
			glColor3f(0.0, 0.0, 1.0);
			drawSquare(5);

		}glPopMatrix();
		glColor3f(1.0, 0.0, 0.0);
		drawSquare(10.0);
	}glPopMatrix();

}

//draws half sphere
void drawsphere(float radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=sqrt(radius*radius-h*h);
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}

	}
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glColor3f(1.0,(double)i/(double)stacks,0.3);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}

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
		    // head looking up
		    //if(hangle < 0) hangle = 0, hx = hy = hz = 0;
		    if(hz == 1) hangle = 0;
		    hy = hz = 0;
		    if(hangle < 45) hangle += 5;
		    hx = 1;
			break;
		case GLUT_KEY_PAGE_DOWN:
            //if(hangle > 0) hangle = 0, hx = hy = hx = 0;
            if(hz == 1) hangle = 0;
            hy = hz = 0;
            if(hangle > -45) hangle -= 5;
            hx = 1;
			break;

		case GLUT_KEY_INSERT:
		    //if(hangle < 0) hangle = 0, hx = hy = hz = 0;
		    if(hx == 1) hangle = 0;
		    hx = hy = 0;
		    if(hangle < 45) hangle += 5;
		    hz = 1;
			break;

		case GLUT_KEY_HOME:
		    //if(hangle > 0) hangle = 0, hx = hy = hx = 0;
		    if(hx == 1) hangle = 0;
		    hx = hy = 0;
            if(hangle > -45) hangle -= 5;
            hz = 1;
			break;
		case GLUT_KEY_END:
            legmove = true;
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
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void DRAWHEAD()
{
    //drawsphere(10,10,10);
    glPushMatrix();{
        glColor3ub(255,0,0);
        glTranslatef(0,0,20);
        glRotatef(hangle,hx,hy,hz);
        glutWireSphere(10,5,5);
    }glPopMatrix();
}
void DRAWBODY()
{
    glPushMatrix();{
        glColor3ub(139,58,58);
        glRotatef(90,1,0,0);
        drawSquare(10);
    } glPopMatrix();
}
void DRAWLEGS()
{
    glPushMatrix();{
        glColor3ub(100,255,255);
        glTranslatef(5,0,-23);
        glRotatef(legangle,1,0,0);
        glutWireCube(7);
    }glPopMatrix();
    glPushMatrix();{
        glColor3ub(100,255,255);
        glTranslatef(-5,0,-23);
        glRotatef(-1*legangle,1,0,0);
        glutWireCube(7);
    }glPopMatrix();
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
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


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

	//glColor3f(1,0,0);

    DRAWBODY();
    DRAWHEAD();
    DRAWLEGS();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
    if(legmove)
    {
        if(dir)
        {
            if(legangle < 30) legangle += 0.1;
            else dir = 0;
        }
        else{
            if(legangle > 0) legangle -= 0.1;
            else dir = 1;
        }
    }
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=120.0;
	cameraAngle=1.0;
	angle=0;
	hangle = hx = hy = hz = 0;
    legangle = 0;
    dir = 1;
    legmove = false;
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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

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
