#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <vector>
#include<GL/glut.h>
#include <windows.h>
#include<glm/glm.hpp>
using namespace std;
#define pi 2*acos(0.0)



int drawgrid;
int drawaxes;
int spotLightflag;
double spotLightangle;

GLuint grass,redSandstone;
GLUquadric* IDquadric=gluNewQuadric() ;//=new GLUquadricObj

int num_texture = -1;
int LoadBitmap(char *filename)
{
    int i, j=0;
    FILE *l_file;
    unsigned char *l_texture;

    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++;

    if( (l_file = fopen(filename, "rb"))==NULL) return (-1);

    fread(&fileheader, sizeof(fileheader), 1, l_file);

    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

 for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {
            fread(&rgb, sizeof(rgb), 1, l_file);

            l_texture[j+0] = rgb.rgbtRed;
            l_texture[j+1] = rgb.rgbtGreen;
            l_texture[j+2] = rgb.rgbtBlue;
            l_texture[j+3] = 255;
            j += 4;
    }
    fclose(l_file);

    glBindTexture(GL_TEXTURE_2D, num_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
     gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture);

    return (num_texture);

}

void textureInit(void)
{
	grass =LoadBitmap("Grass.bmp");
	redSandstone =LoadBitmap("red_sandstone.bmp");

}


struct point
{
	double x,y,z;
	point(double a,double b,double c)
	{
	    x = a/1000.0;
	    y = b/1000.0;
	    z = c/1000.0;
	}
	point() {}
};
vector <point> points_basement_bottom,points_basement_top,points_walkway_top_outer,points_walkway_top_inner;
vector <point> points_walkway_bottom_outer,points_walkway_bottom_inner;
vector <point> points_stairs;

class Camera
{
private:
	glm::vec3 eye;
	glm::vec3 u,v,n;
	double viewAngle, aspect, nearDist, farDist;
	void setModelviewMatrix();

public:
	void set(glm::vec3 eye, glm::vec3 look, glm::vec3 up);
	void roll(double angle); // roll it
	void pitch(double angle); // increase pitch
	void yaw(double angle); // yaw it
	void slide(float delU, float delV, float delN); // slide it
};

void Camera::setModelviewMatrix()
{
	float m[16];

	m[0]=(float)u.x; m[4]=(float)u.y; m[8]=(float)u.z; m[12]=(float) -glm::dot(eye,u);
	m[1]=(float)v.x; m[5]=(float)v.y; m[9]=(float)v.z; m[13]=(float) -glm::dot(eye,v);
	m[2]=(float)n.x; m[6]=(float)n.y; m[10]=(float)n.z; m[14]=(float) -glm::dot(eye,n);
	m[3]=0.0f; m[7]=0.0f; m[11]=0.0f; m[15]=1.0f;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);

}


void Camera::set(glm::vec3 Eye, glm::vec3 look, glm::vec3 up)
{
	eye = Eye;
	n = (eye - look);
	u = glm::cross(up,n);
	n = glm::normalize(n);
	u = glm::normalize(u);
	v = glm::cross(n,u);
	setModelviewMatrix();

}

void Camera::slide(float delU, float delV, float delN)
{
	eye.x += delU * u.x + delV * v.x + delN * n.x;
	eye.y += delU * u.y + delV * v.y + delN * n.y;
	eye.z += delU * u.z + delV * v.z + delN * n.z;
	setModelviewMatrix();
}

void Camera :: roll(double angle)
{

	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t = u;
	u = glm::vec3(cs*t.x - sn*v.x, cs*t.y - sn*v.y, cs*t.z - sn*v.z);
	v = glm::vec3(sn*t.x + cs*v.x, sn*t.y + cs*v.y, sn*t.z + cs*v.z);
	setModelviewMatrix();
}

void Camera :: pitch(double angle)
{

	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t = v;
	v = glm::vec3(cs*t.x - sn*n.x, cs*t.y - sn*n.y, cs*t.z - sn*n.z);
	n = glm::vec3(sn*t.x + cs*n.x, sn*t.y + cs*n.y, sn*t.z + cs*n.z);
	setModelviewMatrix();
}

void Camera :: yaw(double angle)
{

	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t = n;
	n = glm::vec3(cs*t.x - sn*u.x, cs*t.y - sn*u.y, cs*t.z - sn*u.z);
	u = glm::vec3(sn*t.x + cs*u.x, sn*t.y + cs*u.y, sn*t.z + cs*u.z);
	setModelviewMatrix();
}

