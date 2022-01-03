

/*******************************************************/
/*					didac.c							   */
/*******************************************************/
/*													   */
/*	Didactitiel n°1 OpenGL sous Glut				   */
/*  ESGI : 2I année 						           */
/*													   */
/*******************************************************/
/*													   */
/*  Objectif : afficher à l'écran un carré en couleur  */
/*													   */
/*******************************************************/



#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Point.h"
#include "Polygon.h"

using std::vector;

int x0, y0;  // clic souris
             // coin inférieur gauche du carré

int xContext, yContext;

double c;    // coté du carré

struct Color
{
    float r,g,b;
};

//vector<Point> points;
Poly *poly = new Poly();
vector<Color> colors;

/* prototypes de fonctions */
void affichage(void);                             // modélisation
void contextMenu(void);                             // modélisation
void clavier(unsigned char touche,int x,int y);   // fonction clavier
void mouse(int bouton,int etat,int x,int y);      // fonction souris


/* Programme principal */
int main(int argc,       // argc: nombre d'arguments, argc vaut au moins 1
		  char **argv){  // argv: tableau de chaines de caractères, argv[0] contient le nom du programme lancé (plus un éventuel chemin)


	/* Initialisation de glut et creation de la fenetre */
    glutInit(&argc, argv);                       // Initialisation
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // mode d'affichage RGB, et test prafondeur
    glutInitWindowSize(500, 500);                // dimension fenêtre
	glutInitWindowPosition (100, 100);           // position coin haut gauche
	glutCreateWindow("Un carré dans tous ses états");  // nom

	/* Repère 2D délimitant les abscisses et les ordonnées*/
	gluOrtho2D(-250.0,250.0,-250.0,250.0);

	/* Initialisation d'OpenGL */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);       // couleur: blanc
	glPointSize(2.0);               // taille d'un point: 2px

	/* Enregistrement des fonctions de rappel
     => initialisation des fonctions callback appelées par glut */
    glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutMouseFunc(mouse);

    /* rq: le callback de fonction (fonction de rappel) est une fonction qui est passée en argument à une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en même temps)
	alors qu'il ne les connaît pas par avance.*/



	/* Entrée dans la boucle principale de glut, traitement des évènements */
    glutMainLoop();         // lancement de la boucle de réception des évènements
    return 0;
}


void contextMenu(){


// dessin du carré
// (x0,y0) point inférieur gauche du carré

glBegin(GL_POLYGON);
glColor3f(1.0,0.0,0.0);
glVertex2f(xContext,yContext);
glVertex2f(xContext,yContext + 10);
glVertex2f(xContext + 10,yContext + 10);
glVertex2f(xContext + 10,yContext);
glEnd();

// On force l'affichage du résultat
glFlush();
}


void affichage(){


glClear(GL_COLOR_BUFFER_BIT);


// dessin du carré
// (x0,y0) point inférieur gauche du carré

vector<Point> points = poly->Getpoints();
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glBegin(GL_POLYGON);
for(int i = 0; i < points.size(); i++)
{
    glColor3f(colors[i].r,colors[i].g,colors[i].b);
    float x = points[i].Getx();
    float y = points[i].Gety();
    glVertex2f(x,y);
}
glEnd();

// On force l'affichage du résultat
glFlush();
}

void mouse(int button,int state,int x,int y)
{

	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x0 = x - 250; //on sauvegarde la position de la souris
		y0 = -y + 250;
		Point *p = new Point(x0, y0);

		poly->Addpoint(*p);
        colors.push_back(Color{1.0,0.0,0.0});
		printf("%d\n",poly->Getpoints().size());
		for(int i = 0; i < poly->Getpoints().size(); i++)
        {
            printf("%0.2f - %0.2f",poly->Getpoints()[i].Getx(), poly->Getpoints()[i].Gety());
        }
        printf("\n");
		affichage();
		return;
	}

	// Si on appuie sur le bouton de droite
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		xContext = x - 250; //on sauvegarde la position de la souris
		yContext = -y + 250;
		contextMenu();
		return;
	}

}



/* Evènement du clavier */
void clavier(unsigned char touche,int x,int y){
	switch (touche){

		case 'p':/* affichage du carré plein*/
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glutPostRedisplay();     // permet l'affichage
			break;

		case 'f':/* affichage en mode fil de fer*/
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glutPostRedisplay();
			break;

		case 's':/* affichage en mode sommets seuls*/
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			glutPostRedisplay();
			break;

		case 'q':/* Quitter le programme */
			exit(0);
	}
}


