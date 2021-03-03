struct shared_use_st{
    /*
    Structure to be "masked" onto shared memory
    */
    int array_m[4][4];
    int array_n[4][4];
    int array_q[4][4];
    int ranked[4];          // Used to store largest value found by child process
};