// copied from Juan Palacios

#pragma once
#include "Queue"
#include "thread"
#include "mutex"
#include "condition_variable"
#include "FactorNode.h"
using namespace std;

class FactorQueue
{
public: 

	// constructor and destructor
	FactorQueue();
	~FactorQueue();

	// Add and remove
	FactorNode Pop();
	void Pop(FactorNode & Node);
	void Push(const FactorNode & Node);
	void Push(FactorNode && Node);

	// Info about the queue
	int ReturnQueueSize();

	// Save Queue entry to file and read back
	void WriteNodeToFile(FactorNode Node, const char * FileName);
	FactorNode ReadNodeFromFile();


private:

	queue<FactorNode> queue_;
	mutex mutex_;
	condition_variable cond_;

};