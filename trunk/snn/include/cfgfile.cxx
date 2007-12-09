#include "cfgfile.h"
// definitions of forward declarations

void str_tokenize (const std::string& str, std::vector<std::string>* tokens)
{ // splits string up into white space delimited strings (contained in vector = tokens)
	std::string buf;
	std::stringstream ss(str);
	while ( ss >> buf )
		tokens->push_back(buf);
}

void CfgFile::advance ()
{ // advance to next line of data (skipping comments)
	if (! file_stream.eof()) // check end of file
	{
		// remove whitespace and blank lines
		file_stream >> std::ws;
		// fill curr_line
		getline(file_stream, curr_line);
		if (curr_line[0] == '#')
		{ // this is a comment, advance to next line
			advance();
		}else{ // not a comment
			// clear existing data vector
			data.clear();
			// fill data vector with line
			str_tokenize(curr_line, &data);
		}
		cfg_eof = file_stream.eof();
	}else{
		// at the end of file, don't advance
		// set eof marker to true
		cfg_eof = true;
	}
}
