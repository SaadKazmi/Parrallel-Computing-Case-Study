#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>
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
    int i, j;

    for (i = 0; i < matrix->dimensions.rows; i++) 

    {
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
///////////////////////////////////////////////////////////////////////
Matrix multiply(Matrix matrix_a, Matrix matrix_b) 
{
    Matrix finalmatrix;
    finalmatrix.mtype = matrix_a.mtype;

    finalmatrix.dimensions.rows = matrix_a.dimensions.rows;

    finalmatrix.dimensions.cols = matrix_b.dimensions.cols;

    finalmatrix.data = malloc(finalmatrix.dimensions.rows * finalmatrix.dimensions.cols * sizeof(double));

    int rowsA = matrix_a.dimensions.rows;

    int colsA = matrix_a.dimensions.cols;

    int colsB = matrix_b.dimensions.cols;
    int i;

    for ( i = 0; i < rowsA; ++i) 
    {
        int j;

        for ( j = 0; j < colsB; j += 2) 
          {
            __m128d sum0 = _mm_setzero_pd();

            __m128d sum1 = _mm_setzero_pd();

            int k;

            for ( k = 0; k < colsA; k++) 
            {
                // two elements from matrix_a
                __m128d a = _mm_loadu_pd((double *)(matrix_a.data + (i * colsA + k) * sizeof(double)));

                // two elements from matrix_b
                __m128d b0 = _mm_loadu_pd((double *)(matrix_b.data + (k * colsB + j) * sizeof(double)));
                __m128d b1 = _mm_loadu_pd((double *)(matrix_b.data + (k * colsB + j + 1) * sizeof(double)));

                //  for the first sum
                sum0 = _mm_add_pd(sum0, _mm_mul_pd(a, b0));

                //  for the second sum
                sum1 = _mm_add_pd(sum1, _mm_mul_pd(a, b1));
            }

            // Store results in the result_matrix
            _mm_storeu_pd((double *)(finalmatrix.data + (i * finalmatrix.dimensions.cols + j) * sizeof(double)), sum0);

            _mm_storeu_pd((double *)(finalmatrix.data + (i * finalmatrix.dimensions.cols + j + 1) * sizeof(double)), sum1);

        }

        printf("Progress: Completed row %d/%d\n", i + 1, rowsA);
    }

    return finalmatrix;
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
int main(int argc, char *argv[])
 
{
    srand(time(NULL)); // random number generation

    char *input_file_path = argv[1];

    char *output_file_path = argv[2];

    FILE *input_file = fopen(input_file_path, "r");

    int opcode, matrix_a_type, matrix_b_type;

    fscanf(input_file, "%d", &opcode);

    M_dimensions matrix_a_dimensions, matrix_b_dimensions;

    fscanf(input_file, "%d", &matrix_a_type);

    fscanf(input_file, "%d %d", &matrix_a_dimensions.rows, &matrix_a_dimensions.cols);

    fscanf(input_file, "%d", &matrix_b_type);

    fscanf(input_file, "%d %d", &matrix_b_dimensions.rows, &matrix_b_dimensions.cols);

    fclose(input_file);

    Matrix matrix_a, matrix_b;

    create_matrix(&matrix_a, matrix_a_type, matrix_a_dimensions.rows, matrix_a_dimensions.cols);

    create_matrix(&matrix_b, matrix_b_type, matrix_b_dimensions.rows, matrix_b_dimensions.cols);

    struct timespec startt, endt;

    double execution_time;

    clock_gettime(CLOCK_MONOTONIC, &startt);

    Matrix finalmatrix = multiply(matrix_a, matrix_b);

    clock_gettime(CLOCK_MONOTONIC, &endt);

    execution_time = (endt.tv_sec - startt.tv_sec) + (endt.tv_nsec - startt.tv_nsec) / 1e9;
    printf("Execution Time: %lf seconds\n", execution_time);

    FILE *output_file = fopen(output_file_path, "w");

    write_output(output_file, finalmatrix, execution_time);

    fclose(output_file);
///////////////////////////////////////////////////////////////////////
    // Free allocated memory
    free(matrix_a.data);
    free(matrix_b.data);
    free(finalmatrix.data);
///////////////////////////////////////////////////////////////////////
    return 0;
}

