Welcome To Trents Compiler for CMPT 432 in the Spring of 2023. I will be making my compiler in C++. Here is in ridiculous detail how to build and deploy my code.

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

After building the project, you can use the command "./compiler input.txt" to run the project with a filename (that is in the same directory as compiler.cpp) with it as an input file

In order to create my make file I used the following references

References:
https://bytes.usc.edu/cs104/wiki/gcc/
https://bytes.usc.edu/cs104/wiki/makefile/
@misc{brown2020language,
    title={Language Models are Few-Shot Learners},
    author={Tom B. Brown and Benjamin Mann and Nick Ryder and Melanie Subbiah and Jared Kaplan and Prafulla Dhariwal and Arvind Neelakantan and Pranav Shyam and Girish Sastry and Amanda Askell and Sandhya Prabhakaran and Ariel Herbert-Voss and Gretchen Krueger and Tom Henighan and Rewon Child and Aditya Ramesh and Daniel M. Ziegler and Jeffrey Wu and Clemens Winter and Christopher Hesse and Mark Chen and Eric Sigler and Mateusz Litwin and Jasmine Wang and Alec Radford and Ilya Sutskever},
    year={2020},
    eprint={2005.14165},
    archivePrefix={arXiv},
    primaryClass={cs.CL}
}

What Branch to Grade?
Grade the master branch please, always, I may make another branch for out side the box ideas, but not likely.


