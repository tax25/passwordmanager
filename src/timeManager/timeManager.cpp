#include"timeManager.h"

std::string TimeManager::getDate(){

  std::string date;
  std::stringstream stringStream;
  
  std::string dayOfMonth_final;
  std::string month_final;
  std::string year_final;

  time_t gd_time;
  time(&gd_time);
  
  int dayOfMonth, month, year;
  
  struct tm *local = localtime(&gd_time);
  
  dayOfMonth = local -> tm_mday;
  month = local -> tm_mon + 1; // because it starts from 0
  year = local -> tm_year + 1900; // coz idk
  
  stringStream << dayOfMonth; stringStream >> dayOfMonth_final; stringStream.clear();
  stringStream << month; stringStream >> month_final; stringStream.clear();
  stringStream << year; stringStream >> year_final; stringStream.clear(); 

  date = dayOfMonth_final + "-" + month_final + "-" + year_final;

  return date;
}
