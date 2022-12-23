#include <random>
#include <iostream>
using namespace std;

class Field{
    public:
    int xMax;
    int yMax;
    //big grid for bomb locations and adjascency tiles
    int** arrayPointer;
    
    //big grid to store revealed tiles and marked tiles {-1:marked, 0:covered, 1:revealed}
    int** revealedArray;

    Field();

    Field(int x, int y){
        this->xMax = x;
        this->yMax = y;
        //cout << "swag";
        
        //creating field and initializing values to 0
        int **bigArray = new int*[y];
        int **largeArray = new int*[y];
        //initializing bigarray pointers to newly declared "empty" array
        for(int i=0; i<y; i++){
            cout << "\n";
            //small guy filled with zeros
            int *miniArray = new int[x];
            int *smallArray = new int[x];
            for(int n=0; n<x; n++){
                //cout << "0 ";
                smallArray[n] = 0;
                
                //initializing revealedArray to all 0s since nothing is initially revealed
                miniArray[n] = 0;
            }
            bigArray[i] = smallArray;
            largeArray[i] = miniArray;
        }
        arrayPointer = bigArray;
        revealedArray = largeArray;
    }

    void populate(int bombs, int area){
        if(bombs > area) return;
        
        //initialize numArray to 99999...990000....000000
        int numArray[area];
        for(int i = 0; i<bombs; i++){
            numArray[i] = 9;
        }
        for(int n = bombs; n<area; n++){
            numArray[n] = 0;
        }

        //sets random seed and random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        //Fisher-Yates shuffle on binary array
        for(int i = 0; i<area-1; i++){
            std::uniform_int_distribution<int> uni(i, (area-1));
            int j = uni(gen);
            int a = numArray[i];
            numArray[i] = numArray[j];
            numArray[j] = a;
        }

        //populate 2D arrayPointer with bombs from numArray
        for(int i = 0; i<this->yMax; i++){
            for(int n = 0; n<this->xMax; n++){
                this->arrayPointer[i][n] = numArray[(i*yMax) + n];
            }
        }

        return;
    }

    //check number of bombs adjascent to (x, y), if (x, y) is a bomb, return -1.
    int checktile(int x, int y){
        if(arrayPointer[y][x] != 0) return -1;
        int sum = 0;
        if((0 < y) && (y < yMax-1)){
            //chillest y case
            if(x != 0){
                //lower x values exist, so indexing is okay
                sum += arrayPointer[y-1][x-1] + arrayPointer[y][x-1] + arrayPointer[y+1][x-1];
            }
            if(x != xMax-1){
                //higher x values exist, so indexing is okay
                sum += arrayPointer[y-1][x+1] + arrayPointer[y][x+1] + arrayPointer[y+1][x+1];
            }
            sum += arrayPointer[y-1][x] + arrayPointer[y+1][x];
        }
        
        else if(y == 0){
            //low y case, don't index [y-1].
            if(x != 0){
                //lower x values exist, so indexing is okay
                sum += arrayPointer[y][x-1] + arrayPointer[y+1][x-1];
            }
            if(x != xMax-1){
                //higher x values exist, so indexing is okay
                sum += arrayPointer[y][x+1] + arrayPointer[y+1][x+1];
            }
            sum += arrayPointer[y+1][x];
        }
        
        else if(y == yMax-1){
            //max case y, don't index [y+1]
            if(x != 0){
                //lower x values exist, so indexing is okay
                sum += arrayPointer[y-1][x-1] + arrayPointer[y][x-1];
            }
            if(x != xMax-1){
                //higher x values exist, so indexing is okay
                sum += arrayPointer[y-1][x+1] + arrayPointer[y][x+1];
            }
            sum += arrayPointer[y-1][x];
        }
        
        return sum / 9;
    }
    
    //once fieldinitialization is called, field should be fully initialized, ready to play.
    //at this point in the arrayPointer array, bombs are represented by 9, and each other square should have its corresponding adjascency sum.
    void fieldinitialization(){
        //can call populate in here to simplify main execution
        
        
        //once populate is called, i.e. once bombs are distributed in field
        for(int i = 0; i < this->yMax; i++){
            for(int n = 0; n < this->xMax; n++){
                int c = this->checktile(n, i);
                if(c != -1) arrayPointer[i][n] = c;
                //cout << gameField->arrayPointer[i][n];
            }
        }
        
        
    }
    
    //return 0 if already uncovered, return -1 if uncovered tile contains bomb (game over), return 1 if successfully uncovered
    int uncover(int x, int y){
        if(arrayPointer[y][x] == 9) return -1;
        else if (revealedArray[y][x] == 1) return 0;
        revealedArray[y][x] = 1;
        return 1;
    }
    
    bool iscovered(int x, int y){
        return revealedArray[y][x] == 0;
    }
    
    //for case where tile with 0 adjascent mines is uncovered, perform BFS on tile to uncover all adjascent 0 tiles (diagonals accepted)
    //also reveal adjascent non-bomb fields
    void zerouncovered(int x, int y){
        if(x < 0 || y < 0 || x > xMax || y > yMax || !iscovered(x,y)){
            return;
        }
        else{
            revealedArray[y][x] = 1;
            //reveal each tile surrounding current tile, call zerouncovered on surrounding zero tiles
            
        }
    }
    
};


int main(){
    cout << "running main \n";
    Field *gameField = new Field(30, 15);
    cout << "field created \n";

    gameField->populate(99, (gameField->xMax*gameField->yMax));
    gameField->fieldinitialization();
    for(int i = 0; i < gameField->yMax; i++){
        for(int n = 0; n < gameField->xMax; n++){
            //cout << gameField->checktile(n, i);
            if(gameField->arrayPointer[i][n] != 9) cout << gameField->arrayPointer[i][n];
            else cout << "*";
            cout << " ";
            //cout << gameField->arrayPointer[i][n];
        }
        cout << "\n";
    }
}
