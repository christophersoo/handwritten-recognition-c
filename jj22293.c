#include<stdio.h> // The standard library for C Programming Language
#include<stdlib.h> // A utility library for Memory Allocation, Type Conversions, etc
#include<math.h> // A C Programming library for mathematical notations / operations


// Here we define a C structure called matrix
// We use this struct to group several related variables, in this case, we want to group matrices
struct matrix
{
    int numrow; // One property of the matrix is the number of rows which is an integer
    int numcol; // Another property of the matrix is the number of columns which is an integer
    int *elements; // The most important property of the matrix is the elements defined as a pointer
    // array
    // In this case, the elements stored in the matrix structure is in row-major form
};
typedef struct matrix Matrix;
// typedef struct allows users to initialise a structure without writing the struct keyword
// In this case it can also be used to read an image to store it as a matrix structure


// Here we define a function to print out an image in the form of a row-major array using symbols
// We do this to visualise the corresponding image, and to compare them to our prediction
void image2char(int image[], int Height, int Width)
{
    // We are looping through each Height and Width, since the length of the array is Height times Width
    // We do this so that we can print out the symbols in the form of a two-dimensional canvas.
    // And leverage the new-line property of C Programming Language
    // This loops through the Height of the image
    for (int row = 0; row < Height; row++)
    {
        // This loops through the Width of the image
        for (int col = 0; col < Width; col++)
        {
            // Here we convert the row and col indexing to row-major form to access the elements
            // Saves the converted index to the integer variable index
            int index = row * Width + col;

            // Here we are checking if the element of the index is lower or equal to 85
            // We do this to check the brightness of the specific pixel
            // Since brightness or colors are stored as numbers from 0 to 255
            if (image[index] <= 85)
            {
                // We print out an empty whitespace representing a high brightness pixel
                // If the element is less than 85
                printf("  ");
            }
            else if (image[index] <= 170)
            {
                // We print out the letter 'I' representing a gray-ish brightness pixel
                // If the element is less than 170 but more than 85
                printf(" I");
            }
            else
            {
                // We print out the letter 'M' representing a darker brightness pixel
                // If the element is more than 170
                printf(" M");
            }
        }
        // We print a newline to go to the next row after finish printing the current row
        printf("\n");
    }
}


// Here we define a new Matrix Function called read_matrix
// The function takes in a pointer called filename as an input
// The pointer filename points to the memory located in another file within the same directory
Matrix read_matrix(char *filename)
{
    // Here we use the function fopen to read the file and save it into the FILE pointer f
    // rb represents read binary, so the pointer f stores a binary file and is used to connect to the file
    FILE *f = fopen(filename, "rb");

    // Here we use the function getw to read the binary data from the file
    // And advances the file position to the next integer
    int numrow = getw(f); // saves the int variable to numrow
    int numcol = getw(f); // saves the int variable to numcol

    // Now we create a new matrix M using the Matrix keyword defined by the typedef above
    // And we initialise the matrix M with...
    // The integer variable numrow to the integer variable numrow
    // The integer variable numcol to the integer variable numcol
    // HEAP Memory allocation with size numrow * numcol to the array pointer elements
    Matrix M = {numrow, numcol, calloc(numrow * numcol, sizeof(int))};

    // We are using a for loop to iterate through the size of the matrix M
    // numrow represents the height of the matrix
    // numcol represents the width of the matrix
    for (int i = 0; i < M.numrow; i++)
    {
        for (int j = 0; j < M.numcol; j++)
        {
            // Here we are trying to assign the i, jth element of the matrix
            // To the corresponding character in the file using the pointer f we defined
            // By using the row-major formula
            // The function getc reads a character from the file
            M.elements[i*numcol + j] = getc(f);
        }
    }

    // Now since all operations are completed, we can close the file pointer f using the fclose function
    fclose(f);

    // Now we return the matrix structure M
    return M;
}


// This is a helper function we created to get the specific element from a row-major matrix
// Stored in an array using the row index i and the column index j as the parameter
int get_elem(Matrix M, int i, int j)
{
    // Here we check if the input parameter exceeds the range of the Matrix's limits
    // Although it is not necessary in this project, it is a good practice for generality and safety.
    if (i > M.numrow || j > M.numcol || i < 0 || j < 0)
    {
        // We exit with code 0 if it does and prints Bad Input
        // The exit function stops all processes and terminates the script
        printf("Bad Input");
        exit(0);
    }
    else
    {
        // If the row index and column index are within the boundaries
        // We can use the row-major formula to convert the row / column index to the row-major index
        int index = i*M.numcol + j;

        // Now we can return the element of the array using the row-major index
        return M.elements[index];
    }
}


