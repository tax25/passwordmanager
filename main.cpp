#include<iostream>
#include<limits>
#include<cstring> 
#include<vector>
#include<cstdlib>

#include"timeManager/timeManager.h"
#include"DBManager/DBManager.h"


#define MINIMUM_PASSWORD_LENGHT 7
#define MINIMUM_WEBSITE_OR_APP_NAME 5
#define NUMBER_OF_CHARACTER_ARRAY_CELLS 3
enum MainMenuChoices{

  MM_CREATE_PASSWORD = 1, 
  MM_SEARCH_PASSWORD = 2, 
  MM_UPDATE_PASSWORD = 3

};

enum CreatePasswordChoices{

  CPC_DEFAULT_PASSWORD = 1, 
  CPC_CUSTOM_PASSWORD = 2

};

enum SearchPwdChoices{
  
  SP_THROUGH_NAME = 1,
  SP_THROUGH_DATE = 2

};

void welcomeUser();
void showChoices();

short int askForPasswordLeght(std::string prompt, std::string errorPrompt, int minLenght);
std::string askForWebSiteOrAppName(std::string prompt, std::string errorPrompt, int minNameLenght);
std::vector<bool> askForSpecialCharacters(std::string firstPrompt, std::string secondPrompt, std::string thirdPrompt, short int numberOfCells);
bool askForSomething(std::string prompt, std::string errorPrompt);
std::string generatePassword(short int length, std::vector<bool> specialChararactersParameters);
char selectCharacterFromString(std::string stringToUse);
void showGeneratedPassword(std::string stringToUse);

