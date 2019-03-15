/*	C++14 | logging.cpp
 *
 *	Used for logging messages to the terminal
 */

//TODO: LOG TO FILE

#ifndef BASE_IO

#include <string>
#include <stdio.h>
#include <iostream>
#define BASE_IO

#endif

#ifndef U_LOG
#define U_LOG


// if DBG is defined DEBUG and WARn messages will appear.

enum logType {
	DEBUG,
	WARN,
	ERR,
	PANIC,
};

void lg(std::string message, logType type = DEBUG) {
#if defined(DBG)
	if (type == DEBUG)
		std::cout << "[DEBUG] " << message << std::endl;
	if (type == WARN)
		std::cout << "[WARNING] " << message << std::endl;
#endif
	if (type == ERR)
		std::cout << "[ERROR] " << message << std::endl;
	if (type == PANIC)
		std::cout << "\n\n[PANIC!!!] " << message << std::endl;
}


#endif
