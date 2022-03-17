TODOs for the project:

- [x] Checking if a password for a determined website/app is already been saved
- [x] Modify the code to make the user change the name of the app (or website) if it already exists
- [x] Implement a method that just prints everything that is saved in the db just in case the user doesnt remember the pwd name
- [x] Detect Ctrl+c to clean screen when the user force-closes the program
- [x] Fix program creating db in every directory it is called and find a way to make it refer to a single db in a fixed dir
- [x] Make the 'exit' option available everywhere
- [x] Complete the search through date of creation
- [x] Find a way to write something when no results are found in search of password (2nd option of main menu) (and even in the 3rd option, since
    if the user types a non existent password, it wont be found and the program probably crashes)
- [x] Control if a password exists when in update part, so that the program won't crash when committing the update command

- [] Create documentation for DBManager and TimeManager
- [x] Review Search pwd code and the Update code part
- [] Find a way to show results even if the value searched is not the same (like if I search "example.com", but i saved "example" it will show anyways. As the code is written now, it wouldn't show)
- [] Update all sql-code-running methods to sqlite3_prepare_v2 - sqlite3_step() - sqlite3)_finalize() workflow.
- [] Develop doesNameAlreadyExist(...)
