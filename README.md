# fractals

Generates escape time fractals using the equation z<sub>n</sub> = z<sub>n-1</sub><sup>2</sup> + c

Generation is multi threaded such that each thread is responsible for a strip of the screen. The number of thread can be changed by editing `numThreads` in `main.cpp`.

## Compilation

SFML must be installed for the Makefile to run, this can be done by running `sudo apt-get install libsfml-dev` on Linux machines. The command `make run` will compile and link the source code, and then run the executable which is generated. `make clean` can be used to delete the object files generated during compilation as well as the executable.
