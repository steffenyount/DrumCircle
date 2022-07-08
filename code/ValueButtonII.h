//--------------------------------------------------------------------
//	
//	ValueButtonII.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _ValueButtonII_h
#define _ValueButtonII_h

#include "ValueButton.h"


//====================================================================
//	CLASS: ValueButtonII

class ValueButtonII : public ValueButton
{
	//----------------------------------------------------------------
	//	Constructors, destructors, operators

public:
	ValueButtonII(uint * variable, const char *name, const char *labelprefix,
		const char *labelsuffix, uint minval, uint maxval, const BRect & r);

	void			MessageReceived(BMessage *);
				

	//----------------------------------------------------------------
	//	Member variables
	
private:
};

#endif /* _ValueButtonII_h */