//---------------------------------------------------------------------
//---------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------
#include <Math.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>//Required on some systems
#include <iostream>
#include "glut.h"

using namespace std;
//---------------------------------------------------------------------
class GameOfLife {
public:
    GameOfLife(void);
    ~GameOfLife(void);

	/* Methods */
	void Update(void);
    //--------------------Graphics---------------------------
	void ChangeSize(int w, int h);
	void Draw();
    void Gfx_Text(float x, float y, const char *fmt, ...);//c_str

	/* Variables */
	float mW, mH;

private:
	/* Variables */
	enum STATES
	{
		M1_IS_CURRENT,
		M2_IS_CURRENT
	};
	const static int M = 80;
	const static int N = 50;
	float(*mVertices)[N];
	bool(*mM1)[N];
	bool(*mM2)[N];
	int mCurrent;
	int mCounter;
	int mMouseX, mMouseY;
	int mMouseButton, mMouseState, mGlutGetModifiers;
	int mSamples;
	int mUpdateRate;

	/* Methods */
	int ReceiveAnswers(int validAnswer1, int validAnswer2, int validAnswer3, int validAnswer4);
	int CheckNeighbours(int i, int j, bool(*currentM)[N]);
	void Init();
	void GOLCycle();
	void InitCross();
	void InitCube();
	void InitLine();
	void InitSpaceShip();
	
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
