// Errors.h

#ifndef ERRORS_H
#define	ERRORS_H

#include <string>

namespace Tearsplash
{

	extern void fatalError(std::string errorString);
	extern void softError(std::string errorString);

}

#endif // !ERRORS_H