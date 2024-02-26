#include <iostream>
#include <vector>
#define N 9

using namespace std;

// Function to read in a Sudoku puzzle
void EnterSudoku(vector<vector<int>> &salmon) {
    salmon.resize(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> salmon[i][j];
        }
    }
}

// Function to output the Sudoku puzzle
void ConsoleOut(vector<vector<int>> &salmon) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if(i != 8 && j==8){
                cout << salmon[i][j] << "\n";
            }
            else{
                cout << salmon[i][j] << " ";
            }
        }
        /*if(i != 8){
        cout << ;
        }*/
    }
}


bool Puzzled(int n, int c, int num, vector<vector<int>> &salmon) {
    for (int i = 0; i < N; i++) {
        if (salmon[n][i] == num || salmon[i][c] == num) {
            return false;
        }
    }

    int startRow = (n / 3) * 3;
    int startCol = (c / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (salmon[i][j] == num) {
                return false;
            }
        }
    }

    return true;
}


bool solveSudoku(vector<vector<int>> &salmon) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (salmon[i][j] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (Puzzled(i, j, num, salmon)) {
                        salmon[i][j] = num;

                        if (solveSudoku(salmon)) {
                            return true;
                        }

                        salmon[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

int main() {
    vector<vector<int>> salmon;
    EnterSudoku(salmon);
    if (solveSudoku(salmon)) {
        ConsoleOut(salmon);
    } else {
        cout << "No Solution"<<endl;
    }
    return 0;
}
