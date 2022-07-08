//--------------------------------------------------------------------
//	
//	globals.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _globals_h
#define _globals_h


#include <GraphicsDefs.h>

class DrumCircleData;
class DrumCircleMidi;


//--------------------------------------------------------------------
//	Message IDs

const uint32 MSG_PLAY_BUTTON 		= 'play';
const uint32 MSG_HIPPIE_AI			= 'h_ai';
const uint32 MSG_RESET				= 'rset';

const uint32 MSG_PROFILE_SELECTED 	= 'prof';
const uint32 MSG_PROFILE_ADD	 	= 'padd';
const uint32 MSG_PROFILE_LOAD	 	= 'plod';
const uint32 MSG_PROFILE_RENAME 	= 'pren';
const uint32 MSG_PROFILE_SAVE	 	= 'psav';
const uint32 MSG_PROFILE_REMOVE 	= 'prem';

const uint32 MSG_RENAME_TO			= 'pnam';







//--------------------------------------------------------------------
//	Strings

extern const char* STR_APP_SIG;
extern const char* STR_PROFILE_SIG;
extern const char* STR_SETTING_SIG;


// Simple localization scheme
#define LOCALE_USA

extern const char* STR_APP_NAME;
extern const char* STR_SEPARATOR;
extern const char* STR_PLAY;
extern const char* STR_MNU_FILE;
extern const char* STR_MNU_FILE_ABOUT;
extern const char* STR_MNU_FILE_CLOSE;

extern const char* STR_PROF_ACTIVE;
extern const char* STR_PROF_ADD;
extern const char* STR_PROF_OPEN;
extern const char* STR_PROF_RENAME;
extern const char* STR_PROF_SAVE;
extern const char* STR_PROF_REMOVE;

extern const char* STR_ABOUT_TITLE;
extern const char* STR_ABOUT_BUTTON;
extern const char* STR_ABOUT_DESC;


//--------------------------------------------------------------------
//	Shortcuts

extern const char CMD_FILE_CLOSE;
extern const char CMD_TEST_ICON_SIZE;

//--------------------------------------------------------------------
//	Colors

extern const rgb_color BKG_GREY;


//----------------------------------------------------------------
//	Global variables & synchronizing functions for data
	
extern 	void 				beguse();
extern 	void 				enduse();
extern 	void 				begchange();
extern 	void 				endchange();

extern 	void 				dl_beguse();
extern 	void 				dl_enduse();
extern 	void 				dl_begchange();
extern 	void 				dl_endchange();


extern	uint				newmeasurelen;
extern	uint				newdrumcount;
extern	void				resizedata();

extern	const uint			MAX_HPM;	
extern	uint				hpb;
extern 	uint				bpm;
extern	uint				measurepos;
extern	bool				groovin;
extern	bool				datachanged;
extern	bool				varschanged;
extern	DrumCircleMidi *	dcmidi;
extern	DrumCircleData * 	data;
extern	DrumCircleData * *	data_list;
extern	uint				d_list_size;
extern	uint				d_list_curr;
extern	uint				d_list_isrt;
extern 	float				minbottom;
extern	float				varviewleft;
extern	float				varviewtop;
extern	float				drumviewleft;
extern	float				drumviewtop;





#endif /* _globals_h */