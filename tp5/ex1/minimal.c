#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535898
#define NBSEG 70

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

struct tm * timeinfo;


void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void drawSquare(int fill) {
    /*dessine un carré de 1x1 centré sur l'origine*/
    if (fill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
        glVertex2f(-0.5,-0.5);
        glVertex2f(-0.5,0.5);
        glVertex2f(0.5,0.5);
        glVertex2f(0.5,-0.5);
    glEnd();
}

void drawCircle (int fill) {
    /*dessine un cercle de diamètre 1, centré sur l'origine 
    et découpé en NBSEG segments*/
    float teta = 0;
    int i=0;
    if (fill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glBegin(GL_POLYGON);
    for (i=0; i<NBSEG; i++) {
        teta = i*2*PI/NBSEG;
        glVertex2f(0.5*cos(teta),0.5*sin(teta));
    }
    glEnd();       
}

void drawClockBG() {
    glPushMatrix();
        glColor3ub(0, 0, 0);
        glScalef(2,2,1);
        drawCircle(1);

        glColor3ub(255, 255, 255);
        glScalef(0.95,0.95,1);
        drawCircle(1);
        
        glColor3ub(0, 0, 0);
        glScalef(0.98,0.98,0);
        drawCircle(1);
        
        glColor3ub(255, 255, 255);
        glScalef(0.99,0.99,0);
        drawCircle(1);
    glPopMatrix();
}

void drawClockTicks() {
    int i=0;
    glColor3ub(0,0,0);
    for (i=0; i<60; i++) {
        if (i%5==0) {
            glPushMatrix();
                glRotatef(6 * i,0,0,1);
                glTranslatef(0.75,0,0);
                glScalef(0.15,0.02,0);
                drawSquare(1);
            glPopMatrix();   
        } else {
            glPushMatrix();
                glRotatef(6 * i,0,0,1);
                glTranslatef(0.75,0,0);
                glScalef(0.1,0.01,0);
                drawSquare(1);
            glPopMatrix();
        }
    }
}

int createClockIDList() {
    //distance des joints : 60
    GLuint id = glGenLists(1);
    glNewList(id, GL_COMPILE);
        drawClockBG();
        drawClockTicks();
    glEndList();
    return id;
}

void drawSecBranch(int a) {
    glPushMatrix();
        glRotatef(a,0,0,1);
        glScalef(0.9,0.007,0);
        glTranslatef(0.4,0,0);
        drawSquare(1);
    glPopMatrix();
}

void drawMinBranch(int a) {
    glPushMatrix();
        glRotatef(a,0,0,1);
        glScalef(0.8,0.015,0);
        glTranslatef(0.4,0,0);
        drawSquare(1);
    glPopMatrix();
}

void drawHourBranch(int a) {
    glPushMatrix();
        glRotatef(a,0,0,1);
        glScalef(0.6,0.022,0);
        glTranslatef(0.4,0,0);
        drawSquare(1);
    glPopMatrix();
}

int main(int argc, char** argv) {

    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Ouverture d'une fenêtre et création d'un contexte OpenGL
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("td05", NULL);
    resizeViewport();

    // Variables utiles

    time_t rawtime;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    int currSec = timeinfo->tm_sec;

    int clockID = createClockIDList();
    float angleSec, angleMin, angleHour;


 
    //Boucle de dessin
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        time_t rawtime;
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        
        /*
        printf("hours %d\n", timeinfo->tm_hour);
        printf("minutes %d\n", timeinfo->tm_min);
        printf("seconds %d\n", timeinfo->tm_sec);
        */
        
        // Code de dessin
        if (currSec != timeinfo->tm_sec) {
            glClear(GL_COLOR_BUFFER_BIT);
            glCallList(clockID);
            glColor3ub(0,0,0);
            
            angleHour = - timeinfo->tm_hour * 360 / 24;
            angleMin = - timeinfo->tm_min * 360 / 60;
            angleSec = - timeinfo->tm_sec * 360 / 60;

            drawHourBranch(angleHour + (angleMin + angleSec / 60) /60);
            drawMinBranch(angleMin + angleSec / 60);
            drawSecBranch(angleSec);
            
            currSec = timeinfo->tm_sec;
        }
        // Fin du code de dessin

        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();

                default:
                    break;
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    
    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
