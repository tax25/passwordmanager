#include"DBManager.h"


bool DBManager::generalMethodToDoSomethingToDB(bool query, std::string sqlInstruction, std::string specificErrorPromptIfOperationFails){

  sqlite3 *database;
  int exit = 0;

  exit = sqlite3_open(dbName.c_str(), &database);

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
  exit = sqlite3_open(dbName.c_str(), &database);
  exit = sqlite3_exec(database, sqlCreateTableStatement.c_str(), NULL, 0, &errorMessage);
  if(exit != SQLITE_OK){
    std::cout << errorMessage << std::endl;
    sqlite3_free(errorMessage);
    return DB_FAIL;
  }
  sqlite3_close(database);

  return DB_SUCCESS;
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
  exit = sqlite3_open(dbName.c_str(), &database);
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
