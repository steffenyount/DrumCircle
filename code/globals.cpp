//--------------------------------------------------------------------
//	
//	globals.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include <Locker.h>
#include <iostream.h>
#include "globals.h"
#include "DrumCircleMidi.h"
#include "DrumCircleData.h"

extern const char* STR_APP_SIG
	= "application/x-vnd.SY.DrumCircle";
	
extern const char* STR_PROFILE_SIG
	= "application/x-vnd.SY.DrumCircle-Profile";
	
extern const char* STR_SETTING_SIG
	= "application/x-vnd.SY.DrumCircle-Setting";
	
#if defined(LOCALE_USA)

//--------------------------------------------------------------------
//	American English Strings


extern const char* STR_APP_NAME
	= "DrumCircle";

extern const char* STR_SEPARATOR
	= "--------------------";

extern const char* STR_PLAY
	= "Play that funky beat!!!";
	
extern const char* STR_MNU_FILE
	= "File";
	
extern const char* STR_MNU_FILE_ABOUT
	= "About...";

extern const char* STR_MNU_FILE_CLOSE
	= "Close";
	
extern const char* STR_PROF_ACTIVE
	= "Make Active";
	
extern const char* STR_PROF_ADD
	= "Duplicate";

extern const char* STR_PROF_OPEN
	= "Open Profile";

extern const char* STR_PROF_RENAME
	= "Rename";

extern const char* STR_PROF_SAVE
	= "Save Profile";

extern const char* STR_PROF_REMOVE
	= "Remove";


extern const char* STR_ABOUT_TITLE
	= "About DrumCircle";
	
extern const char* STR_ABOUT_BUTTON
	= "Great!!!";
	
extern const char* STR_ABOUT_DESC
	= "DrumCircle 1.0 is a simple midi sequencer with a\n"
	"knack for simulating Hippe Drum Circles\n"
	"Tweek the Tweekables and have fun!\n\n"
	"Drum Circle was written by Steffen Yount\n";
	
#endif /* LOCALE_USA */


//--------------------------------------------------------------------
//	Shortcuts

extern const char CMD_FILE_CLOSE
	= 'W';
	
extern const char CMD_TEST_ICON_SIZE
	= 'I';

//--------------------------------------------------------------------
//	Colors

extern const rgb_color BKG_GREY		= { 216, 216, 216, 0 };

	
//====================================================================
//	Global var's init & function's implementaion

extern const uint	MAX_HPM		= 1000;
uint				hpb			= 4;
uint				bpm 		= 120;
uint				measurepos 	= 0;
bool				groovin 	= true;
bool				datachanged	= true;
bool				varschanged	= true;
DrumCircleMidi *	dcmidi		= NULL;
DrumCircleData *	data 		= new DrumCircleData;

DrumCircleData * *	data_list	= new DrumCircleData *[1];
/*
	The line above orignially looked like the one below this comment.
	
	The original version creates an array of pointers (with one item) and 
	inits that item to "data". In the version above, I just create the array.
	
	The metrowerks compiler doesn't want to intitialize data_list[0] when in 
	the global scope, but will happily do so inside a function. I have placed
	the initializer "data_list[0] = data;" as the first line of the main() function,
	(before the BApplication is created) instead. It seems to work...
	
	Will Dyson
	will@cs.earlham.edu
*/

// DrumCircleData * *	data_list	= new DrumCircleData *[1] = {data};			

uint				d_list_size	= 1;
uint				d_list_curr = 0;
uint				d_list_isrt	= 0;
float 				minbottom	= 0;
float				varviewleft	= 0;
float				varviewtop	= 0;
float				drumviewleft= 0;
float				drumviewtop	= 0;


uint				newmeasurelen = data->Measurelen();
uint				newdrumcount = data->Drumcount();

int32				users 		= 0;
BLocker				mylock;
int32				usersII		= 0;
BLocker				mylockII;


// locking functions for the current data pointer
extern void beguse()
{
	if (atomic_add(&users, 1) == 0)
	{
		mylock.Lock();
	}
}

extern void enduse()
{
	if (atomic_add(&users, -1) == 1)
	{
		mylock.Unlock();
	}
}
extern void begchange()
{
	mylock.Lock();
}
extern void endchange()
{
	mylock.Unlock();
}

// locking functions for the datalist

extern void dl_beguse()
{
	if (atomic_add(&usersII, 1) == 0)
	{
		mylockII.Lock();
	}
}

extern void dl_enduse()
{
	if (atomic_add(&usersII, -1) == 1)
	{
		mylockII.Unlock();
	}
}
extern void dl_begchange()
{
	mylockII.Lock();
}
extern void dl_endchange()
{
	mylockII.Unlock();
}

extern	void resizedata()
{
	if (newmeasurelen<1 || newdrumcount<1)
		return;
	DrumCircleData * newdat = new DrumCircleData(newmeasurelen, newdrumcount);
	beguse();
	newdat->Setto(*data);
	enduse();
	begchange();
	delete data;
	data = newdat;
	endchange();
	datachanged = true;		
	dl_begchange();
	data_list[d_list_curr] = data;
	dl_endchange();
}

