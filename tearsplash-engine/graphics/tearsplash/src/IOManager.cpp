/**********************************************************************/
// File:	IOManager.cpp
// Summary:	This is a static class that manages file input and output
//
// Version: Version 1.0 - 2018-08-23
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-23 File created
/**********************************************************************/

// Includes -------------------------
#include <fstream>
#include "Tearsplash/IOManager.h"

// ----------------------------------
// Reads a file at filePath into buffer
bool IOManager::readFileIntoBuffer(const std::string& filePath, std::vector<unsigned char>& buffer)
{
	// Create file object
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	// Figure out file length
	file.seekg(0, std::ios::end);
	int fileSize = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);
	
	// Reduce fileSize by any header information contained in file
	fileSize -= static_cast<int>(file.tellg());

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);
	file.close();

	return true;
}