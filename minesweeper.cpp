#include <random>
#include <iostream>
#include <string>
using namespace std;

class Field{
    public:
    
    bool gameOver;
    int xMax;
    int yMax;
    //big grid for bomb locations and adjascency tiles
    int** arrayPointer;
    
    //big grid to store revealed tiles and marked tiles {-1:marked, 0:covered, 1:revealed}
    int** revealedArray;

    Field();

    Field(int x, int y){
        gameOver = 0;
        this->xMax = x;
        this->yMax = y;
        //cout << "swag";
        
        //creating field and initializing values to 0
        int **bigArray = new int*[y];
        int **largeArray = new int*[y];
        //initializing bigarray pointers to newly declared "empty" array
        for(int i=0; i<y; i++){
            //cout << "\n";
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

    /*
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
     */
    
    //updated populate function, populates bombs around selected starting tile, guaranteeing that starting tile is a 0 tile.
    void populate(int bombs, int area, int x, int y){
        if(bombs > area) return;
        
        
        int adjascents;
        string adjascentcase;
        
        //corner case, 3 adjascents
        if((x == 0 && y == 0) || (x == 0 && y == yMax-1) || (x == xMax - 1 && y == 0) || (x == xMax - 1 && y == yMax + 1)){
            adjascents = 3;
        }
        
        //edge cases, 5 adjascents
        else if((x == 0 && (0 < y && y < yMax-1)) || (y == 0 && (0 < x && x < xMax-1)) || (x == xMax-1 && (0 < y && y < yMax-1)) || (y == yMax-1 && (0 < x && x < xMax-1))){
            adjascents = 5;
        }
        else{
            adjascents = 8;
        }
                
        int numArray[area - (adjascents)];
        
        //initialize numArray to 99999...990000....000000
        for(int i = 0; i<bombs; i++){
            numArray[i] = 9;
        }
        for(int n = bombs; n<area-adjascents; n++){
            numArray[n] = 0;
        }
        

        //sets random seed and random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        //Fisher-Yates shuffle on binary array
        for(int i = 0; i<area-(adjascents+2); i++){
            std::uniform_int_distribution<int> uni(i, (area-adjascents-1));
            int j = uni(gen);
            int a = numArray[i];
            numArray[i] = numArray[j];
            numArray[j] = a;
        }
        
        //case 1: non-edge, non-corner :)
        //  x: normal potential for bomb - o: no bombs
        //
        //  x x x x x
        //  x o o o x
        //  x o o o x
        //  x o o o x
        //  x x x x x
        //
        int offset = 0;
        
        if((0 < x && x < xMax-1) && (0 < y && y < yMax - 1)){
            for(int i = 0; i<this->yMax; i++){
                //if we are on rows adjascent or equal to y
                if(i == y-1 || i == y || i == y+1){
                    
                    for(int n = 0; n<this->xMax; n++){
                        
                        //want to put 0s here in all slots, but not increment space in numArray.
                        if(n == x-1 || n == x || n == x+1){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        else{
                            this->arrayPointer[i][n] = numArray[(i*yMax) + n - offset];
                        }
                    }
                }
                else{
                    for(int n = 0; n<this->xMax; n++){
                        this->arrayPointer[i][n] = numArray[(i*yMax) + n];
                    }
                }
            }
        }
        
        //case 2: edges & corners (UGH!)
        //
        //  x x x       o o x
        //  o o x       o o x
        //  o o x  ...  x x x
        //  o o x
        //  x x x
        //
        
        else{
            
            for(int i = 0; i<this->yMax; i++){
                //if we are on top 2 rows and y coordinate is 0:
                if((i == y || i == y+1) && y == 0){
                    
                    for(int n = 0; n<this->xMax; n++){
                        
                        //top row, not corner
                        if((n == x-1 || n == x || n == x+1) && (0 < x && x < xMax-1)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //top row, left corner
                        else if((n == x || n == x+1) && (x == 0)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //top row, right corner
                        else if((n == x || n == x-1) && (x == xMax-1)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //other cases
                        else{
                            this->arrayPointer[i][n] = numArray[(i*yMax) + n - offset];
                        }
                    }
                }
                
                //if we are on bottom two rows and y = yMax - 1 (upper bound)
                else if((i == y || i == y-1) && y == yMax-1){
                    
                    for(int n = 0; n<this->xMax; n++){
                        
                        //bottom row, not corner
                        if((n == x-1 || n == x || n == x+1) && (0 < x && x < xMax-1)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //bottom row, left corner
                        else if((n == x || n == x+1) && (x == 0)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //bottom row, right corner
                        else if((n == x || n == x-1) && (x == xMax-1)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //other cases
                        else{
                            this->arrayPointer[i][n] = numArray[(i*yMax) + n - offset];
                        }
                    }
                }
                
                //y != yMax-1 && y != 0 --> can only be left or right edge, not corners
                else{
                    for(int n = 0; n<this->xMax; n++){
                        //on the left edge
                        if((n == x+1 || n == x) && (x == 0)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //on the right edge
                        else if((n == x-1 || n == x) && (x == xMax-1)){
                            this->arrayPointer[i][n] = 0;
                            offset++;
                        }
                        
                        //other cases
                        else{
                            this->arrayPointer[i][n] = numArray[(i*yMax) + n - offset];
                        }
                    }
                }
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
    
    /*
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
     */
    
    //updated field initialization to include generation around starting tile
    void fieldinitialization(int bombs, int area, int x, int y){
        //can call populate in here to simplify main execution
        this->populate(bombs, area, x, y);
        
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
    //if tile uncovered is an "empty" (no adjascent bombs) call zerouncovered
    int uncover(int x, int y){
        
        //bomb case (-1, game over)
        if(arrayPointer[y][x] == 9) return -1;
        
        //"empty" tile case
        else if (arrayPointer[y][x] == 0){
            //if not uncovered, uncover using zerouncovered func., return 1 for successsful uncover
            if(revealedArray[y][x] == 0){
                zerouncovered(x, y);
                return 1;
            }
            
            //already uncovered, no need to do anything
            else return 0;
        }
        
        //in case where already revealed, return 0, do nothing
        else if (revealedArray[y][x] == 1) return 0;
        
        //all other cases, reveal square and return 1 for successful uncovering
        revealedArray[y][x] = 1;
        return 1;
    }
    
    //reveal all adjascent tiles for (x, y)
    void uncoveradjascent(int x, int y){
        //DO NOT UNCOVER SHIT IF THESE INDEXES ARE NOT VALID:
        if(x < 0 || y < 0 || x >= xMax || y >= yMax) return;
        
        //only want to uncover y+1 and y
        if(y == 0){
            //if x is not on lower bound then we can uncover x-1
            if(x != 0){
                revealedArray[y][x-1] = 1;
                revealedArray[y+1][x-1] = 1;
            }
            //if x is not on upper bound then we can uncover x+1
            if(x != xMax-1){
                revealedArray[y][x+1] = 1;
                revealedArray[y+1][x+1] = 1;
            }
            //for all x cases, reveal x
            //revealedArray[y][x] = 1; -- not necessary since zerouncovered should uncover y,x
            revealedArray[y+1][x] = 1;
        }
        //only want to uncover y and y-1
        else if(y == yMax-1){
            if(x != 0){
                revealedArray[y-1][x-1] = 1;
                revealedArray[y][x-1] = 1;
            }
            if(x != xMax-1){
                revealedArray[y-1][x+1] = 1;
                revealedArray[y][x+1] = 1;
            }
            revealedArray[y-1][x] = 1;
            //revealedArray[y][x] = 1;
        }
        //want to uncover y-1, y and y+1
        else{
            if(x != 0){
                revealedArray[y-1][x-1] = 1;
                revealedArray[y][x-1] = 1;
                revealedArray[y+1][x-1] = 1;
            }
            if(x != xMax-1){
                revealedArray[y-1][x+1] = 1;
                revealedArray[y][x+1] = 1;
                revealedArray[y+1][x+1] = 1;
            }
            //revealedArray[y][x] = 1;
            revealedArray[y-1][x] = 1;
            revealedArray[y+1][x] = 1;
        }
        return;
    }
    
    //simple checker function to handle ugly x and y values input in zerouncovered (i am too lazy to write the checker in that function)
    bool isempty(int x, int y){
        if(x < 0 || y < 0 || x >= xMax || y >= yMax) return 0;
        else{
            return (arrayPointer[y][x] == 0 && revealedArray[y][x] == 0);
        }
    }
    
    
    //for case where tile with 0 adjascent mines is uncovered, perform BFS on tile to uncover all adjascent 0 tiles (diagonals accepted)
    //also reveal adjascent non-bomb fields
    void zerouncovered(int x, int y){
        if(x < 0 || y < 0 || x >= xMax || y >= yMax || revealedArray[y][x] == 1){
            return;
        }
        else{
            //reveal each tile surrounding current tile, call zerouncovered on surrounding zero tiles
            revealedArray[y][x] = 1;
            for(int i = -1; i < 2; i++){
                for(int n = -1; n < 2; n++){
                    if (isempty(x+i, y+n)){
                        zerouncovered(x+i, y+n);
                    }
                }
            }
            uncoveradjascent(x, y);
            return;
        }
    }
    
    void play(){
        while(gameOver == 0){
            int x = -1;
            int y = -1;
            //devdisplay();
            string input = "";
            cout << "uncover, flag or check (for completion): \n";
            getline(cin, input);
            if(input.compare("flag") == 0){
                while(y < 0 || y >= yMax){
                    cout << "enter row: ";
                    cin >> y;
                    if(y >= yMax){
                        cout << "error: y value too high. \n";
                        if(y == yMax) cout << "note: row numbers start from 0 at the top to " << this->yMax - 1 << " at the bottom. \n";
                    }
                    if(y < 0){
                        cout << "error: y value too low. (trying negative numbers? you sly dog) \n";
                    }
                }
                cout << "\n";
                
                while(x < 0 || x >= xMax){
                    cout << "enter column: ";
                    cin >> x;
                    if(x >= xMax){
                        cout << "error: x value too high. \n";
                        if(x == xMax) cout << "note: column numbers start from 0 at the left to " << this->xMax - 1 << " at the right. \n";
                    }
                    if(x < 0){
                        cout << "error: x value too low. (trying negative numbers? you sly dog) \n";
                    }
                }
                
                //if we get here, x and y values should be valid
                revealedArray[y][x] = -1;
            }
            
            
            else if(input.compare("uncover") == 0){
                while(y < 0 || y >= yMax){
                    cout << "enter row: ";
                    cin >> y;
                    if(y >= yMax){
                        cout << "error: y value too high. \n";
                        if(y == yMax) cout << "note: row numbers start from 0 at the top to " << this->yMax - 1 << " at the bottom. \n";
                    }
                    if(y < 0){
                        cout << "error: y value too low. (trying negative numbers? you sly dog) \n";
                    }
                }
                cout << "\n";
                
                while(x < 0 || x >= xMax){
                    cout << "enter column: ";
                    cin >> x;
                    if(x >= xMax){
                        cout << "error: x value too high. \n";
                        if(x == xMax) cout << "note: column numbers start from 0 at the left to " << this->xMax - 1 << " at the right. \n";
                    }
                    if(x < 0){
                        cout << "error: x value too low. (trying negative numbers? you sly dog) \n";
                    }
                }
                if(uncover(x, y) == -1) gameOver = 1;
            }
            
            else if(input.compare("check") == 0){
                this->check();
            }
            else if(input.compare("exit") == 0){
                this->gameOver = 1;
            }
            else{
                cout << "please enter a valid command. \n";
            }
            display();
            cout << "\n";
            //devdisplay();
            //cout << "\n";
        }

        cout << "game over :( better luck next time!\n";
    }
    
    //implement later; check game for completion (every tile is uncovered, every bomb is marked)
    void check(){
        
    }
    
    //displays current state of the game
    /*currently:
        O: covered tile
        1 ... 8: uncovered nonbomb tile with adjascent bombs
        _: uncovered nonbomb tile with no adjascent bombs
        M: marked tile (suspected bomb)
        *: bomb location(s) for game over
    */
    void display(){
        for(int i = 0; i < yMax; i++){
            for(int n = 0; n < xMax; n++){
                if(revealedArray[i][n] == 0) cout << "O";
                //cout << gameField->checktile(n, i);
                else if(revealedArray[i][n] == -1) cout << "M";
                else if(revealedArray[i][n] == 1 && arrayPointer[i][n] != 9) cout << arrayPointer[i][n];
                else cout << "*";
                cout << " ";
                //cout << gameField->arrayPointer[i][n];
            }
            cout << "\n";
        }
    }
    
    //display full bomb field
    void devdisplay(){
        for(int i = 0; i < yMax; i++){
            for(int n = 0; n < xMax; n++){
                if(arrayPointer[i][n] != 9) cout << arrayPointer[i][n];
                else cout << "*";
                cout << " ";
                //cout << gameField->arrayPointer[i][n];
            }
            cout << "\n";
        }
    }
    
};


int main(){
    int xMax = 30;
    int yMax = 15;
    Field *gameField = new Field(xMax, yMax);
    //gameField->display();
    //cout << "field created \n";
    
    //can polish later w junk cleanup etc, just get starting x and y values to reveal
    int x = -1;
    int y = -1;

    while(y < 0 || y >= yMax){
        cout << "enter row: ";
        cin >> y;
        if(y >= yMax){
            cout << "error: y value too high. \n";
            if(y == yMax) cout << "note: row numbers start from 0 at the top to " << yMax - 1 << " at the bottom. \n";
        }
        if(y < 0){
            cout << "error: y value too low. (trying negative numbers? you sly dog) \n";
        }
    }

    while(x < 0 || x >= xMax){
        cout << "enter column: ";
        cin >> x;
        if(x >= xMax){
            cout << "error: x value too high. \n";
            if(x == xMax) cout << "note: column numbers start from 0 at the left to " << xMax - 1 << " at the right. \n";
        }
        if(x < 0){
            cout << "error: x value too low. (trying negative numbers? you sly dog) \n";
        }
    }

    
    

    //gameField->populate(99, (gameField->xMax*gameField->yMax));
    gameField->fieldinitialization(99, (gameField->xMax*gameField->yMax), x, y);

    gameField->uncover(x,y);
    
    
    gameField->play();
    
    //cout << "tests passed. \n";
}
