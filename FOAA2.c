/* Program that transforms a given initial two-dimensional matrix into a target
matrix by applying a sequence of matrix manipulations.
Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
September 2025, with the intention that it be modified by students
to add functionality, as required by the assignment specification.
All included code is (c) Copyright University of Melbourne, 2025.
Authorship Declaration:
(1) I certify that except for the code provided in the initial skeleton file,
the program contained in this submission is completely my own individual
work, except where explicitly noted by further comments that provide details
otherwise. I understand that work that has been developed by another student,
or by me in collaboration with other students, or by non-students as a result
of request, solicitation, or payment, may not be submitted for assessment in
this subject. I understand that submitting for assessment work developed by
or in collaboration with other students or non-students constitutes Academic
Misconduct, and may be penalized by mark deductions, or by other penalties
determined via the University of Melbourne Academic Honesty Policy, as
described at https://academicintegrity.unimelb.edu.au.
(2) I also certify that I have not provided a copy of this work in either
softcopy or hardcopy or any other form to any other student, and nor will I
do so until after the marks are released. I understand that providing my work
to other students, regardless of my intention or any undertakings made to me
by that other student, is also Academic Misconduct.
(3) I further understand that providing a copy of the assignment specification
to any form of code authoring or assignment tutoring service, or drawing the
attention of others to such services and code that may have been made
available via such a service, may be regarded as Student General Misconduct
(interfering with the teaching activities of the University and/or inciting
others to commit Academic Misconduct). I understand that an allegation of
Student General Misconduct may arise regardless of whether or not I personally
make use of such solutions or sought benefit from such actions.
Signed by: Hoang Phuc Le
Dated: 25 September 2025
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"  // stage delimiter
#define THEEND "==THE END============================\n"   // end message

#define MTXDIM "%dx%d\n"  // matrix dimensions input format
#define DASHING \
    "-------------------------------------\n"  // dashing in between matricies
#define TADA "TA-DAA!!! SOLVED IN %d STEP(S)!\n"

#define MAXLINE 256  // maximum number of character in a line

/* TYPE DEFINITIONS ----------------------------------------------------------*/
// Compressed Sparse Row (CSR) matrix representation
typedef struct {
    int rows;   // number of rows in this matrix
    int cols;   // number of columns in this matrix
    int nnz;    // number of stored non-zeros values in this matrix
    int cap;    // matrix capacity to hold non-zero values
    int *vals;  // non-zero values in this matrix
    int *cidx;  // column indices of non-zero values, in row-major order
    int *rptr;  // row pointers
} CSRMatrix_t;

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void read_maxtrix_input(CSRMatrix_t *A, int cols);
void set_matrix(CSRMatrix_t *A, int r, int c, int v);
void resize_matrix(CSRMatrix_t *A);
void print_matrix(CSRMatrix_t *A);
int get_matrix_value(CSRMatrix_t *A, int r, int c);
void stage_0(CSRMatrix_t *A, CSRMatrix_t *B, int rows, int cols);

int read_instructions(CSRMatrix_t *A, CSRMatrix_t *B, int stage);
// stage 1 manipulations
void manipulation_s(CSRMatrix_t *A, int r, int c, int v);
void manipulation_S(CSRMatrix_t *A, int r1, int c1, int r2, int c2);
void manipulation_m(CSRMatrix_t *A, int v);
void manipulation_a(CSRMatrix_t *A, int v);
// stage 2 manipulations
void manipulation_r(CSRMatrix_t *A, int r1, int r2);
void manipulation_c(CSRMatrix_t *A, int c1, int c2);
void manipulation_R(CSRMatrix_t *A, int r1, int r2);
void manipulation_C(CSRMatrix_t *A, int r1, int r2);

