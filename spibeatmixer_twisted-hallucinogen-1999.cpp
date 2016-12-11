////////////////////////////////////////////////////////////////
//nakedsoftware.org, spi@oifii.org or stephane.poirier@oifii.org
//
//
//2013oct12, creation for mixing beats
//
//
//nakedsoftware.org, spi@oifii.org or stephane.poirier@oifii.org
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <fstream>
#include <vector>

#include <iostream>
#include <sstream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"

#define BUFF_SIZE	2048


#include <ctime>
#include "spiws_WavSet.h"
#include "spiws_Instrument.h"
#include "spiws_InstrumentSet.h"

#include "spiws_partition.h"
#include "spiws_partitionset.h"

#include "spiws_WavSet.h"

#include <assert.h>
#include <windows.h>



// Select sample format
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif


//The event signaled when the app should be terminated.
HANDLE g_hTerminateEvent = NULL;
//Handles events that would normally terminate a console application. 
BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType);

int Terminate();

InstrumentSet* pInstrumentSet0=NULL;
InstrumentSet* pInstrumentSet1=NULL;
InstrumentSet* pInstrumentSet2=NULL;
InstrumentSet* pInstrumentSet3=NULL;
InstrumentSet* pInstrumentSet4=NULL;
InstrumentSet* pInstrumentSet5=NULL;
InstrumentSet* pInstrumentSet6=NULL;
InstrumentSet* pInstrumentSet7=NULL;
InstrumentSet* pInstrumentSet8=NULL;

