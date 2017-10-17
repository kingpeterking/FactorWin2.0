#include "pch.h"
#include "LongNumber.h"
#include "iostream"
#include "cmath"
#include "string"
#include "vector"

// for GPU
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
__global__ void MultiplyVectorKernel(const int * ArgA, const int * ArgB, int * Result); 

using namespace std;

LongNumber::LongNumber()
{
}



LongNumber::LongNumber(int LongNumberLengthReq)
{

	LongNumberValue.reserve(LongNumberLengthReq);
	for (int iCount = 0; iCount < LongNumberLengthReq; iCount++)
	{
		LongNumberValue.push_back(0);
	}

	// cout << "Length" << endl;
	LongNumberLength = LongNumberLengthReq;

}

LongNumber::LongNumber(char * NoA)
{
	int NumStart = 0;

	// Constructor from char array pointer
	int NoALen = 0;
	while (NoA[NoALen] != '\0')
	{
		NoALen++;
		//cout << NoALen;
	}

	LongNumberValue.reserve(NoALen);

	for (int iCount = 1; iCount <= NoALen; iCount++)
	{
		LongNumberValue.push_back(NoA[NoALen - iCount] - 48);
		//cout << iCount << " : " << (NoALen) - (iCount + 1) + HasSign <<  " : " << NoA[(NoALen)-(iCount + 1) + HasSign] - 48 << endl;
	}
	LongNumberLength = NoALen;

}


LongNumber::LongNumber(const char * NoA)
{
	int NumStart = 0;

	// Constructor from char array pointer
	int NoALen = 0;
	while (NoA[NoALen] != '\0')
	{
		NoALen++;
		//cout << NoALen;
	}

	LongNumberValue.reserve(NoALen);

	for (int iCount = 0; iCount < NoALen; iCount++)
	{
		this->SetLongNumber(NoA[(NoALen)-(iCount + 1)] - 48, iCount);
		// was LongNumberValue[iCount] = NoA[(NoALen)-(iCount + 1)] - 48;
		//cout << iCount << " : " << (NoALen) - (iCount + 1) + HasSign <<  " : " << NoA[(NoALen)-(iCount + 1) + HasSign] - 48 << endl;
	}
	LongNumberLength = NoALen;

}




LongNumber::~LongNumber()
{
}

void LongNumber::SetLongNumber(int AValue, int APos)
{
	// was LongNumberValue[APos] = AValue;
	// check that the number is long enough 
	if (this->GetLongNumberLength() <= APos)
	{
		this->LongNumberValue.reserve(APos + 1);
		this->LongNumberLength++;
		LongNumberValue.emplace(LongNumberValue.begin() + APos, AValue);
	}
	else
	{
		LongNumberValue[APos] = AValue;
	}

	
}


int LongNumber::GetValue(int i)
{
	if (this->GetLongNumberLength() <= i) {
		return 0;
	}
	if (i < 0) { return -1; }

	return LongNumberValue[i];
}


int LongNumber::GetLongNumberLength()
{
	return LongNumberLength;
}

Platform::String^ LongNumberChar(LongNumber ArgA)
{
	int LenA = ArgA.GetLongNumberLength(); 
	Platform::String ^ ReturnChar = ref new Platform::String();

	if (LenA == 0)
	{
		ReturnChar = "";
		return ReturnChar;
	}

	int RetPos = 0;
	for (int iCount = ArgA.GetLongNumberLength() - 1; iCount >= 0; iCount--)
	{
		switch (ArgA.GetValue(iCount))
		{
		case 0: {ReturnChar = Platform::String::Concat(ReturnChar, "0"); break; }
		case 1: {ReturnChar = Platform::String::Concat(ReturnChar, "1"); break; }
		case 2: {ReturnChar = Platform::String::Concat(ReturnChar, "2"); break; }
		case 3: {ReturnChar = Platform::String::Concat(ReturnChar, "3"); break; }
		case 4: {ReturnChar = Platform::String::Concat(ReturnChar, "4"); break; }
		case 5: {ReturnChar = Platform::String::Concat(ReturnChar, "5"); break; }
		case 6: {ReturnChar = Platform::String::Concat(ReturnChar, "6"); break; }
		case 7: {ReturnChar = Platform::String::Concat(ReturnChar, "7"); break; }
		case 8: {ReturnChar = Platform::String::Concat(ReturnChar, "8"); break; }
		case 9: {ReturnChar = Platform::String::Concat(ReturnChar, "9"); break; }
		}
	}

	return ReturnChar;
}

