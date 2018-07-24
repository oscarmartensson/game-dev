//---------------------------------------------------------------------
// File:	Astar.cpp
// Summary:	This file allows the user to find the shortest path from a
//			start node to a target node with the A* path finding algorithm.
// Version: Version 1.0 - 2018-07-01
// Author:	Oscar Mårtensson
//-------------------------------------------
// Log:	    2018-07-01 File created
//---------------------------------------------------------------------
#include "Astar.h"
#include "PriorityQueue.h"

//---------------------------------------------------------------------
// Constructor
Astar::Astar(void)
{
	int layoutSetup;
	mGrid = new Node[M][N]();

	// Setup window and graphics related things
	mCounter = 1;
	mW = 1272.f, mH = 732.f;
	mMouseX = mMouseY = 0;
	mMouseButton = mMouseState = 0,
	mGlutGetModifiers = 0;
	mUpdateRate = 1000;

	// Initialize grid
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			mGrid[i][j].SetX(j);
			mGrid[i][j].SetY(i);
		}
	}

	// Initialize playground and search for shortest path
	layoutSetup = Init();
	if (layoutSetup == 1 || layoutSetup == 2)
	{
		this->SearchPath(&mGrid[45][25], &mGrid[5][25]);
	}
	else
	{
		this->SearchPath(&mGrid[45][37], &mGrid[5][25]);
	}
}

//---------------------------------------------------------------------
// Destructor
Astar::~Astar(void)
{
	delete[] mGrid;
}

//---------------------------------------------------------------------
// Changes viewport size
void Astar::ChangeSize(int w, int h){
	mW = w, mH = h;
	glViewport(0,0,mW,mH);                         // Reset Viewport
	glMatrixMode(GL_PROJECTION); glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(0.0f,mW,mH,0.0f,-1.0f,1.0f);           // Create Ortho View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();  // Reset The Modelview Matrix
}

//---------------------------------------------------------------------
// Called each frame
void Astar::Update(void)
{
	this->Draw();
}

//---------------------------------------------------------------------
// Draw elements of the A* algorithm in the window so the user can more
// easily see what's going on
void Astar::Draw(void)
{
	float xCoord, yCoord;

	// Initialize all coordinates
	xCoord = (mW * 0.5f) - (xCoordSpace * N * 0.5);
	yCoord = (mH * 0.5f) - (yCoordSpace * M * 0.5);

	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(6);
	glBegin(GL_POINTS);
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (mGrid[i][j].GetWalkable() == false)
			{
				// Dark gray
				glColor3ub(100, 100, 100);
			}

			else if ((mGrid[i][j].GetStart() == true) ||
					 (mGrid[i][j].GetGoal() == true))
			{
				// Blue
				glColor3ub(0, 0, 255);
			}

			else if (mGrid[i][j].GetPath() == true)
			{
				// Light blue
				glColor3ub(0, 191, 255);
			}

			else if (mGrid[i][j].GetVisited() == true)
			{
				// Red
				glColor3ub(255, 0, 0);
			}

			else if (mGrid[i][j].GetNeighbour() == true)
			{
				// Green
				glColor3ub(0, 255, 0);
			}

			else
			{
				// Black
				glColor3ub(255, 255, 255);
			}
			glVertex2f(xCoord, yCoord);
			xCoord = xCoord + xCoordSpace;
		}
		xCoord = (mW * 0.5f) - (xCoordSpace * N * 0.5);
		yCoord = yCoord + yCoordSpace;
	};
	glEnd(); 
	glutSwapBuffers();
}

