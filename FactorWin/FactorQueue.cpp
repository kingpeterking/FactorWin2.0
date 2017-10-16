#include "pch.h"
#include "FactorQueue.h"
#include "Queue"
#include "thread"
#include "mutex"
#include "condition_variable"
#include "FactorNode.h"
#include "iostream"
#include "fstream"

using namespace std;

const int MaxQueueLen = 50000000;
int FileNodeCount;

// Constructor = default
FactorQueue::FactorQueue() = default;

// Destructor = none
FactorQueue::~FactorQueue()
{
}



FactorNode FactorQueue::Pop()
{
	FactorNode FNReturn;
	unique_lock<mutex> mlock(mutex_);
	while (queue_.empty())
	{
		cond_.wait(mlock);
	}
	if (FileNodeCount > 0)
	{
		FNReturn = ReadNodeFromFile();
	}
	else
	{
		FNReturn = queue_.front();
		queue_.pop();
	}

	// Was FactorNode FNReturn = queue_.front();

	return FNReturn;
}

void FactorQueue::Pop(FactorNode & Node)
{
	unique_lock<mutex> mlock(mutex_);
	while (queue_.empty())
	{
		cond_.wait(mlock);
	}
	Node = queue_.front();
	queue_.pop();

}

void FactorQueue::Push(const FactorNode & Node)
{
	const char * FileName = "QueueNode.txt";
	unique_lock<mutex> mlock(mutex_);
	int QS = queue_.size();
	if (QS < MaxQueueLen)
	{
		queue_.push(Node);
	}
	else
	{
		WriteNodeToFile(Node, FileName);
	}
	mlock.unlock();
	cond_.notify_one();

}

void FactorQueue::Push(FactorNode && Node)
{
	unique_lock<mutex> mlock(mutex_);
	queue_.push(move(Node));
	mlock.unlock();
	cond_.notify_one();

}

int FactorQueue::ReturnQueueSize()
{
	return queue_.size();
}

void FactorQueue::WriteNodeToFile(FactorNode Node, const char * FileName)
{
	filebuf QF;
	QF.open(FileName, ios::app);
	ostream QueueFile(&QF);
	LongNumber AValue = Node.LNGetAValue();
	LongNumber BValue = Node.LNGetBValue();
	int Level = Node.LNGetLevel();
	QueueFile << AValue << endl;
	QueueFile << BValue << endl;
	QueueFile << Level << endl;
	FileNodeCount++;
	QF.close();

}

FactorNode FactorQueue::ReadNodeFromFile()
{
	const char * FileName = "QueueNode.txt";
	const char * FileNameTemp = "QueueNodeTemp.txt";

	ifstream QueueFile(FileName, ios::in);

	// read first value into the return value 
	char InChar[1000];
	QueueFile >> InChar;
	LongNumber AValue(InChar);
	QueueFile >> InChar;
	LongNumber BValue(InChar);
	QueueFile >> InChar;
	int Level = atoi(InChar);

	FactorNode FNReturn(Level, AValue, BValue, false);
	FileNodeCount--;

	// copy the rest into a new file
	ofstream QueueFileTemp(FileNameTemp, ios::app);

	while (!QueueFile.eof())
	{
		QueueFile >> InChar;
		QueueFileTemp << InChar << endl;
	}

	QueueFile.close();
	QueueFileTemp.close();
	remove(FileName);
	rename(FileNameTemp, FileName);

	return FNReturn;
}