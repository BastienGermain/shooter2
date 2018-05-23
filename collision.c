#include <stdlib.h>
#include <stdio.h>

#include "header.h"

int checkCollision(BoundingBox box1, BoundingBox box2) {

	/* Collisions par l'avant */
	if (box1.pMaxX > box2.pMinX && box1.pMaxX < box2.pMaxX) {
		
		/* Collisions par dessus */
		if (box1.pMinY < box2.pMaxY && box1.pMinY > box2.pMinY) {
			return 1;
		} 

		/* Collisions par dessous */
		else if (box1.pMaxY > box2.pMinY && box1.pMaxY < box2.pMaxY) {
			return 1;
		}
	}

	/* Collisions par l'arrière */
	else if (box1.pMinX < box2.pMaxX && box1.pMinX > box2.pMinX) {

		/* Collisions par dessus */
		if (box1.pMinY < box2.pMaxY && box1.pMinY > box2.pMinY) {
			return 1;
		} 

		/* Collisions par dessous */
		else if (box1.pMaxY > box2.pMinY && box1.pMaxY < box2.pMaxY) {			
			return 1;
		}
	}

	return 0;
}

void collEnemiesMissiles(MissileList *missile, EnemyList *enemy) {
    MissileList tmp = *missile;

    if (tmp != NULL) {
    
        int coll = collEnemies(enemy, tmp->box);

        if (coll == 1) {
            supprimerMissileFromList(tmp, missile);
        }

        collEnemiesMissiles(&tmp->next, enemy);
    }

}

void collObstaclesMissiles(MissileList *missile, ObstacleList *obstacle) {
    MissileList tmp = *missile;

    if (tmp != NULL) {
    
        int coll = collObstacles(obstacle, tmp->box);

        if (coll == 1) {
            supprimerMissileFromList(tmp, missile);
        }

        collObstaclesMissiles(&tmp->next, obstacle);
    }

}