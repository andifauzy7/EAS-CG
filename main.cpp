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
GLfloat Zoom = 10;
GLfloat New_x, New_y;
GLuint _textureMaps, _textureSky;
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

class camera {
    public:
    float AngleX;
    float AngleY;
    float AngleZ;
    camera(float aAngleX, float aAngleY, float aAngleZ){
        this->AngleX = aAngleX;
        this->AngleY = aAngleY;
        this->AngleZ = aAngleZ;
    }
};

class weapon {
public:
    bool hitEnemy = false;
    float rangeAmmo = 0.0;
    float degree;
    float coordinateX;
    float coordinateY;
    float coordinateZ;
    weapon(float aCoordinateX, float aCoordinateY, float aCoordinateZ){
        this->coordinateX = aCoordinateX;
        this->coordinateY = aCoordinateY;
        this->coordinateZ = aCoordinateZ;
    }
    void updatePosition(float aCoordinateX, float aCoordinateY, float aCoordinateZ, float aDegree){
        this->degree = aDegree;
        this->coordinateX = aCoordinateX;
        this->coordinateY = aCoordinateY;
        this->coordinateZ = aCoordinateZ;
    }
    void shot(){
        glPushMatrix();
            this->coordinateZ = coordinateZ + (-1*rangeAmmo);
            glTranslatef(coordinateX,coordinateY,coordinateZ);
            glRotated(degree,0.0,5.0,0.0);
            glTranslatef(0,1,-.5);
            glColor3f(0.0/255.0f, 255.0/255.0f, 0.0/255.0f);
            glutSolidTeapot(.1);
        glPopMatrix();
    }
};

class car {
public:
    boolean lifeStatus;
    float movementSpeed;
    float degree;
    float health;
    float coordinateX;
    float coordinateY;
    float coordinateZ;
    car(float aCoordinateX, float aCoordinateY, float aCoordinateZ){
        this->coordinateX = aCoordinateX;
        this->coordinateY = aCoordinateY;
        this->coordinateZ = aCoordinateZ;
        this->health = 100;
        this->lifeStatus = true;
        this->movementSpeed = 0.2;
    }
    float frontBody(){
        return this->coordinateZ - 1.0;
    }
    float backBody(){
        return this->coordinateZ + 1.0;
    }
    float rightBody(){
        return this->coordinateX + .6;
    }
    float leftBody(){
        return this->coordinateX - .6;
    }
    void buildCar(){
        glPushMatrix();
            glTranslatef(coordinateX,coordinateY,coordinateZ);
            glRotated(degree,0.0,5.0,0.0);
            // WHEELS CAR.
            glPushMatrix();
                glTranslatef(-.5,-.2,0.5);
                glColor3f(0.0/255.0f, 0.0/255.0f, 255.0/255.0f);
                glRotated(90.0,0.0,5.0,0.0);
                glutWireTorus(.1,.2,8,8);
                glTranslatef(1,0,0);
                glutWireTorus(.1,.2,8,8);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(1,0,-1);
                glTranslatef(-.5,-.2,0.5);
                glColor3f(0.0/255.0f, 0.0/255.0f, 255.0/255.0f);
                glRotated(270.0,0.0,5.0,0.0);
                glutWireTorus(.1,.2,8,8);
                glTranslatef(1,0,0);
                glutWireTorus(.1,.2,8,8);
            glPopMatrix();

            // BODY CAR.
            glPushMatrix();
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

            glPushMatrix();
                glTranslatef(0,1,-.5);
                glColor3f(0.0/255.0f, 99.0/255.0f, 99.0/255.0f);
                glScalef(0.5,1,1.5);
                glutSolidCube(.5);
            glPopMatrix();

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

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _textureSky);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBegin(GL_QUADS); //Belakang
                glTexCoord3f(0.0,1.0,1);  glVertex3f(-50,50,-50);
                glTexCoord3f(0.0,0.0,1);  glVertex3f(-50,-1.5,-50);
                glTexCoord3f(0.0,1.0,1);  glVertex3f( 50,-1.5,-50);
                glTexCoord3f(1.0,1.0,1);  glVertex3f( 50,50,-50);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _textureSky);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBegin(GL_QUADS); //Kiri
                glTexCoord3f(0.0,1.0,1);  glVertex3f(-50,  50,-50);
                glTexCoord3f(0.0,0.0,1);  glVertex3f(-50,-1.5,-50);
                glTexCoord3f(0.0,1.0,1);  glVertex3f(-50,-1.5, 50);
                glTexCoord3f(1.0,1.0,1);  glVertex3f(-50,  50, 50);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _textureSky);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBegin(GL_QUADS); //Kanan
                glTexCoord3f(0.0,1.0,1);  glVertex3f(50,  50,-50);
                glTexCoord3f(0.0,0.0,1);  glVertex3f(50,-1.5,-50);
                glTexCoord3f(0.0,1.0,1);  glVertex3f(50,-1.5, 50);
                glTexCoord3f(1.0,1.0,1);  glVertex3f(50,  50, 50);
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
	_textureSky = loadTexture("sky.bmp");
}

// -- Inisiasi Objek.
    car mobilKolbak(0,0,0);
    weapon senjata(0,0,0);
    maps mapnya;
    car mobilMusuh(5,0,-5);
    weapon senjataMusuh(5,0,-5);
    camera angleCamera(0, 10.0, 0);

