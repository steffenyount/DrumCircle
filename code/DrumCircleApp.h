//--------------------------------------------------------------------
//	
//	DrumCircleApp.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _DrumCircleApp_h
#define _DrumCircleApp_h

#include <Application.h>
#include <File.h>
#include <MidiSynth.h>

class DrumCircleWindow;

//====================================================================
//	CLASS: DrumCircleApp

class DrumCircleApp : public BApplication
{
	//----------------------------------------------------------------
	//	Constructors, destructors, operators

public:
				DrumCircleApp();
	
	//----------------------------------------------------------------
	//	functions, & Virtual function overrides

public:
	void		AboutRequested(void);	
	void		ReadyToRun(void);
virtual void	RefsReceived(BMessage *message); 
	void 		MessageReceived(BMessage *message); 
	void 		addprof(BFile * file);

	
private:
	BMidiSynth * msynth;
	DrumCircleWindow * window;
	
};

#endif /* _DrumCircleApp_h */
