

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
#include <stack>
#include <list>
#include <tuple>
#include "Point.h"
#include "Polygon.h"
#include "Color.h"

using std::tuple;
using std::stack;
using std::vector;


static int window;
int menu, color_menu, color_polygon_menu, color_window_menu, color_cut_polygon_menu, color_remplissage, remplissage_menu, fenetrage_menu;

int xClic, yClic;  // clic souris
             // coin inf�rieur gauche du carr�

int xContext, yContext;

double c;    // cot� du carr�

//vector<Point> points;
//vector<Color> colors;
Color polyColor, windowColor, windowPolyColor, remplissageColor;
Poly *poly;
vector<Poly> polygons;
vector<Poly> windows;
vector<Poly> polywindow;
vector<Point> pixelColoriser;
int mode = 0;

/* prototypes de fonctions */
void affichage(void);                             // mod�lisation
void mouse(int bouton,int etat,int x,int y);      // fonction souris
void createMenu(void);
void createSubMenu(char colorA[], char colorB[], char colorC[], int iA, int iB, int iC);
void RemplissageLCA(Poly polygon, Color CR);
void CreatSI(Poly polygon);
void processMenuEvents(int option);
void processColorEvents(int option);
void processRemplissageEvents(int option);
void affichePixel(int x, int y, Color CR);
void RemplissageRegionConnexite4A(int x, int y, Color CC, Color CR);
void RemplissageRegionConnexite4B(int x, int y, Color CC, Color CR);
void RemplissageRectEG(Poly pol, Color CR);
void RemplissageLigne(int x, int y, Color cc, Color cr);
void afficheLigne(int xg, int xd, int y, Color cr);
bool inPolygon(Point p1, Point p2, Poly poly);

void newPolygon();
void newWindow();
void Fenetrage(int mode);
Color GetColorPixel(int x, int y);
bool coupe(Point P1, Point P2, Point P3, Point P4);
bool visible(Point S, Point F, Point F1, bool antiHoraire);
Point intersection(Point P1, Point P2, Point P3, Point P4);

/***-- Partie Main --***/
int main(int argc, char **argv){
    polyColor = Color(1.0f, 0.0f, 0.0f);
    windowColor = Color(1.0f, 1.0f, 0.0f);
    windowPolyColor = Color(0.376f, 0.376f, 0.376f);
    remplissageColor = Color(0.466f, 0.709f, 0.996f);

    poly = new Poly(polyColor);

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
	glPointSize(1.0);               // taille d'un point: 2px

	/* Enregistrement des fonctions de rappel
     => initialisation des fonctions callback appel�es par glut */
    glutDisplayFunc(affichage);
	glutMouseFunc(mouse);

    /* rq: le callback de fonction (fonction de rappel) est une fonction qui est pass�e en argument � une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en m�me temps)
	alors qu'il ne les conna�t pas par avance.*/


    //printf("%f, %f, %f", color->Getred(), color->Getgreen(), color->Getblue());

    Point *P1 = new Point(-100.0, 100.0);
    Point *P2 = new Point(100.0, 100.0);
    Point *P3 = new Point(100.0, -100.0);
    Point *P4 = new Point(-100.0, -100.0);

    Point *P5 = new Point(0.0, 0.0);
    Point *P6 = new Point(200.0, 0.0);
    Point *P7 = new Point(200.0, -200.0);
    Point *P8 = new Point(0.0, -200.0);

    //bool c = coupe(*P1, *P2, *P3, *P4);

    //printf("%d", c);
    //if(c)
    {
        //Point temp = intersection(*P1, *P2, *P3, *P4);
        //printf("\n%f - %f", temp.Getx(), temp.Gety());
    }

    //bool v1 = visible(*P4, *P1, *P2, true);
    //bool v2 = visible(*P3, *P1, *P2, true);

    //printf("\n%d - %d", v1, v2);

	/* Entr�e dans la boucle principale de glut, traitement des �v�nements */
    glutMainLoop();         // lancement de la boucle de r�ception des �v�nements
    return 0;
}
/***-- Fin Partie Main --***/

