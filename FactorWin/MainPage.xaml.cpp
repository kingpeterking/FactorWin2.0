//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "LongNumber.h"
#include "LongNumberDivision.h"
#include "FactorQueue.h"
#include <atlstr.h>
#include <gdiplus.h>


#include <atomic>
#include <collection.h>


using namespace FactorWin;
using namespace Platform;

// System added before Windows for concurrency 
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::UI;

// using namespace System::Runtime::InteropServices;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409


// Global variables used to save passing lots of parameters and to aid concurrency 
// Create one, zero as long numbers - global scope so they don't get reinitiatlised for each thread
LongNumber One(1);
LongNumber Zero(1);

// Set the target as a global variable so we don't need to pass it around 
LongNumber LNTargetA;
LongNumber LNTargetB;
int ResultLen;
int ResultLenB;

// Set up the threading variables
int IterCount;
int ThreadCount; 
int MaxThread = 8;
int ThreadQueueRatio = 1000; 
int QueueBatchSizeValue = 100;

// Create the queues 
FactorQueue FNQueue;
FactorQueue SolvedQueue; 
FactorQueue PrimeQueue;

// Counter for number solved - global so doesn't need to be passed around 
int SolvedCount;

// declare the functions we will use later to build the nodes and solve the factor and update the screen
void CreateChidNodesQueue();




MainPage::MainPage()
{
	InitializeComponent();

	// Set up standard long numbers  - one, zero 
	One.SetLongNumber(1, 0);
	Zero.SetLongNumber(0, 0);

	ResultLen = 0;
	IterCount = 0;


}




void FactorWin::MainPage::FactorButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
	// Start the timer
	typedef std::chrono::high_resolution_clock Clock;
	auto StartTime = Clock::now();

	// Create the head of the Queue 
	FactorNode FNHeadofQueue(0, Zero, Zero, false);
	FNQueue.Push(FNHeadofQueue);
	IterCount = 0;
	ThreadCount = 0;

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	// Set up the progress bar
	FactorWin::MainPage::QueueProgress->Maximum = LNTargetA.GetLongNumberLength() * 2000; 
	FactorWin::MainPage::ThreadCountBar->Maximum = 10;

	Windows::Foundation::IAsyncActionWithProgress<int>^ ScanandDisp = FactorWin::MainPage::CreateChidNodesQueueAS();
	ScanandDisp->Progress =
		ref new Windows::Foundation::AsyncActionProgressHandler<int>
		([this](Windows::Foundation::IAsyncActionWithProgress<int>^, int progress)
	{
		
		// Update Queue info
		FactorWin::MainPage::QueueText->Text = progress.ToString();
		FactorWin::MainPage::QueueProgress->Value = progress;
		FactorWin::MainPage::IterCountText->Text = IterCount.ToString(); 
		FactorWin::MainPage::ThreadCountText->Text = ThreadCount.ToString();
		FactorWin::MainPage::ThreadCountBar->Value = ThreadCount; 

		// Update results as they become available
		while (SolvedQueue.ReturnQueueSize() > 0)
		{
			FactorNode FNItem = SolvedQueue.Pop();
			LongNumber AValuePassed = TrimLongNumber(FNItem.LNGetAValue());
			LongNumber BValuePassed = TrimLongNumber(FNItem.LNGetBValue());
			Platform::String ^ AValueStr = LongNumberChar(AValuePassed);
			Platform::String ^ BValueStr = LongNumberChar(BValuePassed);
			Platform::String ^ Display = "A : " + AValueStr + " B : " + BValueStr;
			FactorWin::MainPage::ResultsList->Items->Append(Display);

		}

	}
	);
	
	// Stop the timer
	auto EndTime = Clock::now();
	auto ElapsedTime = EndTime - StartTime; 
	FactorWin::MainPage::CalcTime.Text = ElapsedTime; 

}



