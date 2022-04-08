# DBManager Documentation

## Methods

DBManager has different methods to do different things. Here are all the public methods.

### std::string quoteSql(std::string stringToQuote)

```c++

  std::string quoteSql(std::string stringToQuote);

```

#### What it does

quoteSql is a method that takes in one parameter, stringToQuote, and returns it as a string with apostrophes on its ends. It is used when creating an SQL statement with parameters (like "INSERT INTO tableName (column1, column2, column3) VALUES ('1', '2', '3')").

### bool createDatabase()

```c++

  bool createDatabase();

```

#### What it does

createDatabase is a method that deals with the actual creation and first connection to the database. 

It opens the connection.

```c++

  int exit = 0;

  exit = sqlite3_open(dbName.c_str(), &database);
  
```

Then it checks if the connection happened successfully and returns true (DB\_SUCCESS) or false (DB\_FAIL) wether the operation was successful or not (after closing the connection with the db file).

```c++ 

  if(exit != SQLITE_OK){
    return DB_FAIL;
  }
  sqlite3_close(database);
  
  return DB_SUCCESS;

```

### bool createDatabase(std::string directoryToCreateDBIn, std::string nameOfFolderToPutDBIn)

```c++

  bool createDatabase(std::string directoryToCreateDBIn, std::string nameOfFolderToPutDBIn);

```

#### What it does

It does basically the same thing as createDatabase(), but you can specify a directory in which you want to create the database. For example you may want to create the db in a folder in the home directory. With this method you have just to supply the path you want. 

```c++ 
  
  //creating the command to execute in order to create the directory wanted
  std::string commandToCreateDir = "mkdir" + (std::string)" " 
  + (std::string)directoryToCreateDBIn 
  + (std::string) "/"  
  + (std::string)nameOfFolderToPutDBIn; 
  
  // executing the command
  const int creatingDirError = system(commandToCreateDir.c_str());
  if(creatingDirError == -1){
    std::cerr << "error creating directory" << std::endl;
  }

  std::string dbPath = directoryToCreateDBIn + "/" + nameOfFolderToPutDBIn + "/" + dbName;
  databasePath = dbPath; 
 
```


### bool createTable(std::string tableName, std::vector\<std::string\> columnsNameAndType)

```c++

  bool createTable(std::string tableName, std::vector<std::string> columnsNameAndType);

```

#### What it does

createTable deals with the creation of the tables in the database. It takes in two parameters, the name of the table and a vector containing the column's name and type that the method will loop through to create the SQL statement to execute in order to create the desired table. 

Creating the SQL statement looping through the vector.

```c++

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

```

Then it opens a connection with the db, executes the statement and returns true (DB\_SUCCESS) or false (DB\_FAIL) wether the operation was successful or not. 