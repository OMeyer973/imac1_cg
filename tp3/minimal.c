#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define PI 3.1415926535898

typedef struct point {
    float x, y; //position
    unsigned char r,g,b; //couleur
    struct point* next; //point suivant à dessiner
} Point, *PointList;


/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;





void resizeWindow() { /*redimensionnement de la fenetre*/
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.,100.,-100.,100.);
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

void drawCircle (int fill, int nbSeg) {
    /*dessine un cercle de diamètre 1, centré sur l'origine 
    et découpé en nbSeg segments*/
    float teta = 0;
    int i=0;
    if (fill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glBegin(GL_POLYGON);
    for (i=0; i<nbSeg; i++) {
        teta = i*2*PI/nbSeg;
        glVertex2f(0.5*cos(teta),0.5*sin(teta));
    }
    glEnd();       
}

void drawPoint(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x,y);
    glEnd();       

}

void drawRepere(int w, int h) {
    //dessine un repère de case de 10x10 de largenr 2*w et hauteur 2*h
    int i, j;
    for (i=-w; i<=w; i+=10) {        
        for (j=-h; j<=h; j+=10) 
            drawPoint(i, j);
    }
}

void drawRoundedSquare (float r) {
    //dessine un carré cannonique arrondi avec r comme rayon d'arrondi
    //ronds pour les coins
    glPushMatrix();
        glTranslatef(0.5-r,0.5-r,0);
        glScalef(2*r,2*r,1);
        drawCircle(1,20);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.5-r,-(0.5-r),0);
        glScalef(2*r,2*r,1);
        drawCircle(1,20);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-(0.5-r),0.5-r,0);
        glScalef(2*r,2*r,1);
        drawCircle(1,20);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-(0.5-r),-(0.5-r),0);
        glScalef(2*r,2*r,1);
        drawCircle(1,20);
    glPopMatrix();
    //rectangles pour le centre
    glPushMatrix();
        glScalef(1-2*r,1,1);
        drawSquare(1);
    glPopMatrix();
    glPushMatrix();
        glScalef(1,1-2*r,1);
        drawSquare(1);
    glPopMatrix();
}

void drawFirstArm() {
    //distance des joints : 60
    glColor3ub(255, 50, 0);
    //cercles
    glPushMatrix();
        glScalef(40,40,1);
        drawCircle(1,20);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(60,0,0);
        glScalef(20,20,1);
        drawCircle(1,20);
    glPopMatrix();
    //connection
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
        glVertex2f(0,20);
        glVertex2f(60,10);
        glVertex2f(60,-10);
        glVertex2f(0,-20);
    glEnd();
}

void drawSecondArm() {
    //distance des joints : 40
    //bouts
    glColor3ub(150, 200, 0);
    glPushMatrix();
        glScalef(10,10,1);
        drawRoundedSquare(0.3);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(40,0,0);
        glScalef(10,10,1);
        drawRoundedSquare(0.3);
    glPopMatrix();
    glColor3ub(100, 100, 0);
    //bras
    glPushMatrix();
        glTranslatef(20,0,0);
        glScalef(46,6,1);
        drawSquare(1);
    glPopMatrix();   
}

void drawThirdArm() {
    //distance des joints : 37
    //bouts carré
    glColor3ub(0, 200, 255);
    glPushMatrix();
        glScalef(6,6,1);
        drawRoundedSquare(0.2);
    glPopMatrix();
    //bout rond
    glColor3ub(0, 100, 255);
    glPushMatrix();
        glTranslatef(37,0,0);
        glScalef(8,8,1);
        drawCircle(1,10);
    glPopMatrix();
    //bras
    glPushMatrix();
        glTranslatef(18,0,0);
        glScalef(40,4,1);
        drawSquare(1);
    glPopMatrix();   
}

void drawArm(float a, float b, float c) {
    //dessine le bras complet avec a,b,c comme angles entre les 3 bras
    glPushMatrix();
        glRotatef(a,0,0,1);
        drawFirstArm();
        
        glTranslatef(60,0,0);
        glRotatef(b,0,0,1);
        drawSecondArm();

        glTranslatef(40,0,0);
        glRotatef(c,0,0,1);
        drawThirdArm();
    glPopMatrix(); 

}

float min (float a, float b) {
    if (a <= b)
        return a;
    return b;
}

float max (float a, float b) {
    if (a >= b)
        return a;
    return b;
}

float clamp(float a,float b,float c) {
    //clamp a betwen b & c
    return min(max(a,b),c);
}

int main(int argc, char** argv) {
    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
       fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
       return EXIT_FAILURE;
    }
    
    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
       fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
       return EXIT_FAILURE;
    }

    //variables utilisées
    float x,y; //coordonées temporaires de la souris
    float alpha=0, beta=0, gamma=0; //angles des bras
    float dAlpha=0, dBeta=0, dGamma=0; //vitesse de rotation des bras
    int time=0; //variable tmp de temps pour pouvoir modifier les angles des bras de manière aléatoire


    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Nous on AIME LA FORET", NULL);
    
    /*initialisation de la matrice de transforamtion 
    pour afficher un repère allant de -4 à 4 en x et -3 à 3 en y, centré en 0*/
    gluOrtho2D(-4.,4.,-3.,3.);
        
    /* Boucle d'affichage */
    int loop = 1;
    
    while(loop) {
        glClear(GL_COLOR_BUFFER_BIT);

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        gluOrtho2D(-100.,100.,-100.,100.);
        /*
        glColor3ub(255, 110, 255);
        drawRoundedSquare(0.3);
        drawFirstArm();
        drawSecondArm();
        drawThirdArm();
        */
        drawArm(alpha,beta,gamma);
        //glColor3ub(255, 255, 255);
        drawRepere(100,100);
        glLoadIdentity();

        alpha += dAlpha;
        beta += dBeta;
        gamma += dGamma;
        if (time >= 20) {
            dAlpha = (rand()%100-50)/50.;
            dBeta = (rand()%100-50)/50.;
            dGamma = (rand()%100-50)/50.;
            time = 0;
        }
        time++;

        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();
       
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
        /* L'utilisateur ferme la fenêtre : */
        if(e.type == SDL_QUIT) {
            loop = 0;
            break;
       }
        
        /* Quelques exemples de traitement d'evenements : */
        switch(e.type) {
            /* Clic souris */
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                x = -1 + 2. * e.button.x / WINDOW_WIDTH;
                y = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);    
                
                if (e.button.button == SDL_BUTTON_RIGHT) {
                }
                if (e.button.button == SDL_BUTTON_LEFT) {
                }
                break;

            case SDL_MOUSEMOTION:                    
                break;
           
            /* Touche clavier */
            case SDL_KEYDOWN:
                printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                break;
            /* resize */
            case SDL_VIDEORESIZE:
                WINDOW_HEIGHT = e.resize.h;
                WINDOW_WIDTH = e.resize.w;
                printf("RESIZED to %d, %d\n",WINDOW_WIDTH, WINDOW_HEIGHT);
                resizeWindow();
                break;

            default:
                break;
          }
       }
       /* Calcul du temps écoulé */
       Uint32 elapsedTime = SDL_GetTicks() - startTime;
       /* Si trop peu de temps s'est écoulé, on met en pause le programme */
       if(elapsedTime < FRAMERATE_MILLISECONDS) {
        SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
       }
    }

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
