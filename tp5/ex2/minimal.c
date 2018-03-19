#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include <dirent.h>
#include <math.h>

#define PI 3.1415926535898
#define NBSEG 32

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


typedef struct infoPlanete{
    int diametre; //km
    int periodeOrbit; //j
    int dureeJournee; //h
    int distSoleil; //10^6 km
} InfoPlanete;

InfoPlanete infoSysteme[9];

void initInfoSysteme(InfoPlanete infoSysteme[9]) {
    infoSysteme[0].diametre = 696392;
    infoSysteme[1].diametre = 0.330;
    infoSysteme[2].diametre = 4.87;
    infoSysteme[3].diametre = 5.97;
    infoSysteme[4].diametre = 0.073;
    infoSysteme[5].diametre = 0.642;
    infoSysteme[6].diametre = 1898;
    infoSysteme[7].diametre = 568;
    infoSysteme[8].diametre = 86.8;
    infoSysteme[9].diametre = 102;

    infoSysteme[0].periodeOrbit = 1000000;
    infoSysteme[1].periodeOrbit = 88.0;
    infoSysteme[2].periodeOrbit = 224.7;
    infoSysteme[3].periodeOrbit = 365.2;
    infoSysteme[4].periodeOrbit = 27.3;
    infoSysteme[5].periodeOrbit = 687.0;
    infoSysteme[6].periodeOrbit = 4331;
    infoSysteme[7].periodeOrbit = 10747;
    infoSysteme[8].periodeOrbit = 30589;
    infoSysteme[9].periodeOrbit = 59800;

    infoSysteme[0].distSoleil = 0;
    infoSysteme[1].distSoleil = 57.9;
    infoSysteme[2].distSoleil = 108.2;
    infoSysteme[3].distSoleil = 149.6;
    infoSysteme[4].distSoleil = 0.384;
    infoSysteme[5].distSoleil = 227.9;
    infoSysteme[6].distSoleil = 778.6;
    infoSysteme[7].distSoleil = 1433.5;
    infoSysteme[8].distSoleil = 2872.5;
    infoSysteme[9].distSoleil = 4495.1;
}

void afficheInfoSys(InfoPlanete infoSysteme[9]) {
int i = 0;
    for (i=0; i<10; i++) {
        printf("planete : %d\n", i);
        printf("    diametre : %d\n", infoSysteme[i].diametre);
        printf("    periodeOrbit : %d\n", infoSysteme[i].periodeOrbit);
        printf("    distSoleil : %d\n", infoSysteme[i].distSoleil);
    }
}

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void drawTexturedCircle(GLuint textureID) {
    /*dessine un cercle de diamètre 1, centré sur l'origine 
    et découpé en NBSEG segments*/
    float teta = 0;
    int i=0;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);  
    glBegin(GL_POLYGON);
    for (i=0; i<NBSEG; i++) {
        teta = i*2*PI/NBSEG;
        glTexCoord2f(0.5*cos(teta)+0.5,-0.5*sin(teta)+0.5);
        glVertex2f(0.5*cos(teta),0.5*sin(teta));
    }
    glEnd();       
}

int isLoaded (SDL_Surface* image) {
    if (image == NULL) {
        printf("Texture loading failed\n");
        SDL_Quit();
        return 0;
    }
    return 1;
}

int getImgId(char* filename) {
    /*retourne l'id de l'image à partir de son nom de fichier*/
    return strtol(filename, NULL, 10); //converti les premiers caractère char* filename en entier base 10
}


void drawSolarSystem(GLuint texImgs[], InfoPlanete infoSysteme[], int timer) {
    /*dessin de l'horloge à partir du tableau des ids des textures à afficher*/
    float scale = 0.1;
    float spacing = 1.2;
    int i = 0;
    for(i=0; i<8; i++){
        glPushMatrix();
            glRotatef(1000.0 / infoSysteme[i].periodeOrbit * timer,0,0,1);
            glTranslatef((float)infoSysteme[i].distSoleil / 2000,0,0);
            glScalef((float)infoSysteme[i].diametre /80000000, (float)infoSysteme[i].diametre/80000000,1);
            drawTexturedCircle(texImgs[i]);
        glPopMatrix();
    }
}

char* filenames[11];
GLuint textureID;
GLuint texImgs[11];

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
    SDL_WM_SetCaption("td04", NULL);
    resizeViewport();


    //ouverture et parcours du dossier contenant les images    
    char* dirPath = "./img/";
    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);

    if (!d) {
        printf("erreur à l'ouverture du dossier\n");
        return 0;
    }

    // Chargement et traitement de la texture
    SDL_Surface* imgs[11];
    glGenTextures(11, texImgs);    

    int i = 0;
    int timer = 0; //mon entier incrémenté utilisé pour mesurer le temps 

    initInfoSysteme(infoSysteme);
    afficheInfoSys(infoSysteme);

    //parcours du dossier des images nombres
    while ((dir = readdir(d)) != NULL) {
        //on veut juste cibler nos fichiers images (pas le . et ..)
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
            
            i = getImgId(dir->d_name);
            
            //strtmp : chemin complet de l'image pour chargement
            char strTmp[50];
            strcpy(strTmp, dirPath);
            strcat(strTmp, dir->d_name);
            printf("Loading tex %s\n", strTmp);
            
            imgs[i] = IMG_Load(strTmp);
            if (!isLoaded(imgs[i])) {
                printf("img not loaded correctly : %d abort\n", i);
                return 0;  
            } 
            
            glBindTexture(GL_TEXTURE_2D, texImgs[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            
            // Envoie des données texture à la carte graphique
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                imgs[i]->w,
                imgs[i]->h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                imgs[i]->pixels);
            
            // Libération des données CPU
            SDL_FreeSurface(imgs[i]);
            glBindTexture(GL_TEXTURE_2D, 0);

            
        }   
    }
    closedir(d);

    //Boucle de dessin
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        timer++;

        // Code de dessin
        glClear(GL_COLOR_BUFFER_BIT);
        drawSolarSystem(texImgs, infoSysteme, timer);

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
    

    // Libération des données GPU
    glDeleteTextures(1, &textureID);

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
