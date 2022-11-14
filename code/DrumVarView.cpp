//--------------------------------------------------------------------
//	
//	DrumVarView.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------


#include <Window.h>
#include <math.h>
#include <Rect.h>
#ifdef __HAIKU__
#include <strstream>
#elif __POWERPC__
#include "my_strstream.h"
#elif __INTEL__
#include <strstream.h>
#endif
#include <Picture.h>
#include <View.h>
#include <Button.h>
#include <Font.h>
#include <Slider.h>
#include <Application.h>

#include "globals.h"
#include "DrumVarView.h"
#include "DrumCircleData.h"
#include "DrumMap.h"
#include "DrumCircleMidi.h"

#ifdef __HAIKU__
using std::ostrstream;
#endif

//====================================================================
//	DrumVarView Implementation


//--------------------------------------------------------------------
//	DrumVarView constructors, destructors, operators

DrumVarView::DrumVarView(uint32 resizingMode)
	: BView(BRect(0, 0, 280, 280), "DrumCircle View", resizingMode,
		B_WILL_DRAW | B_PULSE_NEEDED)
{
	butt = NULL;
	butt2 = NULL;
//	SetViewColor(BKG_GREY);
}

DrumVarView::~DrumVarView()
{
	if (butt != NULL)
		delete butt;
	if (butt2 != NULL)
		delete butt2;
}

void DrumVarView::AttachedToWindow(void)
{
	BButton * b = new BButton(BRect(0,0,BUTTON_WIDTH-2,24), "temp", "", NULL);
//	b->SetViewColor(BKG_GREY);
	b->Hide();
	AddChild(b);
	b->BeginPicture(new BPicture());
	b->Draw(b->Bounds());
	butt = b->EndPicture();
	b->ResizeTo(BUTTON_WIDTH*3,24);
	b->BeginPicture(new BPicture());
	b->Draw(b->Bounds());
	butt2 = b->EndPicture();
	RemoveChild(b);
	delete b;
}

void DrumVarView::Pulse(void)
{
	static bool oldgroovin = groovin;
	static uint olddrumcount = data->Drumcount();

	if (oldgroovin != groovin || olddrumcount != data->Drumcount())
	{
		oldgroovin = groovin;
		olddrumcount = data->Drumcount();
		Window()->Lock();
		FixSize();
		Window()->Unlock();
		Invalidate();
	} 
	else
	{
		if (varschanged)
		{
			varschanged = false;
			Window()->Lock();
			Draw(Bounds());
			Window()->Unlock();
		}
	}
	
	
}

