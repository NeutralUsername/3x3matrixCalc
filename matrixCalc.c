#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void printMatrix(int length, int *matrix);
void createMatrix(int *numbers, char identifier);
FILE *loadMatrix(char identifier);
int *getMatrixFromStream(FILE *fp);
int *multiply3x3Matrices(int *matrixA, int* matrixB);
char readMatrixIdentifier(char m);
int quit();

int main(int argc, char *argv[]){
    FILE *fpA;
    FILE *fpB;
    while(1) { // Loop until user quits
        do { // Loop until user enters existing matrix identifier for matrix A
            fpA = loadMatrix(readMatrixIdentifier('A'));  // request user to enter matrix identifier. try to load matrix from file. if user enters '+' he will be asked to create new a new matrix
        } while(fpA == NULL);
        do { // same as for matrix B
            fpB = loadMatrix(readMatrixIdentifier('B'));
        } while(fpB == NULL);
        int *aMatrix = getMatrixFromStream(fpA); // get matrix as int array from file stream
        int *bMatrix = getMatrixFromStream(fpB); // get matrix as int array from file stream
        printMatrix(9, aMatrix); // print int array as matrix
        printMatrix(9, bMatrix); // print int array as matrix
        int *resultMatrix = multiply3x3Matrices(aMatrix, bMatrix); // multiply matrices and return result as int array
        createMatrix(resultMatrix, readMatrixIdentifier('+')); // create new matrix from result int array. user will be asked to enter matrix identifier. if user enters '+' he will be asked to create new a new matrix
        printMatrix(9, resultMatrix); // print result matrix
        free(aMatrix); // free memory
        free(bMatrix); // free memory
        free(resultMatrix); // free memory
        if(quit()) break; 
    }
    return 0;
}

FILE *loadMatrix(char identifier) {
    char filename[13];
    sprintf(filename, "matrix_%c.txt\0", identifier); // create filename from identifier
    FILE *fp = fopen(filename, "r"); // open file
    return fp;
}
int *getMatrixFromStream(FILE *fp) {
    int *matrix = malloc(sizeof(int) * 9); // allocate memory for matrix
    int i = 0;
    int num = 0;
    int c;
    do { // read stream until EOF
        c = fgetc(fp); // read character from file
        if(isdigit(c)) {  // if character is digit
            num = num * 10 + (c - '0'); // multiply num by 10(decimal) and add new digit
        }
        else {
            matrix[i] = num; //number composition finished. save number to matrix
            num = 0; // reset number
            i++; // increment matrix index
        }
    } while(c != EOF); // loop until end of file. 
    return matrix;
}

int *readMatrixValues(){
    int *nrs = malloc(9 * sizeof(int));
    for(int i= 0; i < 9; i++) {
        printf("enter integer %d:", i);
        char t; //temp to clear buffer
        while(1) {
            if(scanf("%d", &nrs[i]) == 1) { // if scanf returns 1, it means it read an integer
                while((t = getchar()) != '\n' && t != EOF); // clear buffer
                break;
                
            }
            else while((t = getchar()) != '\n' && t != EOF); // clear buffer
            printf("not a valid number, try again:");
        }
        printMatrix(i+1, nrs);
    }
    return nrs;
}

char readMatrixIdentifier(char m) {
    char c;
    char t; //temp to clear buffer
    while(1) {
        if(m == 'A' || m == 'B')
            printf("enter matrix %c identifier:", m);
        else printf("enter new matrix identifier:");
        c = getchar();
        if((isalpha(c) || isdigit(c))) { // if character is alpha or digit
            while((t = getchar()) != '\n' && t != EOF); // clear buffer
            return c; // return character
        }
        else if(c == '+') { // if character is '+'
            while((t = getchar()) != '\n' && t != EOF); // clear buffer 
            int *nrs = readMatrixValues(); // read matrix values from user
            createMatrix(nrs, readMatrixIdentifier('+')); // read matrix identifier from user and create new matrix
            free(nrs); // free memory
        } 
        else while((t = getchar()) != '\n' && t != EOF); // clear buffer
    }
}

void createMatrix(int *numbers, char identifier) {
    FILE *fp; //file pointer 
    char filename[13]; //filename. format : matrix_[identifier].txt
    sprintf(filename, "matrix_%c.txt\0", identifier);  // create filename from identifier
    fp = fopen(filename, "w"); 
    for(int i = 0; i < 9; i++) {
        fprintf(fp, "%d ", numbers[i]);
    }
    printf("'%s' created\n", filename);
    fclose(fp); // close file, save changes and free memory
}

int *multiply3x3Matrices(int *matrixA, int* matrixB) {
    int *resultMatrix = malloc(9 * sizeof(int));
    int i, j, k;
    for(i = 0; i < 3; i++) { // loop through rows of matrix A
        for(j = 0; j < 3; j++) { // loop through columns of matrix B
            int sum = 0;
            for(k = 0; k < 3; k++) { // loop through rows of matrix B and columns of matrix A at the same time to calculate dot product
                sum += matrixA[i * 3 + k] * matrixB[k * 3 + j];
            }
            resultMatrix[i * 3 + j] = sum;
        }
    }
    return resultMatrix;
}

void printMatrix(int length, int *matrix) {
    int linebreak = 0;
    for(int i = 0; i < length; i++) {
        printf("%d\t", matrix[i]);
        if(linebreak == 2) {
            printf("\n");
            linebreak = 0;
        } 
        else {
            linebreak++;
        }
    }
    printf("\n");
}

int quit() {
    //repeat until y or n
    char c;
    char t;
    do {
        printf("quit? (y/n):");
        c = getchar();
        while((t = getchar()) != '\n' && t != EOF); // clear buffer
    } while(c != 'y' && c != 'n');
    return c == 'y'; // return 1 if y, 0 if n
}