//---------------------------------------------------------------------
// A simple C-string based print function
void Astar::Gfx_Text(float x, float y, const char *fmt, ...){
	char text[256]; va_list	ap;
	if (fmt == NULL) return;
	va_start(ap, fmt); vsprintf_s(text, fmt, ap); va_end(ap);
	glPushMatrix(); glLoadIdentity();
    glRasterPos2f(x,y);
    for (char *c=text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	glPopMatrix();
}
//---------------------------------------------------------------------
// Init function to setup what type of obstacles the algorithm should face
int Astar::Init()
{
	int ans;
	cout << "---------------------------------------------------------------------" << endl;
	cout << "Welcome to a A* path-finding demo." << endl;
	cout << "---------------------------------------------------------------------" << endl;
	cout << "Please choose one of the follow starting obstacles: " << endl;
	cout << "1 : Wall" << endl;
	cout << "2 : Triangle" << endl;
	cout << "3 : Diagnoal wall" << endl;
	ans = ReceiveAnswers(1,2,3);
	if (ans == 1)
	{
		this->InitWall();
	}

	else if (ans == 2)
	{
		this->InitTriangle();
	}

	else
	{
		this->InitDiagonal();
	}

	return ans;
}

//---------------------------------------------------------------------
// Input function that checks whether answers are correct
int Astar::ReceiveAnswers(int validAnswer1, int validAnswer2, int validAnswer3)
{
	int answer = -1;
	while ((answer != validAnswer1) &&
		   (answer != validAnswer2) &&
		   (answer != validAnswer3))
	{
		cout << "Input (" << validAnswer1 << "/" << validAnswer2 << "/" << validAnswer3 << ") : ";
		cin >> answer;
		if (cin.fail())
		{
			// Flush buffer
			cin.clear();
			cin.ignore();
		}
	}
	cout << endl;
	return answer;
}

//---------------------------------------------------------------------
// Initializes a wall obstacle
void Astar::InitWall()
{
	int wallLength = 12;
	for (int j = (N >> 1) - wallLength; j <= (N >> 1) + wallLength; j++)
	{
		mGrid[(M >> 1)][j].SetWalkable(false);
	}

	// Init goal and start node
	mGrid[5][25].SetGoal(true);
	mGrid[45][25].SetStart(true);
}

//---------------------------------------------------------------------
// Initializes a triangle obstacle
void Astar::InitTriangle()
{
	int triangleLength = 22;
	mGrid[(M >> 1)][(N >> 1)].SetWalkable(false);
	mGrid[(M >> 1) - 1][(N >> 1)].SetWalkable(false);
	for (int i = 1; i < triangleLength; i++)
	{
		mGrid[(M >> 1) + i][(N >> 1) - i].SetWalkable(false);
		mGrid[(M >> 1) + i][(N >> 1) + i].SetWalkable(false);
		mGrid[(M >> 1) + i - 1][(N >> 1) - i].SetWalkable(false);
		mGrid[(M >> 1) + i - 1][(N >> 1) + i].SetWalkable(false);
	}

	// Init goal and start node
	mGrid[5][25].SetGoal(true);
	mGrid[45][25].SetStart(true);
}
//---------------------------------------------------------------------
// Initializes a diagnoal wall obstacle
void Astar::InitDiagonal()
{
	int wallLength = 45;
	for (int i = 13; i < wallLength; i++)
	{
		mGrid[i][i].SetWalkable(false);
		mGrid[i - 1][i].SetWalkable(false);
	}

	// Init goal and start node
	mGrid[5][25].SetGoal(true);
	mGrid[45][37].SetStart(true);
}

//---------------------------------------------------------------------
// Searches for shortest path from nodeA to nodeB using the A* path 
// finding algorithm
void Astar::SearchPath(Node* nodeA, Node* nodeB)
{
	// Declare variables to use in the path finding
	PriorityQueue openSet;
	std::vector<Node*> closedSet(20);
	std::vector<Node*>::iterator it;
	int tentativeGCost = 0;
	bool targetFound = false;

	// Add start node to open list
	openSet.push(nodeA);

	while ((openSet.empty() == false) &&
		   (targetFound == false))
	{
		// Grab first element of OpenSet, add to closed list
		Node* currentNode = openSet.top();
		currentNode->SetVisited(true);
		closedSet.push_back(currentNode);
		openSet.pop();

		// Find all neighbours adjacent (left, up, right, down) to current node
		std::vector<Node*> neighbours = ReturnNeighbours(currentNode);
		it = neighbours.begin();
		for (int i = 0; i < (int)neighbours.size(); i++)
		{
			if ((*it)->GetWalkable() == true &&
				(std::find(closedSet.begin(), closedSet.end(), (*it)) == closedSet.end()))
			{
				// Mark that the node is a neighbour for vizualisation
				(*it)->SetNeighbour(true);
				if (currentNode->GetGoal() == true)
				{
					// Target node found, return
					(*it)->SetParent(currentNode);
					targetFound = true;
					break;
				}

				// Evaluate distance
				tentativeGCost = currentNode->GetGCost() + CalcDistance(currentNode->GetXPos(), (*it)->GetXPos(), currentNode->GetYPos(), (*it)->GetYPos());
				if ((tentativeGCost < (*it)->GetGCost()) ||
					(openSet.find(*it)) == false)
				{
					// This is a better path
					(*it)->EstimateHCost(nodeB->GetXPos(), nodeB->GetYPos());
					(*it)->WriteGCost(tentativeGCost);
					(*it)->SetParent(currentNode);
					openSet.push((*it));
				}
			}
			it++;
		}
	}

	if(targetFound == true)
	{
		// Reverse so that it's in correct order
		RetracePath(nodeA, nodeB);
	}
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
std::vector<Node*> Astar::ReturnNeighbours(Node* currentNode)
{
	int i = currentNode->GetXPos();
	int j = currentNode->GetYPos();
	std::vector<Node*> neighbours;

	// loop through nodes around the area of currentNode
	for (int k = i - 1; k <= i + 1; k++)
	{
		for (int p = j - 1; p <= j + 1; p++)
		{
			if ((k < 0) ||
				(k > M - 1) ||
				(p < 0) ||
				(p > N - 1) ||
				(k == i) && (p == j))
			{
				// Index out of bounds in some way, or checking own position
				continue;
			}

			else
			{
				neighbours.push_back(&mGrid[p][k]);
			}
		}
	}
	return neighbours;
}

//---------------------------------------------------------------------
// Calculates the distance from one point to another
int Astar::CalcDistance(int xA, int xB, int yA, int yB)
{
	int dX = abs(xB - xA);
	int dY = abs(yB - yA);
	int dist;

	if (dX > dY)
	{
		dist = 14 * dY + 10 * (dX - dY);
	}
	else
	{
		dist = 14 * dX + 10 * (dY - dX);
	}
	return dist;
}

//---------------------------------------------------------------------
// Retraces the path
std::stack<Node*> Astar::RetracePath(Node* startNode, Node* endNode)
{
	std::stack<Node*> path;
	Node* currentNode = endNode;

	while (currentNode != startNode)
	{
		// Insert element on top of stack
		path.push(currentNode);
		// Mark for vizualisation
		currentNode->SetPath(true);
		currentNode = currentNode->GetParent();
	}

	return path;
}