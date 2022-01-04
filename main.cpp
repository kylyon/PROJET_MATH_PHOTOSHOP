

/*******************************************************/
/*					didac.c							   */
/*******************************************************/
/*													   */
/*	Didactitiel n�1 OpenGL sous Glut				   */
/*  ESGI : 2I ann�e 						           */
/*													   */
/*******************************************************/
/*													   */
/*  Objectif : afficher � l'�cran un carr� en couleur  */
/*													   */
/*******************************************************/



#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Point.h"
#include "Polygon.h"
#include "Color.h"

using std::vector;

static int window;
int menu, color_menu;

int x0, y0;  // clic souris
             // coin inf�rieur gauche du carr�

int xContext, yContext;

double c;    // cot� du carr�

//vector<Point> points;
//vector<Color> colors;
Color *color;
Poly *poly;
vector<Poly> polygons;
bool createMode = false;

/* prototypes de fonctions */
void affichage(void);                             // mod�lisation
void clavier(unsigned char touche,int x,int y);   // fonction clavier
void mouse(int bouton,int etat,int x,int y);      // fonction souris
void createMenu(void);
void processMenuEvents(int option);
void processColorEvents(int option);
void newPolygon();

/* Programme principal */
int main(int argc,       // argc: nombre d'arguments, argc vaut au moins 1
		  char **argv){  // argv: tableau de chaines de caract�res, argv[0] contient le nom du programme lanc� (plus un �ventuel chemin)

    color = new Color(1.0f,0.0f,0.0f);
    poly = new Poly(*color);

	/* Initialisation de glut et creation de la fenetre */
    glutInit(&argc, argv);                       // Initialisation
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // mode d'affichage RGB, et test prafondeur
    glutInitWindowSize(500, 500);                // dimension fen�tre
	glutInitWindowPosition (100, 100);           // position coin haut gauche
	window = glutCreateWindow("Maths Projet 4A3DJV");  // nom
    createMenu();
	/* Rep�re 2D d�limitant les abscisses et les ordonn�es*/
	gluOrtho2D(-250.0,250.0,-250.0,250.0);

	/* Initialisation d'OpenGL */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);       // couleur: blanc
	glPointSize(2.0);               // taille d'un point: 2px

	/* Enregistrement des fonctions de rappel
     => initialisation des fonctions callback appel�es par glut */
    glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutMouseFunc(mouse);

    /* rq: le callback de fonction (fonction de rappel) est une fonction qui est pass�e en argument � une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en m�me temps)
	alors qu'il ne les conna�t pas par avance.*/


    printf("%f, %f, %f", color->Getred(), color->Getgreen(), color->Getblue());
	/* Entr�e dans la boucle principale de glut, traitement des �v�nements */
    glutMainLoop();         // lancement de la boucle de r�ception des �v�nements
    return 0;
}

void createMenu(void){
    color_menu = glutCreateMenu(processColorEvents);
    glutAddMenuEntry("Rouge",1);
    glutAddMenuEntry("Vert",2);
    glutAddMenuEntry("Bleu",3);
    glutAddMenuEntry("Blanc",4);
	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Couleurs", color_menu);
	glutAddMenuEntry("Polygone a decouper",2);
	glutAddMenuEntry("Trace fenetre",3);
	glutAddMenuEntry("Fenetrage",4);
	glutAddMenuEntry("Remplissage",5);
	glutAddMenuEntry("Quitter", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processColorEvents(int option) {
    switch (option) {
	    case 1 :
	        color = new Color(1.0f,0.0f,0.0f);
	        newPolygon();
	        break;
		case 2 :
		    color = new Color(0.0f,1.0f,0.0f);
			newPolygon();
			break;
		case 3 :
			color = new Color(0.0f,0.0f,1.0f);
			newPolygon();
			break;
		case 4 :
		    color = new Color(1.0f,1.0f,1.0f);
			newPolygon();
			break;
	}
}

void processMenuEvents(int option) {
	switch (option) {
	    case 0 :
	        printf("0");
	        glutDestroyWindow(window);
            exit(0);
	        break;
		case 1 :
			printf("1");
			break;
		case 2 :
			printf("2");
			newPolygon();
			break;
		case 3 :
			printf("3");
			break;
		case 4 :
			printf("4");
            break;
        case 5 :
            printf("5");
            break;
	}
}

void affichage(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // dessin du carr�
    // (x0,y0) point inf�rieur gauche du carr�

    for(int i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }
    // On force l'affichage du r�sultat
    glFlush();
}

void newPolygon()
{
    poly = new Poly(*color);
    polygons.push_back(*poly);
    createMode = true;
}

void mouse(int button,int state,int x,int y)
{

	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x0 = x - 250; //on sauvegarde la position de la souris
		y0 = -y + 250;
		if(createMode){
            Point *p = new Point(x0, y0);
            polygons[polygons.size() - 1].Addpoint(*p);
		}
        printf("Avant\n");
		affichage();
		return;
	}

	// Si on appuie sur le bouton de droite
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		xContext = x - 250; //on sauvegarde la position de la souris
		yContext = -y + 250;
		return;
	}

}

/* Ev�nement du clavier */
void clavier(unsigned char touche,int x,int y){
	switch (touche){

		case 'p':/* affichage du carr� plein*/
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


