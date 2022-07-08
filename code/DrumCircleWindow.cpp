//--------------------------------------------------------------------
//	
//	DrumCircleWindow.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include <Application.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <File.h>
#include <TranslationUtils.h>
#include <iostream.h>
#include <PictureButton.h>
#include <Bitmap.h>
#include <Button.h>
#include <CheckBox.h>
#include <ScrollView.h>
#include <Roster.h>
#include <Window.h>
#if __POWERPC__
#include "my_strstream.h"
#endif
#if __INTEL__
#include <strstream.h>
#endif
#include <Path.h>
#include <FilePanel.h>
#include <Entry.h>
#include <TextControl.h>
#include <NodeInfo.h>

#include "globals.h"
#include "DrumCircleWindow.h"
#include "DrumCircleData.h"
#include "DrumCircleView.h"
#include "DrumCircleMidi.h"
#include "ValueButton.h"
#include "ValueButtonII.h"
#include "DrumVarView.h"

//====================================================================
//	DrumCircleWindow Implementation

#define MAX_TEST_STATUS_CHARS 25


BMenu * DrumCircleWindow::getoptionmenu(const char * name)
{
	BMenu * tmp;
	tmp = new BMenu(name);
	tmp->AddItem(new BMenuItem(STR_PROF_ACTIVE, new BMessage(MSG_PROFILE_SELECTED)));	
	tmp->AddItem(new BMenuItem(STR_PROF_ADD, new BMessage(MSG_PROFILE_ADD)));
	tmp->AddItem(new BMenuItem(STR_PROF_OPEN, new BMessage(MSG_PROFILE_LOAD)));
	tmp->AddItem(new BMenuItem(STR_PROF_RENAME, new BMessage(MSG_PROFILE_RENAME)));
	tmp->AddItem(new BMenuItem(STR_PROF_SAVE, new BMessage(MSG_PROFILE_SAVE)));
	tmp->AddItem(new BMenuItem(STR_PROF_REMOVE, new BMessage(MSG_PROFILE_REMOVE)));
	return tmp;
}

//--------------------------------------------------------------------
//	DrumCircleWindow constructors, destructors, operators

