#ifndef _CFGFILE_H
 #define _CFGFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/**
 * \file cfgfile.h
 * configuration file handling classes and routines
 * \author Brett Graham
 * \date 12-1-07
 * \todo
 * - allow for changing of file name
*/

/// CfgLineItems typedef
/**
	CfgLineItems is a pointer to a vector of strings (std::vector<std::string>*)
*/
typedef std::vector<std::string>* CfgLineItems;

/// str_tokenize
/**
	takes a string (in this case a config file line, and splits it up into white space delimited strings and stores them in a vector
*/
extern void str_tokenize (const std::string& str, std::vector<std::string>* tokens);

/// CfgFile
/**
	general configuration file class allows the loading of configuration files line-by-line and item-by-item 
	usage :
	\code
		CfgFile myCfg("filename");
		CfgLineItems lineItems = myCfg.get_data_ptr();
		while (! myCfg->get_cfg_eof())
			for (unsigned it i=0; i < lineItems->size(); i++)
			{
				//process
			}
			myCfg.advance();
		}
	\endcode
*/
class CfgFile {
	public:
		/// Constructor
		CfgFile (const std::string& cfg_file_name)
		{ //constructor
			file_name = cfg_file_name;
			// open file
			file_stream.open (file_name.c_str());
//			file_stream.open (cfg_file_name.c_str());
			// set eof marker to false
			//cfg_eof = false;
			cfg_eof = (! file_stream.is_open() );
			// advance one line (skipping comments), and fill data
			advance ();
		}
		~CfgFile ()
		{ //destructor
			if ( file_stream.is_open() )
			{
				file_stream.close ();
			}
		}
		/// advance configuration file to next line
		void advance();
		/// get_data_ptr : return pointer to vector of line items
		/**
			used to retrieve the CfgLineItems pointer for a config file
		*/
		std::vector<std::string>* get_data_ptr () { return(&data);};
		/// get_cfg_eof
		/**
			check if this is the end!
		*/
		bool get_cfg_eof () { return(cfg_eof);};
		bool get_cfg_is_open () { return(file_stream.is_open());};
		/// reset_cfg_file
		/**
			close and then reopen the file starting over at the beginning
		*/
		void reset_cfg_file ()
		{
			if ( file_stream.is_open() )
			{
				file_stream.close();
			}
			file_stream.open(file_name.c_str());
			cfg_eof = (! file_stream.is_open() );
			advance();
		};
	private:
		std::ifstream file_stream;
		std::vector<std::string> data;
		std::string curr_line;
		std::string file_name;
		bool cfg_eof;			
};

// ------ testing
// int main () {
// 	cfg_file arch("conn.cfg");
// 	std::vector<std::string>* data_ptr = arch.get_data_ptr() ;
// 	while (! arch.get_cfg_eof() )
// 	{
// 		for (unsigned int i=0; i < data_ptr->size(); i++)
// 		{	
// 			std::cout << data_ptr->at(i) << "\t";
// 		};
// 		std::cout << "\n";
// 		arch.advance();
// 	};
// 	return 0;
// }
// -------/testing

// 	// Open config file for reading
// 	std::ifstream cfg_file;
// 	cfg_file.open ("arch.cfg");
// 	std::string line;
// 	
// 	//check if file is open
// 	if (! cfg_file.is_open())
// 	{
// 		std::cout<<"File not open\n";
// 		return 1;
// 	};
// 	cfg_file.setf(std::ios::skipws);

// 	while (! cfg_file.eof())
// 	{
// 		// remove any whitespace, also removes blank lines
// 		cfg_file >> std::ws;
// 		// get line
// 		getline(cfg_file, line);
// 		// is comment?
// 		if (line[0] == '#')
// 		{
// 			std::cout << "comment: " << line << "\n";
// 		}else{
// 			std::vector<std::string> tokens ;
// 			str_tokenize(line, &tokens);
// // 			tokens.push_back("1");
// // 			tokens.push_back("2");
// // 			tokens.push_back("3");
// 			std::cout << "-data-" << tokens.size() << "\n";
// 			for (unsigned int i = 0; i < tokens.size(); i++)
// 			{
// //				std::cout << i << "\n";
// 				std::string token = tokens.at(i);
// 				std::cout << token << "\t" ;
// 			}; 		
// 			std::cout << "\n/data\n" ;
// //			std::cout << tokens.at(0) << "\n";
// //			std::cout << "-data- " << line << "\n";
// 		};
// 		// line identifier
// 	};	
// 	
// 	//close file
// 	cfg_file.close();
// 	return 0;
//}
#endif