Camera cam;

void textureBasement(void)
{
		int i;
	for(i=0;i<points_basement_bottom.size()-1;i++)
	{
		point p0 = points_basement_bottom[i];
		point p1 = points_basement_top[i];
		point p2 = points_basement_top[i+1];
		point p3 = points_basement_bottom[i+1];

		if(((p0.x-p3.x)*(p0.x-p3.x) + (p0.y-p3.y)*(p0.y-p3.y))<600)
		{

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
					glTexCoord2f(0.25,0); glVertex3f(p1.x, p1.y ,p1.z);
					glTexCoord2f(0.25,0.25); glVertex3f(p2.x, p2.y ,p2.z);
					glTexCoord2f(0,0.15); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}

		else
		{
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,1); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,1); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}
	}

		point p0 = points_basement_bottom[points_basement_bottom.size()-1];
		point p1 = points_basement_top[points_basement_bottom.size()-1];
		point p2 = points_basement_top[0];
		point p3 = points_basement_bottom[0];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,1); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,1); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
}

void textureWalkwayGrass(void)
{
		int i;
	for(i=0;i<points_basement_bottom.size()-1;i++)
	{
		point p0 = points_basement_top[i];
		point p1 = points_walkway_top_outer[i];
		point p2 = points_walkway_top_outer[i+1];
		point p3 = points_basement_top[i+1];

		if(((p0.x-p3.x)*(p0.x-p3.x) + (p0.y-p3.y)*(p0.y-p3.y))<450)
		{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
					glTexCoord2f(0,0.25); glVertex3f(p1.x, p1.y ,p1.z);
					glTexCoord2f(0.25,0.25); glVertex3f(p2.x, p2.y ,p2.z);
					glTexCoord2f(0.15,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}

		else
		{
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,0.25); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,0.25); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}
	}

		point p0 = points_basement_top[points_basement_bottom.size()-1];
		point p1 = points_walkway_top_outer[points_basement_bottom.size()-1];
		point p2 = points_walkway_top_outer[0];
		point p3 = points_basement_top[0];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,0.25); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,0.25); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
}

void textureSlopeGrass(void)
{
		int i;
	for(i=0;i<points_basement_bottom.size()-1;i++)
	{
		point p0 = points_walkway_top_inner[i];
		point p1 = points_walkway_bottom_outer[i];
		point p2 = points_walkway_bottom_outer[i+1];
		point p3 = points_walkway_top_inner[i+1];

		if(((p0.x-p3.x)*(p0.x-p3.x) + (p0.y-p3.y)*(p0.y-p3.y))>350)
		{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,1); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,1); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}


	}

		point p0 = points_walkway_top_inner[points_basement_bottom.size()-1];
		point p1 = points_walkway_bottom_outer[points_basement_bottom.size()-1];
		point p2 = points_walkway_bottom_outer[0];
		point p3 = points_walkway_top_inner[0];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,grass);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,1); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,1); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
}

void textureWalkwaySandstone(void)
{
		int i;
	for(i=0;i<points_basement_bottom.size()-1;i++)
	{
		point p0 = points_walkway_top_outer[i];
		point p1 = points_walkway_top_inner[i];
		point p2 = points_walkway_top_inner[i+1];
		point p3 = points_walkway_top_outer[i+1];

		if(((p0.x-p3.x)*(p0.x-p3.x) + (p0.y-p3.y)*(p0.y-p3.y))<400)
		{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,redSandstone);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
					glTexCoord2f(0,0.25); glVertex3f(p1.x, p1.y ,p1.z);
					glTexCoord2f(0.25,0.25); glVertex3f(p2.x, p2.y ,p2.z);
					glTexCoord2f(0.15,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}

		else
		{
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,redSandstone);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,0.25); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,0.25); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}
	}

		point p0 = points_walkway_top_outer[points_basement_bottom.size()-1];
		point p1 = points_walkway_top_inner[points_basement_bottom.size()-1];
		point p2 = points_walkway_top_inner[0];
		point p3 = points_walkway_top_outer[0];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,redSandstone);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,0.25); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,0.25); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
}

