# timeManager Documentation

## Methods

timeManager has only one method, that returns the date as a string. StringStream is used.

```c++

  std::string getDate();

```

## What it does

What getDate() does is, well, getting the date, as simple as that. 

It gets the date from the system date, i'm not using an ntp server.

It assigns the struct values of time to 3 variables (dayOfMonth, month, year).
  
```c++
 
  dayOfMonth = local -> tm_mday;
  month = local -> tm_mon + 1;
  year = local -> tm_year + 1900;

```

After that it creates a string containing all three values it got from the struct using stringStream.

```c++

  stringStream << dayOfMonth; stringStream >> dayOfMonth_final; stringStream.clear();
  stringStream << month; stringStream >> month_final; stringStream.clear();
  stringStream << year; stringStream >> year_final; stringStream.clear();

  date = dayOfMonth_final + "-" + month_final + "-" + year_final;

```

Eventually it returns 'date':

```c++

  return date;

```
