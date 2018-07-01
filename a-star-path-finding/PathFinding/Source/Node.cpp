//---------------------------------------------------------------------
// File:	Node.cpp
// Summary:	Support class for Astar. The search algorithm uses a grid of
//			this Node to perform its operations
// Version: Version 1.0 - 2018-07-01
// Author:	Oscar Mårtensson
//-------------------------------------------
// Log:	    2018-07-01 File created
//---------------------------------------------------------------------
#include "Node.h"
#include <cmath>

// Constructor
Node::Node()
{
	xPos = 0;
	yPos = 0;
	gCost = 0;
	hCost = 0;
	parent = NULL;
	this->walkable = true;
	this->goal = false;
	this->start = false;
	this->visited = false;
	this->neighbour = false;
	this->path = false;
}

// Returns whether node is walkable
bool Node::GetWalkable()
{
	return this->walkable;
}

// Returns whether node is the goal
bool Node::GetGoal()
{
	return this->goal;
}

// Returns whether node is the start
bool Node::GetStart()
{
	return this->start;
}

// Returns whether node is visited
bool Node::GetVisited()
{
	return this->visited;
}

// Returns whether node is visited
bool Node::GetNeighbour()
{
	return this->neighbour;
}

// Returns whether node part of final path
bool Node::GetPath()
{
	return this->path;
}

// Returns x position
int Node::GetXPos()
{
	return this->xPos;
}

// Returns y position
int Node::GetYPos()
{
	return this->yPos;
}

// Returns h cost
int Node::GetHCost()
{
	return this->hCost;
}

// Returns g cost 
int Node::GetGCost()
{
	return this->gCost;
}

// Returns f cost
int Node::GetFCost()
{
	return this->gCost + this->hCost;
}

// Estimates the hCost from this node to goal node
void Node::EstimateHCost(int xDist, int yDist)
{
	int dX = abs(xDist - xPos);
	int dY = abs(yDist - yPos);
    this->hCost = 10 * (dX + dY);
}

// Write gCost
void Node::WriteGCost(int dist)
{
	this->gCost = dist;
}

// Set -coordinate
void Node::SetX(int x)
{
	this->xPos = x;
}

// Set y-coordinate
void Node::SetY(int y)
{
	this->yPos = y;
}

// Set whether walkable or not
void Node::SetWalkable(bool state)
{
	this->walkable = state;
}

// Set whether the goal node
void Node::SetGoal(bool state)
{
	this->goal = state;
}

// Set whether the start node
void Node::SetStart(bool state)
{
	this->start = state;
}

// Set whether node is visited
void Node::SetVisited(bool state)
{
	this->visited = state;
}

// Set whether neighbour or not
void Node::SetNeighbour(bool state)
{
	neighbour = state;
}

// Set whether neighbour or not
void Node::SetPath(bool state)
{
	path = state;
}

// Returns parent node
Node* Node::GetParent()
{
	return parent;
}

// Set node parent
void Node::SetParent(Node* parentNode)
{
	parent = parentNode;
}