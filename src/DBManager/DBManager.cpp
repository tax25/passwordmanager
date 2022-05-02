#include"DBManager.h"


bool DBManager::generalMethodToDoSomethingToDB(bool query, std::string sqlInstruction, std::string specificErrorPromptIfOperationFails){

  sqlite3 *database;
  int exit = 0;

  exit = sqlite3_open(databasePath.c_str(), &database);

  if(exit != SQLITE_OK){
    std::cout << openingDBError << std::endl;
    return DB_FAIL;
  }
  
  char *errorMessage;
  if(query){
    exit = sqlite3_exec(database, sqlInstruction.c_str(), callback, NULL, &errorMessage);
  }else{
    exit = sqlite3_exec(database, sqlInstruction.c_str(), NULL, 0, &errorMessage);
  } 
  
  if(exit != SQLITE_OK){
    std::cout << specificErrorPromptIfOperationFails << std::endl;
    std::cout << errorMessage << std::endl;
    return DB_FAIL;
  }
  sqlite3_close(database);
  
  return DB_SUCCESS;
}

bool DBManager::checkIfAlreadySaved(bool isSimilar, std::string tableName, std::string databaseColumn, std::string columnValue){

  std::string sqlSearchStatement = "";

  if(isSimilar){
  
    sqlSearchStatement = "SELECT * FROM "
    + tableName
    + " WHERE "
    + databaseColumn + " LIKE "
    + privateQuoteSql(returnStringWithPercentage(columnValue));
 
  }else{

    sqlSearchStatement = "SELECT * FROM " 
      + tableName 
      + " WHERE "
      + databaseColumn + " = "
      + privateQuoteSql(columnValue);  
  }
  
  sqlite3 *database;
  
  int exit = 0;
  char* errorMessage;
  exit = sqlite3_open(databasePath.c_str(), &database);
  if(exit != SQLITE_OK){
    std::cout << openingDBError << std::endl;
    sqlite3_free(errorMessage);
  }
  struct sqlite3_stmt *selectStatement;
    
  exit = sqlite3_prepare_v2(database, sqlSearchStatement.c_str(), -1, &selectStatement, NULL);
  if(exit != SQLITE_OK){
    std::cerr << "ERROR WHILE COMPILING SQL STATEMENT, ERROR CODE: " << exit  << std::endl;
    std::cout << sqlite3_errmsg(database) << std::endl;
  }
  
  if(sqlite3_step(selectStatement) != SQLITE_ROW){
    sqlite3_finalize(selectStatement);
    sqlite3_close(database);
    return RECORD_NOT_EXISTING;
  }
  sqlite3_finalize(selectStatement);
  sqlite3_close(database);
  return RECORD_ALREADY_EXISTING;


}

std::string DBManager::privateQuoteSql(std::string stringToQuote_2){
  return std::string("'") + stringToQuote_2 + std::string("'");
}

void DBManager::setDBName(std::string databaseName){  
  dbName = databaseName;
}

int DBManager::callback(void *data, int argc, char** argv, char** azColName){

  // fprintf(stderr, "%s: ", (const char*)data);
  // printf("\n");
  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  
  printf("\n");
  return 0;

}

std::string DBManager::quoteSql(std::string stringToQuote){
  return std::string("'") + stringToQuote + std::string("'");
}

std::string DBManager::returnStringWithPercentage(std::string stringToUse){

   return std::string("%") + stringToUse + std::string("%");

}

bool DBManager::createDatabase(){

  sqlite3 *database;
  int exit = 0;

  exit = sqlite3_open(dbName.c_str(), &database);

  if(exit != SQLITE_OK){
    return DB_FAIL;
  }
  sqlite3_close(database);
  
  return DB_SUCCESS;

}


bool DBManager::createDatabase(std::string directoryToCreateDBIn, std::string nameOfFolderToPutDBIn){

  sqlite3 *database;
  int exit = 0;
  std::string commandToCreateDir = "mkdir" + (std::string)" " + (std::string)directoryToCreateDBIn + (std::string) "/"  + (std::string)nameOfFolderToPutDBIn; 
  const int creatingDirError = system(commandToCreateDir.c_str());
  if(creatingDirError == -1){
    std::cerr << "error creating directory" << std::endl;
  }

  std::string dbPath = directoryToCreateDBIn + "/" + nameOfFolderToPutDBIn + "/" + dbName;
  databasePath = dbPath; // setting the private variable to the path here created so that i can use this var instead of re-writing everything always
  exit = sqlite3_open(databasePath.c_str(), &database);

  if(exit != SQLITE_OK){
    return DB_FAIL;
  }
  sqlite3_close(database);
  
  return DB_SUCCESS;

}

