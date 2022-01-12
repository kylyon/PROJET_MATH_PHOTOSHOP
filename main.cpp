

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
#include <limits>
#include "Point.h"
#include "Polygon.h"
#include "Color.h"

using std::vector;

static int window;
int menu, color_menu, color_polygon_menu, color_window_menu, color_cut_polygon_menu;

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
vector<Poly> polywindow;
int mode = 0;

/* prototypes de fonctions */
void affichage(void);                             // mod�lisation
void clavier(unsigned char touche,int x,int y);   // fonction clavier
void mouse(int bouton,int etat,int x,int y);      // fonction souris
void createMenu(void);
void createColorMenu();
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
    color_polygon_menu = glutCreateMenu(processColorEvents);
    createColorMenu();
    color_window_menu = glutCreateMenu(processColorEvents);
    createColorMenu();
    color_cut_polygon_menu = glutCreateMenu(processColorEvents);
    createColorMenu();
    color_menu = glutCreateMenu(processColorEvents);
    glutAddSubMenu("Polygone a decouper", color_polygon_menu);
	glutAddSubMenu("Fenetrage", color_window_menu);
	glutAddSubMenu("Polygone decouper", color_cut_polygon_menu);
	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Couleurs", color_menu);
	glutAddMenuEntry("Polygone a decouper",2);
	glutAddMenuEntry("Trace fenetre",3);
	glutAddMenuEntry("Fenetrage",4);
	glutAddMenuEntry("Remplissage",5);
	glutAddMenuEntry("Quitter", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void createColorMenu() {
    glutAddMenuEntry("Rouge",1);
    glutAddMenuEntry("Vert",2);
    glutAddMenuEntry("Bleu",3);
    glutAddMenuEntry("Blanc",4);
}

void processColorEvents(int option) {
    switch (option) {
	    case 1 :
	        color = new Color(1.0f,0.0f,0.0f);
	        //newPolygon();
	        break;
		case 2 :
		    color = new Color(0.0f,1.0f,0.0f);
			//newPolygon();
			break;
		case 3 :
			color = new Color(0.0f,0.0f,1.0f);
			//newPolygon();
			break;
		case 4 :
		    color = new Color(1.0f,1.0f,1.0f);
			//newPolygon();
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

    for(int i = 0; i < polywindow.size(); i++)
    {
        polywindow[i].display();
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

bool inPolygon(Point p1, Point p2, Poly poly)
{
    vector<Point> normals;
    Point *normal;
    for(int k = 0; k < poly.Getpoints().size(); k++)
    {
        float dx = poly.Getpoints()[(k + 1)%poly.Getpoints().size()].Getx() - poly.Getpoints()[k].Getx();
        float dy = poly.Getpoints()[(k + 1)%poly.Getpoints().size()].Gety() - poly.Getpoints()[k].Gety();
        normal = new Point(dy, -dx);
        if(!poly.isHoraire())
        {
            normal = new Point(-dy, dx);
        }
        normals.push_back(*normal);
    }

    vector<Point> PolyCB = poly.Getpoints();

    float t, tsup, tinf, DX, DY, WN, DN;
    float x1, x2, y1, y2;
    Point newP1, newP2;
    Point C;
    int i, Nbseg;

    tsup = std::numeric_limits<float>::max();
    tinf = (float)std::numeric_limits<int>::min();
    //printf("\n tsup %f - tinf %f", tsup, tinf);

    DX = p2.Getx() - p1.Getx();
    DY = p2.Gety() - p1.Gety();
    Nbseg = poly.Getpoints().size();
    for(i = 0; i< Nbseg; i++){
        C = PolyCB[i];
        DN = DX * normals[i].Getx() + DY * normals[i].Gety();
        WN = (p1.Getx() - C.Getx()) * normals[i].Getx() + (p1.Gety() - C.Gety()) * normals[i].Gety();
        //printf("\n DN %f - WN %f", DN, WN);
        if(DN == 0) { /* Division impossible, le segment est réduit à un point */
            return (WN >= 0);
        }else{
            t = -(WN)/(DN);
            printf("\n t : %f", t);
            if(DN > 0) { /* calcul du max des tinf */
                if(t > tinf) {
                    tinf = t;
                }
            }else{ /* calcul du min des tsup */
                if(t < tsup){
                    tsup = t;
                }
            }
        }
    }
    //printf("\n tinf : %f - tsup : %f", tinf, tsup);
    if(tinf < tsup){ /* Intersection possible */
        if((tinf < 0) && (tsup > 1)){ /* Segment intérieur */
            return true;
        }else{
            if((tinf > 1) | (tsup < 0)){ /* Segment extérieur */
                return false;
            }else{
                if(tinf < 0){ /* A : origine du segment intérieure */
                    return true;
                }
                return false;
            }
        }
    }else{ /* Segment extérieur */
        return false;
    }
}

bool cyrusBeck(Point *p1, Point *p2, Poly window, Poly *temp)
{
    vector<Point> normals;
    Point *normal;
    for(int k = 0; k < window.Getpoints().size(); k++)
    {
        float dx = window.Getpoints()[(k + 1)%window.Getpoints().size()].Getx() - window.Getpoints()[k].Getx();
        float dy = window.Getpoints()[(k + 1)%window.Getpoints().size()].Gety() - window.Getpoints()[k].Gety();
        normal = new Point(dy, -dx);
        if(!window.isHoraire())
        {
            normal = new Point(-dy, dx);
        }
        normals.push_back(*normal);
    }

    vector<Point> PolyCB = window.Getpoints();

    float t, tsup, tinf, DX, DY, WN, DN;
    float x1, x2, y1, y2;
    Point newP1, newP2;
    Point C;
    int i, Nbseg;

    tsup = std::numeric_limits<float>::max();
    tinf = std::numeric_limits<float>::min();

    DX = p2->Getx() - p1->Getx();
    DY = p2->Gety() - p1->Gety();
    Nbseg = window.Getpoints().size();
    for(i = 0; i< Nbseg; i++){
        C = PolyCB[i];
        DN = DX * normals[i].Getx() + DY * normals[i].Gety();
        WN = (p1->Getx() - C.Getx()) * normals[i].Getx() + (p1->Gety() - C.Gety()) * normals[i].Gety();
        if(DN == 0) { /* Division impossible, le segment est réduit à un point */
            return (WN >= 0);
        }else{
            t = -(WN)/(DN);
            //printf("\n t : %f", t);
            if(DN > 0) { /* calcul du max des tinf */
                if(t > tinf) {
                    tinf = t;
                }
            }else{ /* calcul du min des tsup */
                if(t < tsup){
                    tsup = t;
                }
            }
        }
    }
    //printf("\n tinf : %f - tsup : %f", tinf, tsup);
    if(tinf < tsup){ /* Intersection possible */
        if((tinf < 0) && (tsup > 1)){ /* Segment intérieur */
            return true;
        }else{
            if((tinf > 1) | (tsup < 0)){ /* Segment extérieur */
                return false;
            }else{
                if(tinf < 0){ /* A : origine du segment intérieure */
                    tinf = 0;
                }else{
                    if (tsup > 1){ /* B : extrémité du segment intérieure */
                        tsup = 1;
                    }
                }
                /* Calcul des nouvelles intersections donnant le segment découpé */
                //printf("\nAncien : (%f, %f)->(%f,%f)",p1->Getx(), p1->Gety(), p2->Getx(), p2->Gety() );

                x2 = p1->Getx() + DX * tsup;
                y2 = p1->Gety() + DY * tsup;
                x1 = p1->Getx() + DX * tinf;
                y1 = p1->Gety() + DY * tinf;
                newP1 = Point(x1,y1);
                newP2 = Point(x2,y2);
                temp->Addpoint(newP1);
                temp->Addpoint(newP2);

                /*p2->Setx(p1->Getx() + DX * tsup);
                p2->Sety(p1->Gety() + DY * tsup);
                p1->Setx(p1->Getx() + DX * tinf);
                p1->Sety(p1->Gety() + DY * tinf);*/
                //printf("\nNouveau : (%f, %f)->(%f,%f)",p1->Getx(), p1->Gety(), p2->Getx(), p2->Gety() );
                return true;
            }
        }
    }else{ /* Segment extérieur */
        return false;
    }
}

bool coupe(Point P1, Point P2, Point P3, Point P4)
{
    float a = P2.Getx() - P1.Getx();
    float b = P3.Getx() - P4.Getx();
    float c = P2.Gety() - P1.Gety();
    float d = P3.Gety() - P4.Gety();
    float k = 1/((a*d)-(b*c));

    float bx = P3.Getx() - P1.Getx();
    float by = P3.Gety() - P1.Gety();

    float t = k*d*bx - k*-b*by;
    float s = k*-c*bx - k*a*by;
    printf("\n (%f, %f)->(%f, %f) , (%f, %f)->(%f, %f) , t : %f , s : %f", P1.Getx(), P1.Gety(),P2.Getx(), P2.Gety(),P3.Getx(), P3.Gety(),P4.Getx(), P4.Gety(),t, s);

    float det = (P3.Gety() - P4.Gety()) * (P2.Getx() - P1.Getx()) - (P3.Getx() - P4.Getx()) * (P2.Gety() - P1.Gety());
    return det != 0 && -1 < t && t < 1 && -1 < s && s < 1 ;
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
    if(!antiHoraire)
    {
        normal = new Point(-dy, dx);
    }

    //printf("\nNormale : %f, %f", normal->Getx(), normal->Gety());

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

    //printf(", Produit Scalaire : %f", (normal->Getx() * v->Getx()) + (normal->Gety() * v->Gety()));

    return (normal->Getx() * v->Getx()) + (normal->Gety() * v->Gety()) >= 0;
}

Poly sutherlandHodgman(Poly p, Poly window)
{
    vector<Point> PL = p.Getpoints();
    vector<Point> PW = window.Getpoints();
    PW.push_back(window.Getpoints()[0]);

    printf("\nTaille %d - %d", PL.size(), PW.size());

    vector<Point> PS;
    int n;
    Point S,F,I;

    for(int i = 0; i < PW.size() - 1 ; i++)
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
            if(visible(S,PW[i], PW[(i+1)%PW.size()], window.isHoraire()))
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


    Poly temp = Poly(Color(1.0,1.0,1.0));

    for(int i = 0; i < PL.size(); i++)
    {
        temp.Addpoint(PL[i]);
    }


    return temp;

}

Color GetColorPixel(int x, int y) { // 0/0 réel est en x+250 et y+250
    GLubyte data[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE , &data);
    return Color((float)data[0]/255, (float)data[1]/255, (float)data[2]/255);
}

void mouse(int button,int state,int x,int y)
{

	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xClic = x - 250; //on sauvegarde la position de la souris
		yClic = -y + 250;
		printf("\n%d, %d", xClic, yClic);
		if(mode != 0){
            Point *p = new Point(xClic, yClic);
            switch(mode)
            {
            case 1:
                polygons[polygons.size() - 1].Addpoint(*p);
                break;
            case 2:

                windows[windows.size() - 1].Addpoint(*p);
                if(windows[windows.size() - 1].Getpoints().size() >= 3)
                {
                    polywindow.clear();
                    for(int i = 0; i < polygons.size(); i++)
                    {
                        //Poly temp = sutherlandHodgman(polygons[i], windows[windows.size() - 1]);
                        //polywindow.push_back(temp);
                        Poly temp = Poly(Color(1.0,1.0,1.0));
                        for(int j = 0; j < polygons[i].Getpoints().size(); j++){
                            Point p1;
                            Point p2;
                            int m = 1;
                            bool b = cyrusBeck(&polygons[i].GetpointsPointer()->at(j), &polygons[i].GetpointsPointer()->at((j + 1)%polygons[i].Getpoints().size()), windows[windows.size() - 1], &temp);
                        }
                        printf("\n Size poly : %d - Size window : %d - Size polywindow : %d",polygons.size(), windows.size(), polywindow.size() );

                        for(int j = 0; j < windows.back().Getpoints().size(); j++)
                        {
                            float tempX1, tempY1, tempX2, tempY2;
                            tempX1 = windows.back().Getpoints()[j].Getx();
                            tempY1 = windows.back().Getpoints()[j].Gety();
                            tempX2 = 250;
                            tempY2 = windows.back().Getpoints()[j].Getx();

                            bool inpoly = inPolygon(Point(tempX1, tempY1), Point(tempX2, tempY2), polygons[i]);
                            printf("\n (%f, %f) in polygon ? %d", tempX1, tempY1, inpoly);
                            if(inpoly)
                            {
                                temp.Addpoint(windows.back().Getpoints()[j]);
                            }

                        }
                        temp.sortedPoints();
                        polywindow.push_back(temp);
                    }
                }
                break;
            }
		}
        printf("\nAvant\n");
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


