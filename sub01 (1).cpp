#include <iostream>
#include <cmath>
#include <string>

#define MAX_K 10000

//#define SIZE 16
#define SIZE 25
//#define SIZE 9


class Node {
    public:
        Node *left; Node *right; Node *up; Node*down; Node *head;
        int size;	int kwidd[3];                    
};

 int squareofsize = SIZE*SIZE;
 int sqrootsize = sqrt((double)SIZE);
 const int ROW_NB = SIZE*SIZE*SIZE;
 const int COL_NB = 4 * SIZE*SIZE;

class Node Head;
class Node* HeadNode = &Head;
class Node* solution[MAX_K];
class Node* orig_values[MAX_K];
bool matrix[ROW_NB][COL_NB] = { { 0 } };
bool isSolved = false;
void mapping(int Sudoku[][SIZE]);
void outputgrid(int Sudoku[][SIZE]);

void coverColumn(Node* col) {
    col->left->right = col->right;
    col->right->left = col->left;
    Node* node = col->down;
    while (node != col) {
        Node* temp = node->right;
        while (temp != node) {
           temp->down->up = temp->up;
            temp->up->down = temp->down;
            temp->head->size--; 

        temp = temp->right;
    }
    node = node->down;
    }
}

void uncoverColumn(Node* col) {
    Node* node = col->up;
    while (node != col) {
        Node* temp = node->left;
        while (temp != node) {
            temp->head->size++;
            temp->down->up = temp;
            temp->up->down = temp;

            temp = temp->left;
    }
    node = node->up;
    }
    col->left->right = col;
    col->right->left = col;
}

