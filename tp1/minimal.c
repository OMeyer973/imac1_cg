/*
bastienlaby.fr/imac/
*/

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

void resizeWindow();


/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


int main(int argc, char** argv) {

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);*/
    
    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Moi je préfère les pâtes au gruyère", NULL);

    glClearColor(0.1, 0.1, 0.1, 1); /*couleur de fond originale*/
    float red, green, blue; /*tmp colors*/
    int x[3], y[3]; /*tmp coords*/
    int maxPoints = 1; /*nombre de points nécéssaire avant de pouvoir dessiner la primitive*/
    int nbPoints = 0; /*nombre de points scannés*/
    int color[3] = {255,255,255}; /*couleur active*/
    int paletteScreen = 0; /*est ce que la pallete est affichée ?*/
    char mode = 'p'; /*mode de dessin : p point, l ligne, t triangle, ' ' palette*/

    /*on va dessiner en blanc*/
    glColor3ub(color[0],color[1],color[2]);

    /* Boucle d'affichage */
    int loop = 1;

    glClear(GL_COLOR_BUFFER_BIT);
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        if (nbPoints == maxPoints) {
            switch (mode) {
                case 'p':
                printf("dessin d'un point\n");
                    glBegin(GL_POINTS);
                        glVertex2f(-1 + 2. * x[0] / WINDOW_WIDTH, -(-1 + 2. * y[0] /WINDOW_HEIGHT));
                    glEnd();
                break;

                case 'l':
                printf("dessin d'une ligne\n");
                    glBegin(GL_LINES);
                        glVertex2f(-1 + 2. * x[0] / WINDOW_WIDTH, -(-1 + 2. * y[0] /WINDOW_HEIGHT));
                        glVertex2f(-1 + 2. * x[1] / WINDOW_WIDTH, -(-1 + 2. * y[1] /WINDOW_HEIGHT));
                    glEnd();
                break;
                
                case 't':
                printf("dessin d'un triangle\n");
                    glBegin(GL_TRIANGLES);
                        glVertex2f(-1 + 2. * x[0] / WINDOW_WIDTH, -(-1 + 2. * y[0] / WINDOW_HEIGHT));
                        glVertex2f(-1 + 2. * x[1] / WINDOW_WIDTH, -(-1 + 2. * y[1] / WINDOW_HEIGHT));
                        glVertex2f(-1 + 2. * x[2] / WINDOW_WIDTH, -(-1 + 2. * y[2] / WINDOW_HEIGHT));
                    glEnd();
                break;

                default:
                    break;
            }
        }


        if (paletteScreen) {
            glBegin(GL_QUADS);
                glColor3ub(23, 18, 25);
                glVertex2f(-1 + 2. * 0, -(-1 + 2. * 0));
                glVertex2f(-1 + 2. * 0, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 1/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 1/5, -(-1 + 2. * 0));
                
                glColor3ub(34, 85, 96);
                glVertex2f(-1 + 2. * 1/5, -(-1 + 2. * 0));
                glVertex2f(-1 + 2. * 1/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 2/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 2/5, -(-1 + 2. * 0));
                                
                glColor3ub(237,240,96);
                glVertex2f(-1 + 2. * 2/5, -(-1 + 2. * 0));
                glVertex2f(-1 + 2. * 2/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 3/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 3/5, -(-1 + 2. * 0));
                
                glColor3ub(240,128,60);
                glVertex2f(-1 + 2. * 3/5, -(-1 + 2. * 0));
                glVertex2f(-1 + 2. * 3/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 4/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 4/5, -(-1 + 2. * 0));
                
                glColor3ub(49,13,32);
                glVertex2f(-1 + 2. * 4/5, -(-1 + 2. * 0));
                glVertex2f(-1 + 2. * 4/5, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 1, -(-1 + 2. * 1));
                glVertex2f(-1 + 2. * 1, -(-1 + 2. * 0));

                glColor3ub(color[0],color[1],color[2]);
            glEnd();
        }

        if (nbPoints >= maxPoints) {
            nbPoints = 0;
        }
        
        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();
        

        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            
            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) {

                /* L'utilisateur ferme la fenêtre : */
                case SDL_QUIT:
                loop = 0;
                break;

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);

                    if (paletteScreen) {
                        if (e.button.x >= 0 && e.button.x < 1. / 5 * WINDOW_WIDTH) {
                            color[0] = 23;
                            color[1] = 18;
                            color[2] = 25;
                        } else if (e.button.x < 2. / 5 * WINDOW_WIDTH) {
                            color[0] = 34;
                            color[1] = 85;
                            color[2] = 96;
                        } else if (e.button.x < 3. / 5 * WINDOW_WIDTH) {
                            color[0] = 237;
                            color[1] = 240;
                            color[2] = 96;
                        } else if (e.button.x < 4. / 5 * WINDOW_WIDTH) {
                            color[0] = 240;
                            color[1] = 128;
                            color[2] = 60;
                        }else {
                            color[0] = 49;
                            color[1] = 13;
                            color[2] = 32;
                        }

                    } else {
                        x[nbPoints] = e.button.x;
                        y[nbPoints] = e.button.y;

                        nbPoints = (nbPoints + 1);

                        printf("point enregistré\n");
                    }
                    break;

                /* move souris */
                case SDL_MOUSEMOTION:
                    
                    /*changer la couleur de fond au mouvement de souris*/
                    /*                    
                    red = (float)e.button.x / WINDOW_WIDTH;
                    green = e.button.y; 
                    green /= WINDOW_HEIGHT;
                    blue = 0;
                    
                    glClearColor((float)e.button.x / WINDOW_WIDTH, green, blue, 1);
                    glClear(GL_COLOR_BUFFER_BIT);
                    */
                    break;

                /* Touche clavier */
                case SDL_KEYUP:
                    printf("touche levée (code = %d)\n", e.key.keysym.sym);
                    if (e.key.keysym.sym == 'q') {
                        printf("quit\n");
                        loop = 0;
                    }else if (e.key.keysym.sym == 'p') {
                        printf("mode points\n");
                        mode = 'p';
                        maxPoints = 1;
                    }else if (e.key.keysym.sym == 'l') {
                        printf("mode lignes\n");
                        mode = 'l';
                        maxPoints = 2;
                    }else if (e.key.keysym.sym == 't') {
                        printf("mode triangles\n");
                        mode = 't';
                        maxPoints = 3;
                    }else if (e.key.keysym.sym == ' ') {
                        printf("fin mode palette\n");
                        paletteScreen = 0;
                    }
                    break;
                
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);

                    if (e.key.keysym.sym == ' ') {
                        printf("mode palette\n");
                        paletteScreen = 1;
                    }
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


void resizeWindow() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.,-1.,-1.,1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
}