import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;
//These were all the libraries that would be required for the process
//We started by making a class for the Matrix dimensions
///////////////////////////////////////////////////////////////////////////
class MatrixDimensions 
{
    int rows;//matrix rows
    int cols;//matrix columns

    MatrixDimensions(int rows, int cols) 
    
    {
        this.rows = rows;//give to the obj
        this.cols = cols;//give to the obj
    }
}
///////////////////////////////////////////////////////////////////////////
//I made a class for the matrix
class Matrix 
{
    int type;//The type of matrix
    MatrixDimensions dimensions;//Its dimensions
    Object data;//The data it will hold
   //Perform object assignment then

    Matrix(int type, MatrixDimensions dimensions) 
    {
        this.type = type;
        this.dimensions = dimensions;
        this.data = new Number[dimensions.rows][dimensions.cols];
    }
}
///////////////////////////////////////////////////////////////////////////
//The enumerator will hold all the matrix types that we have to compute for
enum MatrixType 
{
    INT32(1),
    FLOAT32(2),
    INT64(3),
    FLOAT64(4);

    int value;

    MatrixType(int value) 
    {
        this.value = value;//again obj assignment
    }
}
///////////////////////////////////////////////////////////////////////////
//This is actually the main class of the code
public class javamatrix 
{

    static Random random = new Random();//Used to randomly generate our matrix numbers

    static void create_matrix(Matrix matrix, MatrixType dataType, int rows, int cols) 
    {
        matrix.type = dataType.value;

        matrix.dimensions = new MatrixDimensions(rows, cols);

        Number[][] matrixData = (Number[][]) matrix.data;

        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < cols; j++) 
            {
                //In java switch seemed to be a better option to check to see
                //What type of matrix was encountered
                switch (dataType) 
                {
                    case INT32://int32
                        matrixData[i][j] = random.nextInt(100);

                        break;

                    case FLOAT32://float32
                        matrixData[i][j] = random.nextFloat();

                        break;

                    case INT64://int64
                        matrixData[i][j] = random.nextLong() % 100;

                        break;

                    case FLOAT64://float64
                        matrixData[i][j] = random.nextDouble();

                        break;
                }
            }
        }
    }
///////////////////////////////////////////////////////////////////////////
       //This function will perform the multiplication
       static Matrix multiply(Matrix matrixA, Matrix matrixB) 
       {
           int rowsA = matrixA.dimensions.rows;
           int colsA = matrixA.dimensions.cols;
           int colsB = matrixB.dimensions.cols;
           //Compute the resultant matrix
           Matrix resultMatrix = new Matrix(matrixA.type, new MatrixDimensions(rowsA, colsB));

           Number[][] resultData = (Number[][]) resultMatrix.data;
   
           Number[][] dataA = (Number[][]) matrixA.data;

           Number[][] dataB = (Number[][]) matrixB.data;
   
           for (int i = 0; i < rowsA; i++) 
           {
               for (int j = 0; j < colsB; j++) 
               {
                   resultData[i][j] = 0;
   
                   for (int k = 0; k < colsA; k++)
                    {
                       resultData[i][j] = resultData[i][j].doubleValue() +

                               (dataA[i][k].doubleValue() * dataB[k][j].doubleValue());
                   }
               }
   
               
               System.out.println("Progress: Completed row " + (i + 1) + "/" + rowsA);
           }
   
           return resultMatrix;
       }
       ///////////////////////////////////////////////////////////////////////////
   //This function will actually wtrite the result of the computation to the output file
    static void Output(FileWriter file, Matrix resultMatrix, double executionTime) throws IOException 

    {
        file.write(resultMatrix.type + "\n");//Write type

        file.write(resultMatrix.dimensions.rows + " " + resultMatrix.dimensions.cols + "\n");
        //Write the data along with the dimensions of matrix
        Number[][] matrixData = (Number[][]) resultMatrix.data;
        for (int i = 0; i < resultMatrix.dimensions.rows; i++) 
        {
            for (int j = 0; j < resultMatrix.dimensions.cols; j++) 
            {
                file.write(matrixData[i][j] + " ");
            }

            file.write("\n");
        }
      //Then finally also write the computation time
        file.write("Execution Time: " + executionTime + " seconds\n");
    }
///////////////////////////////////////////////////////////////////////////

    public static void main(String[] args) throws IOException 
    {
      

        String inputFilePath = args[0];

        String outputFilePath = args[1];

        try (BufferedReader inputFile = new BufferedReader(new FileReader(inputFilePath));
             FileWriter outputFile = new FileWriter(outputFilePath)) 
             {

            int opcode = Integer.parseInt(inputFile.readLine().trim());
///////////////////////////////////////////////////////////////////////////
            int matrixAType = Integer.parseInt(inputFile.readLine().trim());
            String[] matrixADimensions = inputFile.readLine().trim().split(" ");

            int matrixARows = Integer.parseInt(matrixADimensions[0]);
            int matrixACols = Integer.parseInt(matrixADimensions[1]);

            int matrixBType = Integer.parseInt(inputFile.readLine().trim());
            String[] matrixBDimensions = inputFile.readLine().trim().split(" ");

            int matrixBRows = Integer.parseInt(matrixBDimensions[0]);
            int matrixBCols = Integer.parseInt(matrixBDimensions[1]);

            Matrix matrixA = new Matrix(matrixAType, new MatrixDimensions(matrixARows, matrixACols));
            Matrix matrixB = new Matrix(matrixBType, new MatrixDimensions(matrixBRows, matrixBCols));

            create_matrix(matrixA, MatrixType.values()[matrixAType - 1], matrixARows, matrixACols);
            create_matrix(matrixB, MatrixType.values()[matrixBType - 1], matrixBRows, matrixBCols);

            long startTime = System.currentTimeMillis();

            Matrix resultMatrix = multiply(matrixA, matrixB);

            long endTime = System.currentTimeMillis();
            
            double executionTime = (endTime - startTime) / 1000.0;

            System.out.println("Execution Time: " + executionTime + " seconds");

            Output(outputFile, resultMatrix, executionTime);
        }
    }
}
///////////////////////////////////////////////////////////////////////////