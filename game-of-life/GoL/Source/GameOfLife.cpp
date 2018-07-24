//---------------------------------------------------------------------
// File:	GameOfLife.cpp
// Summary:	This is the behavior for the Game of Life game.
// Version: Version 1.0 - 2018-07-24
// Author:	Oscar Mårtensson
//-------------------------------------------
// Log:	    2018-07-24 File created
//---------------------------------------------------------------------
#include "GameOfLife.h"

//---------------------------------------------------------------------
// Constructor
GameOfLife::GameOfLife(void){

	/* Allocate matrices */
	mM1 = new bool[M][N];
	mM2 = new bool[M][N];
	memset(mM1, (int)false, sizeof(bool) * M * N);
	memset(mM2, (int)false, sizeof(bool) * M * N);

	mCurrent = M1_IS_CURRENT;

	/* Setup window and graphics related things */
	mCounter = 1;
	mW = 1272.f, mH = 732.f;
	mMouseX = mMouseY = 0;
	mMouseButton = mMouseState = 0,
	mGlutGetModifiers = 0;
	mUpdateRate = 1000;
	this->Init();
}

//---------------------------------------------------------------------
// Destructor
GameOfLife::~GameOfLife(void)
{
	delete[] mM1;
	delete[] mM2;
}

//---------------------------------------------------------------------
// Changes size of window
void GameOfLife::ChangeSize(int w, int h){
	mW = w, mH = h;
	glViewport(0,0,mW,mH);                         // Reset Viewport
	glMatrixMode(GL_PROJECTION); glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(0.0f,mW,mH,0.0f,-1.0f,1.0f);           // Create Ortho View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();  // Reset The Modelview Matrix
}

//---------------------------------------------------------------------
// Update method for the game
void GameOfLife::Update(void)
{
	this->Draw();

	/* Don't run too often or it will be hard to see */
	if (mCounter % mUpdateRate == 0)
	{
		this->GOLCycle();
	}
	mCounter++;
}

//---------------------------------------------------------------------
// Draws the game of life board
void GameOfLife::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	bool (*currentM)[N];
	float xCoord, yCoord, xCoordSpace, yCoordSpace;

	xCoordSpace = 10.0f;
	yCoordSpace = 8.0f;
	xCoord = (mW * 0.5f) - (xCoordSpace * N * 0.5);
	yCoord = (mH * 0.5f) - (yCoordSpace * M * 0.5);

	// Choose current buffer
	if (mCurrent == M1_IS_CURRENT)
	{
		currentM = mM1;
	}
	else
	{
		currentM = mM2;
	}

	// Draw
	glPointSize(4);
	glBegin(GL_POINTS);
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (currentM[i][j] == false)
			{
				glColor3ub(255, 255, 255);
			}
			else
			{
				glColor3ub(255, 0, 0);
			}
			glVertex2f(xCoord, yCoord);
			xCoord = xCoord + xCoordSpace;
		}
		xCoord = (mW / 2) - (xCoordSpace * N * 0.5);
		yCoord = yCoord + yCoordSpace;
	};

	glEnd(); 
	glutSwapBuffers();
}

//---------------------------------------------------------------------
//A simple C-string based print function
void GameOfLife::Gfx_Text(float x, float y, const char *fmt, ...){
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
// Lets the user choose a game setting and fires the game up
void GameOfLife::Init()
{
	int ans;

	cout << "---------------------------------------------------------------------" << endl;
	cout << "Welcome to the Game of Life." << endl;
	cout << "---------------------------------------------------------------------" << endl;
	cout << "Please choose one of the follow starting settings: " << endl;
	cout << "1 : Cross pattern" << endl;
	cout << "2 : Cube pattern" << endl;
	cout << "3 : Line pattern" << endl;
	cout << "4 : Light weight space ship" << endl;

	ans = ReceiveAnswers(1,2,3,4);

	if (ans == 1)
	{
		this->InitCross();
	}

	else if (ans == 2)
	{
		this->InitCube();
	}

	else if (ans == 3)
	{
		this->InitLine();
	}

	else
	{
		this->InitSpaceShip();
	}
}

//---------------------------------------------------------------------
// Receives answers from input and check whether they're valid
int GameOfLife::ReceiveAnswers(int validAnswer1, int validAnswer2, int validAnswer3, int validAnswer4)
{
	int answer = -1;

	while ((answer != validAnswer1) &&
		   (answer != validAnswer2) &&
		   (answer != validAnswer3) &&
		   (answer != validAnswer4))
	{
		cout << "Input (" << validAnswer1 << "/" << validAnswer2 << "/" << validAnswer3 << "/" << validAnswer4 << ") : ";
		cin >> answer;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
		}
	}

	cout << endl;
	return answer;
}

