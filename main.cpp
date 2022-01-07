

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
#include <cmath>
#include "Point.h"
#include "Polygon.h"
#include "Color.h"

using std::vector;

static int window;
int menu, color_menu;

int xClic, yClic;  // clic souris
             // coin inf�rieur gauche du carr�

int xContext, yContext;

double c;    // cot� du carr�

//vector<Point> points;
//vector<Color> colors;
Color *color;
Poly *poly;
vector<Poly> polygons;
vector<Poly> windows;
int mode = 0;

/* prototypes de fonctions */
void affichage(void);                             // mod�lisation
void clavier(unsigned char touche,int x,int y);   // fonction clavier
void mouse(int bouton,int etat,int x,int y);      // fonction souris
void createMenu(void);
void processMenuEvents(int option);
void processColorEvents(int option);
void newPolygon();
void newWindow();
bool coupe(Point P1, Point P2, Point P3, Point P4);
bool visible(Point S, Point F, Point F1, bool antiHoraire);
Point intersection(Point P1, Point P2, Point P3, Point P4);

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


    //printf("%f, %f, %f", color->Getred(), color->Getgreen(), color->Getblue());

    Point *P1 = new Point(0.0, 1.0);
    Point *P2 = new Point(1.0, -1.0);
    Point *P3 = new Point(0.0, -1.0);
    Point *P4 = new Point(1.0, 1.0);

    bool c = coupe(*P1, *P2, *P3, *P4);

    printf("%d", c);
    if(c)
    {
        Point temp = intersection(*P1, *P2, *P3, *P4);
        printf("\n%f - %f", temp.Getx(), temp.Gety());
    }

    bool v1 = visible(*P4, *P1, *P2, true);
    bool v2 = visible(*P3, *P1, *P2, true);

    printf("\n%d - %d", v1, v2);

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
			newWindow();

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
    // (xClic,yClic) point inf�rieur gauche du carr�

    for(int i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    for(int i = 0; i < windows.size(); i++)
    {
        windows[i].display();
    }
    // On force l'affichage du r�sultat
    glFlush();
}

void newPolygon()
{
    poly = new Poly(*color);
    polygons.push_back(*poly);
    mode = 1;
}

void newWindow()
{
    poly = new Poly(*color);
    windows.push_back(*poly);
    mode = 2;
}

bool cyrusBeck(Point p1, Point p2, Poly window)
{
    vector<Point> normal;
    for(int i = 0; i < window.Getpoints().size(); i++)
    {
        //calcule des normales
    }
}

bool coupe(Point P1, Point P2, Point P3, Point P4)
{
    float det = (P3.Gety() - P4.Gety()) * (P2.Getx() - P1.Getx()) - (P3.Getx() - P4.Getx()) * (P2.Gety() - P1.Gety());
    return det != 0;
}

Point intersection(Point P1, Point P2, Point P3, Point P4)
{
    float a, b;
    if(P1.Getx() == P2.Getx())//Test si la droite du polygone est verticale
    {
        if(P3.Gety() == P4.Gety())//Test si la droite de la fenetre est horizontale
        {
            Point *p =  new Point(P1.Getx(), P3.Gety());
            return *p;
        }
        a = (P4.Gety() - P3.Gety())/(P4.Getx() - P3.Getx());
        b = (P4.Gety() - a * P4.Getx());

        Point *p =  new Point(P1.Getx(), a * P1.Getx() + b);
        return *p;
    }

    if(P4.Getx() == P3.Getx())//Test si la droite de la fenetre est verticale
    {
        if(P1.Gety() == P2.Gety())//Test si la droite du polygone est horizontale
        {
            Point *p =  new Point(P3.Getx(), P1.Gety());
            return *p;
        }
        a = (P2.Gety() - P1.Gety())/(P2.Getx() - P1.Getx());
        b = (P1.Gety() - a * P1.Getx());

        Point *p =  new Point(P3.Getx(), a * P3.Getx() + b);
        return *p;
    }

    float a1, b1, a2, b2, x, y;
    a1 = (P2.Gety() - P1.Gety())/(P2.Getx() - P1.Getx());
    b1 = (P1.Gety() - a1 * P1.Getx());

    a2 = (P4.Gety() - P3.Gety())/(P4.Getx() - P3.Getx());
    b2 = (P4.Gety() - a2 * P4.Getx());

    x = (b2 - b1)/(a1 - a2);
    y = a2 * x + b2;

    Point *p =  new Point(x, y);
    return *p;
}

