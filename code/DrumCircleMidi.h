//--------------------------------------------------------------------
//	
//	DrumCircleMidi.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _DrumCircleMidi_h
#define _DrumCircleMidi_h

#include <midi/Midi.h>


//====================================================================
//	CLASS: DrumCircleMidi

class DrumCircleMidi:  public BMidi
{
public:
	void				Run();
	void				playit(uint drum);
	
	// Thread_func must be global or static
	static int32	regroove(void* foo);
};

class offlist;

class offevent
{
friend class offlist;
public:
	offevent(uint c_chan, uint c_note, uint c_vel, uint32 c_when);
	uint chan;
	uint note;
	uint vel;
	uint32 when;
private:
	offevent * next;	

};

class offlist
{
public:
	offlist();
	~offlist();
	uint32 nexttime();
	offevent * getnext();
	void addevent(offevent *);
private:
	offevent * nextevent;
};
	
	



#endif /* _DrumCircleMidi_h */
