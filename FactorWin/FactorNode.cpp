#include "pch.h"
#include "FactorNode.h"
#include "iostream"
using namespace std;

FactorNode::FactorNode()
{
	// Create 0 as a Long Number
	LongNumber ZeroA(1);
	ZeroA.SetLongNumber(0, 0);

	// Creates a new node with no data or values 
	Level = 0;
	AValue = ZeroA;
	BValue = ZeroA;
	FactorComplete = false;
}


FactorNode::FactorNode(
	int LevelPassed,
	LongNumber AValuePassed,
	LongNumber BValuePassed,
	bool FactorCompletePassed)
{
	// Creates a new node with the data passed
	Level = LevelPassed;
	AValue = AValuePassed;
	BValue = BValuePassed;
	FactorComplete = FactorCompletePassed;

}

FactorNode::~FactorNode()
{
}


void FactorNode::SetLevel(int LevelPassed)
{
	Level = LevelPassed;
}

void FactorNode::SetAValue(LongNumber AValuePassed)
{
	AValue = AValuePassed;
}

void FactorNode::SetBValue(LongNumber BValuePassed)
{
	BValue = BValuePassed;

}

void FactorNode::SetFactorComplete(bool YN)
{
	FactorComplete = YN;
}



LongNumber FactorNode::LNGetAValue()
{
	return AValue;
}

LongNumber FactorNode::LNGetBValue()
{
	return BValue;
}


int FactorNode::LNGetLevel()
{
	return Level;
}

bool FactorNode::GetFactorComplete()
{
	return FactorComplete;
}


void PrintFactorNode(FactorNode * FNode)
{
	// Print the details for this node
	cout << "Node : " << endl;
	cout << "  Level : " << FNode->LNGetLevel() << endl;
	cout << "  AValue : ";
	PrintLongNumberLR(FNode->LNGetAValue());
	cout << "  BValue : ";
	PrintLongNumberLR(FNode->LNGetBValue());
	cout << "  Factor Complete: ";
	if (FNode->GetFactorComplete())
	{
		cout << "Y";
	}
	else
	{
		cout << "N";
	}
	cout << endl;

}
