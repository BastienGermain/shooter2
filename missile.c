#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "header.h"

int createMissile(MissileList *list, float y) {

	Missile *new = malloc(sizeof(Missile));

	if (new == NULL){ /* Si malloc échoue */
		return 0;
	} else {
		// recentre le missile au milieu du vaisseau
		y = y + (TAILLE_VAISSEAU / 3);

		new->posX = 2.5;
		new->posY = y;

		BoundingBox box;

		box.pMinX = 2.5;
		box.pMaxX = 2.5 + LONGUEUR_MISSILE;

		box.pMinY = y;
		box.pMaxY = y + HAUTEUR_MISSILE;

		new->box = box;

		new->next = *list;
		*list = new; 

		return 1;
	}

}

void drawMissiles(MissileList list) {

    if (list != NULL) {

        glBegin(GL_QUADS);
            glColor3f(1, 1, 1);
            glVertex2f(list->posX, list->posY);
            glVertex2f(list->posX + LONGUEUR_MISSILE, list->posY);
            glVertex2f(list->posX + LONGUEUR_MISSILE, list->posY + HAUTEUR_MISSILE); 
            glVertex2f(list->posX, list->posY + HAUTEUR_MISSILE);            
        glEnd();

        drawMissiles(list->next);
    }
}

void moveMissiles(Missile *missile) {

    if (missile != NULL) {
        
        missile->posX += VITESSE_DEFILEMENT;

        BoundingBox box;

        box.pMinX = missile->posX;
        box.pMaxX = missile->posX + LONGUEUR_MISSILE;

        box.pMinY = missile->posY;
        box.pMaxY = missile->posY + HAUTEUR_MISSILE;

        missile->box = box;

        moveMissiles(missile->next);
    }
}

void supprimerMissileFromList(Missile *toSuppr, MissileList *list) {

    if (toSuppr == *list){    

        *list = toSuppr->next;

    } else if (toSuppr == (*list)->next) {

        (*list)->next = toSuppr->next;

    } else {

        MissileList conserve = *list;

        while (toSuppr != (*list)->next) {
            *list = (*list)->next; 
        }

        (*list)->next = toSuppr->next;

        *list = conserve;
    }

    free(toSuppr);
}