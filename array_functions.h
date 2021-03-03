void copy_array(int arrayin[4][4], int arrayout[4][4]){
    /*
    Copies one array into another element by element
    */
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            arrayout[i][j] = arrayin[i][j];
        }
    }
}

void print_array(char arrayletter, int array[4][4]){
    /* 
    Prints an array place by place, tab separated
    */

    printf("Array: %c\n", arrayletter);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j<4; j++){
            printf("%d \t", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int highest_row_value(int arrayRow[4]){
    /*
    Returns the largest value contained in the 1 x 4 matrix passed
    */

    int highest_val = arrayRow[0];
    for(int i = 1; i < 4; i++){
        if(highest_val < arrayRow[i])
            highest_val = arrayRow[i];
    }
    return highest_val;
}