/***-- Partie Menu --***/
void createMenu(void) {
    color_polygon_menu = glutCreateMenu(processColorEvents); //Partie couleurs
    createSubMenu("Rouge", "Bleu", "Vert", 1, 2, 3);
    color_window_menu = glutCreateMenu(processColorEvents);
    createSubMenu("Jaune", "Rose", "Violet", 4, 5, 6);
    color_cut_polygon_menu = glutCreateMenu(processColorEvents);
    createSubMenu("Gris", "Orange", "Marron", 7, 8, 9);
    color_remplissage = glutCreateMenu(processColorEvents);
    createSubMenu("Bleu ciel", "Beige", "Mauve", 10, 11, 12);
    color_menu = glutCreateMenu(processColorEvents);
    glutAddSubMenu("Polygone a decouper", color_polygon_menu);
	glutAddSubMenu("Fenetrage", color_window_menu);
	glutAddSubMenu("Polygone decouper", color_cut_polygon_menu);
    glutAddSubMenu("Remplissage", color_remplissage);

    remplissage_menu = glutCreateMenu(processRemplissageEvents);
    createSubMenu("Algo 1", "Algo 2", "Algo 3", 1, 2, 3);
    glutAddMenuEntry("Remplissage polygone fenetre", 6);
    fenetrage_menu = glutCreateMenu(processRemplissageEvents);
    glutAddMenuEntry("Cyrus Beck", 4);
    glutAddMenuEntry("Sutherland-Hodgdman", 5);

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Couleurs", color_menu);
	glutAddMenuEntry("Polygone a decouper",2);
	glutAddMenuEntry("Trace fenetre",3);
	glutAddSubMenu("Fenetrage", fenetrage_menu);
	glutAddSubMenu("Remplissage", remplissage_menu);
	glutAddMenuEntry("Reinitialiser", 6);
	glutAddMenuEntry("Quitter", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void createSubMenu(char colorA[], char colorB[], char colorC[], int iA, int iB, int iC) {
    glutAddMenuEntry(colorA, iA);
    glutAddMenuEntry(colorB, iB);
    glutAddMenuEntry(colorC, iC);
}

void processColorEvents(int option) {
    switch (option) {
	    case 1:
	        polyColor = Color(1.0f, 0.0f, 0.0f); // Rouge
	        break;
		case 2:
		    polyColor = Color(0.0f, 0.0f, 1.0f); // Bleu
			break;
		case 3:
			polyColor = Color(0.0f, 1.0f, 0.0f); // Vert
			break;
		case 4:
		    windowColor = Color(1.0f, 1.0f, 0.0f); // Jaune
			break;
        case 5:
		    windowColor = Color(0.992f, 0.423f, 0.619f); // Rose
			break;
        case 6:
		    windowColor = Color(0.4f, 0.0f, 0.6f); // Violet
			break;
        case 7:
		    windowPolyColor = Color(0.376f, 0.376f, 0.376f); // Gris
			break;
        case 8:
		    windowPolyColor = Color(0.929f, 0.498f, 0.063f); // Orange
			break;
        case 9:
		    windowPolyColor = Color(0.345f, 0.160f, 0.0f); // Marron
			break;
        case 10:
		    remplissageColor = Color(0.466f, 0.709f, 0.996f); // Bleu ciel
			break;
        case 11:
		    remplissageColor = Color(0.784f, 0.678f, 0.498f); // Beige
			break;
        case 12:
		    remplissageColor = Color(0.831f, 0.451f, 0.831f); // Mauve
			break;
	}
}

void processRemplissageEvents(int option) {
    switch (option) {
	    case 1 :
	        if(polygons.size()) {
                Point p = polygons.back().GetMiddle();
                RemplissageRegionConnexite4A(0, 0, Color(1.0, 0.0, 0.0), remplissageColor); //Point, couleur contour, couleur remplissage
                affichage();
	        } else {
                printf("\n Erreur: Pas de polygone a remplir... \n");
	        }
	        break;
		case 2 :
		    if(polygons.size()) {
                Point p = polygons.back().GetMiddle();
               // RemplissageRegionConnexite4B((int)p.Getx(), (int)p.Gety(), polygons.back().Getcolor(), remplissageColor); //Point, couleur contour, couleur remplissage
                RemplissageLigne(0, 0, Color(1.0, 0.0, 0.0), remplissageColor);
                affichage();
	        } else {
                printf("\n Erreur: Pas de polygone a remplir... \n");
	        }
			break;
		case 3 :
		    if(polygons.size()) {
                RemplissageRectEG(polygons[(polygons.size() - 1)], remplissageColor); //Point, couleur contour, couleur remplissage
                affichage();
	        } else {
                printf("\n Erreur: Pas de polygone a remplir... \n");
	        }
			break;
        case 4 :
            if(windows.size()) {
                Fenetrage(1);
            } else {
                printf("\n Erreur: Pas de fenetre dessinee... \n");
            }
            break;
        case 5 :
            if(windows.size()) {
                Fenetrage(0);
            } else {
                printf("\n Erreur: Pas de fenetre dessinee... \n");
            }
            break;
        case 6:
            printf("%d \n", polywindow.size());
            if(polywindow.size()) {
                RemplissageRectEG(polywindow.back(), remplissageColor); //Point, couleur contour, couleur remplissage
                affichage();
	        } else {
                printf("\n Erreur: Pas de polygone creer par une fenetre... \n");
	        }
            break;
	}
}

void processMenuEvents(int option) {
	switch (option) {
	    case 0 :
	        glutDestroyWindow(window);
            exit(0);
	        break;
		case 1 :
			break;
		case 2 :
			newPolygon();
			break;
		case 3 :
			newWindow();
			break;
		case 4 :
            break;
        case 5 :
            break;
        case 6 :
            polygons.clear();
            windows.clear();
            polywindow.clear();
            pixelColoriser.clear();
            affichage();
            break;
	}
}
/***-- Fin Partie Menu --***/

/***-- Partie Affichage --***/
void affichage(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0, 0);
    glEnd();

    for(int i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    for(int i = 0; i < pixelColoriser.size(); i++) {
        glBegin(GL_POINTS);
        glColor3f(remplissageColor.Getred(), remplissageColor.Getgreen(), remplissageColor.Getblue());
        glVertex2f((int)pixelColoriser[i].Getx(), (int)pixelColoriser[i].Gety());
        glEnd();
        glFlush();
    }

    for(int i = 0; i < windows.size(); i++)
    {
        windows[i].display();
    }

    for(int i = 0; i < polywindow.size(); i++)
    {
        polywindow[i].display();
    }

    glFlush();
}
/***-- Fin Partie Affichage --***/

/***-- Partie Initialisation --***/
void newPolygon() {
    poly = new Poly(polyColor);
    polygons.push_back(*poly);
    mode = 1;
}

void newWindow() {
    poly = new Poly(windowColor);
    windows.push_back(*poly);
    mode = 2;
}
/***-- Fin Partie Initialisation --***/



Color GetColorPixel(int x, int y) { // 0/0 réel est en x+250 et y+250
    GLubyte data[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE , &data);
    return Color((float)data[0]/255, (float)data[1]/255, (float)data[2]/255);
}

void afficheLigne(int xg, int xd, int y, Color cr) {
    for(int i = xg; i < xd; i++) {
        glBegin(GL_POINTS);
        glColor3f(cr.Getred(), cr.Getgreen(), cr.Getblue());
        glVertex2f(i, y);
        glEnd();
        glFlush();
    }
}

void affichePixel(int x, int y, Color CR) {
    Point p = Point(x, y);
    pixelColoriser.push_back(p);
}

/***-- Partie Remplissage --***/
//Probleme avec ce remplissage: probleme de dépassement de capacité due à la récursivité (ne fonctionne que sur les petits polygones)
void RemplissageRegionConnexite4A(int x, int y, Color CC, Color CR) { // point, couleur contour, couleur remplissage
    Color CP = GetColorPixel(x+250, y+250);
    if(!(CP == CC) && !(CP == CR)) {
        affichePixel(x, y, CR);
        RemplissageRegionConnexite4A(x, y+1, CC, CR);
        RemplissageRegionConnexite4A(x, y-1, CC, CR);
        RemplissageRegionConnexite4A(x+1, y+1, CC, CR);
        RemplissageRegionConnexite4A(x-1, y, CC, CR);
    }
}

//Moins de probleme de capacité grace à la pile mais crash sur des gros polygones tout de meme
void RemplissageRegionConnexite4B(int x, int y, Color CC, Color CR) {
    Color CP;
    stack<Point> p; //Initialisation de la pile à vide
    Point point = Point(x, y);
    p.push(point); // On empile le point dans p
    while(!p.empty()) { // Tant qu'il y a un point à traiter
        Point sommet = p.top(); //Recupere le sommet de la pile
        x = (int)sommet.Getx();
        y = (int)sommet.Gety();
        p.pop(); // Depile
        CP = GetColorPixel(x+250, y+250);
        if(!(CP == CC) && !(CP == CR)) {
            affichePixel(x, y, CR);
        }
        CP = GetColorPixel(x+250, y+250-1);
        if(!(CP == CC) && !(CP == CR)) {
            Point temp = Point(x, y-1);
            p.push(temp);
        }
        CP = GetColorPixel(x+250-1, y+250);
        if(!(CP == CC) && !(CP == CR)) {
            Point temp = Point(x-1, y);
            p.push(temp);
        }
        CP = GetColorPixel(x+250, y+250+1);
        if(!(CP == CC) && !(CP == CR)) {
            Point temp = Point(x, y+1);
            p.push(temp);
        }
        CP = GetColorPixel(x+250+1, y+250);
        if(!(CP == CC) && !(CP == CR)) {
            Point temp = Point(x+1, y);
            p.push(temp);
        }
    }
}

void Remplissage4(int x, int y, Color CC, Color CR) {
    stack<Point> p; //Initialisation de la pile à vide
    int w,e;
    Color CP = GetColorPixel(x+250, y+250);
    if(!(CP == Color(0.0, 0.0, 0.0) || !(CP == Color(1.0, 1.0, 1.0)))) {
        return;
    }
    Point point = Point(x, y);
    p.push(point); // On empile le point dans p
    while(!p.empty()) {
        p.pop();
        if((CP == Color(0.0, 0.0, 0.0) || (CP == Color(1.0, 1.0, 1.0)))) {

        }
    }
}

void RemplissageRectEG(Poly pol, Color CR) {
    int x, y, xMin, xMax, yMin, yMax;
    xMin = (int)pol.GetXMin();
    yMin = (int)pol.GetYMin();
    xMax = (int)pol.GetXMax();
    yMax = (int)pol.GetYMax();
    for(x = xMin; x <= xMax; x++) {
        for(y = yMin; y <= yMax; y++) {
            if(inPolygon(Point(x, y), Point(x+250, y), pol)) {
                affichePixel(x, y, CR);
            }
        }
    }
}

void RemplissageLigne(int x, int y, Color CC, Color CR) { // point de depart, couleur contour = rouge, couleur remplissage = grise
    stack<Point> p; //Initialisation de la pile à vide
    Color CP, CPd, CPg; //couleur du pixel
    int xd, xg;
    Point point = Point(x, y);
    p.push(point); // On empile le point dans p
    while(!p.empty()) { // Tant qu'il y a un point à traiter
        Point sommet = p.top(); //Recupere le sommet de la pile
        x = (int)sommet.Getx();
        y = (int)sommet.Gety();
        p.pop(); // Depile
        CP = GetColorPixel(x+250, y+250);

        // On determine les abscisses extremes xg et xd de la ligne de balayage y
        //recherche de l'extreme droite
        xd = x + 1;
        CPd = CP;
        while(!(CPd == CC) && xd < 250) {
            CPd = GetColorPixel(xd+250, y+250);
            xd += 1;
        }
        xd -= 1;

        // Recherche de l'extreme gauche
        xg = x - 1;
        CPg = CP;
        while(!(CPg == CC) && xg > -250) {
            xg = xg - 1;
            CPg = GetColorPixel(xg+250, y+250);
        }
        xg += 1;

        //Affichage de la ligne
        afficheLigne(xg, xd, y, CR);
        x = xd;
        CP = GetColorPixel(x+250, y+250+1);
        while(x >= xg) {
            while(((CP == CC) || (CP == CR)) && (x >= xg)) {
                x -= 1;
                CP = GetColorPixel(x+250, y+1+250);
            }

            if((x >= xg) && !(CP == CC) && !(CP == CR)) {
                //On empile le nouveau germe au dessus trouvé);
                Point tempo = Point(x, y+1);
                p.push(tempo);
            }
            while(!(CP == CC) && (x >= xg)) {
                x = x-1;
                CP = GetColorPixel(x+250, y+1+250);
            }

        }

        //Recherche de nouveau germe sur la ligne de balayage en-dessous
        x = xd;
        CP = GetColorPixel(x+250, y+250-1);
        while(x >= xg) {
            while(((CP == CC) || (CP == CR)) && (x >= xg)) {
                x = x-1;
                CP = GetColorPixel(x+250, y-1+250);
            }
            if((x >= xg) && !(CP == CC) && !(CP == CR)) {
                //On empile le nouveau germe en dessous trouvé
                Point tempo = Point(x, y-1);
                p.push(tempo);
            }
            while(!(CP == CC) && (x >= xg)) {
                x = x - 1;
                CP = GetColorPixel(x+250, y-1+250);
            }
        }
    }
}

//Creation de la structure SI
void CreatSI(Poly polygon) {
    //Tableau de liste chainée
    //taille du tableau : max du polygon en Y.
}

void RemplissageLCA(Poly polygon, Color CR) {
    //Structure SI
    // Initialisation de la structure LCA à vide ici
}

/***-- Fin Partie Remplissage --***/

/***-- Partie Fenetrage --***/
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
        printf("\n(%f, %f) (%f, %f) -> DN == 0 ? %d", p1->Getx(), p1->Gety(), p2->Getx(), p2->Gety(),  DN == 0);
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
        printf("reeeee");
    }
    printf("\n tinf : %f - tsup : %f", tinf, tsup);
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
                printf("\nNouveau : (%f, %f)->(%f,%f)",p1->Getx(), p1->Gety(), p2->Getx(), p2->Gety() );
                return true;
            }
        }
    }else{ /* Segment extérieur */
        return false;
    }
}

