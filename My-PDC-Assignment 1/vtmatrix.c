#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <immintrin.h>
//Started of by including all of our necessary libraries
//Creating a struct for the matrix dimensions
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

typedef struct 
{
    int thread_id;
    int num_threads;
    Matrix *matrix_a;
    Matrix *matrix_b;
    Matrix *finalmatrix;
} Threadinfo;
///////////////////////////////////////////////////////////////////////
//Creating a struct for the Threads information

///////////////////////////////////////////////////////////////////////
//This function is the one that is used to create the matrices
void create_matrix(Matrix *matrix, Matrixdtype data_type, int rows, int cols) 
{
    matrix->mtype = data_type;
    matrix->dimensions.rows = rows;
    matrix->dimensions.cols = cols;
   //assign all relevant values to matrix from file
//start of by allocating memory for the matrix data
    matrix->data = malloc(matrix->dimensions.rows * matrix->dimensions.cols * sizeof(double));
    int i;
    for (i = 0; i < matrix->dimensions.rows; i++) 
     {
        int j;
        for (j = 0; j < matrix->dimensions.cols; j++) 

           {
            //Matrix will be generated based on the data type encountered
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
void *multiply(void *thread_info) 
{
    Threadinfo *data = (Threadinfo *)thread_info;

    Matrix *matrix_a = data->matrix_a;

    Matrix *matrix_b = data->matrix_b;

    Matrix *finalmatrix = data->finalmatrix;

    int rowsA = matrix_a->dimensions.rows;

    int colsA = matrix_a->dimensions.cols;

    int colsB = matrix_b->dimensions.cols;

    int start_r = (data->thread_id * rowsA) / data->num_threads;

    int end_r = ((data->thread_id + 1) * rowsA) / data->num_threads;

    int i, j, k;

   for (i = start_r; i < end_r; i++) 
    {
        for ( j = 0; j < colsB; j++) 
        {
            __m128i current = _mm_loadu_si128((__m128i *)(finalmatrix->data + (i * finalmatrix->dimensions.cols + j) * sizeof(int)));

            for ( k = 0; k < colsA; k += 4) 
            {
                // Use the SSE for int32 matrix multiplication
                __m128i a = _mm_loadu_si128((__m128i *)(matrix_a->data + (i * colsA + k) * sizeof(int)));
                __m128i b = _mm_loadu_si128((__m128i *)(matrix_b->data + (k * colsB + j) * sizeof(int)));
                __m128i result = _mm_mullo_epi32(a, b);
                current = _mm_add_epi32(current, result);
            }

            _mm_storeu_si128((__m128i *)(finalmatrix->data + (i * finalmatrix->dimensions.cols + j) * sizeof(int)), current);
        }

        printf("Progress: Completed row %d/%d\n", i + 1, rowsA);
    }

    pthread_exit(NULL);
}

///////////////////////////////////////////////////////////////////////
Matrix multiply_with_threads(Matrix matrix_a, Matrix matrix_b, int num_threads) 
{
    Matrix finalmatrix;
    finalmatrix.mtype = matrix_a.mtype;

    finalmatrix.dimensions.rows = matrix_a.dimensions.rows;

    finalmatrix.dimensions.cols = matrix_b.dimensions.cols;
    finalmatrix.data = malloc(finalmatrix.dimensions.rows * finalmatrix.dimensions.cols * sizeof(double));

    pthread_t threads[num_threads];

    Threadinfo thread_data[num_threads];

    int i;
    for (i = 0; i < num_threads; ++i) 
    {
        thread_data[i].thread_id = i ;
        thread_data[i].num_threads = num_threads;
        thread_data[i].matrix_a = &matrix_a;
        thread_data[i].matrix_b = &matrix_b;
        thread_data[i].finalmatrix = &finalmatrix;

        pthread_create(&threads[i], NULL, multiply, (void*)&thread_data[i]);
    }

    for (i = 0; i < num_threads; ++i) 

    {
        pthread_join(threads[i], NULL);
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

    // Use a predefined number of threads, e.g., 8
    int num_threads = 16;

    Matrix finalmatrix = multiply_with_threads(matrix_a, matrix_b, num_threads);

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

