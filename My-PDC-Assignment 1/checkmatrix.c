#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Started of by including all of our necessary libraries
//Creating a struct for the matrix dimensions
///////////////////////////////////////////////////////////////////////
typedef struct 
{
    int rows;
    int cols;
} M_dimensions;
//Creating a struct for the matrix 
///////////////////////////////////////////////////////////////////////
typedef struct 
{
    int mtype;
    M_dimensions dimensions;
    void *data;
} Matrix;
//Creating an enum for the matrix type
///////////////////////////////////////////////////////////////////////
typedef enum
{
    int32 = 1,
    float32 = 2,
    int64 = 3,
    float64 = 4
} Matrixdtype;
///////////////////////////////////////////////////////////////////////
void create_matrix(Matrix *matrix, Matrixdtype data_type, int rows, int cols) 
{
    matrix->mtype = data_type;

    matrix->dimensions.rows = rows;

    matrix->dimensions.cols = cols;

    matrix->data = malloc(matrix->dimensions.rows * matrix->dimensions.cols * sizeof(double));

    int i;
    for (i = 0; i < matrix->dimensions.rows; i++) 
     {
        int j;

        for (j = 0; j < matrix->dimensions.cols; j++) 

           {
            if (matrix->mtype == int32) 
            {
                *((int *)(matrix->data + (i * matrix->dimensions.cols + j) * sizeof(int))) = rand() % 100;

            } 

            else if (matrix->mtype == float32) 
            {
                *((float *)(matrix->data + (i * matrix->dimensions.cols + j) * sizeof(float))) = (float)rand() / RAND_MAX;

            } 

            else if (matrix->mtype == int64)
 
            {
                *((long *)(matrix->data + (i * matrix->dimensions.cols + j) * sizeof(long))) = rand() % 100;

            } 


            else if (matrix->mtype == float64) 
            {
                *((double *)(matrix->data + (i * matrix->dimensions.cols + j) * sizeof(double))) = (double)rand() / RAND_MAX;

            }
        }
    }
}

///////////////////////////////////////////////////////////////////////
Matrix multiply(Matrix matrix_a, Matrix matrix_b) 
{
    Matrix result_matrix;


    result_matrix.mtype = matrix_a.mtype;
    result_matrix.dimensions.rows = matrix_a.dimensions.rows;

    result_matrix.dimensions.cols = matrix_b.dimensions.cols;

    result_matrix.data = malloc(result_matrix.dimensions.rows * result_matrix.dimensions.cols * sizeof(double));

    int rowsA = matrix_a.dimensions.rows;


    int colsA = matrix_a.dimensions.cols;

    int colsB = matrix_b.dimensions.cols;
int i;

for (i = 0; i < rowsA; ++i) 

{
    int j;

    for (j = 0; j < colsB; ++j) 
{
        int k;

        for (k = 0; k < colsA; ++k) 

          {
            if (result_matrix.mtype == int32) 

           {
                *((int *)(result_matrix.data + (i * result_matrix.dimensions.cols + j) * sizeof(int))) +=
                    *((int *)(matrix_a.data + (i * colsA + k) * sizeof(int))) *
                    *((int *)(matrix_b.data + (k * colsB + j) * sizeof(int)));
            } 

           else if (result_matrix.mtype == float32)


           {
                *((float *)(result_matrix.data + (i * result_matrix.dimensions.cols + j) * sizeof(float))) +=
                    *((float *)(matrix_a.data + (i * colsA + k) * sizeof(float))) *
                    *((float *)(matrix_b.data + (k * colsB + j) * sizeof(float)));
            } 

          else if (result_matrix.mtype == int64) 

           {
                *((long *)(result_matrix.data + (i * result_matrix.dimensions.cols + j) * sizeof(long))) +=
                    *((long *)(matrix_a.data + (i * colsA + k) * sizeof(long))) *
                    *((long *)(matrix_b.data + (k * colsB + j) * sizeof(long)));
            } 

          else if (result_matrix.mtype == float64) 

            {
                *((double *)(result_matrix.data + (i * result_matrix.dimensions.cols + j) * sizeof(double))) +=
                    *((double *)(matrix_a.data + (i * colsA + k) * sizeof(double))) *
                    *((double *)(matrix_b.data + (k * colsB + j) * sizeof(double)));
            }
        }
    }


        printf("Progress: Completed row %d/%d\n", i + 1, rowsA);
    }

    return result_matrix;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void write_output(FILE *file, Matrix finalmatrix, double execution_time) 
{
    fprintf(file, "%d\n", finalmatrix.mtype);

    fprintf(file, "%d %d\n", finalmatrix.dimensions.rows, finalmatrix.dimensions.cols);

    int i, j;

    for (i = 0; i < finalmatrix.dimensions.rows; i++) 

      {
        for (j = 0; j < finalmatrix.dimensions.cols; j++) 

           {
            if (finalmatrix.mtype == int32) 
            {
                fprintf(file, "%d ", *((int *)(finalmatrix.data + (i * finalmatrix.dimensions.cols + j) * sizeof(int))));

            } 

           else if (finalmatrix.mtype == float32) 

            {
                fprintf(file, "%f ", *((float *)(finalmatrix.data + (i * finalmatrix.dimensions.cols + j) * sizeof(float))));

            } 

           else if (finalmatrix.mtype == int64) 
            {
                fprintf(file, "%ld ", *((long *)(finalmatrix.data + (i * finalmatrix.dimensions.cols + j) * sizeof(long))));

            } 
          else if (finalmatrix.mtype == float64) 

            {
                fprintf(file, "%lf ", *((double *)(finalmatrix.data + (i * finalmatrix.dimensions.cols + j) * sizeof(double))));

            }
        }
        fprintf(file, "\n");
    }

    fprintf(file, "Execution Time: %lf seconds\n", execution_time);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) 
{


    srand(time(NULL)); // random number generation

    char *input_file_path = argv[1];

    char *output_file_path = argv[2];

    FILE *input_file = fopen(input_file_path, "r");

    int opcode, matrix1, matrix2;

    fscanf(input_file, "%d", &opcode);

    M_dimensions matrix1dimensions, matrix2dimensions;

    fscanf(input_file, "%d", &matrix1);

    fscanf(input_file, "%d %d", &matrix1dimensions.rows, &matrix1dimensions.cols);

    fscanf(input_file, "%d", &matrix2);

    fscanf(input_file, "%d %d", &matrix2dimensions.rows, &matrix2dimensions.cols);

    fclose(input_file);

    Matrix matrix_1, matrix_2;

    create_matrix(&matrix_1, matrix1, matrix1dimensions.rows, matrix1dimensions.cols);

    create_matrix(&matrix_2, matrix2, matrix2dimensions.rows, matrix2dimensions.cols);

    struct timespec start, end;

    double execution_time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    Matrix finalmatrix = multiply(matrix_1, matrix_2);

    clock_gettime(CLOCK_MONOTONIC, &end);

    execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Execution Time: %lf seconds\n", execution_time);

    FILE *output_file = fopen(output_file_path, "w");
  
    write_output(output_file, finalmatrix, execution_time);

    fclose(output_file);
///////////////////////////////////////////////////////////////////////
    // Free allocated memory
    free(matrix_1.data);
    free(matrix_2.data);
    free(finalmatrix.data);
///////////////////////////////////////////////////////////////////////
    return 0;
}
