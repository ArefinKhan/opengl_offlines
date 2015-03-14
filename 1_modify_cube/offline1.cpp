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
double squareSide;
double cylRadius,cylHeight,cylRefX,cylRefY;
double xup,yup,zup;
double separation;
double cubeX,cubeY,cubeZ;
double radius,stacks,slices;
double cylSeparation;
double bottomSphereZ, topSphereZ;

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
			glColor3f(1.0,1.0,(double)i/stacks);
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
    glColor3ub(0,255,255);
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
    drawFilledCircle(0,0,cylRadius,cylHeight);
}

void drawSquare(double squareSide,int opt)
{
    if(opt == 1) glColor3ub(139,0,0);
    else if(opt == 2) glColor3ub(255,0,0);
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
        glTranslatef(cubeX,cubeY,cubeZ);
        glPushMatrix();{
            drawSquare(squareSide,1);
            glPushMatrix();{
                glTranslatef(0,0,squareSide);
                drawSquare(squareSide,1);
            }glPopMatrix();
            glRotatef(90,1,0,0);
            drawSquare(squareSide,2);
            glPushMatrix();{
                glTranslatef(0,0,-squareSide);
                drawSquare(squareSide,2);
            }glPopMatrix();
            glRotatef(90,0,1,0);
            drawSquare(squareSide,3);
            glPushMatrix();{
                glTranslatef(0,0,squareSide);
                drawSquare(squareSide,3);
            }glPopMatrix();
        }glPopMatrix();
    } glPopMatrix();
}

void DRAWCYLINDERS()
{
    glPushMatrix();{
        glTranslatef(cylRefX,cylRefY,2);
        glPushMatrix();{
            glTranslatef(0,0,zup);
            drawCylinder();     // parallel to z axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(0,cylSeparation,zup);
            drawCylinder();     // parallel to z axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cylSeparation,cylSeparation,zup);
            drawCylinder();     // parallel to z axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cylSeparation,0,zup);
            drawCylinder();     // parallel to z axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(0,yup,0);
            glRotatef(-90,1,0,0);
            drawCylinder();     // parallel to y axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(0,yup,cylSeparation);
            glRotatef(-90,1,0,0);
            drawCylinder();     // parallel to y axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cylSeparation,yup,cylSeparation);
            glRotatef(-90,1,0,0);
            drawCylinder();     // parallel to y axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(cylSeparation,yup,0);
            glRotatef(-90,1,0,0);
            drawCylinder();     // parallel to y axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(xup,0,0);
            glRotatef(90,0,1,0);
            drawCylinder();     // parallel to x axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(xup,0,cylSeparation);
            glRotatef(90,0,1,0);
            drawCylinder();     // parallel to x axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(xup,cylSeparation,0);
            glRotatef(90,0,1,0);
            drawCylinder();     // parallel to x axis
        } glPopMatrix();
        glPushMatrix();{
            glTranslatef(xup,cylSeparation,cylSeparation);
            glRotatef(90,0,1,0);
            drawCylinder();     // parallel to x axis
        } glPopMatrix();
    } glPopMatrix();
}

void DRAWPARTSPHERE()
{
    glPushMatrix();{
        glPushMatrix();{
            glTranslatef(7,7,topSphereZ);
            glPushMatrix();{
                glPushMatrix();{
                    glTranslatef(separation,separation,0);
                    drawsphere(radius,slices,stacks);
                } glPopMatrix();
                glPushMatrix();{
                    glTranslatef(-1*separation,-1*separation,0);
                    glRotatef(180,0,0,1);
                    drawsphere(radius,slices,stacks);
                }glPopMatrix();
                glPushMatrix();{
                    glTranslatef(-1*separation,separation,0);
                    glRotatef(90,0,0,1);
                    drawsphere(radius,slices,stacks);
                }glPopMatrix();
                glPushMatrix();{
                    glTranslatef(separation,-1*separation,0);
                    glRotatef(-90,0,0,1);
                    drawsphere(radius,slices,stacks);
                }glPopMatrix();
            } glPopMatrix();
        }glPopMatrix();
        glPushMatrix();{
            glTranslatef(7,7,bottomSphereZ);
            glRotatef(180,1,0,0);
            glPushMatrix();{
                glPushMatrix();{
                    glTranslatef(separation,separation,0);
                    drawsphere(radius,slices,stacks);
                } glPopMatrix();
                glPushMatrix();{
                    glTranslatef(-1*separation,-1*separation,0);
                    glRotatef(180,0,0,1);
                    drawsphere(radius,slices,stacks);
                }glPopMatrix();
                glPushMatrix();{
                    glTranslatef(-1*separation,separation,0);
                    glRotatef(90,0,0,1);
                    drawsphere(radius,slices,stacks);
                }glPopMatrix();
                glPushMatrix();{
                    glTranslatef(separation,-1*separation,0);
                    glRotatef(-90,0,0,1);
                    drawsphere(radius,slices,stacks);
                }glPopMatrix();
            } glPopMatrix();
        }glPopMatrix();
	} glPopMatrix();
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
		    //cameraAngle += 0.03;
		    //if(squareSide<=20) squareSide += 2,zup+=0.2,yup+=0.2,xup+=0.2;
            //if(cylRadius >= 1) cylRadius -= 0.5,cylHeight+=0.8;
            //if(separation<=4.5) separation += 0.5, radius -= 0.5;
            if(separation >= 0.8)
            {
                separation -= 0.8, radius += 1.8, squareSide -= 4, cylSeparation--, cylRefX+=0.4,cylRefY+=0.4;
                double var = cubeX;
                var += 1.2;
                cubeX = cubeY = cubeZ = var;
                topSphereZ-=2;
                bottomSphereZ+=2;
                if(topSphereZ<bottomSphereZ) topSphereZ = 6,bottomSphereZ = 6;
            }
			break;
		case GLUT_KEY_PAGE_DOWN:
		    //cameraAngle -= 0.03;
		    //if(squareSide>=2) squareSide -= 2,zup -= 0.2,yup-=0.2,xup=-0.2;
		    //if(cylRadius<=2) cylRadius += 0.5,cylHeight-=0.8;
            if(separation <= 2.40001)
            {

                separation += 0.8, radius -= 1.8, squareSide += 4, cylSeparation++,cylRefX-=0.4,cylRefY-=0.4;
                double var = cubeX;
                var -= 1.2;
                cubeX = cubeY = cubeZ = var;
                topSphereZ+=2;
                bottomSphereZ-=2;
                //printf("%f %f\n",topSphereZ,bottomSphereZ);
                if(topSphereZ>=12 && bottomSphereZ<=2) topSphereZ = 12,bottomSphereZ = 2;
            }
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
	gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
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
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=25.0;
	cameraAngle=1.0;
	angle=0;
    squareSide = 16.0;
    cylRadius = 1.5;
    cylHeight = 8.0;
    cylSeparation = 10.0;
    zup = 2;
    yup = 2;
    xup = 2;
    separation = 3.2;
    radius = 2;
    stacks = 30;
    slices = 20;
    topSphereZ = 12;
    bottomSphereZ = 2;
	//clear the screen
	glClearColor(0,0,0,0);
	cubeX = cubeY = cubeZ = 0;
    cylRefX = cylRefY = 2;
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