void textureSlopeSandstone(void)
{
		int i;
	for(i=0;i<points_basement_bottom.size()-1;i++)
	{
		point p0 = points_walkway_bottom_outer[i];
		point p1 = points_walkway_bottom_inner[i];
		point p2 = points_walkway_bottom_inner[i+1];
		point p3 = points_walkway_bottom_outer[i+1];

		if(((p0.x-p3.x)*(p0.x-p3.x) + (p0.y-p3.y)*(p0.y-p3.y))<250)
		{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,redSandstone);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
					glTexCoord2f(0,0.1); glVertex3f(p1.x, p1.y ,p1.z);
					glTexCoord2f(0.25,0.1); glVertex3f(p2.x, p2.y ,p2.z);
					glTexCoord2f(0.25,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}

		else
		{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,redSandstone);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,0.1); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,0.1); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}


	}

		point p0 = points_walkway_bottom_outer[points_basement_bottom.size()-1];
		point p1 = points_walkway_bottom_inner[points_basement_bottom.size()-1];
		point p2 = points_walkway_bottom_inner[0];
		point p3 = points_walkway_bottom_outer[0];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,redSandstone);
				glNormal3f(1.0,0.0,0.0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(p0.x, p0.y ,p0.z);
				glTexCoord2f(0,0.1); glVertex3f(p1.x, p1.y ,p1.z);
				glTexCoord2f(2,0.1); glVertex3f(p2.x, p2.y ,p2.z);
				glTexCoord2f(2,0); glVertex3f(p3.x, p3.y ,p3.z);
				glEnd();
				glDisable(GL_TEXTURE_2D);
}
void drawTexture(void)
{
	glColor3f(1, 1, 1);
	 textureBasement();
	 textureWalkwayGrass();
	 textureSlopeGrass();
	 textureWalkwaySandstone();
	 textureSlopeSandstone();
}

void drawLightsrc(void)
{
	if(spotLightflag == 0)
	{
		GLfloat light_emission[] = {1.0, 1.0, 1.0, 1.0};//object emits white light
		glMaterialfv(GL_FRONT, GL_EMISSION, light_emission);
	   glPushMatrix();
	   glTranslatef (50.0*cos(spotLightangle),50.0*sin(spotLightangle),50.0);
	   glutSolidSphere(1, 36, 36);
	   glPopMatrix();
	}
	else
	{
		GLfloat light_emission[] = {0.0, 0.0, 1.0, 1.0};//object emits blue light
		glMaterialfv(GL_FRONT, GL_EMISSION, light_emission);
	   glPushMatrix();
	   glTranslatef (50.0*cos(spotLightangle),50.0*sin(spotLightangle),50.0);
	   glutSolidSphere(1, 36, 36);
	   glPopMatrix();

	}

}