bool coupe(Point Pj, Point Pk, Point F, Point F1, bool antiHoraire)
{
    /*float a = P2.Getx() - P1.Getx();
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
    return det != 0 && -1 < t && t < 1 && -1 < s && s < 1 ;*/

    float dx = F1.Getx() - F.Getx();
    float dy = F1.Gety() - F.Gety();
    Point *normal = new Point(dy, -dx);
    if(!antiHoraire)
    {
        normal = new Point(-dy, dx);
    }

    float vx = Pj.Getx() - F.Getx();
    float vy = Pj.Gety() - F.Gety();

    Point *vj = new Point(vx, vy);

    float posJ = (normal->Getx() * vj->Getx()) + (normal->Gety() * vj->Gety());

    if(posJ > 0 ){
        posJ = 1;
    }

    if(posJ < 1)
    {
        posJ = -1;
    }

    vx = Pk.Getx() - F.Getx();
    vy = Pk.Gety() - F.Gety();

    Point *vk = new Point(vx, vy);

    float posK = (normal->Getx() * vk->Getx()) + (normal->Gety() * vk->Gety());

    if(posK > 0 ){
        posK = 1;
    }

    if(posK < 1)
    {
        posK = -1;
    }


    printf("\n (%f, %f)->(%f, %f) , (%f, %f)->(%f, %f) ,posJ : %f , posK : %f", Pj.Getx(), Pj.Gety(),Pk.Getx(), Pk.Gety(),F.Getx(), F.Gety(),F1.Getx(), F1.Gety(),posJ, posK);


    return posJ != posK;
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
                if(coupe(S,PL[j], PW[i], PW[(i+1)%PW.size()], window.isHoraire() ))
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
            if(coupe(S,F, PW[i], PW[(i+1)%PW.size()], window.isHoraire() ))
            {
                I = intersection(S,F, PW[i], PW[(i+1)%PW.size()] );
                PS.push_back(I);
                n++;
            }
            PL = PS;
        }
    }


    Poly temp = Poly(windowPolyColor);

    for(int i = 0; i < PL.size(); i++)
    {
        temp.Addpoint(PL[i]);
    }


    return temp;

}
/***-- Fin Partie Fenetrage --***/

