#include "minifs.h"
#include <iostream>

#include <fcntl.h>
#include <io.h>

MiniFS::MiniFS()
{
	//Uses _setmode to change stdout to Unicode. 
	//Supports Cyrillic and Ideographic characters (like Chinese characters) appear properly
	//if the computer's console font supports those character sets.
	fflush(stdout);
	int result = _setmode(_fileno(stdout), _O_U16TEXT);
	if (result == -1)
		perror("Cannot set the Unicode mode");
	
}

MiniFS::~MiniFS()
{
	fflush(stdout);
	_setmode(_fileno(stdout), _O_TEXT);
}