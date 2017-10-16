#pragma once
#include <vector>
using namespace std;

class LongNumber
{
public:
	LongNumber();
	//LongNumber(const LongNumber&);											// copy constructor
	//LongNumber LongNumber::operator=(const LongNumber ArgA);				// copy assignment 
	LongNumber(int LongNumberLengthReq);									// create Long Number of required length 
	LongNumber(char *);														// create Long Number from string with or without +/- at start
	LongNumber(const char *);												// create Long Number from string with or without +/- at start
	~LongNumber();															// destructor

	//Set Values
	void SetLongNumber(int AValue, int APos);								// set value at position in Long Number

	// Return Values
	int GetValue(int i);																// returns int in position i 
	int GetLongNumberLength();															// returns length of digits 
	friend void PrintLongNumberLR(LongNumber ArgA);										// prints long number normally left to right with endl
	friend ostream& operator<< (ostream & stream, LongNumber ArgA);						// overload << for file output
	friend Platform::String^ LongNumberChar(LongNumber ArgA);							// convert long number to platform string for display
	friend int LongNumbertoInt(LongNumber ArgA);										// returns int if long number not too long
	friend LongNumber LongNumberReturnPart(LongNumber ArgA, int start, int length);				// returns part of a number - works like Mid - from start for length chars

	// Arithmetic Operations
	friend LongNumber LongNumberAdd(LongNumber ArgA, LongNumber ArgB);				// adds
	friend LongNumber LongNumberMultiply(LongNumber ArgA, LongNumber ArgB);			// multiplies two Long Numbers (base 10)
	friend LongNumber LongNumberMultiplyInt(LongNumber ArgA, int mult);				// multiplies a Long Number by an int
	friend LongNumber LongNumberSubtract(LongNumber ArgA, LongNumber ArgB);			// Subtracts A > B
	
	// comparison operators
	friend int LongNumberCompare(LongNumber ArgA, LongNumber ArgB);				// compares, returns 1 if >, 0 if equal and -1 if <

	// Tidying and concatenation
	friend LongNumber TrimLongNumber(LongNumber ArgA);							// trim leading zeros off Long Number 
	friend LongNumber Concatenate(LongNumber ArgA, LongNumber ArgB);			// Concatenate two Long Numbers
	friend LongNumber ConcatenateIntLN(int A, LongNumber ArgB);
	friend LongNumber Pad(LongNumber ArgA, int PadLen);							// Adds left hand zeros to a Long Number
	

private:

	vector <int> LongNumberValue;					// vector to hold the digits of the long number
	int LongNumberLength;							// number of digits in long number


};

