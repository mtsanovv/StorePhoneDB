# Store Phone Database
-------------------------------------
*Written by Marin Tsanov, 2020*

----------

## Requirements

	- GCC compiler

## Instructions
	- Compile & run on Windows:
```
> gcc -o main.exe main.c
> main
```
	- Compile & run on Unixes/Mac OS X:
```
$ gcc -o main main.c
$ chmod a+x main
$ ./main
```

**More about compiling & running using GCC: ** https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

## Notes

	- This is my course work for the course "Programming 2 (in ANSI C)" at my university, TU-Sofia. It was given maximum points after the defense in May 2020. This is the original task, translated:

> Create a program for the command line interface (CLI) in C, which has a menu interface. The program is meant to maintain data about phones available in a store.
> 
> 1. Phones' data is stored in binary files. Each phone has the following properties:
> 	- ID - an unique 12-digit number;
>	- Model - a string with maximum length of 20;
>	- Price - a real number;
>	- Quantity - an integer.
> 2. The program should be able to perform the following operations:
>	- Adding a new phone to the store;
>	- Changing the available quantities of a given phone (addition and subtraction). When subtracting more phones than available the operation is not executed. If the available quantities equal to zero, the phone is removed from the store;
>	- Showing information about all phones with price above average;
>	- Displaying phone data when given an ID.
> 3. Working with data is done using linked list.