void CreateChidNodesQueue()
{
	if (FNQueue.ReturnQueueSize() != 0)
	{
		for (int iQueueCount = 0; iQueueCount < QueueBatchSizeValue; iQueueCount++)
		{
			if (FNQueue.ReturnQueueSize() == 0) {break; } // stop if queue is empty
			
			// Add new elements to the queue or solve ones that are already there 
			//cout << "Create Children Nodes" << endl;

			// Get the next element from the queue 
			FactorNode FNItem = FNQueue.Pop();

			// Get the details from the FactorNode
			int Level = FNItem.LNGetLevel();
			// cout << "Level : " << Level << endl;
			LongNumber AValuePassed = FNItem.LNGetAValue();
			LongNumber BValuePassed = FNItem.LNGetBValue();
			// cout << " AValuePassed : " << AValuePassed << " BValuePassed : " << BValuePassed << endl;
			LongNumber MultResult(ResultLen * 2);									// Result of A * B 

			int iTarget = LNTargetA.GetValue(Level);
			int iRes = 0;

			for (int iCountA = 0; iCountA <= 9; iCountA++)
			{
				int BCountFrom = 0;
				if (Level == 0) { BCountFrom = iCountA; }
				for (int iCountB = BCountFrom; iCountB <= 9; iCountB++)
				{
					AValuePassed.SetLongNumber(iCountA, Level);
					BValuePassed.SetLongNumber(iCountB, Level);

					MultResult = LongNumberMultiply(AValuePassed, BValuePassed);
					// cout << " MultResult : " << MultResult << " AValuePassed : " << AValuePassed << " BValuePassed : " << BValuePassed << endl;
					iRes = MultResult.GetValue(Level);

					if (iRes == iTarget)
					{
						bool FactorComplete = false;
						int CompResult = LongNumberCompare(MultResult, LNTargetA);			// compares, returns 1 if >, 0 if equal and -1 if <
																							// cout << "Comp Result : " << CompResult << " MultResult : " << MultResult << " LNTarget : " << *LNTarget << endl;
						switch (CompResult)
						{
						case -1:			// less than
						{FactorNode FNAdd(
							Level + 1,			// LevelPassed
							AValuePassed,		// AValuePassed
							BValuePassed,		// BValuePassed
							false				// FactorCompletePassed
						);
						// PrintFactorNode(&FNAdd);
						FNQueue.Push(FNAdd);
						break;
						}
						case 0:				// solved
						{FactorComplete = true;
						FactorNode FNAdd(
							Level + 1,			// LevelPassed
							AValuePassed,		// AValuePassed
							BValuePassed,		// BValuePassed
							true				// FactorCompletePassed
						);
						SolvedQueue.Push(FNAdd);
						SolvedCount++;
						break;
						}
						case 1: // greater than so exit B loop
							goto ExitBLoop;
							break;
						default:
							break;
						} // end switch


					} // end if result

				} // end B loop
			ExitBLoop:;
				IterCount++;
			} // end A loop

		} // end QueueCount loop 

	} // end if queue

} // end function 



void FactorWin::MainPage::NoAInput_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	// first check for a number in NoA
	Platform::String^ NoAIn = NoAInput->Text;
	std::wstring NoAInW(NoAIn->Begin());
	std::string NoAInA(NoAInW.begin(), NoAInW.end());
	const char* NoACharStr = NoAInA.c_str();
	int LenA = NoAIn->Length();

	Messages->Text = "Checking Input\r\n";
	if (NoAIn->Length() == 0)
	{
		Messages->Text = Messages->Text + "No Input\r\n";
		return;
	}
	else
	{
		Messages->Text = Messages->Text + "Length of Arg : " + LenA + "\r\n";
	}

	LongNumber ArgPassed(NoACharStr);
	LNTargetA = ArgPassed;
	ResultLen = LNTargetA.GetLongNumberLength();
}



Windows::Foundation::IAsyncActionWithProgress<int>^ FactorWin::MainPage::CreateChidNodesQueueAS()
{
	
	return Concurrency::create_async([this](Concurrency::progress_reporter <int> reporter) -> void
		{

			int QueueSize = FNQueue.ReturnQueueSize();
			//returnStruct ReturnReporter; 
			//ReturnReporter.QueueCount = QueueSize; 
			//ReturnReporter.ThreadCount = 0;
			reporter.report(QueueSize);
			SolvedCount = 0;							// reset solved counter

			// kick off the threads that will consume the new queue items
			while (QueueSize > 0)
			{

				QueueSize = FNQueue.ReturnQueueSize();
				//ReturnReporter.QueueCount = QueueSize;
				reporter.report(QueueSize);
				ThreadCount = int(QueueSize / ThreadQueueRatio) + 1;
				if (ThreadCount >= MaxThread) { ThreadCount = MaxThread; }

				vector<thread> threads;

				for (int i = 0; i < ThreadCount; i++) {
					threads.push_back(thread(&CreateChidNodesQueue));
				}

				for (int i = 0; i < ThreadCount; i++) 
				{
					threads[i].join();
				}

			}

	}); // end of Async


}





