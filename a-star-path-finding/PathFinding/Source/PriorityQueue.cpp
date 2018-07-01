//---------------------------------------------------------------------
// File:	PriorityQueue.cpp
// Summary:	This class is an extension of the std::priority_queue to also
//			allow to iterate through the list in order to determine if an
//			object is contained in the queue or not.
// Version: Version 1.0 - 2018-07-01
// Author:	Oscar Mårtensson
//-------------------------------------------
// Log:	    2018-07-01 File created
//---------------------------------------------------------------------
#include "PriorityQueue.h"

//---------------------------------------------------------------------
// Searches for Node in the priority queue
bool PriorityQueue::find(const Node* Node) const
	{
		auto First = this->c.begin();
		auto Last = this->c.cend();
		while (First != Last) {
			if (*First == Node) return true;
			++First;
		}
		return false;
	}