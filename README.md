# Copyright 2022 Dinca Tudor-Cristian

## Octave Simulator

In this problem, we needed to dynamically allocate several matrices in memory to perform different operations based on the input characters.

We created several functions which we used to perform the operations:

- A function (`alloc_matrix`) for dynamic memory allocation of a matrix.
- A function (`read_matrix`) for reading matrix elements.
- A function (`print_matrix`) for displaying the matrix.
- A function (`free_matrix`) for freeing allocated memory.
- A function (`alloc_vect_matrix`) for dynamically allocating the matrix vector parameters and their dimensions.
- A function (`swap_matrix`) for swapping matrices and their dimensions.
- A function (`prod_matrix`) for multiplying two matrices.

The program reads characters, each triggering specific commands. The input continues until the character 'Q' is encountered, at which point all previously allocated memory is freed, and the program stops. For each command, there may or may not be index values required to perform the operation. Using these and the previously defined functions, we execute the commands.

### Example:

In the case of the command 'L', we used the `alloc_vect_matrix` function to dynamically allocate the matrix vector and its dimensions. After reading the dimensions, we allocated the matrix using `alloc_matrix` and read the elements using `read_matrix`.

For the 'M' command, two matrices are multiplied. First, we check if the indices of the two matrices exist in the matrix vector, meaning the matrices are available. If they are not, we display "No matrix with the given index". Next, we verify that the number of columns in the first matrix matches the number of rows in the second matrix. If this condition is not met, we display "Cannot perform matrix multiplication". After both checks, we reallocate the matrix vector alongside the vectors holding the row and column counts using `alloc_vect_matrix`. Then, we allocate a new matrix with the row count of the first matrix and the column count of the second matrix using `alloc_matrix`. We initialize this new matrix with zeroes and replace each position with the result from multiplying the two original matrices. The added elements in the matrix are taken modulo \(10^4 + 7\), and if they are negative, they are increased by \(10^4 + 7\). Thus, the product matrix is obtained.

When the 'F' character is read, we must free the matrix at the given index. After verifying the existence of the matrix, we call the `free_matrix` function to free the matrix at the given index. Then, we move all matrices at higher indices one position backward. Finally, we decrease the parameter that keeps track of the number of matrices by 1.