//////////////////////////////////////////
//main
//////////////////////////////////////////
int main(int argc, char *argv[]);
int main(int argc, char *argv[])
{
    //Auto-reset, initially non-signaled event 
    g_hTerminateEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    //Add the break handler
    ::SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    PaStreamParameters outputParameters;
    PaStream* stream;
    PaError err;

	////////////////////////
	// initialize port audio 
	////////////////////////
    err = Pa_Initialize();
    if( err != paNoError )
	{
		fprintf(stderr,"Error: Initialization failed.\n");
		Pa_Terminate();
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}

	outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device 
	if (outputParameters.device == paNoDevice) 
	{
		fprintf(stderr,"Error: No default output device.\n");
		Pa_Terminate();
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}
	outputParameters.channelCount = 2;//pWavSet->numChannels;
	outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;


	//////////////////////////
	//initialize random number
	//////////////////////////
	srand((unsigned)time(0));



	/////////////////////////////
	//loop n sinusoidal samples
	/////////////////////////////
	WavSet* pTempWavSet = new WavSet;
	pTempWavSet->CreateSin(0.3333, 44100, 2, 440.0, 0.5f);
	WavSet* pSilentWavSet = new WavSet;
	pSilentWavSet->CreateSilence(5);
	pSilentWavSet->LoopSample(pTempWavSet, 5, -1.0, 0.0); //from second 0, loop sample during 5 seconds
	pSilentWavSet->Play(&outputParameters);
	if(pTempWavSet)
	{
		delete pTempWavSet;
		pTempWavSet = NULL;
	}
	if(pSilentWavSet)
	{
		delete pSilentWavSet;
		pSilentWavSet = NULL;
	}
	

	

	//////////////////////////////////////////////////////////////////////////
	//populate InstrumentSet according to input folder (folder of sound files)
	//////////////////////////////////////////////////////////////////////////
	string wavfolder_voice = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\bookaudio_wav\\audio_audio-books_ekhart-tolle_the-power-of-now_chapter1-track-no01";
	//string wavfolder2 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_minimal-deep-electro-house-techno\\dj-oifii_minimal-deep-electro-house-techno(beat-offset-8min08sec)";
	//string wavfolder2 = "D:\\oifii-org\\httpdocs\\ns-org\\nsd\\ar\\cp\\audio_wave";

	//string wavfolder = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_minimal-deep-electro-house-techno";
	//string wavfolder = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_gouryella";
	//string wavfolder = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_ibiza";
	string wavfolder1 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\01 - LSD";
	string wavfolder2 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\02 - Orphic Thrench";
	string wavfolder3 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\03 - Alpha Centauri";
	string wavfolder4 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\04 - Dark Magus";
	string wavfolder5 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\05 - Shamanix";
	string wavfolder6 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\06 - Snarling Black Mabel";
	string wavfolder7 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\07 - Fluoro Neuro Sponge";
	string wavfolder8 = "D:\\oifii-org\\httpdocs\\ha-org\\had\\dj-oifii\\raveaudio_wav\\dj-oifii_twisted\\Hallucinogen - 1999\\08 - Solstice";

	/*
	InstrumentSet* pInstrumentSet0=new InstrumentSet;
	InstrumentSet* pInstrumentSet1=new InstrumentSet;
	InstrumentSet* pInstrumentSet2=new InstrumentSet;
	InstrumentSet* pInstrumentSet3=new InstrumentSet;
	InstrumentSet* pInstrumentSet4=new InstrumentSet;
	InstrumentSet* pInstrumentSet5=new InstrumentSet;
	InstrumentSet* pInstrumentSet6=new InstrumentSet;
	InstrumentSet* pInstrumentSet7=new InstrumentSet;
	InstrumentSet* pInstrumentSet8=new InstrumentSet;
	*/
	pInstrumentSet0=new InstrumentSet;
	pInstrumentSet1=new InstrumentSet;
	pInstrumentSet2=new InstrumentSet;
	pInstrumentSet3=new InstrumentSet;
	pInstrumentSet4=new InstrumentSet;
	pInstrumentSet5=new InstrumentSet;
	pInstrumentSet6=new InstrumentSet;
	pInstrumentSet7=new InstrumentSet;
	pInstrumentSet8=new InstrumentSet;

	InstrumentSet* pInstrumentSetArray[9];
	//if(pInstrumentSet!=NULL && pInstrumentSet2!=NULL && pInstrumentSet3!=NULL)
	bool bFailed=false;
	int iFailed=-1;
	if(pInstrumentSet0!=NULL && !bFailed)
	{
		pInstrumentSet0->Populate(wavfolder_voice.c_str(), 0); //0, for no subfolders
	}
	else
	{
		bFailed=true;
		iFailed=0;
	}
	if(pInstrumentSet1!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder1" << endl;
		pInstrumentSet1->Populate(wavfolder1.c_str(), 0); //0, for no subfolders
	}
	else
	{
		bFailed=true;
		iFailed=1;
	}
	if(pInstrumentSet2!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder2" << endl;
		pInstrumentSet2->Populate(wavfolder2.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=2;
	}
	if(pInstrumentSet3!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder3" << endl;
		pInstrumentSet3->Populate(wavfolder3.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=3;
	}
	if(pInstrumentSet4!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder4" << endl;
		pInstrumentSet4->Populate(wavfolder4.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=4;
	}
	if(pInstrumentSet5!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder5" << endl;
		pInstrumentSet5->Populate(wavfolder5.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=5;
	}
	if(pInstrumentSet6!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder6" << endl;
		pInstrumentSet6->Populate(wavfolder6.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=6;
	}
	if(pInstrumentSet7!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder7" << endl;
		pInstrumentSet7->Populate(wavfolder7.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=7;
	}
	if(pInstrumentSet8!=NULL && !bFailed)
	{
		cout << "polulate: wavfolder8" << endl;
		pInstrumentSet8->Populate(wavfolder8.c_str());
	}
	else
	{
		bFailed=true;
		iFailed=8;
	}
	
	if(bFailed)
	{
		assert(false);
		cout << "exiting, instrumentset could not be allocated" << endl;
		if(pInstrumentSet0!=NULL) delete pInstrumentSet0;
		if(pInstrumentSet1!=NULL) delete pInstrumentSet1;
		if(pInstrumentSet2!=NULL) delete pInstrumentSet2;
		if(pInstrumentSet3!=NULL) delete pInstrumentSet3;
		if(pInstrumentSet4!=NULL) delete pInstrumentSet4;
		if(pInstrumentSet5!=NULL) delete pInstrumentSet5;
		if(pInstrumentSet6!=NULL) delete pInstrumentSet6;
		if(pInstrumentSet7!=NULL) delete pInstrumentSet7;
		if(pInstrumentSet8!=NULL) delete pInstrumentSet8;
		Pa_Terminate();
		return -1;
	}
	else
	{
		pInstrumentSetArray[0] = pInstrumentSet0;
		pInstrumentSetArray[1] = pInstrumentSet1;
		pInstrumentSetArray[2] = pInstrumentSet2;
		pInstrumentSetArray[3] = pInstrumentSet3;
		pInstrumentSetArray[4] = pInstrumentSet4;
		pInstrumentSetArray[5] = pInstrumentSet5;
		pInstrumentSetArray[6] = pInstrumentSet6;
		pInstrumentSetArray[7] = pInstrumentSet7;
		pInstrumentSetArray[8] = pInstrumentSet8;
	}

	
	for(int i=0; i<9; i++)
	{
		assert(pInstrumentSetArray[i]->HasOneInstrument());
	}
	
		

	////////////////////////////////////////////////
	//for each instrumentset, pick random instrument
	//then for the random instrument, pick random wavset
	//loop wavset for loopduration_s seconds and play
	/////////////////////////////////////////////////
	Instrument* pAnInstrument = NULL;
	WavSet* pAWavSet = NULL;
	for(int i=0; i<10; i++) //repeat 10 times
	{
		//for(int ii=1; ii<6; ii++)
		for(int ii=1; ii<9; ii++)
		{
			pAnInstrument = pInstrumentSetArray[ii]->GetInstrumentRandomly();
			assert(pAnInstrument);
			cout << "instrument name: " << pAnInstrument->instrumentname << endl;
			pAWavSet = pAnInstrument->GetWavSetRandomly();
			cout << "sound filename: " << pAWavSet->GetName() << endl;
			assert(pAWavSet);
			//pAWavSet->Play(&outputParameters);


			float loopduration_s = 1 * pAWavSet->GetWavSetLength() + 0.050f; //0.050 sec to ensure loopduration_s is larger than sample
			//WavSet* pSilentWavSet = new WavSet;
			pSilentWavSet = new WavSet;
			pSilentWavSet->CreateSilence(loopduration_s); 
			pSilentWavSet->LoopSample(pAWavSet, loopduration_s, -1.0, 0.0); //from second 0, loop sample during loopduration_s seconds
			//pSilentWavSet->Play(&outputParameters);

			/////////////////////////////////////
			//spread another smaller-scale sample
			/////////////////////////////////////
			Instrument* pAnInstrument0 = pInstrumentSet0->GetInstrumentRandomly();
			if(pAnInstrument0)
			{
				/*
				WavSet* pAWavSet0 = pAnInstrument0->GetWavSetRandomly();
				if(pAWavSet0)
				{
					//pSilentWavSet->SpreadSample("1111110011101100", pAWavSet2, loopduration_s, 0.5, 0.0); //pattern over loopduration_s sec, sample duration 0.5sec and with pattern offset of 0sec;// pAWavSet2
					pSilentWavSet->SpreadSample("1011010010101000", pAWavSet0, loopduration_s, 5, 0.0); //pattern over loopduration_s sec, sample duration 5sec and with pattern offset of 0sec;// pAWavSet2
				}
				*/
				pSilentWavSet->SpreadSamples("1011010010101000", pAnInstrument0, loopduration_s, 5, 0.0); //pattern over loopduration_s sec, sample duration 5sec and with pattern offset of 0sec;// pAWavSet2
			}
			pSilentWavSet->Play(&outputParameters);
				
			//pSilentWavSet->Erase();
			//pSilentWavSet->Play(&outputParameters);
			if(pSilentWavSet)
			{
				delete pSilentWavSet;
				pSilentWavSet = NULL;
			}

		}
	}

	/*
	/////////////////////
	//terminate portaudio
	/////////////////////
	Pa_Terminate();
	//if(pInstrumentSet) delete pInstrumentSet;
	printf("Exiting!\n"); fflush(stdout);
	*/
	Terminate();
	return 0;
}

int Terminate()
{
	/////////////////////
	//terminate portaudio
	/////////////////////
	Pa_Terminate();

	//delete all memory allocations
	if(pInstrumentSet0!=NULL) delete pInstrumentSet0;
	if(pInstrumentSet1!=NULL) delete pInstrumentSet1;
	if(pInstrumentSet2!=NULL) delete pInstrumentSet2;
	if(pInstrumentSet3!=NULL) delete pInstrumentSet3;
	if(pInstrumentSet4!=NULL) delete pInstrumentSet4;
	if(pInstrumentSet5!=NULL) delete pInstrumentSet5;
	if(pInstrumentSet6!=NULL) delete pInstrumentSet6;
	if(pInstrumentSet7!=NULL) delete pInstrumentSet7;
	if(pInstrumentSet8!=NULL) delete pInstrumentSet8;

	printf("Exiting!\n"); fflush(stdout);
	return 0;
}

//Called by the operating system in a separate thread to handle an app-terminating event. 
BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType)
{
    if (dwCtrlType == CTRL_C_EVENT ||
        dwCtrlType == CTRL_BREAK_EVENT ||
        dwCtrlType == CTRL_CLOSE_EVENT)
    {
        // CTRL_C_EVENT - Ctrl+C was pressed 
        // CTRL_BREAK_EVENT - Ctrl+Break was pressed 
        // CTRL_CLOSE_EVENT - Console window was closed 
		Terminate();
        // Tell the main thread to exit the app 
        ::SetEvent(g_hTerminateEvent);
        return TRUE;
    }

    //Not an event handled by this function.
    //The only events that should be able to
	//reach this line of code are events that
    //should only be sent to services. 
    return FALSE;
}