void DrumVarView::MouseDown(BPoint point)
{
	uint col = (uint)floor(point.x/BUTTON_WIDTH);
	uint col2 = col + 1;
	if (!groovin)
		col += 2;
	uint drum = (uint)floor(point.y/25) - 1;
	if (drum < data->Drumcount())
	{
		char * winname = NULL;
		ostrstream buf;
		switch (col)
		{
			case 0://hit%
				buf << Titles[col] << " #" << drum + 1 << '\0';
				winname = buf.str();
				Openslider(ConvertToScreen(BPoint(BUTTON_WIDTH + 20, 25*(drum + 1))), 
					winname, MSG_VALUE_HIT, drum, data->Hitlust(drum), 0, 100, 
					"0% (Low)", "100% (High)");
				break;
			case 1://Var%
				buf << Titles[col] << " #" << drum + 1 << '\0';
				winname = buf.str();
				Openslider(ConvertToScreen(BPoint(BUTTON_WIDTH*col2 + 20, 25*(drum + 1))), 
					winname, MSG_VALUE_VAR, drum, data->Changelust(drum), 0, 100, 
					"0% (Low)", "100% (High)");
				break;
			case 2://Chan
				buf << Titles[col] << " #" << drum + 1 << '\0';
				winname = buf.str();
				Openslider(ConvertToScreen(BPoint(BUTTON_WIDTH*col2 + 20, 25*(drum + 1))), 
					winname, MSG_VALUE_CHAN, drum, data->Channel(drum), 1, 16, 
					"1 (Low)", "16 (High)");
				break;
			case 3://Play
				beguse();
				data->On(drum) = !data->On(drum);
				enduse();
				Window()->Lock();
				Draw(BRect(BUTTON_WIDTH*(col2-1), 25*(drum+1), BUTTON_WIDTH*col2, 25*(drum+2)));
				Window()->Unlock();
				break;
			case 4://Vol
				buf << Titles[col] << " #" << drum + 1 << '\0';
				winname = buf.str();
				Openslider(ConvertToScreen(BPoint(BUTTON_WIDTH*col2 + 20, 25*(drum + 1))), 
					winname, MSG_VALUE_VOL, drum, data->Velocity(drum), 0, 127, 
					"0 (Low)", "127 (High)");				
				break;
			case 5://Note
				buf << Titles[col] << " #" << drum + 1 << '\0';
				winname = buf.str();
				Openslider(ConvertToScreen(BPoint(BUTTON_WIDTH*col2 + 20, 25*(drum + 1))), 
					winname, MSG_VALUE_NOTE, drum, (uint)floor(data->Duration(drum)*8), 1, 16, 
					 "0.125 (Low)", "2 (High)");				
				break;
			case 6:
			case 7:
			case 8: //Key / Instrument
				buf << Titles[6] << " #" << drum + 1 << '\0';
				winname = buf.str();
				if (groovin)
					col2 = 9;
				else
					col2 = 7;
				Openslider(ConvertToScreen(BPoint(BUTTON_WIDTH*col2 + 20, 25*(drum + 1))), 
					winname, MSG_VALUE_DRUM, drum, data->Drums(drum), 0, 127, 
					"0 (Low)", "127 (High)");							
				break;
			default:
				break; 
		}
		if (winname != NULL)
			delete winname;
	}
}

void DrumVarView::FixSize()
{
	beguse();
	if (Bounds().bottom != 25*(data->Drumcount() + 2))
	{
		if (groovin)
			ResizeTo(((BUTTON_WIDTH)*9), 25*(data->Drumcount() + 1));
		else
			ResizeTo(((BUTTON_WIDTH)*7), 25*(data->Drumcount() + 1));
	}
	MoveTo(varviewleft, varviewtop);
	drumviewleft = varviewleft + Bounds().right + 5;
	enduse();

}