void drawAxes()
{
	if(drawaxes==1)
	{
	    glPushMatrix();{
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
	    } glPopMatrix();
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
			{
			cam.yaw(-0.05);
			}
			break;

		case '2':
			{
			cam.yaw(0.05);
			}
			break;

			case '3':
			{
			cam.pitch(-0.05);
			}
			break;

		case '4':
			{
			cam.pitch(0.05);
			}
			break;
		case '5':
			{
			cam.roll(0.05);
			}
			break;

		case '6':
			{
			cam.roll(-0.05);
			}
			break;
		case 'l':
			{
				spotLightflag = ~ spotLightflag;
			}
			break;
		case 'a':
			{
				spotLightangle += 0.05;
				if(spotLightangle >= 360)
					spotLightangle = 0;
			}
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cam.slide(0.0f,0.0f,1.0f);
			break;
		case GLUT_KEY_UP:		// up arrow key
			cam.slide(0.0f,0.0f,-1.0f);
			break;

		case GLUT_KEY_RIGHT:
			cam.slide(1.0f,0.0f,0.0f);
			break;
		case GLUT_KEY_LEFT:
			cam.slide(-1.0f,0.0f,0.0f);
			break;

		case GLUT_KEY_PAGE_UP:
			cam.slide(0.0f,1.0f,0.0f);
			break;
		case GLUT_KEY_PAGE_DOWN:
			cam.slide(0.0f,-1.0f,0.0f);
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
    int i;
    int n = points_basement_bottom.size() - 1;
    for(i = 1; i<=n; i++)
    {
        point p1 = points_basement_bottom[i];
        point p2 = points_basement_bottom[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    point p1 = points_basement_bottom[0];
    point p2 = points_basement_bottom[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_basement_top.size() - 1;
    for(i = 1; i<=n; i++)
    {
        p1 = points_basement_top[i];
        p2 = points_basement_top[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_basement_top[0];
    p2 = points_basement_top[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    for(i=1; i+1<=n; i+=3)
    {
        p1 = points_basement_top[i];
        p2 = points_basement_bottom[i];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
        p1 = points_basement_top[i+1];
        p2 = points_basement_bottom[i+1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
}
void connect_top_bottom()
{
    int i,n;
    n = points_walkway_bottom_outer.size() - 1;
    point p1,p2;
    for(i=0; i<=n; i++)
    {
        p1 = points_walkway_bottom_outer[i];
        p2 = points_walkway_top_inner[i];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
}
void DRAW_WALKWAY()
{
    int i,n;
    n = points_walkway_top_outer.size() - 1;
    point p1,p2;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_top_outer[i];
        p2 = points_walkway_top_outer[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_top_outer[0];
    p2 = points_walkway_top_outer[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_walkway_top_inner.size() - 1;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_top_inner[i];
        p2 = points_walkway_top_inner[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_top_inner[0];
    p2 = points_walkway_top_inner[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_walkway_bottom_outer.size() - 1;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_bottom_outer[i];
        p2 = points_walkway_bottom_outer[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_bottom_outer[0];
    p2 = points_walkway_bottom_outer[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_walkway_bottom_inner.size() - 1;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_bottom_inner[i];
        p2 = points_walkway_bottom_inner[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_bottom_inner[0];
    p2 = points_walkway_bottom_inner[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    connect_top_bottom();
}

void DRAW_STAIRS()
{
    int i,n;
    n = points_stairs.size() - 1;
    point p1,p2;
    for(i=1; i<=n; i+=2)
    {
        p1 = points_stairs[i];
        p2 = points_stairs[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
}

void DRAW_WATERBODY()
{

}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ambient light
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
      GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

    drawAxes();
	drawTexture();
	drawLightsrc();
    DRAW_BASEMENT();
    DRAW_WALKWAY();
    DRAW_STAIRS();

    //DRAW_WATERBODY();

	/****************************
	/ Add your objects from here
	****************************/
	//add objects

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
	spotLightflag = 0;
	spotLightangle = 0;
	textureInit();
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

	// light initialization
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	GLfloat global_ambient[] = { .3, .3, .3, 1.0 }; //white light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glEnable(GL_LIGHTING);

	//point source
 GLfloat diffusePoint[] = {1.0, 1.0, 1.0, 1.0}; //Color (0.5, 0.5, 0.5)
   GLfloat position[] = {0.0, 50.0, 50.0, 1.0};
   glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusePoint);
   glLightfv(GL_LIGHT1, GL_POSITION, position);



}

void input()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_basement_bottom.push_back(p);
    }
}
void input2()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_basement_top.push_back(p);
    }
}

void input3()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_top_outer.push_back(p);
    }
}
void input4()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_top_inner.push_back(p);
    }
}
void input5()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_bottom_outer.push_back(p);
    }
}
void input6()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_bottom_inner.push_back(p);
    }
}
void input7()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_stairs.push_back(p);
    }
}

int main(int argc, char **argv){
    freopen("coordinates_basement_bottom.txt","r",stdin);
    input();
    fclose(stdin);
    freopen("coordinates_basement_top.txt","r",stdin);
    input2();
    fclose(stdin);
    freopen("coordinates_walkway_top_outer.txt","r",stdin);
    input3();
    fclose(stdin);
    freopen("coordinates_walkway_top_inner.txt","r",stdin);
    input4();
    fclose(stdin);
    freopen("coordinates_walkway_bottom_outer.txt","r",stdin);
    input5();
    fclose(stdin);
    freopen("coordinates_walkway_bottom_inner.txt","r",stdin);
    input6();
    fclose(stdin);
    freopen("coordinates_stairs","r",stdin);
    input7();
    fclose(stdin);

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

	//setting camera
	glm::vec3 Eye = glm::vec3 (100.0f,100.0f,0.0f);
	glm::vec3 look = glm::vec3 (0.0f,0.0f,0.0f);
	glm::vec3 up = glm::vec3 (0.0f,0.0f,1.0f);
	cam.set(Eye,look,up);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

