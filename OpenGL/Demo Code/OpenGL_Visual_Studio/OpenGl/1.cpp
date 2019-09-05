#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

//#include <GL/glut.h>
//#include <GL/glu.h>
//#include <GL/gl.h>


#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double dCircleSquare, rCircleSquare;

struct point
{
	double x,y,z;
};

struct point camPos, l, r, u;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 300,0,0);
			glVertex3f(-300,0,0);

			glVertex3f(0,-300,0);
			glVertex3f(0, 300,0);

			glVertex3f(0,0, 300);
			glVertex3f(0,0,-300);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
    drawgrid = 1;
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

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1': //rotate/look left
			l.x = l.x*cos(pi*2/180) - r.x*sin(pi*2/180);
			l.y = l.y*cos(pi*2/180) - r.y*sin(pi*2/180);
			l.z = l.z*cos(pi*2/180) - r.z*sin(pi*2/180);

			r.x = r.x*cos(pi*2/180) + l.x*sin(pi*2/180);
			r.y = r.y*cos(pi*2/180) + l.y*sin(pi*2/180);
			r.z = r.z*cos(pi*2/180) + l.z*sin(pi*2/180);
			break;

        case '2': //rotate/look right
			l.x = l.x*cos(-pi*2/180) - r.x*sin(-pi*2/180);
			l.y = l.y*cos(-pi*2/180) - r.y*sin(-pi*2/180);
			l.z = l.z*cos(-pi*2/180) - r.z*sin(-pi*2/180);

			r.x = r.x*cos(-pi*2/180) + l.x*sin(-pi*2/180);
			r.y = r.y*cos(-pi*2/180) + l.y*sin(-pi*2/180);
			r.z = r.z*cos(-pi*2/180) + l.z*sin(-pi*2/180);
			break;

        case '3': //look up
			u.x = u.x*cos(pi*2/180) - l.x*sin(pi*2/180);
			u.y = u.y*cos(pi*2/180) - l.y*sin(pi*2/180);
			u.z = u.z*cos(pi*2/180) - l.z*sin(pi*2/180);

			l.x = l.x*cos(pi*2/180) + u.x*sin(pi*2/180);
			l.y = l.y*cos(pi*2/180) + u.y*sin(pi*2/180);
			l.z = l.z*cos(pi*2/180) + u.z*sin(pi*2/180);
			break;
        case '4': //look down
			u.x = u.x*cos(-pi*2/180) - l.x*sin(-pi*2/180);
			u.y = u.y*cos(-pi*2/180) - l.y*sin(-pi*2/180);
			u.z = u.z*cos(-pi*2/180) - l.z*sin(-pi*2/180);

			l.x = l.x*cos(-pi*2/180) + u.x*sin(-pi*2/180);
			l.y = l.y*cos(-pi*2/180) + u.y*sin(-pi*2/180);
			l.z = l.z*cos(-pi*2/180) + u.z*sin(-pi*2/180);
			break;

        case '5':
            r.x = r.x*cos(pi*2/180) - u.x*sin(pi*2/180);
			r.y = r.y*cos(pi*2/180) - u.y*sin(pi*2/180);
			r.z = r.z*cos(pi*2/180) - u.z*sin(pi*2/180);

			u.x = u.x*cos(pi*2/180) + r.x*sin(pi*2/180);
			u.y = u.y*cos(pi*2/180) + r.y*sin(pi*2/180);
			u.z = u.z*cos(pi*2/180) + r.z*sin(pi*2/180);
			break;

        case '6':
            r.x = r.x*cos(-pi*2/180) - u.x*sin(-pi*2/180);
			r.y = r.y*cos(-pi*2/180) - u.y*sin(-pi*2/180);
			r.z = r.z*cos(-pi*2/180) - u.z*sin(-pi*2/180);

			u.x = u.x*cos(-pi*2/180) + r.x*sin(-pi*2/180);
			u.y = u.y*cos(-pi*2/180) + r.y*sin(-pi*2/180);
			u.z = u.z*cos(-pi*2/180) + r.z*sin(-pi*2/180);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			camPos.x -= 3*l.x;
			camPos.y -= 3*l.y;
			camPos.z -= 3*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			camPos.x += 3*l.x;
			camPos.y += 3*l.y;
			camPos.z += 3*l.z;
			break;

		case GLUT_KEY_RIGHT:
			camPos.x += 3*r.x;
			camPos.y += 3*r.y;
			camPos.z += 3*r.z;
			break;
		case GLUT_KEY_LEFT:
			camPos.x -= 3*r.x;
			camPos.y -= 3*r.y;
			camPos.z -= 3*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    camPos.x += 3*u.x;
			camPos.y += 3*u.y;
			camPos.z += 3*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    camPos.x -= 3*u.x;
			camPos.y -= 3*u.y;
			camPos.z -= 3*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(dCircleSquare != 0)
            {
                dCircleSquare -= 1;
                rCircleSquare += 1;
            }

			break;
		case GLUT_KEY_END:
		    if(rCircleSquare != 0)
            {
                dCircleSquare += 1;
                rCircleSquare -= 1;
            }

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

void drawOneFourthSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}


