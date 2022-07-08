/*
	my_ostrstream.h
	
	Copyright Will Dyson 1999
	will_dyson@pobox.com
	
	Do whatever you want with this code.
	
	Created: 05/16/99 17:54:26
*/

/*
	This is a simplistic implementation of strstream, 
	made to work around the fact that the BeOS R4 PPC 
	compiler does not include a strstream.h, while the 
	intel version does. This can make it easier to port
	apps written for the intel version.
	
	Strstream is supposed to be an IOstream that works on
	an array of charecters directly. However, I don't need
	that functionality, so I am faking it by using a 
	stringstream and implementing the char* str() method (the
	stringstream version of which returns the string used) to
	return a c_string that is not deleted by the strstream.
*/

#include <sstream.h>

template <class Ch, class Tr=char_traits<Ch> >
class basic_ostrstream : public basic_ostringstream<Ch, Tr>
{
public:
	char* str();
};

typedef basic_ostrstream<char> ostrstream;
