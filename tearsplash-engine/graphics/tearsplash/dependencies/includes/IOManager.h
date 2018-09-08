// IOManager.h

#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <vector>
#include <string>

static class IOManager
{
public:
	static bool readFileIntoBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);

};

#endif // !IOMANAGER_H