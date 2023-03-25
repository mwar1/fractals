# fractals

Generates escape time fractals using the equation z<sub>n</sub> = z<sub>n-1</sub><sup>2</sup> + c

Generation is multi threaded such that each thread is responsible for a strip of the screen. The number of thread can be changed by editing `numThreads` in `main.cpp`.