/***-- Partie Interaction --***/
void Fenetrage(int mode)
{
    if(windows[windows.size() - 1].Getpoints().size() >= 3)
    {
        polywindow.clear();
        for(int i = 0; i < polygons.size(); i++)
        {
            Poly temp = Poly(Color(0.0,0.0,0.0));
            switch(mode){
                case 0:
                    //Sutherland Hogdman
                    temp = sutherlandHodgman(polygons[i], windows[windows.size() - 1]);
                    break;
                case 1:
                    temp = Poly(windowPolyColor);

                    //Cyrus Beck
                    for(int j = 0; j < polygons[i].Getpoints().size(); j++){
                        Point p1;
                        Point p2;
                        int m = 1;
                        bool b = cyrusBeck(&polygons[i].GetpointsPointer()->at(j), &polygons[i].GetpointsPointer()->at((j + 1)%polygons[i].Getpoints().size()), windows[windows.size() - 1], &temp);
                    }

                    //Ajout des sommets de la fenetre au nouveau polygon s'ils sont à l'intérieur du polygon découpé
                    for(int j = 0; j < windows.back().Getpoints().size(); j++)
                    {
                        float tempX1, tempY1, tempX2, tempY2;
                        tempX1 = windows.back().Getpoints()[j].Getx();
                        tempY1 = windows.back().Getpoints()[j].Gety();
                        tempX2 = 250;
                        tempY2 = windows.back().Getpoints()[j].Getx();

                        bool inpoly = inPolygon(Point(tempX1, tempY1), Point(tempX2, tempY2), polygons[i]);
                        if(inpoly)
                        {
                            temp.Addpoint(windows.back().Getpoints()[j]);
                        }

                    }


                    if(temp.Getpoints().size())
                    {
                        //Tri des sommets dans l'ordre horaire
                        temp.sortedPoints();
                    }
                    break;
                }
            //printf("\n%f, %f, %f", temp.Getcolor().Getred(),temp.Getcolor().Getgreen(), temp.Getcolor().Getblue() );
            polywindow.push_back(temp);

        }
    }
    affichage();
}

void mouse(int button,int state,int x,int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { 	// Si on appuie sur le bouton de gauche
		xClic = x - 250; //on sauvegarde la position de la souris
		yClic = -y + 250;
		xContext = x;
		yContext = y;

		if(mode != 0) {
            Point *p = new Point(xClic, yClic);
            switch(mode) {
            case 1: // Mode creation de polygone
                polygons[polygons.size() - 1].Addpoint(*p);
                break;
            case 2:
                windows[windows.size() - 1].Addpoint(*p);
                break;
            }
		}
		affichage(); //Refresh l'affichage
		return;
	}
}
/***-- Fin Partie Interaction --***/

