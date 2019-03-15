/* C++14 | curl_handler.cpp
 * Everything that uses CURL goes here
 */

#ifndef BASE_IO
#define BASE_IO
#include <iostream>
#include <string>
#include <stdio.h>
#endif

#include "utils/logging.cpp"

#ifndef CURL_HANDLER
#define CURL_HANDLER
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *userp) {
  FILE *f = (FILE*)userp;
  return fwrite(ptr, size, nmemb, f);
}

std::string getSummary(void)
{
	CURL *curl;
	CURLcode res;

	std::string wBuf;

	curl = curl_easy_init();
	if (curl) {
		std::string sum_url = "http://www.rsbuddy.com/exchange/summary.json";
		lg("Getting " + sum_url + " via curl...");
		curl_easy_setopt(curl, CURLOPT_URL, sum_url.c_str());
    	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "?a=guidePrice&i=2");
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wBuf);
	
		int respCode = 0;

    	res = curl_easy_perform(curl);
    	curl_easy_cleanup(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respCode);

		if (respCode == 200) { //Successful 
			lg("Got Sucessful response.");
			return wBuf;
		} else {
			lg("Failed to get Response (api down?)", ERR);
		}
  	}

	return "FAIL";
}

std::string getGraph(long itemID)
{
	CURL *curl;
	CURLcode res;

	std::string wBuf;

	curl = curl_easy_init();
	if (curl) {
		std::string urrl = "http://www.rsbuddy.com/exchange/graphs/180/"+std::to_string(itemID)+".json";
		lg("Getting " + urrl + " via curl...");
		curl_easy_setopt(curl, CURLOPT_URL, urrl.c_str());
    	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "?a=guidePrice&i=2");
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wBuf);
	
		int respCode = 0;

    	res = curl_easy_perform(curl);
    	curl_easy_cleanup(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respCode);

		if (respCode == 200) { //Successful 
			lg("Got Sucessful response.");
			return wBuf;
		} else {
			lg("Failed to get Response (api down?)", ERR);
		}
  	}

	return "FAIL";
}

#endif