int LongNumbertoInt(LongNumber ArgA)
{
	// returns int if long number less than 10 chars long, returns 0 otherwise
	int LenA = ArgA.GetLongNumberLength();
	int Result = 0;

	if (LenA > 10)
	{
		return Result;
	}
	
	for (int iCount = 0; iCount < LenA; iCount++)
	{
		Result += ArgA.GetValue(iCount) * (int) pow(10, iCount); 
	}

	return Result; 

}

LongNumber LongNumberReturnPart(LongNumber ArgA, int start, int length)
{
	
	// Returns part of the long number
	// starts at position Start for length numbers
	// works like mid
	// works from right to left - so position 0 is the units column

	//cout << "Returning Part" << endl;
	//cout << "Start : " << start << endl;
	//cout << "Length : " << length << endl;
	//PrintLongNumberLR(ArgA);

	// create holder for result
	// should check that length of this is greater than length, but haven't implemented error handling yet!

	LongNumber LNReturn(length);

	// Check length of ArgA vs start and length 
	LongNumber MinusOne(1);
	MinusOne.SetLongNumber(1, 0);

	int LenA = ArgA.GetLongNumberLength();
	if (LenA <= start) { return MinusOne; }
	if (LenA < length) { return MinusOne; }

	// PrintLongNumberLR(ArgA);

	// build the new number character by character
	for (int iCount = 0; iCount < length; iCount++)
	{
		LNReturn.SetLongNumber(ArgA.GetValue(start + iCount), iCount);
	}

	// PrintLongNumberLR(LNReturn);
	return LNReturn;


}






LongNumber TrimLongNumber(LongNumber ArgA)
{
	// Returns a new Long Number with leading zeros removed
	// if all zeros returns a long number with a single zero

	//cout << "Trimming \n";
	int LenA = ArgA.GetLongNumberLength();
	//cout << "Len A : " << LenA << endl; 
	int iCount = LenA - 1;
	//cout << "iCount : " << iCount << endl;

	while (ArgA.GetValue(iCount) == 0)
	{
		iCount--;
	}
	// cout << "iCount after while: " << iCount << endl;

	if (iCount >= 0)
	{
		LongNumber ReturnLN(iCount + 1);

		for (int iAddtoRet = 0; iAddtoRet <= iCount; iAddtoRet++)
		{
			//cout << "iAddtoRet : " << iAddtoRet << endl;
			ReturnLN.SetLongNumber(ArgA.GetValue(iAddtoRet), iAddtoRet);
		}
		// PrintLongNumberLR(ReturnLN);
		return ReturnLN;
	}
	else
	{
		// returning 0
		LongNumber ReturnLN(1);
		ReturnLN.SetLongNumber(0, 0);
		return ReturnLN;
	}


}

LongNumber Concatenate(LongNumber ArgA, LongNumber ArgB)
{
	// Concatenate two long numbers
	// Take the sign from ArgA, ignore the sign from ArgB
	int ArgALen = ArgA.GetLongNumberLength();
	int ArgBLen = ArgB.GetLongNumberLength();
	int iResultLen = ArgALen + ArgBLen;

	// construct holder for return 
	LongNumber LNResult(iResultLen);

	// Put ArgB in by simple assignment
	// Result = ArgB;
	for (int iCount = 0; iCount < ArgBLen; iCount++)
	{
		LNResult.SetLongNumber(ArgB.GetValue(iCount), iCount);
	}

	// Add digits from ArgA
	for (int iCount = ArgBLen; iCount < iResultLen; iCount++)
	{
		LNResult.SetLongNumber(ArgA.GetValue(iCount - ArgBLen), iCount);
	}

	return LNResult;
}

LongNumber ConcatenateIntLN(int A, LongNumber ArgB)
{
	// Overloaded version of Concatenate that adds an integer to the front of the array 
	vector<int>::iterator It;
	It = ArgB.LongNumberValue.begin();
	ArgB.LongNumberValue.insert(It, A);
	return ArgB;
}




