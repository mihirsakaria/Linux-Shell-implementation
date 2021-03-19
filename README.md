# Linux-Shell-implementation
Simple implementation of UNIX shell. Written in C. 
This is simple implementation of linux shell. 
To run the file, simply compile the file using a gcc compiler and then execute.

to run the program, we first need to compile the c file.
it can be done using gcc

gcc -o myshell myshell.c
./myshell

this will open the shell

This shell supports the following commands
basic internal commands like ls,pwd,mkdir, and cd,exit

We can also compile another program like using gcc,g++ etc 

Also most of other basic functions can be run like pip, sudo etc. 

It can also do an interactive or batch mode of execution
piping is also included in this process
For batch execution use command line arguments

Bugs :

I have not included wc in this. So ls | wc will not work. 
Also running while running the pipe functions, it will run the number of times + 1 of the pipe being present. that is, in cat testfile | gzip -c | gunzip -c | tail -n 10.
will run 4 times. 