// In contrast to get_elem function
// We can specify another helper function to help us set the row-major index of an array
// Using the row index, the column index, and the new value to be assigned as the parameter
void set_elem(Matrix M, int i, int j, int value)
{
    // Same thing as before, we check if the indicies are out of bounds
    if (i > M.numrow || j > M.numcol || i < 0 || j < 0)
    {
        // prints Bad input and exits the script if it does
        printf("Bad Input");
        exit(0);
    }
    else
    {
        // If the indicies are within the boundary
        // We use the row-major formula again to assign the new index with the new value
        int index = i*M.numcol + j;
        M.elements[index] = value;
    }
}


// Here we define a function that calculates
// The Euclidean Distances between every element of the matrices X and T
// X refers to the training set and T refers to the testing set
// And we store the distance to the matrix D
// The function takes the parameters of the first two Matrix we want to compare
// And the matrix D to record the distances of both matrices
void pairwise_dist2(Matrix M1, Matrix M2, Matrix D)
{

    // We are looping through each row of the D matrix
    // Where each row represents the comparison of each T row to all X rows
    for (int row = 0; row < D.numrow; row++)
    {
        // We are looping through each column of the D matrix
        // Where each column represents the comparison of each X row to the i-th T row
        for (int col = 0; col < D.numcol; col++)
        {
            int dist_value = 0; // We initialise an integer value dist_value to record the cumulative distance

            // We are looping through the number of columns of X and T which are both M squared
            // This is to access every element in the X row to find the distance to the T row
            for (int M_squared = 0; M_squared < M1.numcol; M_squared++)
            {
                int M1_elem = get_elem(M1, col, M_squared); // We get the jth element of the X row
                int M2_elem = get_elem(M2, row, M_squared); // We get the ith element of the T row

                // We then calculate the distance between the two elements
                // And add the distance to the cumulative distance integer variable dist_value
                // We use the function pow to square the distance
                // We square the distance to emphasize on the small differences
                // And also to eliminate the probability of getting a negative number
                dist_value += pow(M1_elem - M2_elem, 2);
            }
            // After getting the sum of the distances of each column
            // We can finally set the ith jth element of the D matrix to the summed squared distances we received
            set_elem(D, row, col, dist_value);
        }
    }
}


// Here we define another helper function
// This function allows us to find the smallest integer element within an array
// And returns the index of that element
// This function accepts an array and the length of the array as parameters
int find_min_index(int a[], int len)
{
    int current_min = a[0]; // We randomly select an element to be compared to, in this case the first element
    int current_index = 0; // We also want to record the corresponding index of the element

    // We then loop through each element of the array
    for (int index = 0; index < len; index++)
    {
        // If we find an element that is smaller than the array
        if (a[index] < current_min) {
            current_min = a[index]; // We replace the current_min integer variable to the new element
            current_index = index; //  We also replace the current index to the new index of the element
        }
    }
    // After finding the smallest element
    // We return the element's index
    // Why do we return the index but not the element?
    // Because when we transpose the D matrix row,
    // the index of the D column correspond to the index of the Y column, which will give us the label of the image
    return current_index;
}


// Here we define another useful helper function
// This function allows us to find the 5 smallest elements of the array
// And records their indices, and saves them to the indices array
// Since arrays are pass by reference, we specify a void function
// This function accepts the original array, the length of that array and the indices array as parameters
void minimum5(int a[], int len, int indices[])
{
    // We are looping from 1 to 5
    // The number of iterations is subject to the number of minimum indices we are trying to find
    for (int iteration = 0; iteration < 5; iteration++){
        // We then use the find_min_index function to find the index of the smallest element
        // And assign it to the first element of the indices array
        indices[iteration] = find_min_index(a, len);

        // To avoid duplicates, we set the element of the current minimum index to INT_MAX
        // INT_MAX sets the element to the maximum possible integer
        // We do this so that it will be larger than everything else and won't be picked again
        a[indices[iteration]] = INT_MAX;
    }
}