bool visible(Point S, Point F, Point F1, bool antiHoraire)
{
    float dx = F1.Getx() - F.Getx();
    float dy = F1.Gety() - F.Gety();
    Point *normal = new Point(dy, -dx);
    if(antiHoraire)
    {
        normal = new Point(-dy, dx);
    }

    printf("\nNormale : %f, %f", normal->Getx(), normal->Gety());

    //float n_norme = sqrt(pow(normal->Getx(), 2) + pow(normal->Gety(), 2));

    //printf(", Norme Normale : %f", n_norme);

    float vx = S.Getx() - F.Getx();
    float vy = S.Gety() - F.Gety();

    Point *v = new Point(vx, vy);

    //printf(", Vecteur SF : %f, %f", v->Getx(), v->Gety());

    //float v_norme = sqrt(pow(v->Getx(), 2) + pow(v->Gety(), 2));

    //printf(", Norme SF : %f", v_norme);

    //float uv = normal->Getx()*v->Getx() + normal->Gety()*v->Gety();

    //printf(", noraml * SF : %f", uv);

    //float theta = uv/(n_norme*v_norme);

    //printf(", angle : %f", theta);

    printf(", Produit Scalaire : %f", (normal->Getx() * v->Getx()) + (normal->Gety() * v->Gety()));

    return (normal->Getx() * v->Getx()) + (normal->Gety() * v->Gety()) >= 0;
}

Poly sutherlandHodgman(Poly p, Poly window)
{
    vector<Point> PL = p.Getpoints();
    vector<Point> PW = window.Getpoints();
    PW.push_back(window.Getpoints()[0]);

    vector<Point> PS;
    int n;
    Point S,F,I;

    for(int i = 0; i < PW.size() - 1; i++)
    {
        n = 0;
        PS.clear();

        for(int j = 0; j < PL.size(); j++)
        {
            if(j==0)
            {
                F = PL[j];
            }
            else
            {
                if(coupe(S,PL[j], PW[i], PW[(i+1)%PW.size()] ))
                {
                    I = intersection(S,PL[j], PW[i], PW[(i+1)%PW.size()] );
                    PS.push_back(I);
                }
            }
            S = PL[j];
            if(visible(S,PW[i], PW[(i+1)%PW.size()], true))
            {
                PS.push_back(S);
                n++;
            }
        }
        if(n>0)
        {
            if(coupe(S,F, PW[i], PW[(i+1)%PW.size()] ))
            {
                I = intersection(S,F, PW[i], PW[(i+1)%PW.size()] );
                PS.push_back(I);
                n++;
            }
            PL = PS;
        }
    }

    Poly temp = Poly(p.Getcolor());

    for(int i = 0; i < PL.size(); i++)
    {
        temp.Addpoint(PL[i]);
    }

    return temp;

}

void mouse(int button,int state,int x,int y)
{

	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xClic = x - 250; //on sauvegarde la position de la souris
		yClic = -y + 250;
		if(mode != 0){
            Point *p = new Point(xClic, yClic);
            switch(mode)
            {
            case 1:
                polygons[polygons.size() - 1].Addpoint(*p);
                break;
            case 2:
                printf("yz");
                windows[windows.size() - 1].Addpoint(*p);
                printf("\nSize : %d",  polygons.size());
                if(windows[windows.size() - 1].Getpoints().size() >= 3)
                {
                    for(int i = 0; i < polygons.size(); i++)
                    {
                        //Poly temp = sutherlandHodgman(polygons[i], windows[windows.size() - 1]);
                        //polygons.push_back(temp);
                    }
                }
                break;
            }
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


