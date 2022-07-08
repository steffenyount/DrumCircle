/*
	my_ostrstream.cpp
	
	Copyright Will Dyson
	will_dyson@pobox.com
	
	Do whatever you want with this code.
	
	Created: 05/16/99 18:05:56
*/

#include "my_strstream.h"


char* ostrstream::str()
{
	string s = ostringstream::str();
	char* p = new char[s.length()+1]; // note +1
	s.copy(p, string::npos, 0); // copy all chars
	p[s.length()] = '\0'; // terminator
	return p;
}
