#include "cpu.h"
#include <GLUT/glut.h>
#include <iostream>

// globals
C8CPU* c8 = new C8CPU();    
int speed = 10; 

void init()
{
    glViewport(0, 0, width, height);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0, 0, 0, 1.0);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);
}

void display() 
{
    c8->C8CPU::decodeOpcode();
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glRasterPos2i(-1, 1);
    glPixelZoom(10, -10);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, c8->pixels);
    glutSwapBuffers();
}

void clock(int t)
{
    glutPostRedisplay();
    glutTimerFunc(speed, clock, 1);
}

void keyboardPress(unsigned char key, int x, int y)
{
    switch(key) {
        case 'q': exit(0);
        case '0': c8->keyPressed[0x0] = true;
        case '1': c8->keyPressed[0x1] = true;
        case '2': c8->keyPressed[0x2] = true;
        case '3': c8->keyPressed[0x3] = true;
        case '4': c8->keyPressed[0x4] = true;
        case '5': c8->keyPressed[0x5] = true;
        case '6': c8->keyPressed[0x6] = true;
        case '7': c8->keyPressed[0x7] = true;
        case '8': c8->keyPressed[0x8] = true;
        case '9': c8->keyPressed[0x9] = true;
        case 'a': c8->keyPressed[0xA] = true;
        case 'b': c8->keyPressed[0xB] = true;
        case 'c': c8->keyPressed[0xC] = true;
        case 'd': c8->keyPressed[0xD] = true;
        case 'e': c8->keyPressed[0xE] = true;
        case 'f': c8->keyPressed[0xF] = true;
    }
}

void keyboardRelease(unsigned char key, int x, int y)
{
    switch(key) {
        case '0': c8->keyPressed[0x0] = false;
        case '1': c8->keyPressed[0x1] = false;
        case '2': c8->keyPressed[0x2] = false;
        case '3': c8->keyPressed[0x3] = false;
        case '4': c8->keyPressed[0x4] = false;
        case '5': c8->keyPressed[0x5] = false;
        case '6': c8->keyPressed[0x6] = false;
        case '7': c8->keyPressed[0x7] = false;
        case '8': c8->keyPressed[0x8] = false;
        case '9': c8->keyPressed[0x9] = false;
        case 'a': c8->keyPressed[0xA] = false;
        case 'b': c8->keyPressed[0xB] = false;
        case 'c': c8->keyPressed[0xC] = false;
        case 'd': c8->keyPressed[0xD] = false;
        case 'e': c8->keyPressed[0xE] = false;
        case 'f': c8->keyPressed[0xF] = false;
    }
}

int main(int args, char ** argv)
{
    if (args != 3) {
        std::cout << "Usage: ./chip8.bin -[filename] -[speed]" << std::endl;
        return -1;
    }
    c8->filename = argv[1];
    speed = *argv[2];
    glutInit(&args, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(350, 100);
    glutInitWindowSize(width * 10, height * 10);
    glutCreateWindow("C8 Emulator");
    glutTimerFunc(400, clock, 0);
    glutKeyboardFunc(keyboardPress);
    glutKeyboardUpFunc(keyboardRelease);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