LongNumber Pad(LongNumber ArgA, int PadLen)
{
	// Pads ArgA with leading zeros 
	int LenA = ArgA.GetLongNumberLength();
	int ReturnLen = LenA + PadLen;

	// Build the return Long Number
	LongNumber LNReturn(ReturnLen);

	// Copy ArgA into Return
	for (int iCount = 0; iCount < LenA; iCount++)
	{
		LNReturn.SetLongNumber(ArgA.GetValue(iCount), iCount);
	}

	// Add zeros to Return
	for (int iCount = LenA; iCount < PadLen; iCount++)
	{
		LNReturn.SetLongNumber(0, iCount);
	}

	// PrintLongNumberLR(LNReturn);

	return LNReturn;

}


void PrintLongNumberLR(LongNumber ArgA)
{

	for (int iCount = ArgA.GetLongNumberLength() - 1; iCount >= 0; iCount--)
	{
		cout << ArgA.GetValue(iCount);
	}
	cout << "\n";

}

ostream & operator<<(ostream & stream, LongNumber ArgA)
{
	for (int iCount = ArgA.GetLongNumberLength() - 1; iCount >= 0; iCount--)
	{
		stream << ArgA.GetValue(iCount);
	}
	
	return stream;

}



LongNumber LongNumberAdd(LongNumber ArgA, LongNumber ArgB)
{
	// Adds two long numbers
	//cout << "Adding Numbers \n";

	// get the length of both numbers 
	int iCount;
	int ArgALen = ArgA.GetLongNumberLength();
	int ArgBLen = ArgB.GetLongNumberLength();

	int iResultLen = ArgALen;
	//cout << "Length Arg A : " << ArgALen << " : Length Arg B : " << ArgBLen << endl;

	if (ArgBLen > ArgALen)
	{
		//cout << "Len B greater than A\n";
		iResultLen = ArgBLen;
	}
	// no need to test for them being both the same length 

	// Add the two numbers
	int iCarry = 0;
	int iSumUnit = 0;
	int iSum = 0;

	LongNumber LNResult(iResultLen + 1);
	//cout << "Result Len : " << iResultLen << '\n';

	//cout << "Adding Loop \n";
	for (iCount = 0; iCount < iResultLen; iCount++)
	{
		int AVal;
		int BVal;
		if (iCount < ArgALen)
		{
			AVal = ArgA.GetValue(iCount);
		}
		else
		{
			AVal = 0;
		}

		if (iCount < ArgBLen)
		{
			BVal = ArgB.GetValue(iCount);
		}
		else
		{
			BVal = 0;
		}

		iSum = AVal + BVal + iCarry;
		iSumUnit = iSum % 10;
		iCarry = (iSum - iSumUnit) / 10;
		LNResult.SetLongNumber(iSumUnit, iCount);
		//cout << "iSum : " << iSum << endl; 
		//cout << "iSumUnit : " << iSumUnit << endl;
		//cout << "iCarry : " << iCarry << endl;
	}
	if (iCarry > 0)
	{
		LNResult.SetLongNumber(iCarry, iResultLen);
	}

	// PrintLongNumberLR(LNResult);
	LNResult = TrimLongNumber(LNResult);
	// PrintLongNumberLR(LNResult);

	return LNResult;

}


