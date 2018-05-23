#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "header.h"

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

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

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0., 10., 0., 10.);  // Ici on précise que nos coordonées vont de 0 à 10 pour chaque axe
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Jeu", NULL);

    /* Varriables des écrans */
    int mode = 0; /* 0 = menu/accueil; 1 = jeu */

     // Variables enemy
    EnemyList enemies = NULL;

    /* Variables background */
    FILE *bg = fopen("bg1.ppm", "r");
    ObstacleList obstacles = readPPM(bg, &enemies);
    fclose(bg);

    SDL_Surface* imageMur = IMG_Load("mur.png");
    if (imageMur == NULL) {
        printf("Erreur chargement image\n");
    }

    GLuint textureMur;
    glGenTextures(1, &textureMur);

    glBindTexture(GL_TEXTURE_2D, textureMur);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        imageMur->w,
        imageMur->h,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        imageMur->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Libération des données CPUq
    SDL_FreeSurface(imageMur);

    /* Variables vaisseau */
    int lives = 3;
    float shipPosY = 4.5;

    /* Créé la bounding box du vaisseau */

    BoundingBox shipBox;

    shipBox.pMinX = 2;
    shipBox.pMaxX = 2 + TAILLE_VAISSEAU;

    shipBox.pMinY = shipPosY;
    shipBox.pMaxY = shipPosY + TAILLE_VAISSEAU;

    // Variables missile
    int numbMissiles = MAX_MISSILES;
    MissileList missiles = NULL;

    SDL_Surface* imageMissile = IMG_Load("fire.jpg");
    if (imageMissile == NULL) {
        printf("Erreur chargement image\n");
    }

    GLuint textureMissile;
    glGenTextures(1, &textureMissile);

    glBindTexture(GL_TEXTURE_2D, textureMissile);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        imageMissile->w,
        imageMissile->h,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        imageMissile->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Libération des données CPUq
    SDL_FreeSurface(imageMissile);
    
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT); // Nettoie la fenêtre
        glClearColor(1, 1, 1, 1); // fond couleur blanc

        /* Dessine le background */
        moveObstacles(obstacles);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureMur);

        drawObstacles(obstacles); 

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Vaisseau */

        drawShip(2, shipPosY);

        drawLives(lives);

        if (lives == 0) {
            printf("Perdu !\n");
        }

        /* Enemies */
       
        drawEnemies(enemies);
        moveEnemy(enemies);
        int collEnemy = collEnemies(&enemies, shipBox);   

        if (collEnemy == 1) {
            lives -= 1;
        }        

        /* Obstacles */

        int collObst = collObstacles(&obstacles, shipBox);   

        if (collObst == 1) {
            lives -= 1;
            printf("nb lives : %d\n", lives);
        } else if (collObst == 2) {
            printf("Fin de partie !!!\n");
        }

        /* Missiles */

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureMissile);

        drawMissiles(missiles);

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        moveMissiles(missiles);

        collEnemiesMissiles(&missiles, &enemies);
        collObstaclesMissiles(&missiles, &obstacles); 

        drawAmmu(numbMissiles);

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
            
            switch(e.type) {

                case SDL_KEYDOWN:
                    // appuie sur la touche "q" pour fermer la fenêtre 
                    if (e.key.keysym.sym == 113) {
                        loop = 0;
                    }

                    // touche espace
                    if (e.key.keysym.sym == 32) {
                        if (numbMissiles > 0) {
                            createMissile(&missiles, shipPosY);
                            numbMissiles --;
                        }                        
                    }

                    break;

                default:
                    break;
            }

        }

        /* Gère les mouvements du vaisseau */
        
        const Uint8 *state = SDL_GetKeyState(NULL);
        
        if (state[SDLK_UP]) {
            shipPosY += 0.05;
            shipBox.pMinY = shipPosY;
            shipBox.pMaxY = shipPosY + TAILLE_VAISSEAU;
        }

        if (state[SDLK_DOWN]) {
            shipPosY -= 0.05;
            shipBox.pMinY = shipPosY;
            shipBox.pMaxY = shipPosY + TAILLE_VAISSEAU;
        }        

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    // Libération des données GPU
    glDeleteTextures(1, &textureMur);

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}
