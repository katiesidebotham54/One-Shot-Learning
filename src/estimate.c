#include <stdio.h>
#include <stdlib.h>

void print(int row, int col, double **trainingX, double *trainingY, double **transposeX, double **productX, double **inversedX, double **finalX, double *W, double **inputX, double *finalPrices)
{
    printf("TrainingX Matrix: \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%lf ", trainingX[i][j]);
        }
        printf("\n");
    }

    printf("\nVector Y: \n");
    for (int i = 0; i < row; i++)
    {
        printf("%lf \n", trainingY[i]);
    }

    printf("\nTranspose X Matrix: \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%lf ", transposeX[i][j]);
        }
        printf("\n");
    }

    printf("\nProductX Matrix2 : \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%lf ", productX[i][j]);
        }
        printf("\n");
    }

    printf("\nInversed X Matrix: \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%lf ", inversedX[i][j]);
        }
        printf("\n");
    }

    printf("\nFinalX Matrix: \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%lf ", finalX[i][j]);
        }
        printf("\n");
    }

    printf("\nW: \n");
    for (int i = 0; i < row; i++)
    {
        printf("%lf \n", W[i]);
    }

    printf("\nInputX Matrix: \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%lf ", inputX[i][j]);
        }
        printf("\n");
    }
    printf("\nFinal Prices: \n");
    for (int i = 0; i < row; i++)
    {
        printf("%.0f\n", finalPrices[i]);
    }
}

double **inverse(double **M, int n)
{
    double **N = (double **)malloc(n * sizeof(double *));
    N[0] = (double *)malloc(sizeof(double) * n * n);

    for (int m = 1; m < n; m++)
    {
        N[m] = N[m - 1] + n;
    }
    //traverse and insert 1 in corresponding diagonol spots
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                N[i][j] = 1.0;
            }
            else
            {
                N[i][j] = 0.0;
            }
        }
    }
    double pivot;
    for (int i = 0; i < n; i++)
    {
        //gets pivot position
        pivot = M[i][i];
        //traverse row and divide each by pivot
        for (int a = 0; a < n; a++)
        {
            M[i][a] /= pivot;
            N[i][a] /= pivot;
        }
        for (int j = i + 1; j < n; j++)
        {
            pivot = M[j][i];
            for (int b = 0; b < n; b++)
            {
                M[j][b] -= (M[i][b] * pivot);
                N[j][b] -= (N[i][b] * pivot);
            }
        }
    }
    //reverse to make identity matrix
    for (int p = n - 1; p >= 0; p--)
    {
        for (int i = p - 1; i >= 0; i--)
        {
            pivot = M[i][p];
            for (int b = 0; b < n; b++)
            {
                M[i][b] -= (M[p][b] * pivot);
                N[i][b] -= (N[p][b] * pivot);
            }
        }
    }
    return N;
}

double **transpose(double **X, int row, int col)
{
    double **transposeX = (double **)malloc(col * sizeof(double *));
    transposeX[0] = (double *)malloc(sizeof(double) * row * col);
    for (int i = 1; i < col; i++)
    {
        transposeX[i] = transposeX[i - 1] + row;
    }
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            transposeX[i][j] = X[j][i];
        }
    }
    return transposeX;
}

double **multiply(double **matrix1, double **matrix2, int row1, int col1, int row2, int col2)
{

    double **product = (double **)malloc(row1 * sizeof(double *));
    product[0] = (double *)malloc(sizeof(double) * row1 * col2);
    for (int i = 1; i < row1; i++)
    {
        product[i] = product[i - 1] + col2;
    }

    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col2; j++)
        {
            product[i][j] = 0;
            for (int k = 0; k < col1; k++)
            {
                product[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return product;
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("error\n");
        return 1;
    }

    FILE *fp1;
    FILE *fp2;
    char *filename1 = malloc(10);
    char *filename2 = malloc(10);

    int col, row, attrs, houses;
    char buf1[10];
    char buf2[10];
    fp1 = fopen(argv[1], "r");
    fp2 = fopen(argv[2], "r");
    if (!fp1 || !fp2)
    {
        printf("error\n");
        return 1;
    }
    fscanf(fp1, "%s", buf1);
    fscanf(fp1, "%d\n", &col);
    fscanf(fp1, "%d\n", &row);

    fscanf(fp2, "%s", buf2);
    fscanf(fp2, "%d\n", &attrs);
    fscanf(fp2, "%d\n", &houses);
    if (attrs != col)
    {
        printf("error 2\n");
        return 1;
    }
    col++;
    attrs++;

    double **trainingX = (double **)malloc(row * sizeof(double *));
    trainingX[0] = (double *)malloc(sizeof(double) * col * row);
    for (int i = 1; i < row; i++)
    {
        trainingX[i] = trainingX[i - 1] + col;
    }
    double *trainingY = (double *)malloc(row * sizeof(double));
    for (int r = 0; r < row; r++)
    {
        trainingX[r][0] = 1;
        for (int c = 1; c < col; c++)
        {
            fscanf(fp1, "%lf", &trainingX[r][c]);
        }
        fscanf(fp1, "%lf", &trainingY[r]);
    }

    double **inputX = (double **)malloc(houses * sizeof(double *));
    inputX[0] = (double *)malloc(sizeof(double) * attrs * houses);
    for (int i = 1; i < houses; i++)
    {
        inputX[i] = inputX[i - 1] + attrs;
    }
    for (int r = 0; r < houses; r++)
    {
        inputX[r][0] = 1;
        for (int c = 1; c < attrs; c++)
        {
            fscanf(fp2, "%lf", &inputX[r][c]);
        }
    }
    //results in col x row
    double **transposeX = transpose(trainingX, row, col);
    //results in col x col
    double **productX = multiply(transposeX, trainingX, col, row, row, col);
    // results in col x col
    double **inversedX = inverse(productX, col);
    //results in col x row
    double **finalX = multiply(inversedX, transposeX, col, col, col, row);

    //results in a col x 1
    double *W = (double *)malloc(row * sizeof(double));
    for (int i = 0; i < col; i++)
    {
        double c = 0;
        for (int j = 0; j < row; j++)
        {
            c += (trainingY[j] * finalX[i][j]);
        }
        W[i] = c;
    }
    double *finalPrices = (double *)malloc(houses * sizeof(double));
    for (int i = 0; i < houses; i++)
    {
        double c = 0;
        for (int j = 0; j < attrs; j++)
        {
            c += (W[j] * inputX[i][j]);
        }
        finalPrices[i] = c;
    }
    //print(row, col, trainingX, trainingY, transposeX, productX, inversedX, finalX, W, inputX, finalPrices);

    for (int i = 0; i < houses; i++)
    {
        printf("%.0f\n", finalPrices[i]);
    }

    free(trainingX[0]);
    free(trainingX);
    free(trainingY);
    free(transposeX[0]);
    free(transposeX);
    free(productX[0]);
    free(productX);
    free(inversedX[0]);
    free(inversedX);
    free(finalX[0]);
    free(finalX);
    free(W);
    free(inputX[0]);
    free(inputX);
    free(finalPrices);
    free(filename1);
    free(filename2);

    return 0;
}