int matrix_compare(CSRMatrix_t *A, CSRMatrix_t *B);
/* INTERFACE FUNCTIONS FOR WORKING WITH CSR MATRICES -------------------------*/
CSRMatrix_t *csr_matrix_create(int, int);  // create empty CSR matrix
void csr_matrix_free(CSRMatrix_t *);       // free input CSR matrix

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(void) {
    int stage = 0, rows, cols;
    printf(SDELIM, stage++);                   // print Stage 0 header
    assert(scanf(MTXDIM, &rows, &cols) == 2);  // assert matrix dimensions
    CSRMatrix_t *A =
        csr_matrix_create(rows, cols);  // create initial matrix of 0s
    CSRMatrix_t *B =
        csr_matrix_create(rows, cols);  // create target matrix of 0s
    // ...
    stage_0(A, B, rows, cols);

    printf(SDELIM, stage);  // print Stage 1 header
    int step = read_instructions(A, B, stage);
    printf(DASHING);
    printf(TADA, step);   // print TADA line
    printf(THEEND);       // print "THE END" message
    csr_matrix_free(A);   // free initial matrix
    csr_matrix_free(B);   // free target matrix
    return EXIT_SUCCESS;  // algorithms are fun!!!
}

// Create an empty CSR  matrix of nrows rows and ncols columns
CSRMatrix_t *csr_matrix_create(int nrows, int ncols) {
    assert(nrows >= 0 && ncols >= 0);  // check matrix dimensions
    // allocate memory for this matrix
    CSRMatrix_t *A = (CSRMatrix_t *)malloc(sizeof(CSRMatrix_t));
    assert(A != NULL);  // check if memory was allocated
    A->rows = nrows;    // set number of rows in the matrix
    A->cols = ncols;    // set number of columns in the matrix
    A->nnz = 0;         // initialize with no non-zero values
    A->cap = 0;         // initialize capacity to no non-zero values
    A->vals = NULL;     // no values to store...
    A->cidx = NULL;     // so there is no need to store column indices
    // allocate array to store row pointers
    A->rptr = (int *)malloc((size_t)(A->rows + 1) * sizeof(int));
    assert(A->rptr != NULL);
    for (int i = 0; i <= A->rows; i++) {  // no values, so initialize ...
        A->rptr[i] = 0;                   // ... all row pointers to zeros
    }
    return A;
}

// Free input CSR matrix A
void csr_matrix_free(CSRMatrix_t *A) {
    assert(A != NULL);
    free(A->vals);  // free matrix values
    free(A->cidx);  // free column indices
    free(A->rptr);  // free row pointers
    free(A);        // free matrix
}

// Function to read matrix from the input file
void read_matrix_input(CSRMatrix_t *A) {

    // Split each line and read them
    char line[MAXLINE];
    int r, c, v;

    // Loop to read in input data
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '#') {
            break;
        }
        if (sscanf(line, "%d,%d,%d", &r, &c, &v) == 3) {
            set_matrix(A, r, c, v);
        }
    }
}

// Function to set value for a matrix
void set_matrix(CSRMatrix_t *A, int r, int c, int v) {

    // Set initial values
    int start = A->rptr[r], end = A->rptr[r + 1], pos = start;

    // Find insertion point
    while (pos < end && A->cidx[pos] < c) {
        pos++;
    }

    // Found the element position,
    if (pos < end && A->cidx[pos] == c) {

        // If v = 0, delete the element
        if (v == 0) {

            // Shifts element to the left
            for (int i = pos; i < A->nnz - 1; i++) {
                A->vals[i] = A->vals[i + 1];
                A->cidx[i] = A->cidx[i + 1];
            }
            A->nnz--;
            for (int i = r + 1; i <= A->rows; i++) {
                A->rptr[i]--;
            }
        }

        // Or update the element
        else {
            A->vals[pos] = v;
        }
    }

    // If v != 0, resize and add new element
    else if (v != 0) {
        if (A->nnz == A->cap) {
            resize_matrix(A);
        }

        //  Shifts element to the right
        for (int i = A->nnz; i > pos; i--) {
            A->vals[i] = A->vals[i - 1];
            A->cidx[i] = A->cidx[i - 1];
        }
        A->vals[pos] = v;
        A->cidx[pos] = c;
        A->nnz++;
        for (int i = r + 1; i <= A->rows; i++) {
            A->rptr[i]++;
        }
    }
}

// Function to allocate dynamic memory to matrix
void resize_matrix(CSRMatrix_t *A) {

    // Reallocate memory into already exist structure
    // If out of memory, find memory that is twice as big, then reallocate
    int new_cap = (A->cap == 0) ? 1 : 2 * A->cap;
    A->vals = (int *)realloc(A->vals, sizeof(int) * new_cap);
    A->cidx = (int *)realloc(A->cidx, sizeof(int) * new_cap);
    assert(A->vals != NULL && A->cidx != NULL);
    A->cap = new_cap;
}