void DrumVarView::Draw(BRect r)
{
	uint toprow = (uint)floor(r.top/25);
	uint botrow = (uint)floor((r.bottom-1)/25);
	uint leftcol = (uint)floor(r.left/BUTTON_WIDTH);
	uint rightcol = (uint)floor((r.right-1)/BUTTON_WIDTH);
	
		
	BPoint delta;	
	// write labels
	if (toprow == 0)
	{
		SetFont(be_bold_font);
		delta.y = 16;
		if (groovin)
		{
			for (uint k = leftcol; k <= rightcol && k < 6; k++)
			{
				delta.x = (BUTTON_WIDTH)*(k)+((BUTTON_WIDTH-2)/2-StringWidth(Titles[k])/2);
				DrawString(Titles[k], delta);
			}
			if (rightcol >= 6)
			{
				delta.x = (BUTTON_WIDTH)*(6)+((BUTTON_WIDTH*3-2)/2-StringWidth(Titles[6])/2);
				DrawString(Titles[6], delta); 
			}
		}
		else
		{
			for (uint k = leftcol; k <= rightcol && k < 4; k++)
			{
				delta.x = (BUTTON_WIDTH)*(k)+((BUTTON_WIDTH-2)/2-StringWidth(Titles[k+2])/2);
				DrawString(Titles[k+2], delta);
			}
			if (rightcol >= 4)
			{
				delta.x = (BUTTON_WIDTH)*(4)+((BUTTON_WIDTH*3-2)/2-StringWidth(Titles[6])/2);
				DrawString(Titles[6], delta); 
			}
		}
		SetFont(be_plain_font);
	}
		

	//draw buttons
	if (toprow == 0)
		toprow = 1;
	for(uint j = toprow; j <= botrow && j <= data->Drumcount(); j++)
	{
		if (groovin)
		{
			for (uint k = leftcol; k <= rightcol && k < 6; k++)
			{
				MovePenTo((BUTTON_WIDTH)*(k), 25*(j));
				DrawPicture(butt);
			}
			if (rightcol >= 6)
			{
				MovePenTo((BUTTON_WIDTH)*(6), 25*(j));
				DrawPicture(butt2);
			}
		}
		else
		{
			for (uint k = leftcol; k <= rightcol && k < 4; k++)
			{
				MovePenTo((BUTTON_WIDTH)*(k), 25*(j));
				DrawPicture(butt);
			}
			if (rightcol >= 4)
			{
				MovePenTo((BUTTON_WIDTH)*(4), 25*(j));
				DrawPicture(butt2);
			}
		}
	}
	Sync();

	
	// write buttons
	beguse();
	for(uint j = toprow; j <= botrow && j <= data->Drumcount(); j++)
	{
		delta.y = 25*(j)+16;
		if (groovin)
		{
			for (uint k = leftcol; k <= rightcol && k < 6; k++)
			{
				delta.x = (BUTTON_WIDTH)*(k)+((BUTTON_WIDTH-2)/2);
				WriteLabel(k, j-1, delta);
			}
			if (rightcol >= 6)
			{
				delta.x = (BUTTON_WIDTH)*(6)+((BUTTON_WIDTH*3-2)/2);
				WriteLabel(6, j-1, delta);
			}
		}
		else
		{
			for (uint k = leftcol; k <= rightcol && k < 4; k++)
			{
				delta.x = (BUTTON_WIDTH)*(k)+((BUTTON_WIDTH-2)/2);
				WriteLabel(k+2, j-1, delta);
			}
			if (rightcol >= 4)
			{
				delta.x = (BUTTON_WIDTH)*(4)+((BUTTON_WIDTH*3-2)/2);
				WriteLabel(6, j-1, delta); 
			}
		}
	}
	enduse();
}

void DrumVarView::WriteLabel(uint type, uint drum, BPoint & center)
{
	ostrstream * buf;
	char * string = NULL;
	switch (type)
	{
		case 0:
			// hitlust
			buf = new ostrstream();
			*buf << data->Hitlust(drum) << '\0';
			string = buf->str();
			delete buf;	
			break;
		case 1:
			// changelust
			buf = new ostrstream();
			*buf << data->Changelust(drum) << '\0';
			string = buf->str();
			delete buf;	
			break;
		case 2:
			// channel
			buf = new ostrstream();
			*buf << data->Channel(drum) << '\0';
			string = buf->str();
			delete buf;	
			break;
		case 3:
			//mute
			if (data->On(drum))
			{
				center.x = (center.x-StringWidth("On")/2);
				DrawString("On", center);
			} 
			else
			{
				center.x = (center.x-StringWidth("Off")/2);
				DrawString("Off", center); 
			}
			break;
		case 4:
			//volume
			buf = new ostrstream();
			*buf << data->Velocity(drum) << '\0';
			string = buf->str();
			delete buf;	
			break;
		case 5:
			//sustain
			buf = new ostrstream();
			*buf << data->Duration(drum) << '\0';
			string = buf->str();
			delete buf;				
			break;
		case 6:
			// note or Instrument
			center.x = (center.x-StringWidth(Instname(drum))/2);
			DrawString(Instname(drum), center); 
			break;
	}
	if (string != NULL)
	{
		center.x = (center.x-StringWidth(string)/2);
		DrawString(string, center); 
		delete string;		
	}
}