int main(void){

  int choice = 0;
  
  srand(time(NULL));
  
  TimeManager timeManager; // to get date when creating database records
  DBManager dbManager; // to get access to the database
  
  std::string tableName = "PASSWORDSTABLE";
  
  dbManager.setDBName("passwordsDB.db");
  dbManager.createDatabase();
  
  std::vector<std::string> columnsNameAndType = {"DATEOFRECORD DATE", "WEBSITEORAPPNAME TEXT", "PASSWORD TEXT"};
  dbManager.createTable(tableName, columnsNameAndType);
  
  do{
    
    system("clear");
    
    welcomeUser();
  
    showChoices();
  
    //        std::cin fails
    while(not(std::cin >> choice) || ((choice < MM_CREATE_PASSWORD)||(choice > MM_UPDATE_PASSWORD))){

      std::cout << "Please insert a number between the possible choices" << std::endl;
      std::cin.clear();

      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    switch(choice){
  
        case MM_CREATE_PASSWORD:
          {
          bool exit;
          
          short int passwordLenght = askForPasswordLeght("How much do you want your password to be? (lenght > 7)", "Please, insert a number > 7", MINIMUM_PASSWORD_LENGHT);
          std::string webSiteOrAppName = askForWebSiteOrAppName("Which app or website is this password for?", "Please enter a valid name ( > 5)", MINIMUM_WEBSITE_OR_APP_NAME);
          
          std::string firstSpecialCharactersPrompt = "The password normally includes: numbers, special characters and alphanumerical characters.";
          std::string secondSpecialCharactersPrompt = "If you want them all, insert 1.";
          std::string thirdSpecialCharactersPrompt = "If you prefer to choose which characters use and which not, insert 2.";
          
          // i use the first cell of the vector for numbers, the second for alphanumerical characters and the third for special ones. That's why in the function i use explicitely the first, second and third cell
          std::vector <bool> specialCharacters = askForSpecialCharacters(firstSpecialCharactersPrompt, secondSpecialCharactersPrompt, thirdSpecialCharactersPrompt, NUMBER_OF_CHARACTER_ARRAY_CELLS);
           
          std::string generatedPassword = generatePassword(passwordLenght, specialCharacters);
          showGeneratedPassword(generatedPassword);
  
          std::string sqlInsertStatement = "INSERT INTO " + tableName + " VALUES " 
            + "( "
            + dbManager.quoteSql(timeManager.getDate()) + ", "
            + dbManager.quoteSql(webSiteOrAppName) + ", "
            + dbManager.quoteSql(generatedPassword) + ");";
          
          std::cout << sqlInsertStatement << std::endl;
          
          exit = dbManager.insertIntoDB(sqlInsertStatement);
          if(exit != DB_SUCCESS){
            std::cout << "There was an error during password saving in db" << std::endl;
          }else{
            std::cout << "The password has been successfully saved" << std::endl;
          }

          break;
          }
  
        case MM_SEARCH_PASSWORD:
          {
            int mode = 0;
  
            std::cout << "By which method do you want to search the password?" << std::endl;
            std::cout << "(1) Through the name of the website / app" << std::endl;
            std::cout << "(2) Through creation date" << std::endl;
            
            while(not(std::cin >> mode) || ((mode < SP_THROUGH_NAME)&&(mode > SP_THROUGH_DATE))){
  
              std::cout << "Please insert a number between the possible choices" << std::endl;
              std::cin.clear();
  
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            if(mode == SP_THROUGH_NAME){
               
              std::string webOrAppNameToSearch;
              std::cout << "Ok, Insert the name of the app or website: " << std::endl;
              std::cin >> webOrAppNameToSearch;
              
              // DATABASE: CERCA LA PWD E STAMPALA
              std::string sqlSearchStatement = "SELECT * FROM " 
                + tableName 
                + " WHERE "
                + /*column name, i'm gonna put the name of the webOrAppNameToSearch, so: */ "WEBSITEORAPPNAME = "
                + dbManager.quoteSql(webOrAppNameToSearch);
              // dbManager.getEverythingFromTable(tableName);
              std::cout << sqlSearchStatement << std::endl;
              
              bool exit = dbManager.querySomethingFromDB(sqlSearchStatement);
              if(exit != DB_SUCCESS){
                std::cout << "Something went wrong" << std::endl;
              }
  
            }else if(mode == SP_THROUGH_DATE){
              std::string dateOfCreation;
              std::cout << "Ok, Insert the date of the creation of the password (like this: dd-mm-yy)" << std::endl;
              std::cin >> dateOfCreation;
              // DATABASE: CERCA LA PWD E STAMPALA
            }
          break;
          }
  
        case MM_UPDATE_PASSWORD:
          
        std::string webSiteOrAppName = askForWebSiteOrAppName("Of which app/website would you like to change the password?", "Plese enter a valid name ( > 5)", MINIMUM_WEBSITE_OR_APP_NAME);
        std::string newPassword;

        bool newPwdOrNot;
        std::cout << "Do you want to type your new password or you want to create a new one? (0/1)" << std::endl;
        while(not(std::cin >> newPwdOrNot)){
          std::cout << "Please insert 0 or 1" << std::endl;
        }
        
        if(newPwdOrNot){
           
          short int passwordLenght = askForPasswordLeght("How much do you want your password to be? (lenght > 7)", "Please, insert a number > 7", MINIMUM_PASSWORD_LENGHT);
        
          std::string firstSpecialCharactersPrompt = "The password normally includes: numbers, special characters and alphanumerical characters.";
          std::string secondSpecialCharactersPrompt = "If you want them all, insert 1.";
          std::string thirdSpecialCharactersPrompt = "If you prefer to choose which characters use and which not, insert 2.";
        
          // i use the first cell of the vector for numbers, the second for alphanumerical characters and the third for special ones. That's why in the function i use explicitely the first, second and third cell
          std::vector <bool> specialCharacters = askForSpecialCharacters(firstSpecialCharactersPrompt, secondSpecialCharactersPrompt, thirdSpecialCharactersPrompt, NUMBER_OF_CHARACTER_ARRAY_CELLS);
        
          for (bool elementoDue : specialCharacters){
            std::cout << "Elemento dell'array booleano: "<< elementoDue << std::endl;
          }
        
          newPassword = generatePassword(passwordLenght, specialCharacters);
          showGeneratedPassword(newPassword);
  
          // DATABASE: UPDATE PASSWORD, RICORDATI DI PASSARE IL NOME DEL SITO/APP
          
        }else{
          std::cout << "Ok, insert the new password: " << std::endl;
          std::cin >> newPassword;
          std::cout << newPassword << std::endl;
          // DATABASE: UPDATE PASSWORD, RICORDATI DI PASSARE IL NOME DEL SITO/APP
        }

        // Creare l'sqlStatement 
        std::string sqlUpdateStatement = "UPDATE " 
          + tableName +
          + " SET PASSWORD = "
          + dbManager.quoteSql(newPassword) 
          + " WHERE WEBSITEORAPPNAME = "
          + dbManager.quoteSql(webSiteOrAppName);
        bool exit = dbManager.updateSomethingInDB(sqlUpdateStatement);
        if(exit != DB_SUCCESS){
          std::cout << "An error occured while trying to update the password" << std::endl;
        }
        break;
  
    }
  }while(askForSomething("Do you want to do some other operation? (0/1)", "Please insert 0 or 1"));
}

void welcomeUser(){

  std::cout << "--------------------" << std::endl;
  std::cout << "Welcome to passwordManager!" << std::endl;
  std::cout << "--------------------" << std::endl;

}

void showChoices(){

  std::cout << "Choose the operation you want to do [(n) is the number that you have to press]:" << std::endl;
  std::cout << "(1) Create a new password" << std::endl;
  std::cout << "(2) Search for a saved password" << std::endl;
  std::cout << "(3) Change / Update a password" << std::endl;

}

short int askForPasswordLeght(std::string prompt, std::string errorPrompt, int minLenght){
  
  short int enteredLenght = 0;

  std::cout << prompt << std::endl;
    
  while((not(std::cin >> enteredLenght)) || (enteredLenght < minLenght)){

    std::cout << errorPrompt << std::endl;
    std::cin.clear();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return enteredLenght;
}

std::string askForWebSiteOrAppName(std::string prompt, std::string errorPrompt, int minNameLenght){
  
  std::string name;
  
  std::cout << prompt << std::endl;
  
  while((not(std::cin >> name)) || ((int)strlen(name.c_str()) < minNameLenght)){

    std::cout << errorPrompt << std::endl;
    std::cin.clear();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return name;
}

std::vector<bool> askForSpecialCharacters(std::string firstPrompt, std::string secondPrompt, std::string thirdPrompt, short int numberOfCells){

  short int customPasswordOrNot = 0;
  
  std::vector<bool> booleanArray(numberOfCells);

  std::cout << firstPrompt << std::endl;
  std::cout << secondPrompt << std::endl;
  std::cout << thirdPrompt << std::endl;
  
  while(not(std::cin >> customPasswordOrNot) || ((customPasswordOrNot < CPC_DEFAULT_PASSWORD)||(customPasswordOrNot > CPC_CUSTOM_PASSWORD))){

    std::cout << "Please insert a number between the possible choices" << std::endl;
    std::cin.clear();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  
  switch(customPasswordOrNot){

    case 1:
      std::fill(booleanArray.begin(), booleanArray.end(), true);
      return booleanArray;
      break;

    case 2:
      {
      // i chose to use the first cell of the vector for numbers, the second for alphanumerical characters and the third for special ones. That's why in the function i use explicitely the first, second and third cell
      bool numbers = askForSomething("Do you want numbers? (0/1)", "Please insert 0 or 1");
      booleanArray.at(0) = numbers;
      bool alphas = askForSomething("Do you want alpha characters? (0/1)", "Please insert 0 or 1");
      booleanArray.at(1) = alphas;
      bool specials = askForSomething("Do  you want special characters? (0/1)", "Please insert 0 or 1");
      booleanArray.at(2) = specials;
      }
  }
  return booleanArray;
}

bool askForSomething(std::string prompt, std::string errorPrompt){

  bool enteredData;

  std::cout << prompt << std::endl;
    
  while((not(std::cin >> enteredData)) || ((enteredData != 0)&&(enteredData != 1))){

    std::cout << errorPrompt << std::endl;
    std::cin.clear();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return enteredData;
}

std::string generatePassword(short int length, std::vector<bool> specialChararactersParameters){

  std::string pwd;
  
  const std::string upperCaseAlphabet = "ABCDEFGHIJLMNOPQRSTUVWYZ";
  const std::string lowerCaseAlphabet = "abcdefghijlmnopqrstuvwyz";
  
  const std::string specialCharactersString = "!£$%&/()=*+-";
  const std::string alphaNumericalCharactersString = "{}[]#@§;:,.°";
  const std::string numbersString = "0123465789";
  
  for(int i = 0; i <= length; ++i){
  
    short int randomNumber = (rand() % 4);
    
    switch(randomNumber){

      case 0:
        pwd += selectCharacterFromString(upperCaseAlphabet);
        break;

      case 1:
        pwd += selectCharacterFromString(lowerCaseAlphabet);
        break;
      
      // first cell is numbers  
      // second cell alpha numerical characters
      // third cell special characters 
      case 2:
        if(specialChararactersParameters.at(0)){
          pwd += selectCharacterFromString(numbersString);
        }else{--i;} // subtracting one from the counter as one cycle has been used for nothing
        break;

      case 3:
        if(specialChararactersParameters.at(1)){
          pwd += selectCharacterFromString(alphaNumericalCharactersString);
        }else{--i;}
        break;

      case 4:
        if(specialChararactersParameters.at(2)){
          pwd += selectCharacterFromString(specialCharactersString);
        }else{--i;}
        break;
    }
  }
  
  return pwd;
}

void showGeneratedPassword(std::string stringToShow){
  
  std::cout << "--------------------------" << std::endl;
  std::cout << "The password has been generated: \n" << stringToShow << std::endl;
  std::cout << "--------------------------" << std::endl;
  
}

char selectCharacterFromString(std::string stringToUse){
  return stringToUse[(rand() % stringToUse.length())];
}
