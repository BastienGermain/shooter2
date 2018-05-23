#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "header.h"

// Dessine une liste d'enemy
void drawEnemies(EnemyList enemy) {
	if (enemy != NULL) {

		glBegin(GL_QUADS);
	        glColor3ub(255, 0, 0); //marron
	        glVertex2f(enemy->posX, enemy->posY);
	        glVertex2f(enemy->posX + TAILLE_ENEMY, enemy->posY);
	        glVertex2f(enemy->posX + TAILLE_ENEMY, enemy->posY + TAILLE_ENEMY);
	        glVertex2f(enemy->posX, enemy->posY + TAILLE_ENEMY);        
	    glEnd();

		drawEnemies(enemy->next);
	}
}

// Ajoute un enemy en début de liste
int createEnemy(EnemyList *list, float x, float y) {

	Enemy *new = malloc(sizeof(Enemy));

	if (new == NULL){ /* Si malloc échoue */
		return 0;
	} else {
		new->posX = x;
		new->posY = y;

		BoundingBox box;

		box.pMinX = x;
		box.pMaxX = x + TAILLE_ENEMY;

		box.pMinY = y;
		box.pMaxY = y + TAILLE_ENEMY;

		new->box = box;

		new->next = *list;
		*list = new; 

		return 1;
	}

}

// Déplace les enemies
void moveEnemy(Enemy *enemy) {

	if (enemy != NULL) {

		enemy->posX -= VITESSE_DEFILEMENT*1.5;

		BoundingBox box;

		box.pMinX = enemy->posX;
		box.pMaxX = enemy->posX + TAILLE_ENEMY;

		box.pMinY = enemy->posY;
		box.pMaxY = enemy->posY + TAILLE_ENEMY;

		enemy->box = box;

		moveEnemy(enemy->next);
	}
	
}

// Supprime un enemy de la liste
void supprimerEnemyFromList(Enemy *toSuppr, EnemyList *list) {

	/* Si l'enemey à supprimer est en début de liste */
	if (toSuppr == *list){			

		/* On fait pointer la liste sur l'enemy qui suit celui à supprimé */
		*list = toSuppr->next;

	/* Si l'enemy' à supprimer est en seconde position */
	} else if (toSuppr == (*list)->next) {

		/* On fait pointer le premier enemy de la liste sur celui qui suit l'enemy à supprime */
		(*list)->next = toSuppr->next;

	} else {
		/* On conserve la valeur d'entrée de la liste */
		EnemyList conserve = *list;

		/* On boucle jusqu'à arriver à l'enemy qui précède celui à supprimer */
		while (toSuppr != (*list)->next) {
			*list = (*list)->next; /* !!! ici l'entrée de la liste est modifiée !!! */
		}

		/* On fait pointer l'enemy qui précède sur celui qui suit l'enemy à supprimé */
		(*list)->next = toSuppr->next;

		/* On remet la bonne valeur d'entrée pour la liste */
		*list = conserve;
	}

	free(toSuppr);
}

// Gère les collisions avec la liste d'enemy
int collEnemies(EnemyList *enemy, BoundingBox box) {
	EnemyList tmp = *enemy;

	int ret = 0;

	if (tmp != NULL) {
	
		int coll1 = checkCollision(tmp->box, box);
		int coll2 = checkCollision(box, tmp->box);

		if (coll1 == 1 || coll2 == 1) {
			supprimerEnemyFromList(tmp, enemy);
			return 1;
		}

		ret = collEnemies(&tmp->next, box);
	}

	return ret;

}