// Function to print matrix
void print_matrix(CSRMatrix_t *A) {

    // Check if input matrix should be printed as grid
    int print_as_grid = (A->rows <= 35 && A->cols <= 35);
    if (print_as_grid) {
        for (int i = 0; i < A->nnz; i++) {
            if (A->vals[i] < 0 || A->vals[i] > 9) {
                print_as_grid = 0;
                break;
            }
        }
    }

    // Print as grid
    if (print_as_grid) {
        for (int r = 0; r < A->rows; r++) {
            printf("[");
            for (int c = 0; c < A->cols; c++) {
                int val = get_matrix_value(A, r, c);
                if (val == 0) {
                    printf(" ");
                } else {
                    printf("%d", val);
                }
            }
            printf("]\n");
        }
    }

    // Print as list
    else {       
        for (int i = 0; i < A->nnz; i++) {
            int row = 0;
            while (row < A->rows && A->rptr[row + 1] <= i) {
                row++;
            }
            printf("(%d,%d)=%d\n", row, A->cidx[i], A->vals[i]);
        }
    }
}

// Function to get the matrix value given the position
int get_matrix_value(CSRMatrix_t *A, int r, int c) {
    // Set initial value on row
    int start = A->rptr[r];
    int end = A->rptr[r + 1];
    // Loop through row and check each columns to find the matching value
    for (int i = start; i < end; i++) {
        if (A->cidx[i] == c) {
            return A->vals[i];
        }
        if (A->cidx[i] > c) {
            break;
        }
    }
    return 0;
}

void stage_0(CSRMatrix_t *A, CSRMatrix_t *B, int rows, int cols) {

    // Read in initial and target matrix 
    read_matrix_input(A);
    read_matrix_input(B);

    // Print stage 0 content
    printf("Initial matrix: %dx%d, nnz=%d\n", rows, cols, A->nnz);
    print_matrix(A);
    printf(DASHING);
    printf("Target matrix: %dx%d, nnz=%d\n", rows, cols, B->nnz);
    print_matrix(B);
}

int read_instructions(CSRMatrix_t *A, CSRMatrix_t *B, int stage) {

    // Set initial values
    int step = 0;
    char line[MAXLINE];

    // While loop to read in inputs
    while (fgets(line, sizeof(line), stdin)) {

        // Determine manipulation types and it's matching parameters
        char manipulation_type;
        int param[4];
        int num_param = 0;

        // If input not suitable, break 
        if (sscanf(line, "%c:", &manipulation_type) != 1) {
            break;
        }
        // Read in parameters
        char *param_str = strchr(line, ':') + 1;    // Takes the address 
        char *token = strtok(param_str, ",\n");     // Takes the value 

        // Loop to set values in the param array
        while (token && num_param < 4) {
            param[num_param++] = atoi(token);
            token = strtok(NULL, ",\n");
        }

        // Check if it's still stage 1
        if (stage == 1 &&
            (manipulation_type == 'r' || manipulation_type == 'R' ||
             manipulation_type == 'c' || manipulation_type == 'C')) {
            stage++;
            printf(SDELIM, stage);
        }
        step++;

        // Print current manipulation type
        printf("INSTRUCTION %c:", manipulation_type);
        for (int i = 0; i < num_param; i++) {
            printf("%s%d", i == 0 ? "" : ",", param[i]);
        }

        // Perform matching manipulations
        switch (manipulation_type) {
            case 's':
                if (num_param == 3) {
                    manipulation_s(A, param[0], param[1], param[2]);
                }
                break;
            case 'S':
                if (num_param == 4) {
                    manipulation_S(A, param[0], param[1], param[2], param[3]);
                }
                break;
            case 'm':
                if (num_param == 1) {
                    manipulation_m(A, param[0]);
                }
                break;
            case 'a':
                if (num_param == 1) {
                    manipulation_a(A, param[0]);
                }
                break;
            case 'r':
                if (num_param == 2) {
                    manipulation_r(A, param[0], param[1]);
                }
                break;
            case 'R':
                if (num_param == 2) {
                    manipulation_R(A, param[0], param[1]);
                }
                break;
            case 'c':
                if (num_param == 2) {
                    manipulation_c(A, param[0], param[1]);
                }
                break;
            case 'C':
                if (num_param == 2) {
                    manipulation_C(A, param[0], param[1]);
                }
                break;
        }

        // Print stage 1-2 output
        printf("\nCurrent matrix: %dx%d, nnz=%d\n", A->rows, A->cols, A->nnz);
        print_matrix(A);
        printf("Target matrix: %dx%d, nnz=%d\n", B->rows, B->cols, B->nnz);
        print_matrix(B);

        // After manipulating, check if we've reached target
        if (matrix_compare(A, B)) {
            return step;
        }
    }
    return step;
}

