#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "header.h"

void drawShip(float x, float y) {
	
    glBegin(GL_QUADS);
        glColor3ub(150, 100, 250);
        glVertex2f(x, y);
        glVertex2f(x + TAILLE_VAISSEAU, y);
        glVertex2f(x + TAILLE_VAISSEAU, y + TAILLE_VAISSEAU);
        glVertex2f(x, y + TAILLE_VAISSEAU);        
    glEnd();
}

void drawLives(int lives) {

    if (lives == 3) {

        glBegin(GL_QUADS);
            glColor3ub(102, 255, 102); //vert
            glVertex2f(6, 9);
            glVertex2f(9, 9);
            glVertex2f(9, 9.5);
            glVertex2f(6, 9.5);        
        glEnd();

    } else if (lives == 2) {

        glBegin(GL_QUADS);
            glColor3ub(255, 153, 51); //orange
            glVertex2f(7, 9);
            glVertex2f(9, 9);
            glVertex2f(9, 9.5);
            glVertex2f(7, 9.5);        
        glEnd();

    } else if (lives == 1) {

        glBegin(GL_QUADS);
            glColor3ub(200, 0, 0); //rouge
            glVertex2f(8, 9);
            glVertex2f(9, 9);
            glVertex2f(9, 9.5);
            glVertex2f(8, 9.5);        
        glEnd();
    }
}

void drawAmmu(int numbMissiles) {
	int i;

	for (i = 1; i < numbMissiles + 1; i++) {

		float x = i * 0.4;

		glBegin(GL_QUADS);
            glColor3ub(0, 0, 200); //bleu
            glVertex2f(x, 9);
            glVertex2f(x + 0.25, 9);
            glVertex2f(x + 0.25, 9.25);
            glVertex2f(x, 9.25);        
        glEnd();
	}
}