LongNumber LongNumberMultiply(LongNumber ArgA, LongNumber ArgB)
{

	// Multiplies two long numbers
	// cout << "Multiplying Numbers \n";

	// get the length of both numbers 
	int ArgALen = ArgA.GetLongNumberLength();
	int ArgBLen = ArgB.GetLongNumberLength();

	// PrintLongNumberLR(ArgA);
	// PrintLongNumberLR(ArgB);

	// cout << "Length Arg A : " << ArgALen << " : Length Arg B : " << ArgBLen << endl;
	// cout << " Sign A : " << SignA << " Sign B : " << SignB << endl;

	// Set length of result
	int iResultLen = ArgALen;
	if (ArgBLen > ArgALen)
	{
		// cout << "Len B greater than A\n";
		iResultLen = ArgBLen;
	}
	// no need to test for them being both the same length 

	// Multiply the two numbers
	int iCarry = 0;
	int iSumUnit = 0;
	int iSum = 0;
	int iCountA = 0;
	int iCountB = 0;
	int iAVal = 0;
	int iBVal = 0;

	LongNumber LNResult(iResultLen * 2);		// this is the returned value that is built by repeatedly adding the LNResultTemp on each cycle

	// cout << "Multiplying Loop \n";
	for (iCountA = 0; iCountA < iResultLen; iCountA++)
	{

		for (iCountB = 0; iCountB < iResultLen; iCountB++)
		{
			// cout << "Start B loop" << endl;
			int ConstructLen = iResultLen * 2;
			// cout << "Pre LN Constructor" << endl;
			LongNumber LNResultTemp(ConstructLen);
			// cout << endl;
			// cout << "iResultLen : " << iResultLen << endl;
			//cout << "iCountA : " << iCountA << endl;
			//cout << "iCountB : " << iCountB << endl;
			if (iCountA < ArgALen)
			{
				iAVal = ArgA.GetValue(iCountA);
			}
			else
			{
				iAVal = 0;
			}
			if (iCountB < ArgBLen)
			{
				iBVal = ArgB.GetValue(iCountB);
			}
			else
			{
				iBVal = 0;
			}
			//cout << "AVal : " << iAVal << endl;
			//cout << "BVal : " << iBVal << endl;
			iSum = (iAVal * iBVal);
			//cout << "iSum : " << iSum << endl;
			iSumUnit = iSum % 10;
			//cout << "iSumUnit : " << iSumUnit << endl;
			iCarry = (iSum - iSumUnit) / 10;
			//cout << "iCarry : " << iCarry << endl;
			LNResultTemp.SetLongNumber(iSumUnit, iCountB + iCountA);
			LNResultTemp.SetLongNumber(iCarry, iCountB + iCountA + 1);
			//PrintLongNumberLR(LNResultTemp);
			LNResult = LongNumberAdd(LNResult, LNResultTemp);		
			// PrintLongNumberLR(LNResult);
			LNResult = TrimLongNumber(LNResult);
			// PrintLongNumberLR(LNResult);
		}

	}


	//PrintLongNumber(LNResult);
	return LNResult;


}

LongNumber LongNumberMultiplyInt(LongNumber ArgA, int mult)
{
	// Create a Long Number based on mult (using the char constructor
	// Then use the standard multiply function 
	string sMultChar = to_string(mult);
	char const *MultCharPointer = sMultChar.c_str();

	LongNumber LNMult(MultCharPointer);			// constructor to hold Long Number based on int
	//PrintLongNumberLR(LNMult);

	LongNumber Result = LongNumberMultiply(ArgA, LNMult);
	return Result;
}

LongNumber LongNumberSubtract(LongNumber ArgA, LongNumber ArgB)
{
	// subtracts B from A
	// Assumes A greater than B - needs to be checked by calling routine before calling subtract 
	// Also assumes that both are positive - ie signs are ignored and result is always positive
	// cout << "Subtract"; 

	// get the length of both numbers 
	int iCount;
	int ArgALen = ArgA.GetLongNumberLength();
	int ArgBLen = ArgB.GetLongNumberLength();
	int iResultLen = ArgALen;
	// cout << "Length Arg A : " << ArgALen << " : Length Arg B : " << ArgBLen << endl;

	// Subtract the two numbers
	int iCarry = 0;
	int iSumUnit = 0;
	int iSum = 0;
	int SignRes = 1; // holder until implement subtraction 

	LongNumber LNResult(iResultLen + 1);
	// cout << "Result Len : " << iResultLen << '\n';

	// cout << "Subtraction Loop \n";
	for (iCount = 0; iCount < iResultLen; iCount++)
	{
		int AVal;
		int BVal;
		if (iCount < ArgALen)
		{
			AVal = ArgA.GetValue(iCount);
		}
		else
		{
			AVal = 0;
		}

		if (iCount < ArgBLen)
		{
			BVal = ArgB.GetValue(iCount);
		}
		else
		{
			BVal = 0;
		}

		iSum = (AVal - iCarry) - BVal;
		if (iSum < 0)
		{
			iSumUnit = 10 + iSum;
			iCarry = 1;
		}
		else
		{
			iSumUnit = iSum;
			iCarry = 0;
		}
		LNResult.SetLongNumber(iSumUnit, iCount);
		// cout << "iSum : " << iSum << endl; 
		// cout << "iSumUnit : " << iSumUnit << endl;
		// cout << "iCarry : " << iCarry << endl;
	}

	// Not needed for positive subtraction 
	//LNResult.SetLongNumber(iCarry, iResultLen);

	// PrintLongNumber(LNResult);
	LNResult = TrimLongNumber(LNResult);

	return LNResult;



}





