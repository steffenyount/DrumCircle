//--------------------------------------------------------------------
//	
//	DrumVarView.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _DrumVarView_h
#define _DrumVarView_h



//====================================================================
//	CLASS: DrumVarView

class DrumVarView : public BView
{
	//----------------------------------------------------------------
	//	Constructors, destructors, operators

public:
	DrumVarView(uint32 resizingMode);
	~DrumVarView();
	
	void 			Draw(BRect);
	void			WriteLabel(uint type, uint drum, BPoint & center);
	void 			FixSize();
	virtual void 	AttachedToWindow(void);
 	virtual void 	Pulse(void);
 	virtual void 	MouseDown(BPoint point);
	void			MessageReceived(BMessage *);	
	void			Openslider(BPoint here, char * winname, uint32 msg_val, 
		uint drum, uint val, uint low, uint high, char * lowlabel, 
		char * highlabel);
	const char *	Instname(uint drum);
	BWindow *		GetAppWindow(char *name);
			

	//----------------------------------------------------------------
	//	Member variables
	
private:
	BPicture *		butt;
	BPicture *		butt2;
		

};

const uint BUTTON_WIDTH = 33;

const uint32 MSG_ACTION_MASK	= 0xffff0000;
const uint32 MSG_ACTION_CHANGE	= 'VC\0\0';
const uint32 MSG_ACTION_DONE	= 'VD\0\0';

const uint32 MSG_VALUE_MASK		= 0x0000ff00;
const uint32 MSG_VALUE_HIT		= '\0\0H\0';
const uint32 MSG_VALUE_VAR		= '\0\0X\0';
const uint32 MSG_VALUE_CHAN		= '\0\0C\0';
const uint32 MSG_VALUE_VOL		= '\0\0V\0';
const uint32 MSG_VALUE_NOTE		= '\0\0N\0';
const uint32 MSG_VALUE_DRUM		= '\0\0D\0';

const uint32 MSG_NUMBER_MASK	= 0x000000ff;


const char *const Titles[] = {"Hit%", "Var%", "Chan", "Play", "Vol", "Note", "Key/Instrument"};

BBitmap *FetchBitmap(char *filename);

#endif /* _DrumVarView_h */