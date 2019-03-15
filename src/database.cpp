/* C++14 | database.cpp
 * SQLite handler.
 */ 

#ifndef BASE_IO
#define BASE_IO
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#endif

#ifndef SQLITE
#define SQLITE
#include <sqlite3.h>
#endif

#ifndef RAPIDJSON
#define RAPIDJSON
#include "include/rapidjson/document.h"
#endif

#include "utils/logging.cpp"

#ifndef DB_HANDLER
#define DB_HANDLER

bool fexists(const std::string& filename) {
  std::ifstream ifile(filename.c_str());
  return (bool)ifile;
}

sqlite3* currentDB = NULL;


static int dbPrintCB(void *data, int argc, char **argv, char **azColName){
   int i;
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

sqlite3* createSessionItemDB() {
	lg("Creating ItemDB session...");

	bool ctable = false;
	if (!fexists("items.db"))
		ctable = true;

 	sqlite3* DB; 
    int exit = 0; 
    exit = sqlite3_open("items.db", &DB); 
  
    if (exit) { 
		lg("SQL Failed to load ItemsDB", ERR);
		return NULL;
    } 
    else
		lg("Loaded ItemsDB");

	if (ctable) {
	    std::string sql = "CREATE TABLE ITEMS("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "MEMBERS		  BOOL    NOT NULL, "
                      "SP             INT     NOT NULL, "
                      "BUY_AVG        INT     NOT NULL, "
                      "BUY_QTY        INT     NOT NULL, "
                      "SLL_AVG        INT     NOT NULL, "
                      "SLL_QTY        INT     NOT NULL, "
                      "OVERALL_AVG    INT     NOT NULL, "
                      "OVERALL_QTY    INT 	  NOT NULL, " 
                      "BUYSELL        FLOAT   NOT NULL, "
                      "MARGIN         INT     NOT NULL);";
    	char* messaggeError; 
    	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); 
  
    	if (exit != SQLITE_OK) { 
        	sqlite3_free(messaggeError); 
        	lg("Error Create Table", ERR); 
    	} 
    	else
        	lg("Created Items Table"); 
	}

    return DB;
}

void destroySessionItemDB(sqlite3* session) {
	lg("Closing ItemDB session");
   	sqlite3_close(session); 
}

void updateItemDB (const rapidjson::Document& docData) {
	lg("Updating ItemDB");
	std::string cannonballName = docData["6"]["name"].GetString();
	int cannonballPrice = docData["6"]["buy_average"].GetInt();
	lg("Success! -> " + cannonballName);
	lg("         -> " + std::to_string(cannonballPrice) + " GP");

	if (!currentDB)
		currentDB = createSessionItemDB();
	
	for (rapidjson::Value::ConstMemberIterator itr = docData.MemberBegin();itr != docData.MemberEnd(); ++itr)
	{
		std::stringstream addCmd;
		float buySellRatio = 0.0f;
		int sllqty = itr->value["sell_quantity"].GetInt();
		if (sllqty != 0)
			buySellRatio = itr->value["buy_quantity"].GetInt() / itr->value["sell_quantity"].GetInt(); 
		else
			buySellRatio = 0.0f;

		addCmd << "insert or replace into ITEMS(ID,NAME,MEMBERS,SP,BUY_AVG,BUY_QTY,SLL_AVG,SLL_QTY,OVERALL_AVG,OVERALL_QTY,BUYSELL,MARGIN) values (";
   		addCmd << itr->name.GetString() << ",";
   		addCmd << "\"" << itr->value["name"].GetString() << "\",";
   		addCmd << (itr->value["members"].GetBool() ? "TRUE" : "FALSE") << ",";
   		addCmd << itr->value["sp"].GetInt() << ",";
   		addCmd << itr->value["buy_average"].GetInt() << ",";
   		addCmd << itr->value["buy_quantity"].GetInt() << ",";
   		addCmd << itr->value["sell_average"].GetInt() << ",";
   		addCmd << itr->value["sell_quantity"].GetInt() << ",";
   		addCmd << itr->value["overall_average"].GetInt() << ",";
   		addCmd << itr->value["overall_quantity"].GetInt() << ",";
   		addCmd << buySellRatio << ",";
   		addCmd << -itr->value["buy_average"].GetInt() + itr->value["sell_average"].GetInt() << ");";

		
		char* messageError;
		int exit = sqlite3_exec(currentDB, addCmd.str().c_str(), NULL, 0, &messageError); 
    	if (exit != SQLITE_OK) { 
			lg("Error Updating", ERR);
        	sqlite3_free(messageError); 
    	} 
    	else
			lg(addCmd.str());
	}
}

void printItemDB(int iid) {	
	if (!currentDB)
		currentDB = createSessionItemDB();
	std::string sqcmd = "select * from ITEMS where id=" + std::to_string(iid);
	char* messageError;
	int exit = sqlite3_exec(currentDB, sqcmd.c_str(), dbPrintCB, 0, &messageError); 
    if (exit != SQLITE_OK) { 
		lg("Error During Search", ERR);
        sqlite3_free(messageError); 
    } 
    else {
		lg("Success during search");
	}
}

void printItemDBName(std::string name) {	
	if (!currentDB)
		currentDB = createSessionItemDB();
	std::string sqcmd = "select * from ITEMS where name=\"" + name + "\"";
	char* messageError;
	lg(sqcmd);
	int exit = sqlite3_exec(currentDB, sqcmd.c_str(), dbPrintCB, 0, &messageError); 
    if (exit != SQLITE_OK) { 
		lg("Error During Search", ERR);
        sqlite3_free(messageError); 
    } 
    else {
		lg("Success during search");
	}
}

void itemDBAscMargin() {	
	if (!currentDB)
		currentDB = createSessionItemDB();

	std::string sqcmd = "SELECT * FROM ITEMS WHERE MEMBERS=0 AND BUY_AVG!=0 ORDER BY MARGIN DESC LIMIT 10;";
	char* messageError;
	int exit = sqlite3_exec(currentDB, sqcmd.c_str(), dbPrintCB, 0, &messageError); 
    if (exit != SQLITE_OK) { 
		lg("Error During Search", ERR);
        sqlite3_free(messageError); 
    } 
    else {
		lg("Success during search");
	}
}

#endif
