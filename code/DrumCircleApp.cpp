//--------------------------------------------------------------------
//	
//	DrumCircleApp.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include <Alert.h>
#include <NodeInfo.h>
#include <Message.h>
#include <File.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <iostream.h>
#include <Directory.h>
#include <Roster.h>

#include "globals.h"
#include "DrumCircleApp.h"
#include <midi/MidiSynth.h>
#include "DrumCircleMidi.h"
#include "DrumCircleWindow.h"
#include "DrumCircleData.h"
#include "DrumVarView.h"
#include "DrumCircleView.h"
#include "stddlg.h"


//====================================================================
//	DrumCircleApp Implementation



//--------------------------------------------------------------------
//	DrumCircleApp constructors, destructors, operators

DrumCircleApp::DrumCircleApp()
	: BApplication(STR_APP_SIG)
{
	window = new DrumCircleWindow(STR_APP_NAME);
	window->Show();

	dcmidi = new DrumCircleMidi;
	msynth = new BMidiSynth;
	msynth->EnableInput(true, true);

}



//--------------------------------------------------------------------
//	DrumCircleApp virtual function overrides

void DrumCircleApp::AboutRequested(void)
{
	BAlert* aboutBox = new BAlert(STR_ABOUT_TITLE,
		STR_ABOUT_DESC, STR_ABOUT_BUTTON);
	aboutBox->Go(); 
}


void DrumCircleApp::ReadyToRun(void)
{
	
// the part under here needs to be replaced. . . 
// with a settings file type stuff
	entry_ref * ref;
	app_info * appinf = new app_info;
	BEntry * file;
	BDirectory * dir;
	if ( GetAppInfo(appinf) == B_OK )
	{
		BEntry * ent2 = new BEntry(&appinf->ref);
		dir = new BDirectory();
		ent2->GetParent(dir);
		delete ent2;
		BMessage * m;
		file = new BEntry(dir, "Profiles/Default", true);
		if (file->InitCheck() == B_OK)
		{
			ref = new entry_ref;
			file->GetRef(ref);
			m = new BMessage();
			m->AddRef("refs", ref);
			RefsReceived(m);
			delete ref;
			delete m;
			delete file;		
		}
		file = new BEntry(dir, "Profiles/808Set", true);
		if (file->InitCheck() == B_OK)
		{
			ref = new entry_ref;
			file->GetRef(ref);
			m = new BMessage();
			m->AddRef("refs", ref);
			RefsReceived(m);
			delete ref;
			delete m;
			delete file;		
		}
		file = new BEntry(dir, "Profiles/C Chord", true);
		if (file->InitCheck() == B_OK)
		{
			ref = new entry_ref;
			file->GetRef(ref);
			m = new BMessage();
			m->AddRef("refs", ref);
			RefsReceived(m);
			delete ref;
			delete m;
			delete file;		
		}		
		delete dir;
	}
	delete appinf;
	
	
//connect synth
	dcmidi->Connect(msynth);	

//	cout << "ready to run\n";
}

void DrumCircleApp::RefsReceived(BMessage *message) 
{ 
//	cout << "Refs rec'vd\n";
//	ierror("Refs rec'vd\n");
	uint32 type;
	int32 count;
	entry_ref ref;
	message->GetInfo("refs", &type, &count);
//	cout << "got info\n";
	if ( type != B_REF_TYPE )
		return;
	for ( long i = --count; i >= 0; i-- ) 
	{
//		cout << "starting loop " << i << "\n";
		if ( message->FindRef("refs", i, &ref) == B_OK ) 
		{ 
			BFile * file = new BFile(&ref, B_READ_ONLY); 
			addprof(file);
			delete file;
		}
	} 
//	cout << "done with ref\n";
}

void DrumCircleApp::addprof(BFile * file)
{
	char * filetype = new char[B_MIME_TYPE_LENGTH];

			if ( file->InitCheck() == B_OK ) 
			{
//				cout << "opened file\n";
				BNodeInfo(file).GetType(filetype);
//				cout << "got file type: " << filetype << "\n";
				if ( 0 == strcmp(filetype, STR_PROFILE_SIG) ||
				     0 == strcmp(filetype, "application/SY-DrumCircle-Profile") ) // fix for backwards compatib
				{
//					cout << "proper type\n";
					
					dl_begchange();
					int32 i = (int32)(d_list_isrt);
					if ( i != B_ERROR && i < (int32)d_list_size)
					{
						BMessage * m = new BMessage();
						m->Unflatten(file);
						DrumCircleData * dcd = new DrumCircleData(m);
						d_list_size++;
						DrumCircleData * * tmp = data_list;
						data_list = new DrumCircleData *[d_list_size];
						for (int32 j = 0; j <= i; j++)
						{
							data_list[j] = tmp[j];	
						}
						data_list[i+1] = dcd;
						for (uint j = (uint)i+2; j < d_list_size; j++)
						{
							data_list[j] = tmp[j-1];
						}
						if ((int32)d_list_curr > i)
							d_list_curr++;
						delete [] tmp; 
					}
					dl_endchange();
//					cout << "starting to adjust window\n";
					window->m_pProfileOpt->AddItem(new BMenuItem(window->getoptionmenu(data_list[i+1]->Name())), i+1);
					window->Lock();
					window->m_pProfileOpt->Draw(window->m_pProfileOpt->Bounds());
					minbottom = window->m_pProfileOpt->Frame().bottom + 3 + window->bgview->Frame().top;
					varviewleft = window->m_pProfileOpt->Frame().right + 5;
					window->m_pDrumVarView->FixSize();
					window->m_pDrumCircleView->FixSize();
					if (window->Bounds().bottom < minbottom)
					{
						window->ResizeTo(window->Bounds().right, minbottom);
					}			
					window->Unlock();

				}
//				else
//					cout << "no match on mime type\n";
			}
	delete filetype;


}

void DrumCircleApp::MessageReceived(BMessage *message) 
{
	switch(message->what) 
	{
		case B_REFS_RECEIVED:
//			cout << "got refs msg\n";
			break;
		default:
//			cout << "Msg Recv'd: " << message->what <<"\n";
			BApplication::MessageReceived(message);
			break;
	}
}

