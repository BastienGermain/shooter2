#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "header.h"

ObstacleList readPPM(FILE *file, EnemyList *enemyList) {
    int ch;
    int lines = 0;
    int width, height;

    /* Process header (se débarrasse des lignes inutiles dans l'en-tête du fichier PPM) */
    while (lines < 4) { 

        if (lines == 2) {
            fscanf(file, "%d %d\n", &width, &height);
        } else {
            ch = fgetc(file); 
        }

        if (ch == '\n') { 
            lines++; 
        } 
    }

    /* Parcours du fichier PPM */

    int red, green, blue;
    float x = 0;
    int y = height - 1;

    int k = 0;

    Obstacle *obstacleList = malloc(sizeof(Obstacle));
    obstacleList = NULL;

    while (!feof(file)) {
        Obstacle *new = malloc(sizeof(Obstacle));

        /* Récupère les données rvb pour chaque pixel */
        fscanf(file, "%d\n%d\n%d\n", &red, &green, &blue);

        if (red == 200) {
            createEnemy(enemyList, x, y);
        }

        /* On recupère la position de chaque case noire (les obstacles) pour créer les bounding box */
        if (red != 200 && blue != 255){

            new->r = red;
            new->g = green;
            new->b = blue;

            new->posX = x;
            new->posY = y;

            new->box.pMinX = x;
            new->box.pMinY = y;
            new->box.pMaxX = x + 1;
            new->box.pMaxY = y + 1;

            new->levelEnd = 0;

            // Cases de fin de niveau
            if (green == 125 || green == 255) {
                new->levelEnd = 1;
            }

            new->next = obstacleList;
            obstacleList = new; 

            k++;
        }

        /* Passe d'une colonne à une autre jusqu'à la fin de la ligne puis change de ligne une fois au bout */
        if (x == width - 1) {
            y -= 1;
            x = 0;
        } else {
            x += 1;
        }
    }

    return obstacleList;
}


/* Dessine le background et renvoie un tableau contenant la bounding box de chaque case obstacle */
void drawObstacles(ObstacleList list) {

    if (list != NULL) {

        if (list->g == 125 || list->g == 255) {
            glBegin(GL_QUADS);
                glColor3ub(list->r, list->g, list->b);                   
                glVertex2f(list->posX, list->posY);              
                glVertex2f(list->posX + 1, list->posY);                
                glVertex2f(list->posX + 1, list->posY + 1);                            
                glVertex2f(list->posX, list->posY + 1);            
            glEnd();
        } else {
            glBegin(GL_QUADS);
                glColor3f(1, 1, 1);
                glTexCoord2f(list->posX, list->posY);
                glVertex2f(list->posX, list->posY);
                glTexCoord2f(list->posX + 1, list->posY);
                glVertex2f(list->posX + 1, list->posY);
                glTexCoord2f(list->posX + 1, list->posY + 1); 
                glVertex2f(list->posX + 1, list->posY + 1); 
                glTexCoord2f(list->posX, list->posY + 1);            
                glVertex2f(list->posX, list->posY + 1);            
            glEnd();
        }        

        drawObstacles(list->next);
    }
}

void moveObstacles(Obstacle *obstacle) {

    if (obstacle != NULL) {
        
        obstacle->posX -= VITESSE_DEFILEMENT;

        BoundingBox box;

        box.pMinX = obstacle->posX;
        box.pMaxX = obstacle->posX + 1;

        box.pMinY = obstacle->posY;
        box.pMaxY = obstacle->posY + 1;

        obstacle->box = box;

        moveObstacles(obstacle->next);
    }
}

int collObstacles(ObstacleList *obstacle, BoundingBox box) {
    ObstacleList tmp = *obstacle;

    int ret = 0;

    if (tmp != NULL) {
    
        int coll = checkCollision(box, tmp->box);

        if (coll == 1) {

            if (tmp->levelEnd == 1) {
                return 2;
            } else {
                supprimerObstacleFromList(tmp, obstacle);
                return 1;
            }           
            
        }

        ret = collObstacles(&tmp->next, box);
    }

    return ret;

}

void supprimerObstacleFromList(Obstacle *toSuppr, ObstacleList *list) {

    if (toSuppr == *list){    

        *list = toSuppr->next;

    } else if (toSuppr == (*list)->next) {

        (*list)->next = toSuppr->next;

    } else {

        ObstacleList conserve = *list;

        while (toSuppr != (*list)->next) {
            *list = (*list)->next; 
        }

        (*list)->next = toSuppr->next;

        *list = conserve;
    }

    free(toSuppr);
}