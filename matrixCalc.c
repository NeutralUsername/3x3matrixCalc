#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void printMatrix(int length, int *matrix);
void createMatrix(int *numbers, char identifier);
FILE *loadMatrix(char identifier);
int *getMatrixFromStream(FILE *fp);
int *multiply3x3Matrices(int *matrixA, int* matrixB);
char readMatrixIdentifier(char m);

int main(int argc, char *argv[]){
    FILE *fpA;
    FILE *fpB;
    while(1) {
        do {
            fpA = loadMatrix(readMatrixIdentifier('A'));
        } while(fpA == NULL);
        do {
            fpB = loadMatrix(readMatrixIdentifier('B'));
        } while(fpB == NULL);
        int *aMatrix = getMatrixFromStream(fpA);
        int *bMatrix = getMatrixFromStream(fpB);
        printMatrix(9, aMatrix);
        printMatrix(9, bMatrix);
        int *resultMatrix = multiply3x3Matrices(aMatrix, bMatrix);
        createMatrix(resultMatrix, readMatrixIdentifier('+'));
        printMatrix(9, resultMatrix);
        free(aMatrix);
        free(bMatrix);
        free(resultMatrix);
    }
    return 0;
}

FILE *loadMatrix(char identifier) {
    char filename[13];
    sprintf(filename, "matrix_%c.txt\0", identifier);
    FILE *fp = fopen(filename, "r");
    return fp;
}
int *getMatrixFromStream(FILE *fp) {
    int *matrix = malloc(sizeof(int) * 9);
    int i = 0;
    int num = 0;
    int c;
    do {
        c = fgetc(fp);
        if(isdigit(c)) {
            num = num * 10 + (c - '0');
        }
        else {
            matrix[i] = num;
            num = 0;
            i++;
        }
    } while(c != EOF);
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
        if((isalpha(c) || isdigit(c))) {
            while((t = getchar()) != '\n' && t != EOF); // clear buffer
            return c;
        }
        else if(c == '+') {
            while((t = getchar()) != '\n' && t != EOF); // clear buffer
            int *nrs = readMatrixValues();
            createMatrix(nrs, readMatrixIdentifier('+'));
            free(nrs);
        } 
        else while((t = getchar()) != '\n' && t != EOF); // clear buffer
    }
}

void createMatrix(int *numbers, char identifier) {
    FILE *fp; //file pointer 
    char filename[13]; //filename. format : matrix_[identifier].txt
    sprintf(filename, "matrix_%c.txt\0", identifier); 
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
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            int sum = 0;
            for(k = 0; k < 3; k++) {
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