void search(int k) {

    if (HeadNode->right == HeadNode) {
        int Grid[SIZE][SIZE] = { {0} };
        mapping(Grid);
        outputgrid(Grid);
        isSolved = true;
        return;
    }
    Node* Col = HeadNode->right;
    for (Node* temp = Col->right; temp != HeadNode; temp = temp->right)
        if (temp->size < Col->size)
            Col = temp;

    coverColumn(Col);

    for (Node* temp = Col->down; temp != Col; temp = temp->down) {
        solution[k] = temp;
        for (Node* node = temp->right; node != temp; node = node->right) {
            coverColumn(node->head);
        }

        search(k + 1);

        temp = solution[k];
        solution[k] = NULL;
        Col = temp->head;
        for (Node* node = temp->left; node != temp; node = node->left) {
            uncoverColumn(node->head);
        }
    }

    uncoverColumn(Col);
}
void BuildSparseMatrix(bool matrix[ROW_NB][COL_NB]) {

    int j = 0, counter = 0;
    for (int i = 0; i < ROW_NB; i++) {
        matrix[i][j] = 1;
        counter++;
        if (counter >= SIZE) {
            j++;
            counter = 0;
        }
    }
    int x = 0;
    counter = 1;
    for (j = squareofsize; j < 2 * squareofsize; j++) {
        for (int i = x; i < counter*squareofsize; i += SIZE)
            matrix[i][j] = 1;

        if ((j + 1) % SIZE == 0) {
            x = counter*squareofsize;
            counter++;
        }
        else
            x++;
    }
    j = 2 * squareofsize;
    for (int i = 0; i < ROW_NB; i++)
    {
        matrix[i][j] = 1;
        j++;
        if (j >= 3 * squareofsize)
            j = 2 * squareofsize;
    }

    x = 0;
    for (j = 3 * squareofsize; j < COL_NB; j++) {

        for (int l = 0; l < sqrootsize; l++) {
            for (int k = 0; k<sqrootsize; k++)
                matrix[x + l*SIZE + k*squareofsize][j] = 1;
        }

        int temp = j + 1 - 3 * squareofsize;

        if (temp % (int)(sqrootsize * SIZE) == 0)
            x += (sqrootsize - 1)*squareofsize + (sqrootsize - 1)*SIZE + 1;
        else if (temp % SIZE == 0)
            x += SIZE*(sqrootsize - 1) + 1;
        else
            x++;
    }
}
void tanaka(bool matrix[ROW_NB][COL_NB]) {

    Node* header = new Node;
    header->left = header;
    header->right = header;
    header->down = header;
    header->up = header;
    header->size = -1;
    header->head = header;
    Node* temp = header;

    int m=0;
    while(m < COL_NB){
        Node* newNode = new Node;
        newNode->size = 0;
        newNode->up = newNode;
        newNode->down = newNode;
        newNode->head = newNode;
        newNode->right = header;
        newNode->left = temp;
        temp->right = newNode;
        temp = newNode;
        m++;
    }

    int ID[3] = { 0,1,1 };

    m=0;
    while(m < ROW_NB){
        Node* top = header->right;
        Node* prev = NULL;

        if (m != 0 && m%squareofsize == 0) {
            ID[0] -= SIZE - 1;
            ID[1]++;
            ID[2] -= SIZE - 1;
        }
        else if (m!= 0 && m%SIZE == 0) {
            ID[0] -= SIZE - 1;
            ID[2]++;
        }
        else {
            ID[0]++;
        }
        int j=0;
        while(j < COL_NB){
            if (matrix[m][j]) {
                Node* newNode = new Node;
                newNode->kwidd[0] = ID[0];
                newNode->kwidd[1] = ID[1];
                newNode->kwidd[2] = ID[2];
                if (prev == NULL) {
                    prev = newNode;
                    prev->right = newNode;
                }
                newNode->left = prev;
                newNode->right = prev->right;
                newNode->right->left = newNode;
                prev->right = newNode;
                newNode->head = top;
                newNode->down = top;
                newNode->up = top->up;
                top->up->down = newNode;
                top->size++;
                top->up = newNode;
                if (top->down == top)
                    top->down = newNode;
                prev = newNode;
            }

            j++;
            top = top->right;
        }
        m++;
    }
    HeadNode = header;
}
void listingthegrid(int salmon[][SIZE]) {
    int index = 0;
    for(int i = 0 ; i<SIZE; i++ )
        for(int j = 0 ; j<SIZE; j++)
            if (salmon[i][j] > 0) {
                Node* Col = NULL;
                Node* temp = NULL;
                for (Col = HeadNode->right; Col != HeadNode; Col = Col->right) {
                    for (temp = Col->down; temp != Col; temp = temp->down)
                        if (temp->kwidd[0] == salmon[i][j] && (temp->kwidd[1] - 1) == i && (temp->kwidd[2] - 1) == j)
                            goto mainers;
                }
mainers:		coverColumn(Col);
                orig_values[index] = temp;
                index++;
                for (Node* node = temp->right; node != temp; node = node->right) {
                    coverColumn(node->head);
                }

            }

}
void mapping(int salmon[][SIZE]) {
    int i = 0;
    while (solution[i] != NULL) {
        salmon[solution[i]->kwidd[1]-1][solution[i]->kwidd[2]-1] = solution[i]->kwidd[0];
        i++;
    }

    i = 0;
    while (orig_values[i] != NULL){
        salmon[orig_values[i]->kwidd[1] - 1][orig_values[i]->kwidd[2] - 1] = orig_values[i]->kwidd[0];
         i++;
    }
}
void outputgrid(int salmon[][SIZE]){
    const char values[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int value = salmon[i][j];
            if (value >= 0 && value < sizeof(values)) {
                std::cout << values[value];
            }
            if (j != SIZE - 1) {
                std::cout << " ";
            }
        }
        if (i != SIZE - 1) {
            std::cout << std::endl;
        }
    }
}
void SolveSudoku(int salmon[][SIZE]) {
    BuildSparseMatrix(matrix);
    tanaka(matrix);
    listingthegrid(salmon);
    search(0);
    if (!isSolved)
        std::cout << "No Solution\n" << std::endl;
    isSolved = false;
}

int main(){
    int salmon[SIZE][SIZE];
    char sals;
    for (int i=0;i<SIZE;i++){
          for (int j=0;j<SIZE;j++){
                std::cin >> sals;
                if(int(sals) < 58){
                    salmon[i][j]= sals-48;
                }
                else{
                    salmon[i][j]=sals-55;
                }
          }
    }
    SolveSudoku(salmon);
    return 0;
}