// Now we define our main function
void main()
{
    // The Matrix X represents a matrix of images
    // Where each row is an image matrix in row-major order
    // This matrix is our training set
    Matrix X = read_matrix("./X.matrix");
    printf("N: %d, M: %d\n", X.numrow, (int) sqrt(X.numcol)); // Printing the information of X matrix

    // The Matrix T represents a matrix of images
    // Where each row is an image matrix in row-major order
    // This matrix is our Testing-Set
    // Testing Set refers to label-less set that we want our model to predict on
    Matrix T = read_matrix("./T.matrix");
    printf("L: %d\n", T.numrow); // Printing the information of T matrix

    // The Matrix Y represents a column vector of labels
    // Where each row is the label corresponding to the ith image in the X matrix
    Matrix Y = read_matrix("./Y.matrix");

    // Here we are trying to find the amount of 1's in the training set matrix X
    int s = 0;
    // We loop through each row of Y
    for (int i = 0; i < Y.numrow; i++)
    {
        // If the corresponding label is 1
        if(Y.elements[i*Y.numcol + 0] == 1)
            s++; // Adds 1 to the counter s
    }
    printf("Number of 1 in the training set: %d\n", s); // Prints out the number of 1's in the training set matrix X

    // Here we make a new matrix D which represents distance, and it records the distances from matrix X to T
    // each row represents the distances from each T row to every X row
    // each column represents the distances from each X row to the ith T row
    // And we initialise the matrix M with...
    // The integer variable numrow to the T matrix integer variable numrow
    // The integer variable numcol to the X matrix integer variable numrow
    // HEAP Memory allocation with size numrow * numcol to the array pointer elements
    Matrix D;
    D.numrow = T.numrow;
    D.numcol = X.numrow;
    D.elements = malloc(sizeof(int) * D.numrow * D.numcol);

    pairwise_dist2(X, T, D); // We then call the pairwise_dist2 function to find the distances

    // Now we loop through each row in the T matrix
    // Where each row represents each image matrix in row-major order
    // our purpose is for the model to come up with a prediction for each image in T matrix
    // If the image corresponds to a 1 or not 1
    for (int i=0; i< T.numrow; i++)
    {
        // printf("The %d-th testing image:\n", i);
        int T_element[T.numcol]; // Makes a new integer array to record the current row of the T matrix

        // Now we loop through each column of the T matrix
        for (int col = 0; col < T.numcol; col++)
        {
            // And we assign the index of T_element with the current row of the T matrix
            // We do this so that we can call the image2char function to print out the current row or image matrix
            T_element[col] = T.elements[i*T.numcol+col];
        }
        // We now call the function
        // Note that the height and width should be the square root of the T column length
        // Since the row of the T matrix is in row major form which is M squared
        image2char(T_element, (int)sqrt(T.numcol), (int)sqrt(T.numcol));

        int count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // counter to check the frequency of 1's
        int indices[5]; // We make a new integer array indices to record the 5 smallest indices
        int labels[5]; // We make a new integer array labels to record the label of the 5 smallest indices
        int D_element[D.numcol]; // Makes a new integer array to record the current row of the T matrix

        // Now we loop through each column of the D matrix
        for (int col = 0; col < D.numcol; col++){
            // And we assign the index of D_element with the current row of the D matrix
            // We do this so that we can call the minimum5 function to find the 5 smallest indices of the D matrix
            D_element[col] = D.elements[i*D.numcol+col];
        }
        // We now call the function
        // Note that since arrays are pass by reference, we don't need a variable to capture the output
        minimum5(D_element, D.numcol, indices);

        // We now loop through each neighbor
        // In this case this model is finding the 5 nearest neighbors
        for (int k = 0; k < 5; k++){
            // We then assign the elements from the Y matrix based on the minimum indices to the labels array
            // the elements in the Y matrix represents the labels of each X row or images
            labels[k] = Y.elements[indices[k]];
            printf("%d ", labels[k]);
            // Now we check if the label is 1, since we only want to know if the image is 1 or not 1
            // If we want to check for all numbers, we would have to loop through 0 to 9
            // And construct a new array to keep count of the frequency of each number
            // And find the index of the largest element (highest frequency)
            // And that index would be our prediction
            // But for the simplicity of this project, we only want to check if it is 1 or not 1
            for (int j = 0; j < 10; j++){
                if (labels[k] == j){
                    count[j]++; // Adds 1 to the counter if it is
                }
            }
        }

        printf("The %d-th testing image is classified as ", i);

        // Check if the frequency is equal or more than 3
        // Since different images can look similar
        // We set the threshold to 3 just to be sure
        int max = count[0];
        int max_index = 0;

        for (int l = 0; l < 10; l++){
            if (count[l] > max){
                max = count[l];
                max_index = l;
            }
        }
        count[max_index] = INT_MIN;
        int second = count[0];
        int second_index = 0;
        if (max_index == 0){
            second = count[max_index+1];
            second_index = max_index+1;
        } else {
            second = count[max_index-1];
            second_index = max_index-1;
        }
        for (int m = 0; m < 10; m++){
            if (count[m] > second){
                second = count[m];
                second_index = m;
            }
        }

        printf("\n");
        if (max == second){
            printf("%d or %d", max_index, second_index);
        } else {
            printf("%d", max_index);
        }

        printf("\n");
        printf("----------------------------------------\n");
    }

    // Now we free all the HEAP memories we allocated
    free(X.elements);
    free(T.elements);
    free(Y.elements);
    free(D.elements);
}