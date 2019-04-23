# ATMProject

**This program was built exclusively for Windows**

This repository contains a large .cpp file that contains the code for my Computer Science 2 4th Quarter project, an ATM simulator.

***DISCLAIMER: THIS PROGRAM IS PROVIDED AS IS, AND DOES NOT HAVE WARRANTY. THIS PROGRAM DOES NOT CREATE CRYPTOGRAPHICALLY SECURE RECORDS, AND SHOULD NEVER BE USED AS AN ACTUAL ATM APPLICATION. THIS PROGRAM WAS CREATED FOR EDUCATION PURPOSES AND IS NOT BUILT FOR WIDESPREAD USE BUT ONLY FOR OBSERVANCE OR APPLICATION. THE PROGRAM DEVELOPERS ARE NOT RESPONSIBLE FOR ANY DAMAGE DONE BY THE END USER.***

This code was created by a Grade 8 student with beginner understanding of C++. The code here is not optimized and is completely messy. No attempts have been made to organize the code.

## Features

Once started, the program creates two objects in the directory it is found in, `atm.cfg`, which stores the configuration of the project, and `data/`, which holds the data for the entire program.

`data/` will store a log and a list of accounts under the subdirectory `accounts/`

This program supports the creation of individual ATM accounts. Here is a list of all the menus.

```
Main Menu
|
+---- [S]ign Up
|
+---- [L]ogin
|      |
|      +--- [W]ithdraw
|      +--- [D]eposit
|      +--- [T]ransfer Funds
|      +--- [C]lose
|      +--- [L]ogout
|
+---- [A]bout
|
+---- [C]redits
|
+---- [O]ptions
|      |
|      +--- [D]ata Location
|      +--- [A]ccounts Location
|      +--- [L]ogging
|      +--- [E]xit
|
+---- [E]xit
```

You can create an account, and with that account, you may deposit or withdraw funds, transfer funds to another account or close the account. 

You may also configure the location of the program's files.
