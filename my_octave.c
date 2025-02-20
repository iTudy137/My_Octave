#include <stdio.h>
#include <stdlib.h>

#define MODULO 10007

/* Function for swapping matrices and their dimensions*/
void swap_matrix(int ***v, int *lin, int *col, int x, int y)
{
	/* Swap matrices */
	int **mat_aux = v[x];
	v[x] = v[y];
	v[y] = mat_aux;

	/* Swap dimensions */
	int lin_aux = lin[x];
	lin[x] = lin[y];
	lin[y] = lin_aux;

	int col_aux = col[x];
	col[x] = col[y];
	col[y] = col_aux;
}

/* Function for dynamically allocating a matrix */
void alloc_matrix(int ***a, int m, int n)
{
	int i;
	*a = (int **)malloc(sizeof(int *) * m);
	if (!*a)
		return;

	for (i = 0; i < m; i++) {
		(*a)[i] = (int *)malloc(sizeof(int) * n);
		if (!(*a)[i]) {
			for (int j = 0; j < i; j++)
				free((*a)[j]);
			free(*a);
			return;
		}
	}
}

/* Function for reading the elements of a matrix */
void read_matrix(int **a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			scanf("%d", &a[i][j]);
	}
}

/* Function for printing a matrix */
void print_matrix(int **a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

/* Function for freeing the allocated memory of a matrix */
void free_matrix(int **a, int m)
{
	int i;
	for (i = 0; i < m; i++)
		free(a[i]);
	free(a);
}

/* Function for reallocating the vector of matrices
   and the vectors storing the number of rows and columns */
int alloc_vect_matrix(int ****v, int **lin, int **col, int nr)
{
	int ***tmp = (int ***)realloc(*v, sizeof(int **) * nr);
	int *tmp_lin = (int *)realloc(*lin, sizeof(int) * nr);
	int *tmp_col = (int *)realloc(*col, sizeof(int) * nr);

	if (!tmp || !tmp_lin || !tmp_col) {
		for (int i = 0; i < nr; i++)
			free_matrix((*v)[i], (*lin)[i]);
		free(*v);
		free(*col);
		free(*lin);
		return 0;
	}
	*v = tmp;
	*lin = tmp_lin;
	*col = tmp_col;
	return 1;
}

/* Function for computing the product of two matrices */
void prod_matrix(int ****v, int **v_aux, int **lin, int **col, int i1, int i2)
{
	for (int i = 0; i < (*lin)[i1]; i++) {
		for (int j = 0; j < (*col)[i1]; j++) {
			v_aux[i][j] = 0;
			for (int k = 0; k < (*col)[i2]; k++) {
				int prod = ((*v)[i1][i][k] * (*v)[i2][k][j]);
				v_aux[i][j] += prod % MODULO;
				v_aux[i][j] %= MODULO;
			}
			if (v_aux[i][j] < 0)
				v_aux[i][j] += MODULO;
		}
	}
	for (int i = 0; i < (*lin)[i1]; i++)
		free((*v)[i1][i]);
	free((*v)[i1]);

	(*v)[i1] = v_aux;
}

/* Function to dinamically allocate a matrix and read its elements */
void task_L(int ****v, int **lin, int **col, int nr)
{
	nr++;
	if (!alloc_vect_matrix(v, lin, col, nr))
		return;
	scanf("%d%d", &(*lin)[nr - 1], &(*col)[nr - 1]);
	alloc_matrix(&(*v)[nr - 1], (*lin)[nr - 1], (*col)[nr - 1]);
	read_matrix((*v)[nr - 1], (*lin)[nr - 1], (*col)[nr - 1]);
}

/* Function to print the dimensions of a matrix */
void task_D(int *lin, int *col, int nr, int index)
{
	if (index < nr && index >= 0)
		printf("%d %d\n", lin[index], col[index]);
	else
		printf("No matrix with the given index\n");
}

/* Function to print the elements of a matrix from a given index */
void task_P(int ***v, int *lin, int *col, int nr, int index)
{
	if (index < nr && index >= 0)
		print_matrix(v[index], lin[index], col[index]);
	else
		printf("No matrix with the given index\n");
}

/* Function to resize a matrix */
void task_C(int ****v, int **lin, int **col, int nr, int index)
{	/* Dinamically allocate vectors to store the number of rows and columns
	   used for resizing the matrix */
	int nr_linii, *linie;
	scanf("%d", &nr_linii);
	linie = (int *)malloc(sizeof(int) * nr_linii);
	for (int i = 0; i < nr_linii; i++)
		scanf("%d", &linie[i]);

	int nr_coloane, *coloana;
	scanf("%d", &nr_coloane);
	coloana = (int *)malloc(sizeof(int) * nr_coloane);
	for (int i = 0; i < nr_coloane; i++)
		scanf("%d", &coloana[i]);

	if (index >= nr || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		/* Dinamically allocate a new matrix where
		   we will store the resized matrix */
		int **v_aux;
		alloc_matrix(&v_aux, nr_linii, nr_coloane);
		for (int i = 0; i < nr_linii; i++) {
			for (int j = 0; j < nr_coloane; j++)
				v_aux[i][j] = (*v)[index][linie[i]][coloana[j]];
		}
		/* Free the old matrix and update the number of rows and columns */
		free_matrix((*v)[index], (*lin)[index]);
		(*v)[index] = v_aux;
		(*lin)[index] = nr_linii;
		(*col)[index] = nr_coloane;
	}
	/* Free the vectors used for resizing the matrix */
	free(linie);
	free(coloana);
}

/* Function to multiply two matrices */
void task_M(int ****v, int **lin, int **col, int *nr, int index1, int index2)
{
	if (index1 < (*nr) && index2 < (*nr) && index1 >= 0 && index2 >= 0) {
		/* Check if the number of columns of the first matrix is equal
		   to the number of rows of the second matrix */
		if ((*col)[index1] == (*lin)[index2]) {
			/* Dinamically allocate a new matrix where we will store
			   the result of the multiplication */
			alloc_vect_matrix(v, lin, col, (*nr) + 1);
			alloc_matrix(&(*v)[*nr], (*lin)[index1], (*col)[index2]);
			(*lin)[*nr] = (*lin)[index1];
			(*col)[*nr] = (*col)[index2];

			for (int i = 0; i < (*lin)[*nr]; i++) {
				for (int j = 0; j < (*col)[*nr]; j++) {
					(*v)[*nr][i][j] = 0;
					for (int k = 0; k < (*col)[index1]; k++) {
						int prod = ((*v)[index1][i][k] * (*v)[index2][k][j]);
						(*v)[*nr][i][j] += prod % MODULO;
						(*v)[*nr][i][j] %= MODULO;
					}
					if ((*v)[*nr][i][j] < 0)
						(*v)[*nr][i][j] += MODULO;
				}
			}
			(*nr)++;
		} else {
			printf("Cannot perform matrix multiplication\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

/* Function to transpose a matrix */
void task_T(int ****v, int **lin, int **col, int *nr, int index)
{
	if (index < (*nr) && index >= 0) {
		/* Dinamically allocate a new matrix where we will store
		   the result of the transposition */
		alloc_vect_matrix(v, lin, col, (*nr) + 1);
		alloc_matrix(&(*v)[*nr], (*col)[index], (*lin)[index]);
		(*lin)[*nr] = (*col)[index];
		(*col)[*nr] = (*lin)[index];
		for (int i = 0; i < (*lin)[*nr]; i++) {
			for (int j = 0; j < (*col)[*nr]; j++)
				(*v)[*nr][i][j] = (*v)[index][j][i];
		}

		/* Free the old matrix and update the number of rows and columns */
		for (int i = 0; i < (*lin)[index]; i++)
			free((*v)[index][i]);

		free((*v)[index]);

		(*v)[index] = (*v)[*nr];
		(*lin)[index] = (*lin)[*nr];
		(*col)[index] = (*col)[*nr];
	} else {
		printf("No matrix with the given index\n");
	}
}

/* Function to sort the matrices in ascending
   order of the sum of their elements */
void task_O(int ***v, int *lin, int *col, int nr)
{
	int sum[nr];
	for (int i = 0; i < nr; i++)
		sum[i] = 0;

	/* Calculate the sum of the elements of each matrix
	   and store it in an array */
	for (int k = 0; k < nr; k++) {
		for (int i = 0; i < lin[k]; i++) {
			for (int j = 0; j < col[k]; j++)
				sum[k] = (sum[k] + v[k][i][j]) % MODULO;
			if (sum[k] < 0)
				sum[k] += MODULO;
		}
	}

	/* Sort the matrices in ascending order of the sum of their elements */
	for (int i = 0; i < nr - 1; i++) {
		for (int j = i + 1; j < nr; j++) {
			if (sum[i] > sum[j]) {
				int aux_sum = sum[i];
				sum[i] = sum[j];
				sum[j] = aux_sum;
				/* We swap the matrix parameters in
				   case they are not in order */
				swap_matrix(v, lin, col, i, j);
			}
		}
	}
}

/* Function to raise a matrix to a power */
void task_R(int ****v, int **lin, int **col, int *nr, int index, int power)
{
	if (power >= 0) {
		/* Check if the matrix is square */
		if ((*lin)[index] == (*col)[index]) {
			/* Dinamically allocate a new matrix where we will store
			   the result of the power operation */
			alloc_vect_matrix(v, lin, col, (*nr) + 1);
			alloc_matrix(&(*v)[*nr], (*lin)[index], (*col)[index]);
			(*lin)[*nr] = (*lin)[index];
			(*col)[*nr] = (*col)[index];
			/* Initialize the new matrix as the identity matrix */
			for (int i = 0; i < (*lin)[*nr]; i++) {
				for (int j = 0; j < (*col)[*nr]; j++) {
					if (i == j)
						(*v)[*nr][i][j] = 1;
					else
						(*v)[*nr][i][j] = 0;
				}
			}
			while (power) {
				/* We dynamically allocate a matrix where we will multiply the
				   identity matrix with the initial one, then store the result
				   and repeat the process until the power index reaches 0 */
				int **v_aux;
				if (power % 2 != 0) {
					alloc_matrix(&v_aux, (*lin)[index], (*col)[index]);
					prod_matrix(v, v_aux, lin, col, *nr, index);
					power--;
				} else {
					alloc_matrix(&v_aux, (*lin)[index], (*col)[index]);
					prod_matrix(v, v_aux, lin, col, index, index);
					power /= 2;
				}
			}
			/* Free the old matrix and update the number of rows and columns */
			for (int i = 0; i < (*lin)[index]; i++)
				free((*v)[index][i]);
			free((*v)[index]);
			(*v)[index] = (*v)[*nr];
			(*lin)[index] = (*lin)[*nr];
			(*col)[index] = (*col)[*nr];
		} else {
			printf("Cannot perform matrix multiplication\n");
		}
	} else {
		printf("Power should be positive\n");
	}
}

/* Function to free a matrix */
void task_F(int ***v, int *lin, int *col, int *nr, int index)
{
	if (index < (*nr) && index >= 0) {
		/* Free the matrix and update the number of total matrices */
		free_matrix(v[index], lin[index]);
		(*nr)--;
		/* Shift the remaining matrices to the left */
		for (int i = index; i < (*nr); i++) {
			v[i] = v[i + 1];
			lin[i] = lin[i + 1];
			col[i] = col[i + 1];
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

/* Function to free all the resources */
void task_Q(int ***v, int *lin, int *col, int nr)
{	/* Free all the matrices and the vectors
       storing the number of rows and columns */
	for (int i = 0; i < nr; i++)
		free_matrix(v[i], lin[i]);
	free(v);
	free(lin);
	free(col);
}

int main(void)
{
	char lit;
	int ***v = NULL, *lin = NULL, *col = NULL, nr = 0;
	scanf("%c", &lit);
	/* We read letters from the keyboard until encountering the character 'Q',
	   at which point the program execution ends, and then we read
	   the indexes specific to the tasks */
	while (lit != 'Q') {
		if (lit == 'L') {
			task_L(&v, &lin, &col, nr);
			nr++;
		} else if (lit == 'D') {
			int index;
			scanf("%d", &index);
			task_D(lin, col, nr, index);

		} else if (lit == 'P') {
			int index;
			scanf("%d", &index);
			task_P(v, lin, col, nr, index);

		} else if (lit == 'C') {
			int index;
			scanf("%d", &index);
			task_C(&v, &lin, &col, nr, index);

		} else if (lit == 'M') {
			int index1, index2;
			scanf("%d%d", &index1, &index2);
			task_M(&v, &lin, &col, &nr, index1, index2);

		} else if (lit == 'O') {
			task_O(v, lin, col, nr);

		} else if (lit == 'T') {
			int index;
			scanf("%d", &index);
			task_T(&v, &lin, &col, &nr, index);

		} else if (lit == 'R') {
			int index, power;
			scanf("%d%d", &index, &power);
			if (index < nr && index >= 0)
				task_R(&v, &lin, &col, &nr, index, power);
			else
				printf("No matrix with the given index\n");

		} else if (lit == 'F') {
			int index;
			scanf("%d", &index);
			task_F(v, lin, col, &nr, index);

		} else {
			printf("Unrecognized command\n");
		}
		/* Read the next character */
		scanf("%c", &lit);
		while ((lit == ' ') || (lit == '\n'))
			scanf("%c", &lit);
		if (lit == 'Q')
			task_Q(v, lin, col, nr);
	}
	return 0;
}
