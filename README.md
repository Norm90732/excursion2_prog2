# Excursion 2 
## By: Norman Smith, Vignesh Saravanan, Jonas Dickens

## Compile and Execute 
To compile this code, we have made excursion2_prog2.exe an executable
for this particular project using cmake. To run, type 
"g++ main.cpp -o excursion2_prog2" in the terminal and it will compile 
this file. The files included in this project are as follows: 
1. main.cpp
2. FileInputOutput.h
3. TreeMake.h

## Summary
Here in this program, we read the netlist inputs from input.txt and store the data as a vector of inputs to each node. Then we construct 
a NAND/NOT tree using recursion. Finally, we use memoization/dp to find the minimum cost with the technology library. The 
output is written to output.txt. 