DrumCircleWindow::DrumCircleWindow(const char* name)
	: BWindow(BRect(30,30,100,600), name, B_TITLED_WINDOW,
		/*B_NOT_RESIZABLE |*/ B_NOT_ZOOMABLE | B_WILL_DRAW)
{


		
	// menu bars
	BRect dummyFrame(0, 0, 0, 0);
	m_pMenuBar = new BMenuBar(dummyFrame, "Menu Bar");
	
	// File menu
	BMenu* pMenu = new BMenu(STR_MNU_FILE);
	
	BMenuItem* pAboutItem = new BMenuItem(STR_MNU_FILE_ABOUT,
		new BMessage(B_ABOUT_REQUESTED));
	pAboutItem->SetTarget(NULL, be_app);
	pMenu->AddItem(pAboutItem);
	
	pMenu->AddSeparatorItem();
	
	pMenu->AddItem(new BMenuItem(STR_MNU_FILE_CLOSE,
		new BMessage(B_QUIT_REQUESTED), CMD_FILE_CLOSE));

	m_pMenuBar->AddItem(pMenu);
	 
	// add child after menus are added so its initially
	// calculated app_server bounds take added menus into
	// account
	AddChild(m_pMenuBar);
	
	float menuHeight = m_pMenuBar->Bounds().Height();	
	BButton *tempView = new BButton(BRect(0,0,47,47), "temp", "", NULL);
	tempView->Hide();
	
	AddChild(tempView);
	
	//create on picture
	BPicture *on;
   	tempView->BeginPicture(new BPicture); 
	tempView->SetLabel("Stop");
	tempView->ResizeToPreferred();
	tempView->Draw(tempView->Bounds());
   	on = tempView->EndPicture();
   	//create off picture
   	BPicture *off;
   	tempView->BeginPicture(new BPicture); 
	tempView->SetLabel("Play");
	tempView->ResizeToPreferred();
	tempView->Draw(tempView->Bounds());	
   	off = tempView->EndPicture();
   	//get rid of tempview
   	RemoveChild(tempView);

	//addbackgroundview
	
	bgview = new BView(Bounds(), "backgroundview", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	bgview->MoveBy(0, menuHeight);
	bgview->SetViewColor(BKG_GREY);

	//create a picture button using the two pictures

	BPictureButton* plst_Button = new BPictureButton(tempView->Bounds(), 
		"picture", off, on, new BMessage(MSG_PLAY_BUTTON), B_TWO_STATE_BUTTON);
	plst_Button->MoveBy(5, 6); 
	bgview->AddChild(plst_Button);
   	delete tempView;

	
	//add reset button
	BButton * rst = new BButton(BRect(0,0,0,0), "Reset", "Reset", new BMessage(MSG_RESET));
	rst->ResizeToPreferred();
	rst->MoveBy(plst_Button->Frame().left, plst_Button->Frame().bottom + 1);
	bgview->AddChild(rst);
	
	// add check box to switch AI on and off
	BCheckBox * h_AI = new BCheckBox(BRect(0,0,0,0), "Hippie_AI", "Hippie AI",
		 new BMessage(MSG_HIPPIE_AI));
	h_AI->ResizeToPreferred();
	h_AI->SetValue(B_CONTROL_ON);
	h_AI->MoveBy(rst->Frame().left, rst->Frame().bottom + 26);
	bgview->AddChild(h_AI);
	
	// add BPM button
	ValueButton * vb = new ValueButton(&bpm, "ValueButton", " ", " BPM", 5, 300, rst->Bounds());
	vb->MoveBy(h_AI->Frame().left, h_AI->Frame().bottom + 26);
	vb->SetViewColor(BKG_GREY);
	bgview->AddChild(vb);

	// add Hits button
	ValueButtonII * tk = new ValueButtonII(&newmeasurelen, "ValueButton", " ", " Hits", 1, 64, rst->Bounds());
	tk->MoveBy(vb->Frame().left, vb->Frame().bottom + 1);
	tk->SetViewColor(BKG_GREY);
	bgview->AddChild(tk);
	
	// add HPB button
	ValueButton * hb = new ValueButton(&hpb, "ValueButton", " ", " HPB", 1, 8, rst->Bounds());
	hb->MoveBy(tk->Frame().left, tk->Frame().bottom + 1);
	hb->SetViewColor(BKG_GREY);
	bgview->AddChild(hb);		
	
	// add Drums button
	ValueButtonII * dr = new ValueButtonII(&newdrumcount, "ValueButton", "", " Key/Ins", 1, 32, rst->Bounds());
	dr->MoveBy(hb->Frame().left, hb->Frame().bottom + 1);
	dr->SetViewColor(BKG_GREY);
	bgview->AddChild(dr);
	
	//add profiles menu
	m_pProfileOpt = new BMenuBar(rst->Bounds()/*BRect(dr->Frame().left, dr->Frame().bottom + 3, dr->Frame().right, dr->Bounds().bottom + dr->Frame().bottom + 1)*/,
						"Profile Menu", B_FOLLOW_NONE, B_ITEMS_IN_COLUMN, true);
	m_pProfileOpt->SetBorder(B_BORDER_CONTENTS);
	m_pProfileOpt->SetMaxContentWidth(rst->Bounds().right + 10);
	BMenuItem * mi = NULL;
	dl_beguse();
	for (uint i = 0; i < d_list_size; i++)
	{
		m_pProfileOpt->AddItem(mi = new BMenuItem(getoptionmenu(data_list[i]->Name())));
	} 
	dl_enduse();
	if (mi)
		mi->SetMarked(true);
	m_pProfileOpt->SetRadioMode(true);
//	m_pProfileOpt->SetViewColor(BKG_GREY);
	m_pProfileOpt->MoveTo(dr->Frame().left, dr->Frame().bottom + 3);
	bgview->AddChild(m_pProfileOpt);
	
	minbottom = m_pProfileOpt->Frame().bottom + 3 + bgview->Frame().top;
	varviewleft = m_pProfileOpt->Frame().right + 5 + 20;
	varviewtop = 6;
	
	//add varView frame
	m_pDrumVarView = new DrumVarView(B_FOLLOW_NONE);
	m_pDrumVarView->MoveBy(varviewleft, varviewtop);
	m_pDrumVarView->FixSize();
	m_pDrumVarView->SetViewColor(BKG_GREY);
	bgview->AddChild(m_pDrumVarView);
	
	drumviewleft = m_pDrumVarView->Frame().right + 5;
	drumviewtop = 8;

	
	//add rest of the view
	
	m_pDrumCircleView = new DrumCircleView(B_FOLLOW_NONE); 

	m_pDrumCircleView->MoveBy(drumviewleft, drumviewtop);

	bgview->AddChild(m_pDrumCircleView);		
	
	AddChild(bgview);	
	m_pDrumCircleView->FixSize();
	

	SetPulseRate(200000);
}


bool DrumCircleWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void DrumCircleWindow::MessageReceived(BMessage *message) 
{
	BMenuItem * bmi;
	int32 i = 0;
	ostrstream buf;


	switch(message->what) {
		case MSG_PLAY_BUTTON:
			BPictureButton * pb;
			message->FindPointer("source", (void **)&pb);
			if (pb->Value())
			{
				if (!dcmidi->IsRunning())
					dcmidi->Start();
			}
			else
			{
				if (dcmidi->IsRunning())
					dcmidi->Stop();
			}
			break;
		case MSG_HIPPIE_AI:
			BCheckBox * cb;
			message->FindPointer("source", (void **)&cb);
			if(cb->Value())
			{
				if(!groovin)
					groovin = true;
			}
			else
			{
				if(groovin)
					groovin = false;
			}
			break;
		case MSG_RESET:
			beguse();
			for (uint k = 0; k < data->Drumcount(); k++)
				for (uint j = 0; j < data->Measurelen(); j++)
					data->Rythm(k,j) = false;
			enduse();
			datachanged = true;
			break;
		case MSG_PROFILE_SELECTED:
			message->FindPointer("source", (void **)&bmi);
			if (!bmi->Menu()->Superitem()->IsMarked())
			{
				bmi->Menu()->Superitem()->SetMarked(true);
				i = bmi->Menu()->Supermenu()->IndexOf(bmi->Menu()->Superitem());
				dl_beguse();
				if ( i != B_ERROR && i < (int32)d_list_size)
				{
					begchange();
					data = data_list[i];
					d_list_curr = (uint)i;
					endchange();
				}
				dl_enduse();
				datachanged = true;
				varschanged = true;
			}
			Lock();
			bmi->Menu()->Supermenu()->Draw(bmi->Menu()->Supermenu()->Bounds());
			Unlock();
			beguse();
			newmeasurelen = data->Measurelen();
			newdrumcount = data->Drumcount();
			enduse();
//			cout << "MSG_PROFILE_SELECTED--end\n";
			break;
		case MSG_PROFILE_ADD:
			message->FindPointer("source", (void **)&bmi);
			i = bmi->Menu()->Supermenu()->IndexOf(bmi->Menu()->Superitem());
			dl_begchange();
			if ( i != B_ERROR && i < (int32)d_list_size)
			{
				d_list_size++;
				DrumCircleData * * tmp = data_list;
				data_list = new DrumCircleData *[d_list_size];
				for (int32 j = 0; j <= i; j++)
				{
					data_list[j] = tmp[j];	
				}
				if (newmeasurelen > 0 || newdrumcount > 0)
				{
					data_list[i+1] = new DrumCircleData(newmeasurelen, newdrumcount);
				}
				else
				{
					data_list[i+1] = new DrumCircleData(16, 16);
				}
				for (uint j = (uint)i+2; j < d_list_size; j++)
				{
					data_list[j] = tmp[j-1];
				}
				if ((int32)d_list_curr > i)
					d_list_curr++;
				delete [] tmp; 
			}
			dl_endchange();
			if ( i != B_ERROR && i < (int32)d_list_size)
			{
				if (i >= 0)
				{
					data_list[i+1]->Setto(*data_list[i]);
					buf << data_list[i+1]->Name() << "(c)" << '\0';
					char * tstr = buf.str();
					data_list[i+1]->Rename(tstr);
					delete [] tstr;
				}
				bmi->Menu()->Supermenu()->AddItem(new BMenuItem(getoptionmenu(data_list[i+1]->Name())), i+1);
			}	
			bmi->Menu()->Supermenu()->Draw(bmi->Menu()->Supermenu()->Bounds());
			minbottom = bmi->Menu()->Supermenu()->Frame().bottom + 3 + bgview->Frame().top;
			varviewleft = bmi->Menu()->Supermenu()->Frame().right + 5;
			m_pDrumVarView->FixSize();
			m_pDrumCircleView->FixSize();
			if (Bounds().bottom < minbottom)
			{
				ResizeTo(Bounds().right, minbottom);
			}
//			cout << "MSG_PROFILE_ADD\n";
			break;
		case MSG_PROFILE_LOAD:
			message->FindPointer("source", (void **)&bmi);
			i = bmi->Menu()->Supermenu()->IndexOf(bmi->Menu()->Superitem());
			dl_beguse();
			if (i < (int32)d_list_size)
			{
				d_list_isrt = (uint)i;
				entry_ref * ref = new entry_ref;
				app_info * appinf = new app_info;
				BEntry * ent;
				BDirectory * dir;
				if ( be_app->GetAppInfo(appinf) == B_OK )
				{
					BEntry * ent2 = new BEntry(&appinf->ref);
					dir = new BDirectory();
					ent2->GetParent(dir);
					delete ent2;
					ent = new BEntry(dir, "Profiles", true);
					delete dir;
				}
				else
				{
					ent = new BEntry("Profiles", true);
				}	
				delete appinf;
				ent->GetRef(ref);
				delete ent;
				BFilePanel * fp = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), ref, B_FILE_NODE);
				fp->Show();
			}
			dl_enduse();
//			cout << "MSG_PROFILE_LOAD\n";
			break;
		case B_REFS_RECEIVED:
		{
//			cout << "window got refs recv'd\n";
			BFilePanel * fp;
			message->FindPointer("source", (void **)&fp);
			if (fp!= NULL)
				delete fp;
//			else
//				cout << "fp == null\n";
			be_app->RefsReceived(message);
//			cout << "B_REFS_RECEIVED\n";
			break;
		}
		case MSG_PROFILE_RENAME:
			BWindow	*window;
			BTextControl * tc;
			char * winname; 
			message->FindPointer("source", (void **)&bmi);
			i = bmi->Menu()->Supermenu()->IndexOf(bmi->Menu()->Superitem());
			if (i >= (int32)d_list_size)
				break;
			buf.put( ((char)i) + '0');
			buf << "# Rename" << '\0';
			winname = buf.str();
			window = GetAppWindow((char*)data_list[i]->Name());
			if (window != NULL)
			{
				window->Lock();
				window->Quit(); 
			}
			window = NULL;
			{
			BPoint		here;
			BRect		frame;
			here.Set(bmi->Menu()->Supermenu()->Frame().left + bmi->Menu()->Superitem()->Frame().right + 20, 
						bmi->Menu()->Supermenu()->Frame().top + bmi->Menu()->Superitem()->Frame().top);
			here = bgview->ConvertToScreen(here);
			frame.Set(here.x, here.y, here.x + 159, here.y + 15);
			window = new BWindow(frame, data_list[i]->Name(),
						 B_FLOATING_WINDOW_LOOK,
						 B_FLOATING_APP_WINDOW_FEEL,
						 B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_WILL_ACCEPT_FIRST_CLICK);
			window->SetSizeLimits(frame.Width(), frame.Width(),
									frame.Height(), frame.Height());
			window->SetZoomLimits(frame.Width(), frame.Height());
			frame.OffsetTo(0, 0);
			tc = new BTextControl(frame, winname, "Name to:", data_list[i]->Name(), 
									new BMessage(MSG_RENAME_TO));
			tc->SetViewColor(BKG_GREY);
			tc->SetDivider(50);
			window->ResizeTo(tc->Bounds().right, tc->Bounds().bottom);
			window->AddChild(tc);
			tc->SetTarget(this);
			window->Show();
			}
			delete [] winname;
			window->Activate();
//			cout << "MSG_PROFILE_RENAME\n";
			break;
		case MSG_RENAME_TO:
			BTextControl * btc;
			message->FindPointer("source", (void **)&btc);
			i = (int32)((char)btc->Name()[0]-'0');
			dl_beguse();
			if (i >= 0 && i < (int32)d_list_size && strcmp(data_list[i]->Name(), btc->Window()->Name()) == 0)
			{
				data_list[i]->Rename(btc->Text());
				m_pProfileOpt->ItemAt(i)->SetLabel(data_list[i]->Name());
			}
			dl_enduse();
			btc->Window()->Lock();
			btc->Window()->Quit();
//			cout << "MSG_RENAME_TO\n";
			break;
		case MSG_PROFILE_SAVE:
			message->FindPointer("source", (void **)&bmi);
			i = bmi->Menu()->Supermenu()->IndexOf(bmi->Menu()->Superitem());
			dl_beguse();
			if (i < (int32)d_list_size)
			{
				BMessage * mp = new BMessage(B_SAVE_REQUESTED);
				mp->AddInt32("indx", i);
				entry_ref * ref = new entry_ref;
				app_info * appinf = new app_info;
				BEntry * ent;
				BDirectory * dir;
				if ( be_app->GetAppInfo(appinf) == B_OK )
				{
					BEntry * ent2 = new BEntry(&appinf->ref);
					dir = new BDirectory();
					ent2->GetParent(dir);
					delete ent2;
					ent = new BEntry(dir, "Profiles", true);
					delete dir;
				}
				else
				{
					ent = new BEntry("Profiles", true);
				}	
				delete appinf;
				ent->GetRef(ref);
				delete ent;
				BFilePanel * fp = new BFilePanel(B_SAVE_PANEL, new BMessenger(this), ref, B_FILE_NODE, false, mp, NULL, false, true);
				fp->SetSaveText(data_list[i]->Name());
				fp->Show();
			}
			dl_enduse();
//			cout << "MSG_PROFILE_SAVE\n";
			break;		
		case B_SAVE_REQUESTED:
		{	
			message->FindInt32("indx", &i);
			entry_ref * ref = new entry_ref;
			
			message->FindRef("directory", ref);
			const char ** strarr = new const char *[1];
			message->FindString("name", strarr);
			BFile * bf = new BFile(new BDirectory(ref), strarr[0], B_WRITE_ONLY | B_CREATE_FILE);
			dl_beguse();
			if (bf->InitCheck() == B_OK && i < (int32)d_list_size)
			{
				BMessage * m = new BMessage();
				data_list[i]->Archive(m);
				m->Flatten(bf);
				BNodeInfo(bf).SetType(STR_PROFILE_SIG);
				delete bf;
				delete m;
			}
			dl_enduse();
//			cout << "B_SAVE_REQUESTED\n";
			break;
		}
		case B_CANCEL:
		{
			BFilePanel * fp;
			message->FindPointer("source", (void **)&fp);
			if (fp!= NULL)
				delete fp;
//			else
//				cout << "fp == null\n";

//			cout << "B_CANCEL\n";
			break;
		}
		case MSG_PROFILE_REMOVE:
			message->FindPointer("source", (void **)&bmi);
			i = bmi->Menu()->Supermenu()->IndexOf(bmi->Menu()->Superitem());
			dl_begchange();
			if (d_list_size == 1)
			{
				delete data_list[0];
				newmeasurelen = 16;
				newdrumcount = 16;
				data_list[0] = new DrumCircleData(newmeasurelen, newdrumcount);
				begchange();
				data = data_list[0];
				endchange();
				datachanged = true;
				varschanged = true;
				bmi->Menu()->Superitem()->SetLabel(data_list[0]->Name());
				dl_endchange();
			}
			else 
			{
				if (i == (int32)d_list_curr)
				{
					bmi->Menu()->Supermenu()->ItemAt((i+1)%d_list_size)->SetMarked(true);
					begchange();
					d_list_curr = (uint)((i+1)%(int32)d_list_size);
					data = data_list[d_list_curr];
					endchange();
					datachanged = true;
					varschanged = true;
				}
				if ( i != B_ERROR && i < (int32)d_list_size)
				{
					d_list_size--;
					DrumCircleData * * tmp = data_list;
					data_list = new DrumCircleData *[d_list_size];
					for (int32 j = 0; j < i; j++)
					{
						data_list[j] = tmp[j];	
					}			
					delete tmp[i];
					for (uint j = (uint)i; j < d_list_size; j++)
					{
						data_list[j] = tmp[j+1];
					}
					if ((int32)d_list_curr > i)
						d_list_curr--;
					delete [] tmp; 
				}
				dl_endchange();
				if ( i != B_ERROR && i <= (int32)d_list_size)
				{
					delete bmi->Menu()->Supermenu()->RemoveItem(i);					
					m_pProfileOpt->Draw(m_pProfileOpt->Bounds());
					minbottom = m_pProfileOpt->Frame().bottom + 3 + bgview->Frame().top;
					varviewleft = m_pProfileOpt->Frame().right + 5;
					m_pDrumVarView->FixSize();
					m_pDrumCircleView->FixSize();
				}
			}

//			cout << "MSG_PROFILE_REMOVE\n";
			break;		
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

/* loop through the window list of the application, looking for
   a window with a specified name. */
BWindow	*DrumCircleWindow::GetAppWindow(char *name)
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







