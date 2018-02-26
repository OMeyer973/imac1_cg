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
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;





void resizeWindow() { /*redimensionnement de la fenetre*/
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-4.,4.,-3.,3.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) { /*Initialise et retourne un point avec sa mémoire alouée*/
    Point* tmp;
    tmp = malloc(sizeof(Point));
    if (tmp != NULL) {
       tmp->x = x;
       tmp->y = y;
       tmp->r = r;
       tmp->g = g;
       tmp->b = b; 
       tmp->next = NULL;
       return tmp;
    }
    return 0;
}

void addPointToList(Point* point, PointList* list) { /*ajoute un point à une liste chainée de points*/
    point->next = *list;
    *list = point;
}

void drawPointList(PointList list) { /*dessine tous les points d'une liste chaînée de points*/
    glBegin(GL_POINTS);
    while (list != NULL) {
       glColor3ub(list->r, list->g, list->b);
       glVertex2f(-1 + 2. * list->x / WINDOW_WIDTH, -(-1 + 2. * list->y / WINDOW_HEIGHT));
       list = list->next;
    }
    glEnd();
}

void drawBrokenLine(PointList list, int lineOver) { /*dessine Une ligne brisée entre tous les points d'une liste chaînée de points*/
    if (list == NULL) 
        return;
    PointList firstPoint = list;
    glBegin(GL_LINES);
    while (list->next != NULL) {
       glColor3ub(list->r, list->g, list->b);
       glVertex2f(list->x, list->y);
       glColor3ub(list->next->r, list->next->g, list->next->b);
       glVertex2f(list->next->x,list->next->y);
       list = list->next;
    }
    
    if (list != NULL && lineOver) {
    glColor3ub(list->r, list->g, list->b);
    glVertex2f(list->x, list->y);
    
    glColor3ub(firstPoint->r, firstPoint->g, firstPoint->b);
    glVertex2f(firstPoint->x, firstPoint->y);

    }
    
    glEnd();
}

void deletePoints (PointList* list) {
    /*libère les ressources utilisées par une liste de points*/
    PointList next;
    while (*list != NULL) {
       next = (*list)->next;
       free(*list);
       *list = next;
    }
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

void drawLandmark() {
    /*dessine une croix de 1x1 centré sur l'origine*/
    glBegin(GL_LINES);
        glColor3ub(255, 0, 0);
        glVertex2f(-0.5,0);
        glVertex2f(0.5,0);
        glColor3ub(0, 255, 0);
        glVertex2f(0,-0.5);
        glVertex2f(0,0.5);
    glEnd();
}

void drawCircle (int fill, int nbSeg) {
    /*dessine un cercle de diamètre 1, centré sur l'origine 
    et découpé en nbSeg segments*/
    float teta = 0;
    glBegin(GL_POLYGON);
    if (fill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    for (teta=0; teta<=2*PI; teta+=2*PI/nbSeg) {
        glVertex2f(0.5*cos(teta),0.5*sin(teta));
    }
    glEnd();       
}

void drawPoint(float x, float y) {
    glBegin(GL_POINTS);
        glColor3ub(255,255,255);
        glVertex2f(x,y);
    glEnd();       

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
    PointList pointList = NULL;
    int nbPoints = 0;
    int lineOver = 0; //est ce que le polygone a fini d'être tracé ?
    float cJauneX=0, cJauneY=0, cJauneAngle=0; //coordonées du carré jaune
    float RbleuX=0, rBleuY=0, rBleuAngle=0; //coordonées du rond bleu
    int dragging=0; //si on est entrain de faire un drag & drop
    int time=0;

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
        /*exo2*/

        drawBrokenLine(pointList, lineOver);
        gluOrtho2D(-4.,4.,-3.,3.);
        glColor3ub(255, 255, 255);
        drawSquare(0);
        drawLandmark();
        glColor3ub(122, 122, 122);
        drawCircle(0,20);
        /*exo3*/
        //repère
        int i, j;
        for (i=-4; i<=4; i++) {        
            for (j=-4; j<=4; j++) 
                drawPoint(i, j);
        }
        glScalef(8,6,1);
        drawLandmark();
        glLoadIdentity();
        //cercle orange
        //gluOrtho2D(-5,3,-5,1); // marche aussi à la place du repère + le translate !
        gluOrtho2D(-4.,4.,-3.,3.);
        glTranslatef(1,2,0);
        glColor3ub(255, 100, 0);
        drawCircle(0,20);
        glLoadIdentity();
        //carré rouge
        glColor3ub(255, 0, 0);
        gluOrtho2D(-4,4,-3,3);
        glRotatef(45,0,0,1);
        glTranslatef(2,0,0);
        drawSquare(0);
        glLoadIdentity();
        //carré violet
        glColor3ub(255, 0, 255);
        gluOrtho2D(-4,4,-3,3);
        glTranslatef(2,0,0);
        glRotatef(45,0,0,1);
        drawSquare(0);
        glLoadIdentity();
        //en inversant translation et rotation, le carré n'est pas au même endroit, on constate bien un changement de repère
        //carré jaune
        glColor3ub(255, 255, 0);
        glTranslatef(cJauneX,cJauneY,0); //on fait la translation avant car les coordonées de la souris sont repérées entre -1 et 1
        gluOrtho2D(-4,4,-3,3);
        glRotatef(cJauneAngle,0,0,1);
        drawSquare(0);
        glLoadIdentity();
        //rond bleu
        glColor3ub(50, 50, 255);
        gluOrtho2D(-4,4,-3,3);
        glTranslatef(RbleuX,rBleuY,0);
        drawCircle(0,40);
        glLoadIdentity();
        RbleuX += cos(rBleuAngle)/70;
        RbleuX = clamp(RbleuX,-4,4);
        rBleuY += sin(rBleuAngle)/70;
        rBleuY = clamp(rBleuY,-3,3);
        if (time >= 20) {
            rBleuAngle = rand();
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
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    dragging=1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                x = -1 + 2. * e.button.x / WINDOW_WIDTH;
                y = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);    
                
                if (lineOver == 0) {
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    Point* tmpPoint;
                    tmpPoint = allocPoint(x, y, 255, 255, 255);
                    addPointToList(tmpPoint, &pointList);
                }
                nbPoints = (nbPoints + 1);
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    lineOver = 1;
                    dragging = 0;
                }
                if (e.button.button == SDL_BUTTON_LEFT) {
                    cJauneX = x;
                    cJauneY = y;
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging) {
                    x = -1 + 2. * e.button.x / WINDOW_WIDTH;
                    cJauneAngle = x*360;
                }                    
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
    
    deletePoints(&pointList);

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