int LongNumberCompare(LongNumber ArgA, LongNumber ArgB)
{
	// Compares two long numbers
	// Returns 
	// -1 if ArgA less than Arg B
	// 0 if they are the same
	// +1 if ArgA greater than Arg B

	// Check lengths
	// firstly trim
	ArgA = TrimLongNumber(ArgA);
	// PrintLongNumberLR(ArgA);
	ArgB = TrimLongNumber(ArgB);
	int LenA = ArgA.GetLongNumberLength();
	int LenB = ArgB.GetLongNumberLength();

	if (LenA > LenB)
	{
		// A longer than B so must be greater (assumes trimmed) 
		// cout << "A longer than B" << endl;
		return 1;
	}
	if (LenA < LenB)
	{
		// A shorter than B so must be less (assumes trimmed) 
		// cout << "A shorter than B" << endl;
		return -1;
	}

	// got here so have same sign and same length 

	for (int iCount = LenA - 1; iCount >= 0; iCount--)
	{
		int AVal = ArgA.GetValue(iCount);
		int BVal = ArgB.GetValue(iCount);
		if (AVal > BVal)
		{
			// A higher than B
			// cout << "A : " << AVal << endl;
			// cout << "B : " << BVal << endl;
			// cout << "A in pos greater than B : iCount " << iCount << endl;
			return 1;
		}
		if (AVal < BVal)
		{
			// A less than B
			// cout << "A : " << AVal << endl;
			// cout << "B : " << BVal << endl;
			// cout << "A in pos less than B : iCount " << iCount << endl;
			return -1;
		}

	}

	// All of the numbers matched so must be the same
	// cout << "End of function so matched" << endl;
	return 0;

}


// Parallel GPU functions

__global__ void MultiplyVectorKernel(const int * ArgA, const int * ArgB, int * Result)
{
	int i = threadIdx.x;
	Result[i] = ArgA[i] * ArgB[i];

}


LongNumber LongNumberCUDAMultiply(LongNumber ArgA, LongNumber ArgB)
{

	// get length of input variables
	int LenA = ArgA.GetLongNumberLength(); 
	int LenB = ArgB.GetLongNumberLength();
	int LenR = LenA + LenB + 1;

	// Extract int array from Long Numbers to be passed to device
	int * ArgAHost; 
	int * ArgBHost;
	for (int iCount = 0; iCount < LenA; iCount++)
	{
		ArgAHost[iCount] = ArgA.GetValue(iCount); 
	}
	for (int iCount = 0; iCount < LenA; iCount++)
	{
		ArgBHost[iCount] = ArgB.GetValue(iCount);
	}
	int * ReturnHost; 

	// set up device variables
	int * device_ArgA = 0; 
	int * device_ArgB = 0;
	int * device_Result = 0;
	cudaError_t cudaStatus;

	// set device (only one on this PC)
	cudaStatus = cudaSetDevice(0);

	// allocate space for args and result
	cudaStatus = cudaMalloc((void**)&device_ArgA, LenA * sizeof(int));
	cudaStatus = cudaMalloc((void**)&device_ArgB, LenB * sizeof(int));
	cudaStatus = cudaMalloc((void**)&device_Result, LenR * sizeof(int));

	// copy the variables to the device
	cudaStatus = cudaMemcpy(device_ArgA, ArgAHost, LenA * sizeof(int), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(device_ArgB, ArgBHost, LenA * sizeof(int), cudaMemcpyHostToDevice);

	// Perform the multiplication 
	MultiplyVectorKernel<<<1, 100 >>> (device_ArgA, device_ArgB, device_Result);

	// wait for device to finish 
	cudaStatus = cudaDeviceSynchronize();

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(ReturnHost, device_Result, LenR * sizeof(int), cudaMemcpyDeviceToHost);

	// now build a long number to hold the result before returning 
	int ResultLen = sizeof(ReturnHost) / sizeof(int);
	LongNumber Result(ResultLen);
	for (int iCount = 0; iCount < ResultLen; iCount++)
	{
		Result.SetLongNumber(ReturnHost[iCount], iCount);
	}

	return Result; 

}


