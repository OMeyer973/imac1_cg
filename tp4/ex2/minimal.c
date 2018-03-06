#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include <dirent.h>
#include <time.h> 

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

void drawTexturedSquare(GLuint textureID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);        
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1);
                glVertex2f(-0.5,-0.5);
                glTexCoord2f(0, 0);
                glVertex2f(-0.5,0.5);
                glTexCoord2f(1, 0);
                glVertex2f(0.5,0.5);
                glTexCoord2f(1, 1);
                glVertex2f(0.5,-0.5);
            glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
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
    //si le premier caractère est un entier, on le retourne
    if ('0' <= filename[0] && filename[0] <= '9') {
        return filename[0] - '0';
    }
    //sinon on retourne 10 pour l'image des 2 points
    return 10;
}

void makeTimeIDs(struct tm * timeinfo, int* timeIDs) {
    /*on stocke les identifiants des textures à afficher dans l'ordre dans le tableau timeIDs*/
    /*
    printf("hours %d\n", timeinfo->tm_hour);
    printf("minutes %d\n", timeinfo->tm_min);
    printf("seconds %d\n", timeinfo->tm_sec);
    */
    timeIDs[0] = timeinfo->tm_hour/10;
    timeIDs[1] = timeinfo->tm_hour%10;
    timeIDs[2] = 10;
    timeIDs[3] = timeinfo->tm_min/10;
    timeIDs[4] = timeinfo->tm_min%10;
    timeIDs[5] = 10;
    timeIDs[6] = timeinfo->tm_sec/10;
    timeIDs[7] = timeinfo->tm_sec%10;
}

void drawClock(GLuint texNumbers[], int* timeIDs) {
    /*dessin de l'horloge à partir du tableau des ids des textures à afficher*/
    float nbScale = 0.2;
    float nbSpacing = 2;
    int i = 0;
    for(i=0; i<8; i++){
        glPushMatrix();
            glTranslatef((0.1*i-0.4)*nbSpacing+nbScale/2,0,0);
            glScalef(nbScale,nbScale,1);
            drawTexturedSquare(texNumbers[timeIDs[i]]);
        glPopMatrix();
    }
}

char* filenames[11];
GLuint textureID;
GLuint texNumbers[11];
int timeIDs[8];

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
    char* dirPath = "./numbers/";
    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);

    if (!d) {
        printf("erreur à l'ouverture du dossier\n");
        return 0;
    }

    // Chargement et traitement de la texture
    SDL_Surface* numberImg[11];
    glGenTextures(11, texNumbers);    

    int i = 0;

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
            
            numberImg[i] = IMG_Load(strTmp);
            if (!isLoaded(numberImg[i])) return 0;

            glBindTexture(GL_TEXTURE_2D, texNumbers[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            // Envoie des données texture à la carte graphique
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                numberImg[i]->w,
                numberImg[i]->h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                numberImg[i]->pixels);
            
            // Libération des données CPU
            SDL_FreeSurface(numberImg[i]);
            glBindTexture(GL_TEXTURE_2D, 0);
        }   
    }
    closedir(d);

    //Boucle de dessin
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        //get time !
        time_t rawtime;
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        //make tab of ids of tex to print
        makeTimeIDs(timeinfo, timeIDs);

        // Code de dessin
        glClear(GL_COLOR_BUFFER_BIT);
        drawClock(texNumbers, timeIDs);

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
