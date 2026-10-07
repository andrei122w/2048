#include <iostream>
#include <random>
#include <vector>
#include <ctime>

using namespace std;

const int COLS = 4;
const int ROWS = 4;
const int SIZE = 4;

const int StartPieces = 2;

void initialize(vector<vector<int>>& board)
{
    for(int i = 1; i <= ROWS; i++)
    {
        for(int j = 0; j <= COLS; j++)
        {
            board[i][j] = 0;
        }
    }
}

void afisare(int sp[], vector<vector<int>>& board)
{
    for(int i = 1; i <= ROWS; i++)
    {
        for(int j = 1; j <= COLS; j++)
        {
            int nr = board[i][j], cnt = 0;

            do{
                cnt++;
                nr /= 10;
            }while(nr);

            for(int k = 1; k <= sp[j] - cnt; k++) cout << " ";
            cout << " " << board[i][j];
        }
        cout << endl;
    }
}


int occupied(int x, int y, vector<vector<int>>& board)
{
    if(board[x][y] == 0)
        return 0;
    return 1;
}


void spawn_start(vector<vector<int>>& board)
{
    for(int i = 1; i <= StartPieces; i++){
        int poz_x, poz_y;

        do{
            poz_x = rand() % SIZE + 1;
            poz_y = rand() % SIZE + 1;
        }while(occupied(poz_x, poz_y, board));

        int value;
        if(rand() % 10 == 0)
            value = 4;
        else
            value = 2;

        board[poz_x][poz_y] = value;
    }
}

void space(int sp[], vector<vector<int>>& board){
    for(int i = 1; i <= ROWS; i++)
    {
        int maxx = -1, cnt = 0;
        for(int j = 1; j <= COLS; j++)
        {
            if(board[j][i] > maxx) maxx = board[j][i];
        }
        while(maxx){maxx /= 10; cnt++;}
        sp[i] = cnt;
    }
}

void RotateBoard(vector<vector<int>>& board){
    for(int i = 1;i <= SIZE; i++)
        for(int j = i+1; j <= SIZE; j++)
            swap(board[i][j], board[j][i]);

    for(int i = 1;i <= SIZE; i++)
        for(int j = 1, k = SIZE; j < k; j++, k--)
            swap(board[i][j], board[i][k]);
}

void slideAndMerge(vector<vector<int>>& board){
    for(int i = 1; i <= SIZE; i++){
        int v[10], cnt = 0;

        for(int j = 0; j <= 5; j++) v[j] = -1;

        for(int j = 1; j <= SIZE; j++){
            if(board[i][j] != 0){
                if(cnt > 0 && v[cnt] == board[i][j]){
                    v[cnt] *= 2;
                }
                else{
                    v[++cnt] = board[i][j];
                }
            }
        }

        for(int j = 1; j <= SIZE - cnt; j++)
            board[i][j] = 0;
        for(int j = SIZE - cnt + 1, poz = 1; j <= SIZE; j++, poz++)
            board[i][j] = v[poz];
    }
}

void Move(int direction, vector<vector<int>>& board){
    for(int i = 1; i <= direction; i++) RotateBoard(board);

    slideAndMerge(board);

    for(int i = 1; i <= (4 - direction) % 4; i++) RotateBoard(board);
}

void spawn_piece(vector<vector<int>>& board){
    int poz_x, poz_y;

    do{
        poz_x = rand() % SIZE + 1;
        poz_y = rand() % SIZE + 1;
    }while(occupied(poz_x, poz_y, board));

    int value;
    if(rand() % 10 == 0)
        value = 4;
    else
        value = 2;

    board[poz_x][poz_y] = value;
}

int gamelostcheck(vector<vector<int>>& board){

    int cnt = 0;
    for(int i = 1;i <= SIZE; i++)
        for(int j = 1; j <= SIZE; j++){
            if(board[i][j])
                cnt++;
        }
    if(SIZE * SIZE == cnt) return 1;
    return 0;
}

bool gamewincheck(vector<vector<int>>& board) {
    for(int i = 1; i <= SIZE; i++) {
        for(int j = 1; j <= SIZE; j++) {
            if(board[i][j] == 2048) {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    srand(time(0)); // used to create randomness
    vector<vector<int>> board(ROWS+1, vector<int>(COLS+1)); // board

    initialize(board); // initialize
    spawn_start(board); // first piece

    char input; int sp[5];

    while(true){
        //afisare
        space(sp, board);
        afisare(sp, board);

        //input
        cout << "Use wsad to move: ";
        cin >> input;

        //movement
        if(input == 'd') Move(0, board);
        else if (input == 'w') Move(1, board);
        else if (input == 'a') Move(2, board);
        else if (input == 's') Move(3, board);

        //check for win
        if(gamewincheck(board)) {
            space(sp, board);
            afisare(sp, board);
            cout << "You win!" << endl;
            break;
        }

        //check for lose
        if(gamelostcheck(board)){
            space(sp, board);
            afisare(sp, board);
            cout << "you lost!" << endl;
            break;
        }

        //spawning
        spawn_piece(board);
    }

    return 0;
}