// Stage 1 manipulations

// Set value of cell (r,c) to v
void manipulation_s(CSRMatrix_t *A, int r, int c, int v) {

    // Set value at (r,c) 
    set_matrix(A, r, c, v);
}

// Swap values in cells (r1, c1) and (r2, c2)
void manipulation_S(CSRMatrix_t *A, int r1, int c1, int r2, int c2) {

    // Get the v values
    int v1 = get_matrix_value(A, r1, c1);
    int v2 = get_matrix_value(A, r2, c2);

    // Swap v values
    set_matrix(A, r1, c1, v2);
    set_matrix(A, r2, c2, v1);
}

// Multiply non-zero values in the matrix by v
void manipulation_m(CSRMatrix_t *A, int v) {

    // Loop through non-zero values then multiply
    for (int i = 0; i < A->nnz; i++) {
        A->vals[i] *= v;
    }
}

// Add v to non-zero values in the matrix 
void manipulation_a(CSRMatrix_t *A, int v) {

    // Loop through non-zero values then add
    for (int i = 0; i < A->nnz; i++) {
        A->vals[i] += v;
    }
}

// stage 2 manipulations

// Copy all values from r1 to r2
void manipulation_r(CSRMatrix_t *A, int r1, int r2) {

    // Set the all the values in target rows to 0
    for (int c = 0; c < A->cols; c++) {
        set_matrix(A, r2, c, 0);
    }

    // Set initial values
    int start = A->rptr[r1];
    int end = A->rptr[r1 + 1];

    // Loop through rows and set new values
    for (int i = start; i < end; i++) {
        set_matrix(A, r2, A->cidx[i], A->vals[i]);
    }
}

// Copy all values from c1 to c2
void manipulation_c(CSRMatrix_t *A, int c1, int c2) {

    // Set the all the values in target column to 0
    for (int r = 0; r < A->rows; r++) {
        set_matrix(A, r, c2, 0);
    }

    // Loop to find non-zero values and set new values
    for (int r = 0; r < A->rows; r++) {
        int v = get_matrix_value(A, r, c1);
        if (v != 0) {
            set_matrix(A, r, c2, v);
        }
    }
}

// Swap the values in r1 and r2
void manipulation_R(CSRMatrix_t *A, int r1, int r2) {

    // Loop through columns to find values then swap 
    for (int c = 0; c < A->cols; c++) {
        int v1 = get_matrix_value(A, r1, c);
        int v2 = get_matrix_value(A, r2, c);

        set_matrix(A, r1, c, v2);
        set_matrix(A, r2, c, v1);
    }
}

// Swap the values in c1 and c2
void manipulation_C(CSRMatrix_t *A, int c1, int c2) {

    // Loop through rows to find values then swap 
    for (int r = 0; r < A->rows; r++) {
        int v1 = get_matrix_value(A, r, c1);
        int v2 = get_matrix_value(A, r, c2);

        set_matrix(A, r, c1, v2);
        set_matrix(A, r, c2, v1);
    }
}

// Function to compare 2 matrices
int matrix_compare(CSRMatrix_t *A, CSRMatrix_t *B) {

    // Check if their size and number of non-zero values match
    if (A->rows != B->rows || A->cols != B->cols || A->nnz != B->nnz) {
        return 0;
    }

    // Check if their row pointer match
    for (int i = 0; i < A->rows; i++) {
        if (A->rptr[i] != B->rptr[i]) {
            return 0;
        }
    }

    // Check if their column indices and non-zero values match
    for (int i = 0; i < A->nnz; i++) {
        if (A->cidx[i] != B->cidx[i] || A->vals[i] != B->vals[i]) {
            return 0;
        }
    }
    return 1;
}