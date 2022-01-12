

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
void RemplissageLCA(Poly polygon, Color CR);
void CreatSI(Poly polygon);
void processMenuEvents(int option);
void processColorEvents(int option);
void affichePixel(int x, int y, Color CR);
void RemplissageRegionConnexite4A(int x, int y, Color CC, Color CR);
void RemplissageRegionConnexite4B(int x, int y, Color CC, Color CR);
void RemplissageLigne(int x, int y, Color cc, Color cr);
void afficheLigne(int xg, int xd, int y, Color cr);
void newPolygon();
void newWindow();
Color GetColorPixel(int x, int y);
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
	glPointSize(1.0);               // taille d'un point: 2px

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
            Point pt = Point(0, 0);
            Color cr = Color(0.8, 0.8, 0.8);
            RemplissageRegionConnexite4B(0, 0, Color(1.0, 0.0, 0.0), cr);
            //RemplissageLigne((int)pt.Getx(), (int)pt.Gety(), Color(1.0, 0.0, 0.0), cr);
            //RemplissageRegionConnexite4(pt, Color(1.0, 0.0, 0.0), cr);
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

    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0, 0);
    glEnd();
    glFlush();

    glFlush();

    printf("\nCouleur centre: %f, %f, %f \n", GetColorPixel(250, 250).Getred(), GetColorPixel(250, 250).Getgreen(), GetColorPixel(250, 250).Getblue());

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

//Creation de la structure SI
void CreatSI(Poly polygon) {
    //Tableau de liste chainée
    //taille du tableau : max du polygon en Y.

}


void RemplissageLCA(Poly polygon, Color CR) {
    //Structure SI

    // Initialisation de la structure LCA à vide ici


}


void affichePixel(int x, int y, Color CR) {
    glBegin(GL_POINTS);
    glColor3f(CR.Getred(), CR.Getgreen(), CR.Getblue());
    glVertex2f(x, y);
    glEnd();
    glFlush();
}


