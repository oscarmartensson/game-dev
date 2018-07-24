// Astar.h
#ifndef ASTAR_H
#define ASTAR_H

#include <Math.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h> //Required on some systems
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include "glut.h"
#include "Node.h"

using namespace std;

class Astar {
public:
	Astar(void);
    ~Astar(void);

	// Methods
	void Update(void);
    //--------------------Graphics-----------------------
	void ChangeSize(int w, int h);
	void Draw();
    void Gfx_Text(float x, float y, const char *fmt, ...); //c_str
	void SearchPath(Node* nodeA, Node* nodeB);

	// Variables
	float mW, mH;

private:
	// Variables
	const static int N = 50;
	const static int M = 50;
	float xCoordSpace = 10.0f;
	float yCoordSpace = 8.0f;
	int mCounter;
	int mMouseX, mMouseY;
	int mMouseButton, mMouseState, mGlutGetModifiers;
	int mUpdateRate;
	Node(*mGrid)[N];

	// Methods
	std::stack<Node*> RetracePath(Node* startNode, Node* endNode);
	std::vector<Node*> ReturnNeighbours(Node* currentNode);
	int ReceiveAnswers(int validAnswer1, int validAnswer2, int validAnswer3);
	int CalcDistance(int xA, int xB, int yA, int yB);
	int Init();
	void InitWall();
	void InitTriangle();
	void InitDiagonal();
};

#endif // !ASTAR_H