void splitedSphere(double dCircleSquare, double rCircleSquare)
{
    glPushMatrix();
    {
        glTranslatef(dCircleSquare,dCircleSquare,dCircleSquare);
        drawOneFourthSphere(rCircleSquare,24,20); //x +ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare,dCircleSquare,dCircleSquare);
        glRotated(90, 0,0,1);
        drawOneFourthSphere(rCircleSquare,24,20);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare,-dCircleSquare,dCircleSquare);
        glRotated(180, 0,0,1);
        drawOneFourthSphere(rCircleSquare,24,20);  //x -ve, y -ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(dCircleSquare,-dCircleSquare,dCircleSquare);
        glRotated(-90, 0,0,1);
        drawOneFourthSphere(rCircleSquare,24,20);  //x +ve, y -ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(dCircleSquare,dCircleSquare,-dCircleSquare);
        glRotated(180, 1,1,0);
        drawOneFourthSphere(rCircleSquare,24,20);  //x +ve, y +ve, z -ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare,dCircleSquare,-dCircleSquare);
        glRotated(90, 0,0,1);
        glRotated(180, 1,1,0);
        drawOneFourthSphere(rCircleSquare,24,20);  //x -ve, y +ve, z -ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare,-dCircleSquare,-dCircleSquare);
        glRotated(180, 0,0,1);
        glRotated(180, 1,1,0);
        drawOneFourthSphere(rCircleSquare,24,20);  //x -ve, y -ve, z -ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(dCircleSquare,-dCircleSquare,-dCircleSquare);
        glRotated(-90, 0,0,1);
        glRotated(180, 1,1,0);
        drawOneFourthSphere(rCircleSquare,24,20);  //x +ve, y +ve, z -ve
    }
    glPopMatrix();
}



void drawOneFourthCylinder(double radius, double height, int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height*sin(((double)i/(double)stacks)*(pi/2));
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		//h = height;
		r = radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(0,1,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}

void splitedCylinder(double dCircleSquare, double rCircleSquare)
{
    // Around Z axis

    glPushMatrix();
    {
        glTranslatef(dCircleSquare,dCircleSquare,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40); //x +ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare,dCircleSquare,0);
        glRotated(90, 0,0,1);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare,-dCircleSquare,0);
        glRotated(180, 0,0,1);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y -ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(dCircleSquare,-dCircleSquare,0);
        glRotated(-90, 0,0,1);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x +ve, y -ve, z +ve
    }
    glPopMatrix();


    //Around X Axis

    glPushMatrix();
    {
        glTranslatef(0, dCircleSquare, dCircleSquare);
        glRotated(-90, 0,1,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, dCircleSquare, -dCircleSquare);
        glRotated(90, 0,1,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();


    glPushMatrix();
    {
        glTranslatef(0, -dCircleSquare, -dCircleSquare);
        glRotated(-90, 1,0,0);
        glRotated(90, 0,1,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();


    glPushMatrix();
    {
        glTranslatef(0, -dCircleSquare, dCircleSquare);
        glRotated(90, 1,0,0);
        glRotated(-90, 0,1,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();


    //Around Y Axis

    glPushMatrix();
    {
        glTranslatef(dCircleSquare, 0, dCircleSquare);
        glRotated(90, 1,0,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(dCircleSquare, 0, -dCircleSquare);
        glRotated(-90, 1,0,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();


    glPushMatrix();
    {
        glTranslatef(-dCircleSquare, 0, dCircleSquare);
        glRotated(-90, 0,1,0);
        glRotated(90, 1,0,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-dCircleSquare, 0, -dCircleSquare);
        glRotated(90, 0,1,0);
        glRotated(-90, 1,0,0);
        drawOneFourthCylinder(rCircleSquare, dCircleSquare, 24, 40);  //x -ve, y +ve, z +ve
    }
    glPopMatrix();
}



void splitedSquare(double dCircleSquare)
{
    //On +X Axis
    glPushMatrix();
    {
        glColor3f(1,1,1);
        glTranslated(dCircleSquare+rCircleSquare-2, 0, 0);
        glRotated(90, 0,1,0);
        drawSquare(dCircleSquare);
    }
    glPopMatrix();


    //On -X Axis
    glPushMatrix();
    {
        glColor3f(1,1,1);
        glTranslated(-dCircleSquare-rCircleSquare-2, 0, 0);
        glRotated(90, 0,1,0);
        drawSquare(dCircleSquare);
    }
    glPopMatrix();


    //On +Y Axis
    glPushMatrix();
    {
        glColor3f(1,1,1);
        glTranslated(0, dCircleSquare+rCircleSquare+2, 0);
        glRotated(90, 1,0,0);
        drawSquare(dCircleSquare);
    }
    glPopMatrix();


    //On -Y Axis
    glPushMatrix();
    {
        glColor3f(1,1,1);
        glTranslated(0, -dCircleSquare-rCircleSquare+2, 0);
        glRotated(90, 1,0,0);
        drawSquare(dCircleSquare);
    }
    glPopMatrix();

    //On +Z Axis
    glPushMatrix();
    {
        glColor3f(1,1,1);
        glTranslated(0, 0, dCircleSquare+rCircleSquare-2);
        drawSquare(dCircleSquare);
    }
    glPopMatrix();


    //On -Z Axis
    glPushMatrix();
    {
        glColor3f(1,1,1);
        glTranslated(0, 0, -dCircleSquare-rCircleSquare-2);
        drawSquare(dCircleSquare);
    }
    glPopMatrix();

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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(camPos.x,camPos.y,camPos.z,	camPos.x+l.x,camPos.y+l.y,camPos.z+l.z,	 u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
    drawGrid();

    splitedSphere(dCircleSquare,rCircleSquare);

    splitedCylinder(dCircleSquare,rCircleSquare);

    splitedSquare(dCircleSquare);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	camPos = {100, 100, 0};

	l = {-1/sqrt(2), -1/sqrt(2), 0};
	r = {-1/sqrt(2), 1/sqrt(2), 0};
    u = {0, 0, 1};

    dCircleSquare = 20;
    rCircleSquare = 20;


	drawgrid=0;
	drawaxes=1;
	//cameraHeight=150.0;
	//cameraAngle=1.0;
	angle=0;

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
	gluPerspective(80,	1,	1,	1000.0);
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
