
//--------------------------------------------------------------------
//	
//	DrumCircleView.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------


#include <File.h>
#include <TranslationUtils.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <BitmapStream.h>
#include <Application.h>
#include <math.h>

#include "globals.h"
#include "DrumCircleView.h"
#include "DrumCircleData.h"
#include "DrumCircleMidi.h"
#include "stddlg.h"


//====================================================================
//	DrumCircleView Implementation


//--------------------------------------------------------------------
//	DrumCircleView constructors, destructors, operators

DrumCircleView::DrumCircleView(uint32 resizingMode)
	: BView(BRect(0, 0, 100, 100), "DrumCircle View", resizingMode,
		B_WILL_DRAW | B_PULSE_NEEDED)
{
	lgdot = BTranslationUtils::GetBitmapFile("Stuff/lightgreendot");
	dgdot = BTranslationUtils::GetBitmapFile("Stuff/darkgreendot");
	lrdot = BTranslationUtils::GetBitmapFile("Stuff/lightreddot");
	drdot = BTranslationUtils::GetBitmapFile("Stuff/darkreddot");
	if (lgdot == NULL || dgdot == NULL || lrdot == NULL || drdot == NULL)
	{
		ierror("unable to load graphics\n");
		be_app->PostMessage(B_QUIT_REQUESTED);
	}	
	else
	{ 
		SetDrawingMode(B_OP_COPY);
		SetViewColor(BKG_GREY);
	}
	
}

DrumCircleView::~DrumCircleView()
{
	delete lgdot;
	delete dgdot;
	delete lrdot;
	delete drdot;
}

void DrumCircleView::Pulse(void)
{
	static uint oldpos = 0;
	static uint oldhpb = hpb;
	static float olddrumviewleft = drumviewleft;

	if (oldhpb != hpb)
	{
		//bpm = (bpm * oldhpb) / hpb;
		oldhpb = hpb;
		Invalidate();
	}
	else 
	{
		if (datachanged)
		{
			datachanged = false;
			Window()->Lock();
			DrawData();
			FixSize();
			Window()->Unlock();
		}
		
		if (olddrumviewleft != drumviewleft)
		{
			olddrumviewleft = drumviewleft;
			Window()->Lock();
			FixSize();
			Window()->Unlock();
		}

		if (oldpos != measurepos)
		{
			oldpos = measurepos;
			Window()->Lock();
			DrawChasers();
			Window()->Unlock();
		}
	}
}

void DrumCircleView::DrawChasers()
{
	beguse();
	for (uint i = 0; i < data->Measurelen(); i++)
	{
		MovePenTo(BPoint((2*i+(i/hpb))*10, 0));
		if (measurepos != i)
			DrawBitmap(dgdot);
		else
			DrawBitmap(lgdot);
	}

	enduse();
}

void DrumCircleView::DrawData()
{
	beguse();
	for(uint i = 0; i < data->Drumcount(); i++)
	{
		for(uint j = 0; j < data->Measurelen(); j++)
		{
			MovePenTo(BPoint((2*j+(j/hpb))*10, (i+1)*25));
			if(data->Rythm(i, j))
				DrawBitmap(lrdot);
			else
				DrawBitmap(drdot); 
		}	
	}
	enduse();
}

void DrumCircleView::MouseDown(BPoint point)
{
	uint m_pos = (uint)floor((point.x-(10*floor(point.x/(20*hpb+10))))/20);
	uint drum = (uint)floor(point.y/25) - 1;
	beguse();
	if (drum < data->Drumcount() && m_pos < data->Measurelen())
	{
		if (data->Rythm(drum, m_pos))
			data->Rythm(drum, m_pos) = false;
		else
		{
			data->Rythm(drum, m_pos) = true;
			if (dcmidi!= NULL && !dcmidi->IsRunning())
				dcmidi->playit(drum);
		}
	}
//	else
//		cout << "Got big problems with Mousedown read!!!\n";
	enduse();
	datachanged = true;
	
}

void DrumCircleView::FixSize()
{
	beguse();
	if (Bounds().right != 10*(2*data->Measurelen()+(data->Measurelen()/hpb)) || 
			Bounds().bottom != 25*(data->Drumcount() + 1))
		ResizeTo(10*(2*data->Measurelen()+(data->Measurelen()/hpb)), 
			25*(data->Drumcount() + 1));
	enduse();
	
	MoveTo(drumviewleft, drumviewtop);
	
	if (Window() != NULL)
	{
		if (Window()->Bounds().right != Window()->ConvertFromScreen(ConvertToScreen(Bounds())).right ||
				Window()->Bounds().bottom != Window()->ConvertFromScreen(ConvertToScreen(Bounds())).bottom)
		{
			//cout << "told to resize" << Frame().bottom << " " << minbottom << "\n";
			if (Window()->ConvertFromScreen(ConvertToScreen(Bounds())).bottom > minbottom)
			{ 
				Window()->ResizeTo(Window()->ConvertFromScreen(ConvertToScreen(Bounds())).right, 
						Window()->ConvertFromScreen(ConvertToScreen(Bounds())).bottom);
			}
			else
			{
				Window()->ResizeTo(Window()->ConvertFromScreen(ConvertToScreen(Bounds())).right, minbottom);
			}
		}
	}
//	else
//		cout << "Window is Null!!!\n";
}

void DrumCircleView::Draw(BRect)
{
	DrawData();
	DrawChasers();
	FixSize();
}



