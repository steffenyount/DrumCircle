//--------------------------------------------------------------------
//	
//	DrumCircleView.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _DrumCircleView_h
#define _DrumCircleView_h


//====================================================================
//	CLASS: DrumCircleView

class DrumCircleView : public BView
{
	//----------------------------------------------------------------
	//	Constructors, destructors, operators

public:
	DrumCircleView(uint32 resizingMode);
	~DrumCircleView();
	
	void 			Draw(BRect);
 	virtual void 	Pulse(void);
 	virtual void 	MouseDown(BPoint point);
	void			DrawChasers();
	void			DrawData();
	void			FixSize();
			

	//----------------------------------------------------------------
	//	Member variables
	
private:
	BBitmap * 	lgdot;
	BBitmap *	dgdot;
	BBitmap *	lrdot;
	BBitmap *	drdot;	


};

//BBitmap *FetchBitmap(char *filename);

#endif /* _DrumCircleView_h */