void DrawScene() {
	glColor3f(0.7, 0.7, 0.7);
	glPushMatrix();
	// -- Persegi Panjang atau Jalan
	mapnya.buildMaps();

	// -- Mobil Musuh
	if(mobilMusuh.health >= 0 && mobilMusuh.lifeStatus == true){
        mobilMusuh.buildCar();
	}

	// -- Mobil Player
    mobilKolbak.buildCar();
	// -- Senjata
	senjata.updatePosition( mobilKolbak.coordinateX,
                            mobilKolbak.coordinateY,
                            mobilKolbak.coordinateZ,
                            mobilKolbak.degree);
    senjataMusuh.updatePosition(    mobilMusuh.coordinateX,
                                    mobilMusuh.coordinateY,
                                    mobilMusuh.coordinateZ,
                                    mobilMusuh.degree);
	if(senjata.rangeAmmo <= 5.0){
        senjata.shot();
	}
	if(mobilMusuh.lifeStatus==true){
        senjataMusuh.shot();
	} else {
	    car mobilMusuh1(mobilMusuh.coordinateX + 5,mobilMusuh.coordinateY,mobilMusuh.coordinateZ);
        weapon senjataMusuh1(mobilMusuh.coordinateX + 5,mobilMusuh.coordinateY,mobilMusuh.coordinateZ);
        car mobilMusuh2(mobilMusuh.coordinateX - 5,mobilMusuh.coordinateY,mobilMusuh.coordinateZ);
        weapon senjataMusuh2(mobilMusuh.coordinateX - 5,mobilMusuh.coordinateY,mobilMusuh.coordinateZ);
        mobilMusuh1.buildCar();
        mobilMusuh2.buildCar();
	}
	// -- Mobil Collision.
	if(
       (mobilKolbak.frontBody() <= mobilMusuh.backBody() && mobilKolbak.backBody() >= mobilMusuh.frontBody()) &&
       (mobilKolbak.leftBody() <= mobilMusuh.rightBody() && mobilKolbak.rightBody() >= mobilMusuh.leftBody())
       ){
        mobilMusuh.health -= 50;
        if(mobilMusuh.health <= 0){
            mobilMusuh.lifeStatus = false;
        }
    }
    // -- Senjata Collision.
    if(
       (senjata.coordinateZ <= mobilMusuh.backBody() && senjata.coordinateZ >= mobilMusuh.frontBody()) &&
       (senjata.coordinateX <= mobilMusuh.rightBody() && senjata.coordinateX >= mobilMusuh.leftBody())
       ){
        senjata.hitEnemy = true;
        senjata.rangeAmmo = 5;
        mobilMusuh.health -= 50;
        if(mobilMusuh.health <= 0){
            mobilMusuh.lifeStatus = false;
        }
    } else {
        senjata.hitEnemy = false;
    }
    glPopMatrix();
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPerspective(50, 1.0 *(Widthfactor/Heightfactor), 3.0, 200.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        gluLookAt(0.0 + mobilKolbak.coordinateX, angleCamera.AngleY, Zoom + mobilKolbak.coordinateZ - angleCamera.AngleZ,  mobilKolbak.coordinateX, 0.0, mobilKolbak.coordinateZ,  0.0, 5.0, 0.0);
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
    case GLUT_KEY_F4:{
        angleCamera.AngleY = 10.0;
        angleCamera.AngleZ = 0.0;
        break;
    }
    case GLUT_KEY_F3:{
        angleCamera.AngleY = 2.0;
        angleCamera.AngleZ = 5.0;
        break;
    }
    case GLUT_KEY_F2:{
        if(mobilKolbak.movementSpeed < 0.4){
            mobilKolbak.movementSpeed = 0.4;
        } else {
            mobilKolbak.movementSpeed = 0.2;
        }
        break;
    }
    case GLUT_KEY_F1:{
        if(senjata.rangeAmmo>=5.0){
            senjata.rangeAmmo = 0.0;
        }
        break;
    }
    case GLUT_KEY_UP:{
        mobilKolbak.coordinateZ -= mobilKolbak.movementSpeed;
        mobilKolbak.degree = 0;
        glutPostRedisplay();
        break;
    }
    case GLUT_KEY_DOWN:{
        mobilKolbak.coordinateZ += mobilKolbak.movementSpeed;
        mobilKolbak.degree = 0;
        glutPostRedisplay();
	    break;
    }
	case GLUT_KEY_LEFT:{
	    mobilKolbak.coordinateZ -= mobilKolbak.movementSpeed;
	    mobilKolbak.coordinateX -= mobilKolbak.movementSpeed;
	    mobilKolbak.degree = 30;
        glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_RIGHT):{
	    mobilKolbak.coordinateZ -= mobilKolbak.movementSpeed;
	    mobilKolbak.coordinateX += mobilKolbak.movementSpeed;
	    mobilKolbak.degree = -30;
        glutPostRedisplay();
		break;
	}
	case GLUT_KEY_PAGE_UP:{
		Zoom = Zoom + 0.2;
		DrawScene();
		glutPostRedisplay();
		break;
	}
	case GLUT_KEY_PAGE_DOWN:{
		if (Zoom >= 0) {
			Zoom = Zoom - 0.2;
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

void idle(){
    if(mobilMusuh.coordinateZ >= -40){
        mobilMusuh.coordinateZ -= 0.001;
    }
    senjata.rangeAmmo += 0.002;

    if(senjataMusuh.rangeAmmo>5.0){
            senjataMusuh.rangeAmmo = 0.0;
    } else {
            senjataMusuh.rangeAmmo += 0.002;
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
	glutMainLoop();
	return 0;
}
