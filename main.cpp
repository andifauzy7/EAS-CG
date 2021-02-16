#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include "BmpLoader.h"
#define WIDTH 1280
#define HEIGHT 720

using namespace std;

GLfloat Widthfactor;
GLfloat Heightfactor;
GLfloat Zoom = 1;
GLint spin_x = 0;
GLint spin_y = 0;
GLfloat New_x;
GLfloat New_y;
GLfloat xt=0.0,yt=0.0,zt=0.0,xw=0.0,zt1=0.0;   /* x,y,z translation */
GLfloat degree=0.0;
GLuint _textureMaps;
bool isShot = false;
GLuint loadTexture(const char* filename);

GLuint loadTexture(const char* filename) {
	BmpLoader bl(filename);
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 bl.iWidth, bl.iHeight,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 bl.textureData);
	return textureId;
}

class car {
public:
    float frontBody;
    float backBody;
    float rightBody;
    float leftBody;
    float degree;
    float coordinateX;
    float coordinateY;
    float coordinateZ;
    car(float aCoordinateX, float aCoordinateY, float aCoordinateZ){
        this->coordinateX = aCoordinateX;
        this->coordinateY = aCoordinateY;
        this->coordinateZ = aCoordinateZ;
        this->frontBody = aCoordinateZ - 1.0;
        this->backBody = aCoordinateZ + 1.0;
        this->rightBody = aCoordinateX + 0.6;
        this->leftBody = aCoordinateX - 0.6;
    }
    void buildCar(){
        glTranslatef(coordinateX,coordinateY,coordinateZ);
        glRotated(degree,0.0,5.0,0.0);
        glPushMatrix();

            // WHEELS CAR.
            glPushMatrix();
            glTranslatef(-.5,-.2,0.5);
            glColor3f(0.0/255.0f, 0.0/255.0f, 255.0/255.0f);
            glRotated(90.0,0.0,5.0,0.0);
            glutSolidTorus(.1,.2,8,8);
            glTranslatef(1,0,0);
            glutSolidTorus(.1,.2,8,8);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(1,0,-1);
            glTranslatef(-.5,-.2,0.5);
            glColor3f(0.0/255.0f, 0.0/255.0f, 255.0/255.0f);
            glRotated(270.0,0.0,5.0,0.0);
            glutSolidTorus(.1,.2,8,8);
            glTranslatef(1,0,0);
            glutSolidTorus(.1,.2,8,8);
            glPopMatrix();

            // BODY CAR.
            glTranslatef(0,0,0);
            glColor3f(0.0/255.0f, 0.0/255.0f, 99.0/255.0f);
            glScalef(2,1,4);
            glutSolidCube(.5);

            glColor3f(99.0/255.0f, 0.0/255.0f, 0.0/255.0f);
            glTranslatef(0,.3,-.05);
            glScalef(2,3,0.6);
            glutSolidCube(.25);

            glColor3f(0.0/255.0f, 99.0/255.0f, 0.0/255.0f);
            glTranslatef(-.001,.001,-.12);
            glScalef(2.48,1.8,1);
            glRotatef(230, 250, 0, 0);
            glutSolidCube(.1);

            glPopMatrix();

            glTranslatef(0,1,-.5);
            glColor3f(0.0/255.0f, 99.0/255.0f, 99.0/255.0f);
            glScalef(0.5,1,1.5);
            glutSolidCube(.5);

        glPopMatrix();
    }
    void shot(){
        glTranslatef(coordinateX,coordinateY,zt1);
        glPushMatrix();
            glTranslatef(0,1,-1);
            glColor3f(0.0/255.0f, 0.0/255.0f, 255.0/255.0f);
            glutSolidTeapot(.2);
        glPopMatrix();
    }
};

class maps {
public:
    void buildMaps(){
        glPushMatrix();
    	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureMaps );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,50.0,1);  glVertex3f(-50,-1.5,50);
            glTexCoord3f(0.0,0.0,1);  glVertex3f(-50,-1.5,-50);
            glTexCoord3f(50.0,0.0,1);  glVertex3f(50,-1.5,-50);
            glTexCoord3f(50.0,50.0,1);  glVertex3f(50,-1.5,50);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
};

