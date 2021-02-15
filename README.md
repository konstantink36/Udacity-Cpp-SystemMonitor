# CppND-System-Monitor

This is my solution of the System Monitor Project of the Udacity C++ Nanodegree program.
The System Monitor displays system resource usage of the Linux operating system. 
TThe starter code was provided by Udacity. 

The base code can be found here https://github.com/udacity/CppND-System-Monitor

![System Monitor](images/Monitor.png)

# ncurses
ncurses is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Install ncurses within your own Linux environment: sudo apt install libncurses5-dev libncursesw5-dev

# Make
This project uses Make. The Makefile has four targets:

- build compiles the source code and generates an executable
- format applies ClangFormat to style the source code
- debug compiles the source code and generates an executable, including debugging symbols
- clean deletes the build/ directory, including all of the build artifacts

# Instructions
First, make sure you have installed ncurses, which is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output. You can install ncurses within your own Linux environment: sudo apt install libncurses5-dev libncursesw5-dev

Clone the project repository: git clone https://github.com/konstantink36/Udacity-SystemMonitor.git

Build the project: make build

Run the resulting executable: ./build/monitor
