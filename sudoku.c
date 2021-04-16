#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintRed();
void PrintGreen();

void OpenTable(int numTable[9][9], char *filename){
    char buff[82];
    FILE *file;
    PrintGreen("Opening \'%s\' table...\n", filename);
    file = fopen(filename, "rb");
    int i = 0;
    while (i < 81){
        buff[i] = fgetc(file);
        i++;
    }
    buff[81] = '\0';
    int n, m, l, valid = 0, j = 0;
    char numlist[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for (n = 0; n < 9; ++n){
        for (m = 0; m < 9; ++m){
            valid = 0;
            for (l = 0; l < 10; ++l){
                if (buff[j] == numlist[l]){
                    numTable[n][m] = buff[j] - '0';
                    valid = 1;
                    j++;
                    break;
                }
            }
            if (valid == 0){
                PrintRed("The table file is not a valid table: %s", filename);
                exit(0);
            }
        }
    }
}

int IsAvailible(int puzzle[][9], int row, int col, int num){
    int row_start = (row/3) * 3;
    int col_start = (col/3) * 3;
    int i;
    for (i = 0; i < 9; ++i){
        if (puzzle[row][i] == num) return 0;
        if (puzzle[i][col] == num) return 0;
        if (puzzle[row_start + (i%3)][col_start + (i/3)] == num) return 0;
    }
    return 1;
}

int FillSudoku(int puzzle[][9], int row, int col){
    int i;
    if (row < 9 && row < 9){
        if (puzzle[row][col] != 0){
            if ((col + 1) < 9) return FillSudoku(puzzle, row, col + 1);
            else if ((row + 1) < 9) return FillSudoku(puzzle, row + 1, 0);
            else return 1;
        } else {
            for (i = 0; i < 9; ++i){
                if (IsAvailible(puzzle, row, col, i + 1)){
                    puzzle[row][col] = i + 1;
                    if ((col + 1) < 9) {
                        if (FillSudoku(puzzle, row, col + 1)) return 1;
                        else puzzle[row][col] = 0;
                    } else if ((row + 1) < 9){
                        if (FillSudoku(puzzle, row + 1, 0)) return 1;
                        else puzzle[row][col] = 0;
                    } else {
                        return 1;
                    }
                }
            }
            return 0;
        }
    } else {
        return 1;
    }
}


void PrintGreen(char *text){
    printf("\033[0;32m");
    printf("%s", text);
    printf("\033[0;m");
}

void PrintRed(char *text){
    printf("\033[0;31m");
    printf("%s", text);
    printf("\033[0;m");
}

int main(int argc, char *argv[]){
    int i, j;
    int puzzle[9][9] = {{0, 0, 0, 0, 0, 0, 0, 9, 0},
                        {1, 9, 0, 4, 7, 0, 6, 0, 8},
                        {0, 5, 2, 8, 1, 9, 4, 0, 7},
                        {2, 0, 0, 0, 4, 8, 0, 0, 0},
                        {0, 0, 9, 0, 0, 0, 5, 0, 0},
                        {0, 0, 0, 7, 5, 0, 0, 0, 9},
                        {9, 0, 7, 3, 6, 4, 1, 8, 0},
                        {5, 0, 6, 0, 8, 1, 0, 7, 4},
                        {0, 8, 0, 0, 0, 0, 0, 0, 0}};
    if (argc > 1){
        OpenTable(puzzle, argv[1]);
    }
    if (FillSudoku(puzzle, 0, 0)){
        printf("\n+-----+-----+-----+\n");
        for (i = 1; i < 10; ++i){
            for (j = 1; j < 10; ++j) printf("|%d", puzzle[i - 1][j - 1]);
            printf("|\n");
            if (i % 3 == 0) printf("+-----+-----+-----+\n");
        }
    } else {
        PrintRed("No correct solution found.\nExiting...\n");
        return -1;
    }
}