class windowScreen {
public:
    int height;
    int width;
    string title;
    windowScreen(int aHeight, int aWidth, string aTitle){
        this->height = aHeight;
        this->width = aWidth;
        this->title = aTitle;
    }
    void createWindow(){
        char c[title.size() + 1];
        strcpy(c, title.c_str());
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
        glutInitWindowSize(this->width, this->height);
        glutInitWindowPosition(100, 100);
        glutCreateWindow(c);
    }
};

void MyInit() {
    // Warna Dasar (Background)
	glClearColor(178.0f/255.0f, 190.0f/255.0f, 195.0f/255.0f, 1.0);
	_textureMaps = loadTexture("roof.bmp");
}

void DrawScene() {
	glRotatef(spin_x, 0.0, 1.0, 0.0);
	glRotatef(spin_y, 0.0, 0.0, 1.0);
	glScalef(Zoom, Zoom, Zoom);
	glColor3f(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	//glTranslatef(xt,yt,zt);

	// -- Persegi Panjang atau Jalan
	maps mapnya;
	mapnya.buildMaps();

	// -- Mobil Player
    car mobilKolbak(xt,yt,zt);
    mobilKolbak.degree = degree;
    mobilKolbak.buildCar();

    if(isShot == true){
        mobilKolbak.shot();
    }

	// -- Mobil Musuh
	/*
    car mobilMusuh(0,0,-5);
    //mobilMusuh.buildCar();

    if(
       (mobilKolbak.frontBody <= mobilMusuh.backBody && mobilKolbak.backBody >= mobilMusuh.frontBody) &&
       (mobilKolbak.leftBody <= mobilMusuh.rightBody && mobilKolbak.rightBody >= mobilMusuh.leftBody)
       ){
           // Terjadi collision.
           printf("Bingo\n");
    } else {
        mobilMusuh.buildCar();
    } */

    glPopMatrix();
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPerspective(50, 1.0 *(Widthfactor/Heightfactor), 3.0, 50.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        gluLookAt(0.0 + xt, 10.0, 10.0 + zt,  xt, 0.0, zt,  0.0, 5.0, 0.0);
        DrawScene();
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glPopMatrix();
	glFlush();
}

void MyReshape(int NewWidth, int NewHeight) {
	Widthfactor = (GLfloat)NewWidth / (GLfloat)WIDTH;
	Heightfactor = (GLfloat)NewHeight / (GLfloat)HEIGHT;

}

void MySpecial(int key, int x, int y){
	switch (key)
	{
    case GLUT_KEY_F1:{
        if(isShot==false){
            isShot = true;
        } else {
            isShot = false;
        }
        break;
    }
    case GLUT_KEY_UP:{
        zt -= 0.2;
        degree = 0.0;
        glutPostRedisplay();
        break;
    }
    case GLUT_KEY_DOWN:{
        zt += 0.2;
        degree = 0.0;
        glutPostRedisplay();
	    break;
    }
	case GLUT_KEY_LEFT:{
	    zt -= 0.2;
	    xt -= 0.2;
	    degree = 90.0;
        glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_RIGHT):{
	    zt -= 0.2;
	    xt += 0.2;
	    degree = -90.0;
        glutPostRedisplay();
		break;
	}
	case GLUT_KEY_PAGE_UP:{
		Zoom = Zoom + 0.01;
		DrawScene();
		glutPostRedisplay();
		break;
	}
	case GLUT_KEY_PAGE_DOWN:{
		if (Zoom >= 0) {
			Zoom = Zoom - 0.01;
			DrawScene();
		}
		glutPostRedisplay();
		break;
	}
	case GLUT_KEY_END:{
	    exit(1);
	    break;
    }
	}
}

void MyMouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		New_x = x;
		New_y = y;
	}
}

void MyMotion(int x, int y){
	spin_x = x - New_x;
	spin_y = y - New_y;
	DrawScene();
	glutPostRedisplay();
}

void idle(){
    if(isShot == true){
        zt1 -= 0.002;
    } else {
        zt1 = zt;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Inisiasi Variabel.
    windowScreen screen(HEIGHT,WIDTH,"Battle Cars - 181511007");

    // Inisiasi Window.
    glutInit(&argc, argv);
    screen.createWindow();

    // Load texture.
	MyInit();

	// Program Dieksekusi.
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutSpecialFunc(MySpecial);
	glutIdleFunc(idle);
	//glutMouseFunc(MyMouse);
	//glutMotionFunc(MyMotion);
	glutMainLoop();
	return 0;
}
