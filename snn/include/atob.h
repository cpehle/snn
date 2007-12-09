#ifndef _ATOB_H
 #define _ATOB_H

bool atob (const char * str) {
// convert a string to a boolean    !!!!! this only looks at the first character to see if it's t/f, T/F or 1/0
	if (( str[0] == 't' ) or
		( str[0] == 'T' ) or
		( str[0] == '1' ))
	{
//		std::cout << str << " is true" << "\n";
		return true;
	} else if (( str[0] == 'f' ) or
		( str[0] == 'F' ) or
		( str[0] == '0' ))
	{
//		std::cout << str << " is false" << "\n";
		return false;
	} else { 
//		std::cout << str << " was neither??" << "\n";
		return false;
	};
}

#endif