// La funzione di inserting non saprei come renderla multiuso, alla fine diversa gente ha diversi campi, 
// e soprattutto come potrei passare piu' tipi di dato senza dover fare 800 parametri visto che un array puo' 
// tenere solo un tipo di dato? 
// Credo che purtroppo questa parte dovra' essere semplicemente una stringa che viene passata alla funzione e che viene puoi esguita
/*
bool DBManager::insertSomething(){

  sqlite3 *db;
  TimeManager timeManager;

  char *errorMessage;
  std::string roba = "ciao";
  std::string roba2 = "ciao222";
  int exit = sqlite3_open(dbName.c_str(), &db);
  // std::string sqlInsertStatement = "INSERT INTO nomeTabella VALUES ('21-2-2022', 'pincoPallo.com', 'questoEAncoraTesto')";
  std::string sqlInsertStatement = "INSERT INTO nomeTabella VALUES("
    + quoteSql(timeManager.getDate()) + ","
    + quoteSql(roba) + ','
    + quoteSql(roba2) + ")";
  std::cout << sqlInsertStatement << std::endl;
  
  exit = sqlite3_exec(db, sqlInsertStatement.c_str(), NULL, 0, &errorMessage);
  
  if(exit != SQLITE_OK){
    std::cout << "something went wrong" << std::endl;
    std::cout << errorMessage << std::endl;
    return false;
  }
  sqlite3_close(db);
  
  return true;

}
*/


bool DBManager::createTable(std::string tableName, std::vector<std::string> columnsNameAndType){ 
  
  sqlite3 *database;

  std::string sqlCreateTableStatement = "CREATE TABLE IF NOT EXISTS ";

  sqlCreateTableStatement += tableName;
  sqlCreateTableStatement += " ";
  sqlCreateTableStatement += "(";
  
  for(std::string element : columnsNameAndType){
    sqlCreateTableStatement += element;
    if(element != columnsNameAndType.back()){
      sqlCreateTableStatement += ",";
      sqlCreateTableStatement += " ";
    }
  }
  
  sqlCreateTableStatement += ");";

  std::cout << sqlCreateTableStatement << std::endl;
  
  int exit = 0;
  char *errorMessage;
  exit = sqlite3_open(databasePath.c_str(), &database);
  exit = sqlite3_exec(database, sqlCreateTableStatement.c_str(), NULL, 0, &errorMessage);
  if(exit != SQLITE_OK){
    std::cout << errorMessage << std::endl;
    sqlite3_free(errorMessage);
    return DB_FAIL;
  }
  sqlite3_close(database);

  return DB_SUCCESS;
}

bool DBManager::isRecordInDB(bool withSimilarResults, std::string tableName, std::string databaseColumn, std::string columnValue){
  
  return checkIfAlreadySaved(withSimilarResults, tableName, databaseColumn, columnValue);

}

bool DBManager::doesNameAlreadyExist(std::string tableName, std::string nameToSearch){

  if(checkIfAlreadySaved(false, tableName, "WEBSITEORAPPNAME", nameToSearch) == RECORD_ALREADY_EXISTING){
    return NAME_ALREADY_EXISTING; 
  }
  return NAME_NOT_EXISTING;

}

bool DBManager::insertIntoDB(std::string sqlInstruction){
  
  return generalMethodToDoSomethingToDB(false, sqlInstruction, "An error occured while trying to insert data into the database");

}


bool DBManager::querySomethingFromDB(std::string sqlInstruction){
 return generalMethodToDoSomethingToDB(true, sqlInstruction, "An error occured while trying to fetch data from db"); 
}


bool DBManager::updateSomethingInDB(std::string sqlInstruction){

  return generalMethodToDoSomethingToDB(false, sqlInstruction, "An error occured while trying to update data in database");

}


void DBManager::getEverythingFromTable(std::string tableName){

  sqlite3 *database;

  char *errorMessage;
  std::string sqlQueryStatement = "SELECT * FROM " + tableName;
  
  int exit = 0;
  exit = sqlite3_open(databasePath.c_str(), &database);
  if(exit != SQLITE_OK){
    std::cout << openingDBError << std::endl;
    sqlite3_free(errorMessage);
  }
  
  exit = sqlite3_exec(database, sqlQueryStatement.c_str(), callback, NULL, &errorMessage);

  if(exit != SQLITE_OK){
    std::cout << "Something went wrong during db reading" << std::endl;
    std::cout << errorMessage << std::endl;
    sqlite3_free(errorMessage);
  }
  sqlite3_close(database);

}