void DrumVarView::MessageReceived(BMessage *message) 
{
	uint drum = (uint)(message->what & MSG_NUMBER_MASK);
	BSlider	* slider;
	switch((message->what & MSG_ACTION_MASK)) 
	{
		case MSG_ACTION_CHANGE:
			beguse();
			if (drum < data->Drumcount())
			{
				message->FindPointer("source", (void**)&slider);
				uint i = 0;
				if (groovin)
					i = 2;

				switch((message->what & MSG_VALUE_MASK))
				{
					case MSG_VALUE_HIT:
						data->Hitlust(drum) = slider->Value();
						if (groovin)
						{
							Window()->Lock();
							Draw(BRect(0, 25*(drum+1), BUTTON_WIDTH, 25*(drum+2)));
							Window()->Unlock();
						}
						break;
						
					case MSG_VALUE_VAR:
						data->Changelust(drum) = slider->Value();
						if(groovin)
						{
							Window()->Lock();
							Draw(BRect(BUTTON_WIDTH, 25*(drum+1), 
								BUTTON_WIDTH*2, 25*(drum+2)));
							Window()->Unlock();
						}
						break;
						
					case MSG_VALUE_CHAN:
						data->Channel(drum) = slider->Value();
						Window()->Lock();
						Draw(BRect(BUTTON_WIDTH*i, 25*(drum+1), 
							BUTTON_WIDTH*(i+1), 25*(drum+2)));
						Window()->Unlock();
						break;
						
					case MSG_VALUE_VOL:
						data->Velocity(drum) = slider->Value();
						Window()->Lock();
						Draw(BRect(BUTTON_WIDTH*(i+2), 25*(drum+1), 
							BUTTON_WIDTH*(i+3), 25*(drum+2)));
						Window()->Unlock();
						break;
						
					case MSG_VALUE_NOTE:
						data->Duration(drum) = ((float)slider->Value())/8;
						Window()->Lock();
						Draw(BRect(BUTTON_WIDTH*(i+3), 25*(drum+1), 
							BUTTON_WIDTH*(i+4), 25*(drum+2)));
						Window()->Unlock();
						break;
						
					case MSG_VALUE_DRUM:
						data->Drums(drum) = slider->Value();
						Window()->Lock();
						Draw(BRect(BUTTON_WIDTH*(i+4), 25*(drum+1), 
							BUTTON_WIDTH*(i+7), 25*(drum+2)));
						Window()->Unlock();
						break;
				}
			}
			enduse();
			break;
		case MSG_ACTION_DONE:
			beguse();
			if (drum < data->Drumcount())
			{
				switch((message->what & MSG_VALUE_MASK))
				{
					case MSG_VALUE_DRUM:
					case MSG_VALUE_NOTE:
					case MSG_VALUE_VOL:
					case MSG_VALUE_CHAN:
						if (dcmidi!= NULL && !dcmidi->IsRunning())
							dcmidi->playit(drum);
						break;
				}
			}
			enduse();
			break;
		default:
			BView::MessageReceived(message);
			break;
	}
}

void DrumVarView::Openslider(BPoint here, const char * winname, uint32 msg_val,
		uint drum, uint val, uint low, uint high, const char * lowlabel, const char * highlabel)
{
	BRect		frame;
	BSlider		*slider;
	BWindow		*window;

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
		slider = new BSlider(frame, "", NULL, new BMessage(MSG_ACTION_DONE | 
								msg_val | (uint32)drum), low, high);
		//slider->SetViewColor(background_color);
		slider->SetValue(val);
		slider->SetModificationMessage(new BMessage(MSG_ACTION_CHANGE | msg_val | 
										(uint32)drum));
		slider->SetLimitLabels(lowlabel, highlabel);
		window->AddChild(slider);
		slider->SetTarget(this);
		window->Show();
	}
	//else
	//	cout << "window is not null!!\n";
	window->Activate();
	//cout << "done with openslider\n";
}

const char * DrumVarView::Instname(uint drum)
{
	if (data->Channel(drum) == 10)	
		return DrumMap[data->Drums(drum)];
	else
		return NoteMap[data->Drums(drum)];
}


/* loop through the window list of the application, looking for
   a window with a specified name. */
BWindow	*DrumVarView::GetAppWindow(const char *name)
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





