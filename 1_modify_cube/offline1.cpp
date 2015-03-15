#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>

#define pi 2*acos(0)

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct Cube{
    double side;
    double MAX;
    double repX,repY,repZ;
};
struct Curvature{
    double radius;
    double height;
    double slices,stacks;
};

Cube cube;
Curvature curve;

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
		}glEnd();
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_LINES);{
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			/*glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);*/
		}glEnd();
		glColor3f(0.5, 0.5, 0.8);
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


//draws half sphere
void drawsphere(float radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=sqrt(radius*radius - h*h);
		for(j=0;j<=slices;j++)
		{
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z = h;
			//printf("%lf %lf %lf\n",points[i][j].x,points[i][j].y,points[i][j].z);
		}

	}
	for(i=0;i<stacks;i++)
	{
	    //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices/4;j++)
		{
			glColor3f(0.5,(double)i/stacks,0.5);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}

	}
}
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius,double height){
	int i,j;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * pi;
    glColor3ub(139,58,58);
	glBegin(GL_TRIANGLE_FAN); {
        double zcomp = 0.0;
        for(j=0; ; j++)
        {
            if(zcomp>height) break;
            zcomp += 0.1;
            glVertex3f(x, y, zcomp); // center of circle
            for(i = 0; i <= triangleAmount;i++) {
                glVertex3f(x + (radius * cos(i *  twicePi / triangleAmount)),
                    y + (radius * sin(i * twicePi / triangleAmount)),zcomp);
            }
        }
	}glEnd();
}

void drawCylinder()
{
    drawFilledCircle(0,0,curve.radius,curve.height);
}

void drawSquare(double squareSide,int opt)
{
    if(opt == 1) glColor3ub(139,0,0);
    else if(opt == 2) glColor3ub(205,38,38);
    else glColor3ub(139,35,35);
    glBegin(GL_QUADS);{
        glVertex3f(0,0,0);
        glVertex3f(0,squareSide,0);
        glVertex3f(squareSide,squareSide,0);
        glVertex3f(squareSide,0,0);
    }glEnd();
}

void DRAWCUBE()
{
    glPushMatrix();{
        //glTranslatef(cube.repX,cube.repY,cube.repZ);
        glPushMatrix();{
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,0);
                drawSquare(cube.side,1);  // bottom surface
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,cube.MAX);
                drawSquare(cube.side,1);    // top surface
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,0,(cube.MAX-cube.side)/2);
                glRotatef(90,1,0,0);
                drawSquare(cube.side,2);    // surface parallel to x - near
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,cube.MAX,(cube.MAX-cube.side)/2);
                glRotatef(90,1,0,0);        // surface parallel to x - furthest
                drawSquare(cube.side,2);
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef(0,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
                glRotatef(-90,0,1,0);
                drawSquare(cube.side,3);        // surface parallel to y axis - near
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef(cube.MAX,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
                glRotatef(-90,0,1,0);
                drawSquare(cube.side,3);        // surface parallel to y axis - furthest
            }glPopMatrix();
        }glPopMatrix();
    } glPopMatrix();
}

void DRAWCYLINDERS()
{
    glPushMatrix();{
        //glTranslatef(cylRefX,cylRefY,2);
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            drawCylinder();     // parallel to z axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            drawCylinder();     // parallel to z axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            glRotatef(-90,1,0,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
            glRotatef(-90,1,0,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            glRotatef(-90,1,0,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
            glRotatef(-90,1,0,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            glRotatef(90,0,1,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
            glRotatef(90,0,1,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
            glRotatef(90,0,1,0);
            drawCylinder();
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef((cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
            glRotatef(90,0,1,0);
            drawCylinder();
        } glPopMatrix();
    } glPopMatrix();
}

void DRAWPARTSPHERE()
{
    glPushMatrix();{
        glPushMatrix();{
            glPushMatrix();{
                glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            } glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
                glRotatef(180,0,0,1);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
                glRotatef(90,0,0,1);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2);
                glRotatef(-90,0,0,1);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            }glPopMatrix();
        } glPopMatrix();

        glPushMatrix();{
            glPushMatrix();{
                glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
                glRotatef(180,1,0,0);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            } glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
                glRotatef(180,0,1,0);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef(cube.MAX - (cube.MAX-cube.side)/2,cube.MAX - (cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
                glRotatef(180,1,1,0);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            }glPopMatrix();
            glPushMatrix();{
                glTranslatef((cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2,(cube.MAX-cube.side)/2);
                glRotatef(180,0,0,0);
                drawsphere(curve.radius,curve.slices,curve.stacks);
            }glPopMatrix();
        } glPopMatrix();
    }glPopMatrix();
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
            if(cube.side>0) cube.side--, curve.radius+=0.5;
			break;
		case GLUT_KEY_PAGE_DOWN:
            if(cube.side<cube.MAX) cube.side++, curve.radius-=0.5;
            if(cube.side >= cube.MAX) curve.radius = 0;
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
	gluLookAt(150*cos(cameraAngle), 150*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	DRAWPARTSPHERE();
    DRAWCUBE();
    DRAWCYLINDERS();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	curve.height = cube.side;
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=100.0;
	cameraAngle=1.0;
	angle=0;
	cube.MAX = 60;
	cube.side = cube.MAX/2;
    cube.repX = cube.repY = cube.repZ = cube.side/2;
    curve.radius = 15;
    curve.height = cube.side;
    curve.slices = 20;
    curve.stacks = 10;
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

	glutCreateWindow("Offline 1");

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

