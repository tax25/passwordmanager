#pragma once

#include<iostream>
#include<vector>
#include<cstdlib>
#include"sqlite3/sqlite3.h"

enum DBState{

  DB_SUCCESS = true,
  DB_FAIL = false

};

enum DBSearchState{

  NAME_ALREADY_EXISTING = true,
  NAME_NOT_EXISTING = false

};


class DBManager{

private:
  std::string dbName;
  
  std::string databasePath;
  
  static int callback(void *data, int argc, char** argv, char** azColName);
  
  const std::string openingDBError = "An error occured while trying to open the database";
  
  bool generalMethodToDoSomethingToDB(bool query, std::string sqlInstruction, std::string specificErrorPromptIfOperationFails);
  std::string privateQuoteSql(std::string stringToQuote_2); 

public:

  void setDBName(std::string databaseName);

  std::string quoteSql(std::string stringToQuote);
  
  bool createDatabase();
  
  bool createDatabase(std::string directoryToCreateDBIn, std::string nameOfFolderToPutDBIn);

  bool insertSomething();
  
  bool createTable(std::string tableName, std::vector<std::string> columnsNameAndType);

  bool isSomethingAlreadySavedWithSameName(std::string tableName, std::string name); // method strictly related to this project  
  // std::string createQueryStatement(std::string tableName, std::string parameter, std::string condition);
  
  // std::string createUpdateStatement(std::string tableName, std::string parameter, std::string condition);
  
  bool insertIntoDB(std::string sqlInstruction);
  bool querySomethingFromDB(std::string sqlInstruction);
  bool updateSomethingInDB(std::string sqlInstruction);

  // void readDB(std::string tableName, std::string queryStatement);
  
  void getEverythingFromTable(std::string tableName);
  
  // bool save(std::string tableName);
  
  bool deleteTable(std::string tableName);


};
