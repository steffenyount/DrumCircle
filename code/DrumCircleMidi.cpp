//--------------------------------------------------------------------
//	
//	DrumCircleMidi.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------


#include "DrumCircleMidi.h"
#include "globals.h"
#include <stdlib.h>
#include <View.h>
#include <OS.h>
#include <iostream.h>
#include "DrumCircleData.h"

//====================================================================
//	offlist Implementation
offevent::offevent(uint c_chan, uint c_note, uint c_vel, uint32 c_when)
{
chan = c_chan;
note = c_note;
vel = c_vel;
when = c_when;
next = NULL;
}

offlist::offlist()
{
	nextevent = NULL;
}
	
offlist::~offlist()
{
	offevent * mark = nextevent;
	while (mark != NULL)
	{
		mark = nextevent->next;
		delete nextevent;
		nextevent = mark;
	}
	nextevent = NULL;
}

uint32 offlist::nexttime()
{
	if (nextevent != NULL)
		return nextevent->when;
	else
		return B_NOW;
}

offevent * offlist::getnext()
{
	if (nextevent != NULL)
	{
		offevent * mark = nextevent;
		nextevent = nextevent->next;
		return mark;
	}
	else
		return NULL;
}

void offlist::addevent(offevent *evnt)
{
	while(nextevent != NULL && nextevent->note == evnt->note && 
			nextevent->chan == evnt->chan)
	{
		offevent * tmp = nextevent;
		nextevent = nextevent->next;
		delete tmp;				
	}

	if (nextevent == NULL)
	{
		nextevent = evnt;
//		if (evnt->next != NULL)
//			cout << "evnt != NULL" << endl;
	}
	else
	{

		if (nextevent->when >= evnt->when)
		{
			// add before first
			evnt->next = nextevent;
			nextevent = evnt;
		}
		else
		{
			offevent * mark = nextevent;
			// find last less than
			while (mark->next != NULL && (mark->next->when < evnt->when))
			{
				if (mark->next->note == evnt->note && 
						mark->next->chan == evnt->chan)
				{
					offevent * tmp = mark->next;
					mark->next = mark->next->next;
					if (mark->next != NULL && (mark->next->when < evnt->when))
						mark = mark->next;
					delete tmp;
				} 
				else
					mark = mark->next;
			}
			// add after that spot
			evnt->next = mark->next;
			mark->next = evnt;
		}
	}
	
}





//====================================================================
//	DrumCircleMidi Implementation


void DrumCircleMidi::Run()
{
	uint32 tnext = B_NOW;
	srand(tnext);
	offlist ol;
	offevent * off;
	static uint i = 0;

	beguse();

	while(KeepRunning())
	{
		// change up
		if (!i && groovin)
		{		// make it groovey
			thread_id g = spawn_thread(DrumCircleMidi::regroove,
								"Regroover",
								B_LOW_PRIORITY,
								(void*) NULL); // arg not used
			resume_thread(g);
		}
			
		tnext += 60000/(bpm*hpb);	
		
		// take what's availible from sorted list	
		// getnext doesn't get called unless nextime <= tnext
		while (ol.nexttime() <= tnext && (off = ol.getnext()) != NULL)
		{
			SprayNoteOff(off->chan, off->note, off->vel, off->when);
			delete off;
		}
		
		for (uint j = 0; j < data->Drumcount(); j++)
		{
			if (data->Rythm(j, i) && data->On(j))
			{
				uint tempchan = data->Channel(j);
				uint tempdrums = data->Drums(j);
				SprayNoteOff(tempchan, tempdrums, data->Velocity(j), tnext-1);
				SprayNoteOn(tempchan, tempdrums, data->Velocity(j), tnext);
				// add to sorted list
				ol.addevent(new offevent(tempchan, tempdrums, data->Velocity(j), 
					(tnext + ((uint32)((60000*data->Duration(j))/(float)bpm)))));
			}
		}
		enduse(); // don't hold during snooze
		SnoozeUntil(tnext);
		measurepos = i;
		beguse();
		i = (i+1)%(data->Measurelen());
	}
	enduse();		
	while((off = ol.getnext()) !=  NULL)
	{
		SprayNoteOff(off->chan, off->note, off->vel, off->when);
		//delete off;		
	}
}

void DrumCircleMidi::playit(uint drum)
{
	beguse();
	if (drum < data->Drumcount())
	{
		uint tempchan = data->Channel(drum);
		uint tempdrums = data->Drums(drum);

		SprayNoteOn(tempchan, tempdrums, data->Velocity(drum), B_NOW);
		SprayNoteOff(tempchan, tempdrums, data->Velocity(drum), 
			(B_NOW + ((uint32)(60000*(data->Duration(drum)))/(bpm))));
	}
	enduse(); 
}
			
int32 DrumCircleMidi::regroove(void* foo)
{
	beguse();
	for (uint k = 0; k < data->Drumcount(); k++)
	{
		if ((uint)(rand() % 100) < data->Changelust(k))
			for (uint m = 0; m < data->Measurelen(); m++)
			{
				if ((uint)(rand() % 100) < data->Hitlust(k))
				{
					if (data->Rythm(k, m) != true)
						data->Rythm(k, m) = true;
				}
				else
				{
					if (data->Rythm(k, m) != false)
						data->Rythm(k, m) = false;
				}
			} 
	}
	enduse();
	
	datachanged = true;
	
	return B_OK;
}


	
