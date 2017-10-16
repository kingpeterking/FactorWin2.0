#pragma once
#include <LongNumber.h>

class FactorNode
{
public:

	// Constructors and destructors
	FactorNode();									// default constructor
													// constructor with data
	FactorNode(int LevelPassed,	LongNumber AValuePassed, LongNumber BValuePassed, bool FactorCompletePassed);

	~FactorNode();									// Destructor

	// Set Values
	void SetLevel(int LevelPassed);
	void SetAValue(LongNumber AValuePassed);
	void SetBValue(LongNumber BValuePassed);
	void SetFactorComplete(bool YN);

	// Return Values
	int LNGetLevel();
	LongNumber LNGetAValue();
	LongNumber LNGetBValue();
	bool GetFactorComplete();

	// Print Node
	friend void PrintFactorNode(FactorNode* FNode);

private:

	int Level;
	LongNumber AValue;
	LongNumber BValue;
	bool FactorComplete;
};

