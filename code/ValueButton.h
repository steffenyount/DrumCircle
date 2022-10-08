//--------------------------------------------------------------------
//	
//	ValueButton.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _ValueButton_h
#define _ValueButton_h

#include <Button.h>
#include <View.h>

const uint32 MSG_VALUE_BUTTON 	= 'VBUT';
const uint32 MSG_VALUE_CHANGE	= 'VCNG';
const uint32 MSG_VALUE_DONE		= 'VDNE';


//====================================================================
//	CLASS: ValueButton

class ValueButton : public BView
{
	//----------------------------------------------------------------
	//	Constructors, destructors, operators

public:
	ValueButton(uint * variable, const char *name, const char *labelprefix,
		const char *labelsuffix, uint minval, uint maxval, const BRect & r);
 	virtual void 	Pulse(void);
 	virtual void 	AttachedToWindow(void);
	void			MessageReceived(BMessage *);
	void			Openslider(BPoint);
	BWindow	*		GetAppWindow(char *name);
			

	//----------------------------------------------------------------
	//	Member variables
	
protected:
	BButton	*		button;
	uint			pastvarval;
	uint *			currentvar;
	uint 			min;
	uint			max;
	const char *	prefix;
	const char *	suffix;

};

#endif /* _ValueButton_h */
