// CompareNode.h
#ifndef COMPARENODE_H
#define COMPARENODE_H

#include "Node.h"

class CompareNode
{
public:
	bool operator() (Node* NodeA, Node* NodeB) const;
};

#endif // !COMPARENODE_H
