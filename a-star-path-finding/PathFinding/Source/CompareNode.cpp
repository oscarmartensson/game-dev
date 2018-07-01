//---------------------------------------------------------------------
// File:	CompareNode.cpp
// Summary:	This class is a help class used only as comparator for the 
//			std::priority_queue class to determine if the F cost of
//			NodeB is less than the F cost of NodeA
// Version: Version 1.0 - 2018-07-01
// Author:	Oscar Mårtensson
//-------------------------------------------
// Log:	    2018-07-01 File created
//---------------------------------------------------------------------
#include "CompareNode.h"

//---------------------------------------------------------------------
// Compares Node A to Node B
bool CompareNode::operator() (Node* NodeA, Node* NodeB) const
{
	if (NodeB->GetFCost() < NodeA->GetFCost() ||
	   ((NodeB->GetFCost() == NodeA->GetFCost()) &&
	    (NodeB->GetHCost() < NodeA->GetHCost())))
	{
		return true;
	}
	else
	{
		return false;
	}
}
