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

    /* couleur de rafraichissement */
    glClearColor(0.1, 0.1, 0.1, 1);

    float red, green, blue; /*tmp colors*/
    int x = 0, y = 0; /*tmp coords*/

    /*on va dessiner en blanc*/
    glColor3ub(255,255,255);

    /* Boucle d'affichage */
    int loop = 1;

    glClear(GL_COLOR_BUFFER_BIT);
    while(loop) {


        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        glBegin(GL_POINTS);
            glVertex2f(50,50);
            glVertex2f(-1 + 2. * x / WINDOW_WIDTH, -(-1 + 2. * y /WINDOW_HEIGHT));
        glEnd();
        
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

                    /* changer la couleur de fond au clic
                    red = e.button.x;
                    red /= WINDOW_HEIGHT;
                    green = e.button.y; 
                    green /= WINDOW_WIDTH;
                    blue = 0;

                    glClearColor(red, green, blue, 1);
                    */
                    x = e.button.x;
                    y = e.button.y;

                    printf("dessin d'un point\n");


                    break;

                /* move souris */
                case SDL_MOUSEMOTION:
                    /*changer la couleur de fond au mouvement de souris
                    red = e.button.x;
                    red /= WINDOW_WIDTH;
                    green = e.button.y; 
                    green /= WINDOW_HEIGHT;
                    blue = 0;

                    printf("changement de couleur de fond et clear\n");
                    glClearColor(red, green, blue, 1);
                    */
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    if (e.key.keysym.sym == 113/*Q*/) {
                        loop = 0;
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