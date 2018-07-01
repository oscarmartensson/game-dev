#ifndef NODE_H
#define NODE_H

class Node
{
public:
	// Constructor
	Node();

	// Methods
	int   GetXPos();
	int   GetYPos();
	int   GetHCost();
	int   GetFCost();
	int   GetGCost();
	bool  GetWalkable();
	bool  GetStart();
	bool  GetGoal();
	bool  GetVisited();
	bool  GetNeighbour();
	bool  GetPath();
	void  EstimateHCost(int xDist, int yDist);
	void  WriteGCost(int dist);
	void  SetX(int x);
	void  SetY(int y);
	void  SetWalkable(bool state);
	void  SetStart(bool state);
	void  SetGoal(bool state);
	void  SetVisited(bool state);
	void  SetNeighbour(bool state);
	void  SetPath(bool state);
	void  SetParent(Node* parentNode);
	Node* GetParent();

private:
	// Variables
	int xPos, yPos;
	int hCost, gCost;
	bool walkable, visited, start, goal, neighbour, path;
	Node* parent;
};

#endif // !NODE_H