void FactorWin::MainPage::NoBInput_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

	// first check for a number in NoA
	Platform::String^ NoBIn = NoBInput->Text;
	std::wstring NoBInW(NoBIn->Begin());
	std::string NoBInA(NoBInW.begin(), NoBInW.end());
	const char* NoBCharStr = NoBInA.c_str();
	int LenB = NoBIn->Length();

	Messages->Text = "Checking Input\r\n";
	if (NoBIn->Length() == 0)
	{
		Messages->Text = Messages->Text + "No Input\r\n";
		return;
	}
	else
	{
		Messages->Text = Messages->Text + "Length of Arg : " + LenB + "\r\n";
	}

	LongNumber ArgPassed(NoBCharStr);
	LNTargetB = ArgPassed;
	ResultLen = LNTargetB.GetLongNumberLength();

}


void FactorWin::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Add Button 

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	LongNumber LNResult = LongNumberAdd(LNTargetA, LNTargetB); 
	Platform::String ^ ResultStr = LongNumberChar(LNResult);
	Platform::String ^ Display = "Res : " + ResultStr; 
	FactorWin::MainPage::ResultsList->Items->Append(Display);
	Messages->Text = Display; 

}


void FactorWin::MainPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	// Multiply Button 

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	LongNumber LNResult = LongNumberMultiply(LNTargetA, LNTargetB);
	Platform::String ^ ResultStr = LongNumberChar(LNResult);
	Platform::String ^ Display = "Res : " + ResultStr;
	FactorWin::MainPage::ResultsList->Items->Append(Display);
	Messages->Text = Display;

}


void FactorWin::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	// Divide Button 

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	LongNumberDivision LNDResult = LongNumberDivide(LNTargetA, LNTargetB);
	Platform::String ^ ResultStrQ = LongNumberChar(LNDResult.GetQuotient());
	Platform::String ^ ResultStrR = LongNumberChar(LNDResult.GetRemainder());
	Platform::String ^ DisplayQ = "Quotient : " + ResultStrQ;
	FactorWin::MainPage::ResultsList->Items->Append(DisplayQ);
	Platform::String ^ DisplayR = "Remainder : " + ResultStrR;
	FactorWin::MainPage::ResultsList->Items->Append(DisplayR);
	Messages->Text = DisplayQ + " " + DisplayR;


}


void FactorWin::MainPage::Button_Click_3(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Subtract button 

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	LongNumber LNResult = LongNumberSubtract(LNTargetA, LNTargetB);
	Platform::String ^ ResultStr = LongNumberChar(LNResult);
	Platform::String ^ Display = "Res : " + ResultStr;
	FactorWin::MainPage::ResultsList->Items->Append(Display);
	Messages->Text = Display;

}


void FactorWin::MainPage::Button_Click_4(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// GCD Button

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();
	
	bool RemCheck = true; 
	LongNumber ArgA = LNTargetA; 
	LongNumber ArgB = LNTargetB;

	while (RemCheck)
	{
		LongNumberDivision LNDResult = LongNumberDivide(ArgA, ArgB);
		ArgA = ArgB;
		ArgB = LNDResult.GetRemainder();
		if(LongNumberCompare(ArgB, Zero) == 0)
		{
			RemCheck = false; 
		}

	}

	Platform::String ^ ResultStr = LongNumberChar(ArgA);
	Platform::String ^ Display = "GCD : " + ResultStr;
	FactorWin::MainPage::ResultsList->Items->Append(Display);
	Messages->Text = Display;

}


void FactorWin::MainPage::Button_Click_5(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	// Calculates the nth Fibonacci number - n = LNTargetA

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();
	
	LongNumber ArgA = LNTargetA;
	LongNumber Result = Zero;

	if (LongNumberCompare(ArgA, Zero) == 0)
	{
		Result = Zero;
	}
	else
	{

		if (LongNumberCompare(ArgA, One) == 0)
		{
			Result = One;
		}
		else
		{
			LongNumber LastResult = One;
			LongNumber LastLastResult = Zero;
			bool StopLoop = true;

			while (StopLoop)
			{
				Result = LongNumberAdd(LastResult, LastLastResult);
				LastLastResult = LastResult;
				LastResult = Result;

				ArgA = LongNumberSubtract(ArgA, One);

				if (LongNumberCompare(ArgA, One) == 0)
				{
					StopLoop = false;
				}

			}	// end while

		}  // endif

	} // end if

	Platform::String ^ ResultStr = LongNumberChar(Result);
	Platform::String ^ Display = "Fib : " + ResultStr;
	FactorWin::MainPage::ResultsList->Items->Append(Display);
	Messages->Text = Display;

}


