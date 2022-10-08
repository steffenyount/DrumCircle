//--------------------------------------------------------------------
//	
//	ValueButton.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include "ValueButton.h"
#ifdef __HAIKU__
#include <strstream>
#elif __POWERPC__
#include "my_strstream.h"
#elif __INTEL__
#include <strstream.h>
#endif
#include <Slider.h>
#include <Rect.h>
#include <Window.h>
#include <Button.h>
#include <View.h>
#include <Point.h>
#include <Application.h>

#ifdef __HAIKU__
using std::ostrstream;
#endif

//====================================================================
//	ValueButton Implementation



//--------------------------------------------------------------------
//	ValueButton constructors, destructors, operators

ValueButton::ValueButton(uint * variable, const char *name, 
		const char *labelprefix, const char *labelsuffix, 
		uint minval, uint maxval, const BRect & r): 
		BView(r, name, B_FOLLOW_LEFT | B_FOLLOW_TOP, 
		B_WILL_DRAW | B_NAVIGABLE | B_PULSE_NEEDED)
{
	button = new BButton(Bounds(), name, "", new BMessage(MSG_VALUE_BUTTON),
        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	currentvar = variable;
	pastvarval = *currentvar;
	min = minval;
	if (maxval < min)
		maxval = min;
	else 
		max = maxval;
	prefix = labelprefix;
	suffix = labelsuffix;
	ostrstream buf;
	buf << prefix << pastvarval << suffix << '\0';
	button->SetLabel(buf.str());
	if (r == BRect(0,0,0,0))
		button->ResizeToPreferred();
	ResizeTo(button->Bounds().right, button->Bounds().bottom);
	AddChild(button);
}

void ValueButton::Openslider(BPoint here)
{
	BRect		frame;
	BSlider		*slider;
	BWindow		*window;
	char * 		winname;
	ostrstream buf;
	buf << prefix << "ChangeValue" << suffix << '\0';
	winname = buf.str();
	window = GetAppWindow(winname);
	if (window == NULL) {
		frame.Set(here.x, here.y, here.x + 159, here.y + 33);
		window = new BWindow(frame, winname,
							 B_FLOATING_WINDOW_LOOK,
							 B_FLOATING_APP_WINDOW_FEEL,
							 B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_WILL_ACCEPT_FIRST_CLICK);
		window->SetSizeLimits(frame.Width(), frame.Width(),
									frame.Height(), frame.Height());
		window->SetZoomLimits(frame.Width(), frame.Height());
		frame.OffsetTo(0.0, 0.0);
		slider = new BSlider(frame, "", NULL, new BMessage(MSG_VALUE_DONE),
							 min, max);
		//slider->SetViewColor(background_color);
		slider->SetValue(pastvarval);
		slider->SetModificationMessage(new BMessage(MSG_VALUE_CHANGE));
		ostrstream low, high;
		char * l;
		char * h;
		low << prefix << min << suffix << " (low)" << '\0';
		high << prefix << max << suffix << " (high)" << '\0';
		slider->SetLimitLabels(l = low.str(), h = high.str());
		delete l;
		delete h;
		window->AddChild(slider);
		slider->SetTarget(this);
		window->Show();
	}
	//else
	//	cout << "window is not null!!\n";
	window->Activate();
	//cout << "done with openslider\n";
}

/* loop through the window list of the application, looking for
   a window with a specified name. */
BWindow	*ValueButton::GetAppWindow(char *name)
{
	int32		index;
	BWindow		*window;
	
	for (index = 0;; index++) {
		window = be_app->WindowAt(index);
		if (window == NULL)
			break;
		if (window->LockWithTimeout(200000) == B_OK) {
			if (strcmp(window->Name(), name) == 0) {
				window->Unlock();
				break;
			}
			window->Unlock();
		}
	}
	return window; 
}


//--------------------------------------------------------------------
//	ValueButton virtual function overrides

void 	ValueButton::Pulse(void)
{
	if (pastvarval != *currentvar)
	{
		pastvarval = *currentvar;
		ostrstream buf;
		char * it;
		buf << prefix << pastvarval << suffix << '\0';		
		button->SetLabel(it = buf.str());
		delete it;
		//button->ResizeToPreferred();
		ResizeTo(button->Bounds().right, button->Bounds().bottom);
	}
}

void ValueButton::AttachedToWindow()
{
	button->SetTarget(this);
}


void ValueButton::MessageReceived(BMessage *message) 
{
	BSlider	* slider;
	switch(message->what) {
		case MSG_VALUE_BUTTON:
			//cout << "MSG_VALUE_BUTTON recvd\n";
			Openslider(ConvertToScreen(
				BPoint(Bounds().right+20, Bounds().top)));
			break;
		case MSG_VALUE_CHANGE:
		case MSG_VALUE_DONE:
			//cout << "MSG_VALUE_CHANGE recvd\n";
			message->FindPointer("source", (void**)&slider);
			*currentvar = slider->Value();
			break;		
		default:
			//cout << "MSG went through!!\n";
			BView::MessageReceived(message);
			break;
	}
}

