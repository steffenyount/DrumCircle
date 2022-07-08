//--------------------------------------------------------------------
//	
//	main.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include "DrumCircleApp.h"

#include <iostream>
#include "globals.h"

int main()
{
/*
	The line below initializes a list of data structures.
	In the original code, the initialization happened in
	the declaration, but that is not portable to the metrowerks
	compiler.
*/
	
	data_list[0] = data;
	
	DrumCircleApp myapp;
	myapp.Run();
}