void FactorWin::MainPage::Button_Click_6(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Prime factor button 

	// calculate the prime factors of A

	// Go through the solved queue
	// ignore the unit x A solutions 
	// take the next entry 


	// Create the head of the Queue 
	FactorNode FNHeadofQueue(0, Zero, Zero, false);
	FNQueue.Push(FNHeadofQueue);

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	// Set up the progress bar
	FactorWin::MainPage::QueueProgress->Maximum = LNTargetA.GetLongNumberLength() * 2000;

	Windows::Foundation::IAsyncActionWithProgress<int>^ ScanandDisp = FactorWin::MainPage::CreateChidNodesQueueAS();
	ScanandDisp->Progress =
		ref new Windows::Foundation::AsyncActionProgressHandler<int>
		([this](Windows::Foundation::IAsyncActionWithProgress<int>^, int progress)
		{

			// Update Queue info
			FactorWin::MainPage::QueueText->Text = progress.ToString();
			FactorWin::MainPage::QueueProgress->Value = progress;

			// Update results as they become available
			while (SolvedQueue.ReturnQueueSize() > 0)
			{
				FactorNode FNItem = SolvedQueue.Pop();
				LongNumber AValuePassed = TrimLongNumber(FNItem.LNGetAValue());
				LongNumber BValuePassed = TrimLongNumber(FNItem.LNGetBValue());
				if ((LongNumberCompare(AValuePassed, LNTargetA) == 0) && (LongNumberCompare(BValuePassed, One) == 0))
				{
					// Ignore A = LNTarget and B = 1 
				} 
				else if ((LongNumberCompare(BValuePassed, LNTargetA) == 0) && (LongNumberCompare(AValuePassed, One) == 0))
				{
					// Ignore B = LNTargte and A = 1 
				}
				else
				{
					// Next prime factor
					FactorNode PrimeAdd;
					PrimeAdd.SetAValue(AValuePassed);
					PrimeAdd.SetBValue(BValuePassed);
					if ((LongNumberCompare(AValuePassed, One) == 0) || (LongNumberCompare(BValuePassed, One) == 0))
					{
						PrimeAdd.SetFactorComplete(true);			// set to true if either A or B are 1 here having passed first check 
					}
					else
					{
						PrimeAdd.SetFactorComplete(false);			// set to false if neither A or B are 1 here having passed first check 
					}
					PrimeQueue.Push(PrimeAdd);


				}

				
			}

		}
	); 


	// Get all of the entries in the PrimeQueue
	while (PrimeQueue.ReturnQueueSize() > 0)
	{
		FactorNode PrimeAdd = PrimeQueue.Pop();
		Platform::String ^ FactorComplete; 

		if (PrimeAdd.GetFactorComplete())
		{
			FactorComplete = "T";
		}
		else
		{
			FactorComplete = "F";
		}
		Platform::String ^ AValueStr = LongNumberChar(PrimeAdd.LNGetAValue());
		Platform::String ^ BValueStr = LongNumberChar(PrimeAdd.LNGetBValue());
		Platform::String ^ Display = "A : " + AValueStr + " B : " + BValueStr + " Factor Complete : " + FactorComplete;
		FactorWin::MainPage::ResultsList->Items->Append(Display);

	}



}





void FactorWin::MainPage::MaxThreads_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ MT = FactorWin::MainPage::MaxThreads->Text;
	std::wstring MTW(MT->Begin());
	std::string MTA(MTW.begin(), MTW.end());
	const char* MTS = MTA.c_str();
	 
	MaxThread = atoi(MTS);

}


void FactorWin::MainPage::ThreadRatio_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ MT = FactorWin::MainPage::ThreadRatio->Text;
	std::wstring MTW(MT->Begin());
	std::string MTA(MTW.begin(), MTW.end());
	const char* MTS = MTA.c_str();

	ThreadQueueRatio = atoi(MTS);

}


void FactorWin::MainPage::QueueBatchSize_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	Platform::String^ MT = FactorWin::MainPage::QueueBatchSize->Text;
	std::wstring MTW(MT->Begin());
	std::string MTA(MTW.begin(), MTW.end());
	const char* MTS = MTA.c_str();

	QueueBatchSizeValue = atoi(MTS);
}

