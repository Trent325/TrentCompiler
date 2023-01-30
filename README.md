Welcome To Trents Compiler for CMPT 432 in the Spring of 2023. I will be making my compiler in C++. I will soon add in ridiculous detail how to build and deploy my code.

I will be using a make file for my compiler. This allows for the typing of only one command in order to create an executable version of the compiler.
There are 3 functions that the make file allows for, building, running and cleaning. Running the build command will allow for the project to be built.
The build command links the proper dependancys and creates and executable of the project. The run command executes the executable. 
The clean command deletes the executable files and configurations of that build while allows for the build command to be run again.

In order to build the project:
    Navigate to the root directory and type
    "mingw32-make -f makefile"

In order to run the compiler:
    Navigate to the root directory and type:
    "mingw32-make run -f makefile"

In order to clean the compiler:
    Navigate to the root directory and type:
        "mingw32-make clean -f makefile"

I had an issue with the default windows compiler so I downloaded the mingw32 compiler 
In order to use it on windows you have to type mingw32 before using the command 

In order to create my make file I used the following references

References:
https://bytes.usc.edu/cs104/wiki/gcc/
https://bytes.usc.edu/cs104/wiki/makefile/

