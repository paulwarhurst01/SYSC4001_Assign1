#SYSC 4001 - Assignment 1
@Author - Paul Warhurst
@Student Number - 101 087 869

To compile program run make file and execute <./MMULT>

##### METHOD AND DISCCUSSION #####

# Goals of Design: 
-	Calculate the product of two four by four matrices stored in shared memory and placing the product in shared memory.
-	Use 4 subprocesses to achieve this, each one calculating one row of the product.
-	Each subprocess should also find the largest value in the row and store to eventually calculate the largest value in the product matrix.
-	Time the completion time for this program to complete.
# Method: 
-	Parent process initializes by creating a shared memory space and copying the necessary matrices into this shared space. 
-	The system time is found and stored in the parent process.
-	The parent process “forks” four child processes. each of these contain the integer value of the row of matrix M they will multiply N through by, 
    this integer also corresponds to the row of the product matrix the child process will calculate.
-	The parent process pauses until the four children are completed.
-	The child processes calculate the product rows placing them in shared memory.
-	The child processes determine the largest value in its product row and places this in an array in shared memory.
-	On completion of all subprocesses the parent is signaled to stop pausing. 
-	The parent prints the final product matrix, “Q.” 
-	The parent process finds the largest value of the largest values in the rows as found by the children processes. The parent displays this result.
-	The parent process finds the system time and uses this to calculate the completion time which it prints. 

##### RESULTS #####

Mandatory test data Produced Result:

Q = [   990     1150    1530    1990
        800     716     918     1254
        501     1279    2068    2858
        181     321     492     668   ]

-> Returned largest value in array as 2858
-> Complete in  1258 micro sec on this run

Second test completed: 
M = [   4    7   2   4
        50  30  40  90
        30  70  80  90
        80  60  50  20 ]

N = [   70  10  90  50
        20  30  80  80
        70  40  30  90
        80  50  90  40 ]

Calculated Q: 
[    880     530     1340    1100 
    14100   7500    16200   12100 
    16300   10100   18800   17900 
    11900   5600    15300   14100 ]

-> Largest was found to be 18800
-> Completion time for this run was 954 micro sec