//---------------------------------------------------------------------
// File:	Main.cpp
// Summary:	This is the main file for running the Game of Life demo
// Version: Version 1.0 - 2018-07-24
// Author:	Oscar Mårtensson
//-------------------------------------------
// Log:	    2018-07-24 File created
//---------------------------------------------------------------------
#include "GameOfLife.h"

GameOfLife G;

void Display(void){G.Update();}
void ChangeSize(int w, int h){G.ChangeSize(w, h);}

//---------------------------------------------------------------------
// Main function
int main(int argc, char **argv){
	// Init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
	// Window
	glutInitWindowPosition(0,0);
	glutInitWindowSize(G.mW,G.mH);
	glutCreateWindow("Interceptor");
	glutReshapeFunc(ChangeSize);
    
	// Draw
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1);
    
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
    
	// Main
	glutMainLoop();
    return 0;
}
