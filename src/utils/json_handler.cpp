/* C++14 | json_parser
 * Parsing jsdon information / graph data
 */

#ifndef BASE_IO
#define BASE_IO
#include <string>
#include <stdio.h>
#include <iostream>
#endif

#ifndef RAPIDJSON
#define RAPIDJSON
#include "../include/rapidjson/document.h"
#endif

#include "logging.cpp"

#ifndef JSON_PARSE
#define JSON_PARSE

rapidjson::Document parseJson (std::string data) {
	lg("parseJson called!");

	rapidjson::Document doc;
	doc.Parse(data.c_str());

	if (!doc.HasMember("2")) { //Fail
		lg("Failed to Parse JSON", ERR);
		rapidjson::Document empty;
		return empty;
	}

	return doc;
}

#endif
