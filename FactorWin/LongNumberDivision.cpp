#include "pch.h"
#include "LongNumberDivision.h"
#include "LongNumber.h"
#include "iostream"
using namespace std;


LongNumberDivision::LongNumberDivision()
{
}

LongNumberDivision::LongNumberDivision(int QuotientLength, int RemainderLength)
{
	quotient = LongNumber(QuotientLength);
	remainder = LongNumber(RemainderLength);

}


LongNumberDivision::~LongNumberDivision()
{
}

void LongNumberDivision::SetLongNumberDivisionQuotient(LongNumber ArgA)
{
	quotient = ArgA;
}

void LongNumberDivision::SetLongNumberDivisionRemainder(LongNumber ArgA)
{
	remainder = ArgA;
}

LongNumber LongNumberDivision::GetQuotient()
{
	return quotient;
}

LongNumber LongNumberDivision::GetRemainder()
{
	return remainder;
}

void PrintLongNumberDivision(LongNumberDivision ArgA)
{
	cout << "Quotient : ";
	PrintLongNumberLR(ArgA.GetQuotient());
	cout << "Remainder : ";
	PrintLongNumberLR(ArgA.GetRemainder());

}


LongNumberDivision LongNumberDivide(LongNumber ArgA, LongNumber ArgB)
{
	// Long Number Division for base 10

	// Get length of Arguments to function 
	int LenA = ArgA.GetLongNumberLength();
	int LenB = ArgB.GetLongNumberLength();

	// Check that A is greater than B - if not exit
	if (LongNumberCompare(ArgB, ArgA) == 1)
	{
		// Set remainder to be ArgA, quotient is zero
		LongNumber Quotient(1);
		Quotient.SetLongNumber(0, 0);

		LongNumberDivision Result(1, LenB);
		Result.SetLongNumberDivisionQuotient(Quotient);
		Result.SetLongNumberDivisionRemainder(ArgA);
		return Result;

	}

	// Set up the Long Number Registers that will hold the working 
	// ArgA and ArgB are used directly from function arguments
	// LenA is the max length that these can be
	LongNumber Quotient(LenA + 1);
	LongNumber AWorking(LenA + 1);
	LongNumber Sum(LenA + 1);					// Sum needs to be larger to cope with Q = 10 
	LongNumber NextDigit(1);
	LongNumberDivision Result(LenA + 1, LenA + 1);

	// Set up the pointers
	// No need for a ArgB pointer or a S pointer
	int APointer = 0;
	int QPointer = 0;

	// Set up the lengths
	// Len A and B already set above
	int LenQ = 0;
	int LenS = 0;
	int LenAW = 0;

	// Now load initial values into the Registers and set the pointers and lengths
	// Constructor shouls have set all values to zero, so only need to load AW now.  
	// Q, S & R are all zero to start with 
	APointer = LenA - LenB;
	LenAW = LenB;
	AWorking = LongNumberReturnPart(ArgA, APointer, LenAW);
	// PrintLongNumberLR(AWorking);
	QPointer = APointer;

	// Start the main loop that steps through A
	for (APointer; APointer >= 0; APointer--)
	{
		// Test to see if B goes into AW and if so, how many times
		if ((LongNumberCompare(AWorking, ArgB) == 1 ) || (LongNumberCompare(AWorking, ArgB) == 0) )
		{
			// Calculate the number of times B goes into AW into the quotient and store this in the quotient
			for (int QM = 1; QM <= 10; QM++)
			{
				Sum = LongNumberMultiplyInt(ArgB, QM);
				if (LongNumberCompare(Sum, AWorking) == 1)
				{
					QM--;											// we always go one step too far
					Quotient.SetLongNumber(QM, QPointer);			// store in Quotient
					Sum = LongNumberMultiplyInt(ArgB, QM);			// recalc Sum at lower level
					break;											// quit the loop
				}
			}

			// Calculate the remainder
			AWorking = LongNumberSubtract(AWorking, Sum);

			// PrintLongNumberLR(Sum);
			// PrintLongNumberLR(Quotient);
			// PrintLongNumberLR(AWorking);

		}

		// Reset the registers and lengths for the next iteration of the loop
		if (APointer > 0)				// no need to do this if reached end of A
		{
			NextDigit = LongNumberReturnPart(ArgA, APointer - 1, 1);
			AWorking = TrimLongNumber(Concatenate(AWorking, NextDigit));
			// PrintLongNumberLR(AWorking);
			QPointer--;
		}

	}

	// Return the Quotient and Remainder 
	Result.SetLongNumberDivisionQuotient(TrimLongNumber(Quotient));
	// PrintLongNumberLR(AWorking);
	Result.SetLongNumberDivisionRemainder(AWorking);
	return Result;
}