//Probleme avec ce remplissage: probleme de dépassement de capacité due à la récursivité (ne fonctionne que sur les petits polygones)
void RemplissageRegionConnexite4A(int x, int y, Color CC, Color CR) { // point, couleur contour, couleur remplissage
    Color CP;
    CP = GetColorPixel(x+250, y+250);
    if(!(CP == CC) && !(CP == CR)) {
        affichePixel(x, y, CR);
        RemplissageRegionConnexite4A(x, y-1, CC, CR);
        RemplissageRegionConnexite4A(x-1, y, CC, CR);
        RemplissageRegionConnexite4A(x, y+1, CC, CR);
        RemplissageRegionConnexite4A(x+1, y, CC, CR);
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
        printf("\n x: %d y: %d \n", x, y);
        printf("\n taille de la pile : %d \n", p.size());
        p.pop(); // Depile
        printf("\n taille de la pile : %d \n", p.size());
        CP = GetColorPixel(x+250, y+250);
        //printf("%d, %d, %d \n", CP.Getred(), CP.Getgreen(), CP.Getblue());

        // On determine les abscisses extremes xg et xd de la ligne de balayage y
        //recherche de l'extreme droite
        xd = x + 1;
        CPd = CP;
        while(!(CPd == CC)) {
            xd += 1;
            CPd = GetColorPixel(xd+250, y+250);
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

        //Recherche de nouveau germe sur la ligne de balayage au-dessus

        /*x = xd;
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(x, y+1);
        glEnd();
        glFlush();
        CP = GetColorPixel(x+250, y+1+250);
        printf("%d, %f, %f, %f \n", x, CP.Getred(), CP.Getgreen(), CP.Getblue());*/
        x = xd;
        CP = GetColorPixel(x+250, y+250+1);
        //printf("%d, %f, %f, %f \n", x, CPd.Getred(), CPd.Getgreen(), CPd.Getblue());
        //printf("%d, %f, %f, %f \n", xd, CP.Getred(), CP.Getgreen(), CP.Getblue());
        while(x >= xg) {
            while(((CP == CC) || (CP == CR)) && (x >= xg)) {
                x -= 1;
                CP = GetColorPixel(x+250, y+1+250);
            }

            printf("%d, %d, %f, %f, %f \n", x, xg, CP.Getred(), CP.Getgreen(), CP.Getblue());
            if((x >= xg) && !(CP == CC) && !(CP == CR)) {
                //On empile le nouveau germe au dessus trouvé);
                printf("/n X: %d Point numero %d\n", x, y+1);
                Point tempo = Point(x, y+1);
                p.push(tempo);
            }
            while(!(CP == CC) && (x >= xg)) {
                x = x-1;
                CP = GetColorPixel(x+250, y+1+250);
            }
            printf("%d, %d, %f, %f, %f \n", x, xg, CP.Getred(), CP.Getgreen(), CP.Getblue());

        }

        //Recherche de nouveau germe sur la ligne de balayage en-dessous
      /*  x = xd;
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
        }*/
        //printf("%d, %f, %f, %f \n", xd, CPd.Getred(), CPd.Getgreen(), CPd.Getblue());
        //recherche de l'extreme gauche
    }
}

int* rectangleEnglobant(Poly poly) {
    int RectEG[2];
    //recuperer min et max d'un polygone
    return RectEG;
}

bool interieur(int x, int y, Poly pol) {

}


void RemplissageRectEG(Poly pol, Point RectEG[2], Color CR) {
    int x, y, xMin, xMax, yMin, yMax;

}

/*bool cyrusBeck(Point p1, Point p2, Poly window)
{
    /*float dx = F1.Getx() - F.Getx();
    float dy = F1.Gety() - F.Gety();
    Point *normal = new Point(dy, -dx);
    if(!window.isHoraire())
    {
        normal = new Point(-dy, dx);
    }

    float t, tsup, tinf, DX, DY, WN, DN;
    Point C;

    tsup = std::numeric_limits<float>::min();
    tinf = std::numeric_limits<float>::max();

    DX ←− X2 − X1 ; DY ←− Y2 − Y1
    Nbseg ←− Nbsom - 1
    Pour i variant de 1 à Nbseg Faire
        C ←− Poly[i]
        DN ←− DX ∗ Normale[i][x] + DY ∗ Normale[i][y]
        WN ←− (X1 − C[x]) ∗ Normale[i][x] + (Y1 − C[y]) ∗ Normale[i][y]
        Si (DN = 0) Alors /* Division impossible, le segment est réduit à un point */
       /*     Renvoyer (WN > 0)
        Sinon
            t ←− −(W N)/(DN)
            Si (DN > 0) Alors /* calcul du max des tinf
                Si (t > tinf) Alors
                    tinf ←− t
                FinSi
            Sinon /* calcul du min des tsup
                Si (t < tsup) Alors
                    tsup ←− t
                FinSi
            Finsi
        Finsi
    FinPour
    Si (tinf < tsup) Alors /* Intersection possible
        Si ((tinf < 0) et (tsup > 1)) Alors /* Segment intérieur
            Renvoyer Vrai
        Sinon
            Si ((tinf > 1) ou (tsup < 0)) Alors /* Segment extérieur
                Renvoyer Faux
            Sinon
                Si (tinf < 0) Alors /* A : origine du segment intérieure
                    tinf ←− 0
                Sinon
                Si (tsup > 1) Alors /* B : extrémité du segment intérieure
                    tsup ←− 1
                FinSi
            FinSi
            /* Calcul des nouvelles intersections donnant le segment découpé
            X2 ←− X1 + DX ∗ tsup
            Y2 ←− Y1 + DY ∗ tsup
            X1 ←− X1 + DX ∗ tinf
            Y1 ←− Y1 + DY ∗ tinf
            Renvoyer Vrai
        FinSi
    FinSi
    Sinon /* Segment extérieur

        Renvoyer Fau
    FinSi
}*/

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

void mouse(int button,int state,int x,int y)
{

	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xClic = x - 250; //on sauvegarde la position de la souris
		yClic = -y + 250;
		xContext = x;
		yContext = y;
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
                    printf("yaaaa");
                    polywindow.clear();
                    for(int i = 0; i < polygons.size(); i++)
                    {
                        Poly temp = sutherlandHodgman(polygons[i], windows[windows.size() - 1]);
                        polywindow.push_back(temp);
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
			break;
	}
}


