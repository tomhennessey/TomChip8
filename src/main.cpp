#include <iostream>
#include <GL/glut.h>
#include "../include/cpu.h"
#include "../include/defines.h"

// globals
C8CPU* cpu = new C8CPU();
int speed = 1; 
  
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
    cpu->C8CPU::decodeOpcode();
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glRasterPos2i(-1, 1);
    glPixelZoom(10, -10);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, cpu->pixels);
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
        case '0': cpu->keyPressed[0x0] = true;
        case '1': cpu->keyPressed[0x1] = true;
        case '2': cpu->keyPressed[0x2] = true;
        case '3': cpu->keyPressed[0x3] = true;
        case '4': cpu->keyPressed[0x4] = true;
        case '5': cpu->keyPressed[0x5] = true;
        case '6': cpu->keyPressed[0x6] = true;
        case '7': cpu->keyPressed[0x7] = true;
        case '8': cpu->keyPressed[0x8] = true;
        case '9': cpu->keyPressed[0x9] = true;
        case 'a': cpu->keyPressed[0xA] = true;
        case 'b': cpu->keyPressed[0xB] = true;
        case 'c': cpu->keyPressed[0xC] = true;
        case 'd': cpu->keyPressed[0xD] = true;
        case 'e': cpu->keyPressed[0xE] = true;
        case 'f': cpu->keyPressed[0xF] = true;
    }
}

void keyboardRelease(unsigned char key, int x, int y)
{
    switch(key) {
        case '0': cpu->keyPressed[0x0] = false;
        case '1': cpu->keyPressed[0x1] = false;
        case '2': cpu->keyPressed[0x2] = false;
        case '3': cpu->keyPressed[0x3] = false;
        case '4': cpu->keyPressed[0x4] = false;
        case '5': cpu->keyPressed[0x5] = false;
        case '6': cpu->keyPressed[0x6] = false;
        case '7': cpu->keyPressed[0x7] = false;
        case '8': cpu->keyPressed[0x8] = false;
        case '9': cpu->keyPressed[0x9] = false;
        case 'a': cpu->keyPressed[0xA] = false;
        case 'b': cpu->keyPressed[0xB] = false;
        case 'c': cpu->keyPressed[0xC] = false;
        case 'd': cpu->keyPressed[0xD] = false;
        case 'e': cpu->keyPressed[0xE] = false;
        case 'f': cpu->keyPressed[0xF] = false;
    }
}

int main(int args, char ** argv)
{
    if (args > 3) {
        std::cout << "Usage: ./chip8.bin -[filename] -[speed]" << std::endl;
        return -1;
    }
    // dropping command line to hardcode until things get working..
    /*
    cpu->filename = static_cast<std::string>(argv[1]);
    speed = static_cast<int>(*argv[2]);
    */
    cpu->filename = static_cast<std::string>("./roms/pong.ch8");
    speed = 1;
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
