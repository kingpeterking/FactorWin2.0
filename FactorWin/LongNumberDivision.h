#pragma once
#include "LongNumber.h"

class LongNumberDivision 
{
public:
	// Constructors and destructors
	LongNumberDivision();												// Declaration constructor
	LongNumberDivision(int QuotientLength, int RemainderLength);		// constructor with length of quotient and remainder
	~LongNumberDivision();												// Destructor

																		// Set Values
	void SetLongNumberDivisionQuotient(LongNumber ArgA);
	void SetLongNumberDivisionRemainder(LongNumber ArgA);

	// Return Values
	LongNumber GetQuotient();
	LongNumber GetRemainder();
	friend void PrintLongNumberDivision(LongNumberDivision ArgA);

	// Operations for division of Long Numbers
	friend LongNumberDivision LongNumberDivide(LongNumber ArgA, LongNumber ArgB);
	friend LongNumberDivision LongNumberDivideBase(LongNumber ArgA, LongNumber ArgB);
	friend LongNumberDivision operator/ (LongNumber ArgA, LongNumber ArgB);

private:

	LongNumber quotient;
	LongNumber remainder;



};

