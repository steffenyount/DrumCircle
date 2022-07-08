//--------------------------------------------------------------------
//	
//	DrumCircleWindow.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _DrumCircleWindow_h
#define _DrumCircleWindow_h

class DrumCircleView;
class DrumVarView;
class BWindow;

//====================================================================
//	CLASS: DrumCircleWindow

class DrumCircleWindow : public BWindow
{
	//----------------------------------------------------------------
	//	Constructors, destructors, operators

public:
					DrumCircleWindow(const char* name);
				
				
	//----------------------------------------------------------------
	//	Virtual member function overrides

public:	
	bool			QuitRequested(void);
	void			MessageReceived(BMessage *);
	//----------------------------------------------------------------
	//	Member functions

public:
	BMenu * 		getoptionmenu(const char * name);
	
private:
	BWindow	*		GetAppWindow(char *name);
	
	
	//----------------------------------------------------------------
	//	Member variables
	
public:
	BMenuBar* 			m_pMenuBar;
	DrumCircleView*		m_pDrumCircleView;	
	DrumVarView*		m_pDrumVarView;
	BMenuBar*			m_pProfileOpt;
	BView*				bgview;
};


#endif /* _DrumCircleWindow_h */