//---------------------------------------------------------------------
// Initializes a cross
void GameOfLife::InitCross()
{
	int crossExtrude = 3;

	for (int i = 39 - crossExtrude; i <= 39 + crossExtrude; i++)
	{
		mM1[i][24] = true;
	}

	for (int i = 24 - crossExtrude; i <= 24 + crossExtrude; i++)
	{
		mM1[39][i] = true;
	}
}

//---------------------------------------------------------------------
// Initializes a cube
void GameOfLife::InitCube()
{
	int boxExtrude = 3;

	for (int i = 39 - boxExtrude; i <= 39 + boxExtrude; i++)
	{
		for (int j = 24 - boxExtrude; j <= 24 + boxExtrude; j++)
		{
			mM1[i][j] = true;
		}
	}
}

//---------------------------------------------------------------------
// Initializes a line 
void GameOfLife::InitLine()
{
	int lineExtrude = 3;

	for (int i = 39 - lineExtrude; i <= 39 + lineExtrude; i++)
	{
		mM1[i][24] = true;
	}
}

//---------------------------------------------------------------------
// Initializes a space ship
void GameOfLife::InitSpaceShip()
{
	mM1[39][24] = true;
	mM1[39][25] = true;
	mM1[39][26] = true;
	mM1[39][27] = true;
	mM1[40][27] = true;
	mM1[41][27] = true;
	mM1[42][26] = true;
	mM1[42][23] = true;
	mM1[40][23] = true;
}

//---------------------------------------------------------------------
// Iterates through all neighbors surrounding the current coordinate
// to see if neighbor is alive or not. If alive, increment nrOfAliveNeighbours.
int GameOfLife::CheckNeighbours(int i, int j, bool (*currentM)[N])
{
	int nrOfAliveNeighbours = 0;

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
				continue;
			}

			else
			{
				if (currentM[k][p] == true)
				{
					nrOfAliveNeighbours++;
				}
			}
		}
		cout << endl;
	}

	cout << endl;
	return nrOfAliveNeighbours;
}

//---------------------------------------------------------------------
// Iterates a game of life cycle. Swaps between two buffers, M1 and M2
// each iteration, where the next buffer is updated with next rounds cycle.
void GameOfLife::GOLCycle()
{
	bool(*currentM)[N];
	bool(*notCurrentM)[N];
	int aliveNeightbours;
	int nextState;
	bool isAlive;

	if (mCurrent == M1_IS_CURRENT)
	{
		currentM = mM1;
		notCurrentM = mM2;
		nextState = M2_IS_CURRENT;
	}

	else
	{
		currentM = mM2;
		notCurrentM = mM1;
		nextState = M1_IS_CURRENT;
	}
	
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (currentM[i][j] == true)
			{
				isAlive = true;
			}

			else
			{
				isAlive = false;
			}

			aliveNeightbours = CheckNeighbours(i, j, currentM);

			/* Determine what happens to this individual */
			if ((isAlive) && 
				(aliveNeightbours > 3))
			{
				// Current individual dies
				notCurrentM[i][j] = false;
			}

			else if ((isAlive) && 
					 (aliveNeightbours < 2))
			{
				// Current individual dies
				notCurrentM[i][j] = false;
			}

			else if ((isAlive) &&
				     (aliveNeightbours == 2 || aliveNeightbours == 3))
			{
				// Current individual lives
				notCurrentM[i][j] = true;
			}

			else if ((isAlive == false) &&
				     (aliveNeightbours == 3))
			{
				// Current individual resurrects
				notCurrentM[i][j] = true;
			}
		}
	}

	// Clean up current M, otherwise causing some states to retain when they shouldn't
	memset(currentM, (int)false, sizeof(bool) * M * N);
	mCurrent = nextState;
}