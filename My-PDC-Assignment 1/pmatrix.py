import numpy as np
import sys
import time
#i imported all these libraries as they proved necessary
##################################################################################
file_path = sys.argv[1] #Read input file path of our matrix
output_file = sys.argv[2] #Output the result of our multiplication

#This was done to utilize the numpy library so that
#each matrix type can be utilized as a numpy library value
#this is helpful for each computation
matrix_type = {
    1: np.int32,
    2: np.float32,
    3: np.int64,
    4: np.float64
}

matrix_type = {v: k for k, v in matrix_type.items()}
##################################################################################
#This will get our necessary info
def info(file):
    matrix_type = int(file.readline().strip()) #get our matrix type
    matrix_dimensions = tuple(map(int, file.readline().strip().split()))
    #get the necessary dimensions

    matrix_rows = []
    for _ in range(matrix_dimensions[0]):
        row_data = list(map(matrix_type[matrix_type], file.readline().strip().split()))
        #Here the data of each row is recevied 
        matrix_rows.append(row_data)

    return np.array(matrix_rows)#final return statement to return each row

##################################################################################
#This function will actually multiply our two matrices and give the result
def multiply(matrix_a, matrix_b):
    rowsA, colsA = matrix_a.shape
    colsB = matrix_b.shape[1]
    result_matrix = np.zeros((rowsA, colsB), dtype=matrix_a.dtype)

    for i in range(rowsA):

        for j in range(colsB):

            for k in range(colsA):

                result_matrix[i, j] += matrix_a[i, k] * matrix_b[k, j]

        # Tracking the computational progress
        print(f"Progress: Completed row {i + 1}/{rowsA}")

    return result_matrix
##################################################################################
#This function will write the result of our computation
def output(file, finalmatrix, execution_time):

    data_type_str = matrix_type[type(finalmatrix[0, 0])]

    file.write(str(data_type_str) + '\n') #Write the data type to output file

    file.write(' '.join(map(str, finalmatrix)) + '\n') #begin the output
    #of final matrix

    for row in finalmatrix:
        file.write(' '.join(map(str, row)) + '\n')

    file.write(f"Execution Time: {execution_time} seconds\n")
    #Also write output to the file the execution time
##################################################################################
#This is where we actually open our file
with open(file_path, 'r') as input_file:
    opcode = int(input_file.readline().strip()) #Receive the opcode

    matrix_a_type = int(input_file.readline().strip())#The type of matrix
    matrix_a_dimensions = tuple(map(int, input_file.readline().strip().split()))
    #Matrix dimensions
    if matrix_a_type == 1:#if int32
        matrix_a = np.random.randint(0, 100, size=matrix_a_dimensions, dtype=np.int32)

    elif matrix_a_type == 2:#if float32
        matrix_a = np.random.rand(*matrix_a_dimensions).astype(np.float32)

    elif matrix_a_type == 3:#if int64
        matrix_a = np.random.randint(0, 100, size=matrix_a_dimensions, dtype=np.int64)

    elif matrix_a_type == 4:#if float64
        matrix_a = np.random.rand(*matrix_a_dimensions).astype(np.float64)
    #The same for the second matrix
    matrix_b_type = int(input_file.readline().strip())
    matrix_b_dimensions = tuple(map(int, input_file.readline().strip().split()))
   #Repeat the same process
    if matrix_b_type == 1:
        matrix_b = np.random.randint(0, 100, size=matrix_b_dimensions, dtype=np.int32)

    elif matrix_b_type == 2:
        matrix_b = np.random.rand(*matrix_b_dimensions).astype(np.float32)

    elif matrix_b_type == 3:
        matrix_b = np.random.randint(0, 100, size=matrix_b_dimensions, dtype=np.int64)

    elif matrix_b_type == 4:
        matrix_b = np.random.rand(*matrix_b_dimensions).astype(np.float64)

##################################################################################
#If the opcode matched then begin overall procedure
if opcode == 1:

    startt= time.time()
    finalmatrix = multiply(matrix_a, matrix_b)
    endt = time.time()

    multiplication_time = endt - startt

    print("Execution time for matrix multiplication is:", multiplication_time, "seconds")

    with open(output_file, 'w') as output_file:

        output(output_file, finalmatrix, multiplication_time)


#Otherwise output an error
else:
    print("OPCODE can be only 1")
    sys.exit(1)
