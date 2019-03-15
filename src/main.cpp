/* C++14 | main.cpp
 * Main file of osrsExchange
 * Author: lsaa
 */

//debug flag
#define DBG

#ifndef BASE_IO
#define BASE_IO
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#endif

#include "utils/logging.cpp" //lg(), logType
#include "curl_handler.cpp" //getSummary(), getGraph(long itemID)
#include "utils/json_handler.cpp"
#include "database.cpp"

#ifndef RAPIDJSON
#define RAPIDJSON
#include "include/rapidjson/document.h"
#endif

#include <csignal>

void sigHand(int sig) {
	if (sig != 666)
		lg("Interrupt Signal Recieved... Destroying db session; Aborting everything", PANIC);
	if (currentDB)
		destroySessionItemDB(currentDB);
	lg("Success... Closing down.");
	exit(sig);
}


int main (void) {
	lg("main() START");

	signal(SIGINT, sigHand);

	std::string initialSummaryData = getSummary();

	rapidjson::Document data = parseJson(initialSummaryData);

	//updateItemDB(data);

	std::string cmd;
	int tmp;
	std::string tstr;
	lg("Main Loop Started");
	for (;;) {
		std::cout << "IN> ";
		std::cin >> cmd;
		if (cmd == "exit") {
			sigHand(666);
		} else if (cmd == "id") {
			std::cin >> tmp;
			printItemDB(tmp);
		} else if (cmd == "udb")
			updateItemDB(parseJson(getSummary()));
		else if (cmd == "get") {
			char buf[100];
			std::cin.getline(buf, 100);
			std::string ii(buf);
			ii.erase(0,1);
			printItemDBName(ii);
		} else if (cmd == "graphID") {
			std::cin >> tmp;
			std::cout << getGraph(tmp) << std::endl;
		} else if (cmd == "asc_margin") {
			itemDBAscMargin();
		}
	}

	lg("main() END");
}
