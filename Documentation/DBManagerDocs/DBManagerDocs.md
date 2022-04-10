# DBManager Documentation

## Public Methods

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

### bool insertIntoDB(std::string sqlInstruction)

```c++

  bool insertIntoDB(std::string sqlInstruction);

```

#### What it does

insertIntoDB deals with committing and saving information to the db. It takes in input the SQL statement that has to be executed. 
It returns true (DB\_SUCCESS) or false (DB\_FAIL) wether the operation was successful or not.
The statement execution isn't done in insertIntoDB. In fact, as you can notice the method just points to another (private) method: generalMethodToDoSomethingToDB. 

```c++ 

  return generalMethodToDoSomethingToDB(false, sqlInstruction, 
  "An error occured while trying to insert data into the database");

```

### bool querySomethingFromDB(std::string sqlInstruction)

```c++

  bool querySomethingFromDB(std::string sqlInstruction);

```

#### What it does

querySomethingFromDB deals with getting information from the db. 
It returns true or false wether the operation was successful or not. 

As you can notice, this method refers to the same private method as insertIntoDB. Through a control variable (query) the method is able to understand if the SQL statement to execute is a query or not. If it is it will go through a slightly different set of instructions. 

For printing the results of the query, it doesn't have to return a string or an array, as the callback method (which is private) deals already with it. I'm not sure if this workflow is correct, but for now it works so I will keep it. 

### bool updateSomethingInDB(std::string sqlInstruction)

```c++

  bool updateSomethingInDB(std::string sqlInstruction);

```

#### What it does

updateSomethingInDB deals with updating a specific record in the database. It executes the SQL statement through the same method that insertIntoDB and querySomethingFromDB use. 
It returns true or false wether the operation was successful or not. 

### void getEveryThingFromTable(std::string tableName)

```c++

  void getEveryThingFromTable(std::string tableName);

```

#### What it does

getEverythingFromTable does exactly what it does. It prints everything that is saved in a specific table that is chosen through the *tableName* parameter.

### bool deleteTable(std::string tableName)

```c++

  void deleteTable(std::string tableName)

```

#### What it does

It deals with dropping a specific table, that is chosen via the *tableName* parameter.
It returns true or false wether the operation was successful or not.

## Private Methods

The private methods are, of course, private and shall not be used. Here is an explanation on how they work in case you are curious.

### static int callback(void \*data, int argc, char\** argv, char** azColName)

This is a method that is basically mandatory to work with sqlite3. It is called every time that we try to read something from the database. I use it to print to screen the results of the query. 

It is only a for loop that loops through an array with all the results of the query, in this case I cannot explain really how this function works since I didn't write it.

```c++
 
  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }

```

### bool generalMethodToDoSomethingToDB(bool query, std::string sqlInstruction, std::string specificErrorPromptIfOperationFails)

This is a multi-purpose method. It is used in 3 public methods (insertIntoDB, querySomethingFromDB and updateSomethingInDB).   
Its main job is to compile the SQL statement given through the parameter, *sqlInstruction*, and return wether the operation was successful or not. 

It opens the connection with the database:

```c++

  sqlite3 *database;
  int exit = 0;

  exit = sqlite3_open(databasePath.c_str(), &database);

  if(exit != SQLITE_OK){
    std::cout << openingDBError << std::endl;
    return DB_FAIL;
  }

```
If the database can't be opened, it returns false (DB_FAIL) and prints the error.

Then it checks if the statement to compile and run is a query or not, and executes the statement:

```c++

  char *errorMessage;
  if(query){
    exit = sqlite3_exec(database, sqlInstruction.c_str(), callback, NULL, &errorMessage);
  }else{
    exit = sqlite3_exec(database, sqlInstruction.c_str(), NULL, 0, &errorMessage);
  }

```

Finally it checks if the operation was successful or not:

```c++

  if(exit != SQLITE_OK){
    std::cout << specificErrorPromptIfOperationFails << std::endl;
    std::cout << errorMessage << std::endl;
    return DB_FAIL;
  }
  sqlite3_close(database);
  
  return DB_SUCCESS;

```

### std::string privateQuoteSql(std::string stringToQuote_2)

This is the same method as the public one "quoteSql". Simply i didn't know how to call a public method inside another so i created this one. I don't think it is the best solution, but i couldn't find another one on the internet (stackOverflow didn't come in help :[).

```c++

  std::string DBManager::privateQuoteSql(std::string stringToQuote_2){
  	return std::string("'") + stringToQuote_2 + std::string("'");
  }

```


*The method "checkIfAlreadySaved" has not been explained as it is strictly related to the project and to the type of table you have. If interested tell me and I will write it here*