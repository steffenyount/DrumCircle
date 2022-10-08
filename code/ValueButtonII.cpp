//--------------------------------------------------------------------
//	
//	ValueButtonII.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include "ValueButtonII.h"
#include <Slider.h>
#include <Rect.h>
#include <Window.h>
#include <Application.h>
#include <View.h>
#include "globals.h"

//====================================================================
//	ValueButtonII Implementation

ValueButtonII::ValueButtonII(uint * variable, const char *name, 
		const char *labelprefix, const char *labelsuffix, 
		uint minval, uint maxval, const BRect & r): 
		ValueButton(variable, name, labelprefix, labelsuffix,
		minval, maxval, r)
{

}


//--------------------------------------------------------------------
//	ValueButton virtual function overrides


void ValueButtonII::MessageReceived(BMessage *message) 
{
	BSlider	* slider;
	switch(message->what) {
		case MSG_VALUE_BUTTON:
			//cout << "MSG_VALUE_BUTTON recvd\n";
			Openslider(ConvertToScreen(
				BPoint(Bounds().right+20, Bounds().top)));
			break;
		case MSG_VALUE_CHANGE:
			//cout << "MSG_VALUE_CHANGE recvd\n";
			message->FindPointer("source", (void**)&slider);
			beguse();
			*currentvar = slider->Value();
			enduse();
			break;		
		case MSG_VALUE_DONE:
			message->FindPointer("source", (void**)&slider);
			beguse();
			*currentvar = slider->Value();
			enduse();
			resizedata(); 
			break;
		default:
			//cout << "MSG went through!!\n";
			BView::MessageReceived(message);
			break;
	}
}

