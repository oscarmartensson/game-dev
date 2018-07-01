// PriorityQueue.h
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <queue>
#include "Node.h"
#include "CompareNode.h"

class PriorityQueue : public std::priority_queue<Node*, std::vector<Node*>, CompareNode>
{
public:
	bool find(const Node* Node) const;
};

#endif // !PRIORITYQUEUE_H