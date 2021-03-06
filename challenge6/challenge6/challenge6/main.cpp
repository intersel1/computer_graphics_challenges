#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

// Light properties
GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f}; //Color(0.2, 0.2, 0.2)

GLfloat light0color[]    = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light0position[] = { 15.0, 5.0, 15.0, 0.0 };		// x, y, z, 1:directional 0:point

GLfloat cameraDistance = 12, cameraAngle = 0;

// angles = { Y rotation at shoulder, Z rotation at shoulder, Z rotation at elbow, X rotation at wrist }
int arm_angles[5] = {0, 0, 0, 0, 0};
enum { SHOULDER_Y, SHOULDER_Z, ELBOW_Z, WRIST_X, WRIST_Z };

void change_angle(int angle, int delta, int minimum = 0, int maximum = 180) {
    arm_angles[angle] = (arm_angles[angle] + delta) % 360;
    arm_angles[angle] = max(arm_angles[angle], minimum);
    arm_angles[angle] = min(arm_angles[angle], maximum);
}

// Desplegar la escena 3D
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat mat_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat mat_blue[] = { 0.1, 0.2, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat low_shininess[] = { 2.0 };
    
    glLoadIdentity();
    GLfloat x = cameraDistance * sin(cameraAngle);
    GLfloat z = cameraDistance * cos(cameraAngle);
    gluLookAt (x, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
    glLightfv(GL_LIGHT0, GL_POSITION, light0position);
    
    glPushMatrix();
    glRotatef((GLfloat) arm_angles[SHOULDER_Y], 0., 1., 0.);
    glRotatef((GLfloat) arm_angles[SHOULDER_Z], 0., 0., 1.);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_blue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
    
    glTranslatef(1., 0., 0.);
    glPushMatrix();
    glScalef(2.0, 0.4, 1.0);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.);       // shoulder
    glPopMatrix();
    
    glTranslatef(1., 0., 0.);
    glRotatef((GLfloat) arm_angles[ELBOW_Z], 0., 0., 1.);
    glTranslatef(1., 0., 0.);
    glPushMatrix();
    glScalef(2.0, 0.4, 1.0);
    glColor3f(249./255., 238./255., 153./255.);
    glutSolidCube(1.);       // elbow
    glPopMatrix();
    
    glTranslatef(1., 0., 0.);
    glRotatef((GLfloat) arm_angles[WRIST_Z], 0., 0., 1.);
    glRotatef((GLfloat) arm_angles[WRIST_X], 1., 0., 0.);
    
    glTranslatef(1., 0., 0.);
    glPushMatrix();
    glScalef(2.0, 0.4, 1.0);
    glColor3f(207./255., 106./255., 76./255.);
    glutSolidCube(1.);       // wrist
    glPopMatrix();
    glPopMatrix();

    // Ejecutar los comandos 
    glutSwapBuffers();
}


// Se llama cuando cambia el tamaño de la ventana
void reshape(GLsizei w, GLsizei h) {
    // Tamanyo del viewport
	glViewport(0, 0, w, h);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 300.0);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Responder a los eventos de teclas especiales
void specialKeys(int key, int x, int y) {
    GLfloat distanceDelta = 1.0, angleDelta = 5 * M_PI / 180.0;
    if(key == GLUT_KEY_UP) {
        cameraDistance -= distanceDelta;
        cameraDistance = max((GLfloat)2.0, cameraDistance);
    }
    if(key == GLUT_KEY_DOWN) {
        cameraDistance += distanceDelta;
    }
    if(key == GLUT_KEY_LEFT) {
        cameraAngle -= angleDelta;
    }
    if(key == GLUT_KEY_RIGHT) {
        cameraAngle += angleDelta;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    int delta = 5;
    
    switch (key) {
        case 27: //ESC
            exit(0);
        case 'q':
            change_angle(SHOULDER_Y, delta, -180, 0);
            break;
        case 'a':
            change_angle(SHOULDER_Y, -delta, -180, 0);
            break;
        case 'w':
            change_angle(SHOULDER_Z, delta, -90, 90);
            break;
        case 's':
            change_angle(SHOULDER_Z, -delta, -90, 90);
            break;
        case 'e':
            change_angle(ELBOW_Z, delta, 0, 135);
            break;
        case 'd':
            change_angle(ELBOW_Z, -delta, 0, 135);
            break;
        case 'r':
            change_angle(WRIST_X, delta, -45, 45);
            break;
        case 'f':
            change_angle(WRIST_X, -delta, -45, 45);
            break;
        case 't':
            change_angle(WRIST_Z, delta, -15, 90);
            break;
        case 'g':
            change_angle(WRIST_Z, -delta, -15, 90);
            break;
    }
    glutPostRedisplay();
}

void init(){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

// Programa Principal
int main(int argc, char * argv[]) {
    glutInit(&argc, argv);    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Brazo Robot");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    
    // Inicialización
    init();
    glutMainLoop();
}

