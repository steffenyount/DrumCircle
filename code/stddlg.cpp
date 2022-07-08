//--------------------------------------------------------------------
//	
//	stddlg.cpp
//
//	Written by: Owen Smith
//	
//	Copyright 1998 Be, Inc. All Rights Reserved.
//	
//--------------------------------------------------------------------

#include <Alert.h>
#include <string.h>


#include "stddlg.h"

const char* STR_IERROR = "Internal Error:\n";

void ierror(const char* msg)
{
	char* fullMsg = new char[strlen(STR_IERROR) + strlen(msg) + 1];
	strcpy(fullMsg, STR_IERROR);
	strcpy(fullMsg, msg);
	BAlert alert("Internal Error", fullMsg, "OK", NULL, NULL,
		B_WIDTH_AS_USUAL, B_STOP_ALERT);
	alert.Go();
	delete [] fullMsg;
}
