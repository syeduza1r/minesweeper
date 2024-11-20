#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits> // Required for numeric_limits

using namespace std;

class Game {
protected:
    int boardSize;
    char** board;

public:
    virtual ~Game() {
        for (int i = 0; i < boardSize; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    virtual void initializeBoard() = 0;
    virtual void printBoard() = 0;
    virtual bool revealCell(int row, int col) = 0;
    virtual void play() = 0;
};

class Minesweeper : public Game {
private:
    bool** mines;
    const int numMines;

public:
    Minesweeper(int size = 10, int minesCount = 10)
        : numMines(minesCount) {
        boardSize = size;

        board = new char*[boardSize];
        for (int i = 0; i < boardSize; ++i) {
            board[i] = new char[boardSize];
        }

        mines = new bool*[boardSize];
        for (int i = 0; i < boardSize; ++i) {
            mines[i] = new bool[boardSize]();
        }
    }

    ~Minesweeper() {
        for (int i = 0; i < boardSize; ++i) {
            delete[] mines[i];
        }
        delete[] mines;
    }

    void initializeBoard() override {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                board[i][j] = '_';
            }
        }

        int minesPlaced = 0;
        while (minesPlaced < numMines) {
            int row = rand() % boardSize;
            int col = rand() % boardSize;

            if (!mines[row][col]) {
                mines[row][col] = true;
                minesPlaced++;
            }
        }
    }

    int countMines(int row, int col) {
        int mineCount = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newRow = row + i;
                int newCol = col + j;

                if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {
                    if (mines[newRow][newCol]) {
                        mineCount++;
                    }
                }
            }
        }
        return mineCount;
    }

    bool revealCell(int row, int col) override {
        if (board[row][col] != '_') {
            return true;
        }

        if (mines[row][col]) {
            board[row][col] = 'X';
            return false;
        }

        int surroundingMines = countMines(row, col);
        board[row][col] = surroundingMines + '0';

        if (surroundingMines == 0) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int newRow = row + i;
                    int newCol = col + j;

                    if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {
                        revealCell(newRow, newCol);
                    }
                }
            }
        }

        return true;
    }

    void printBoard() override {
        cout << "    ";
        for (int col = 0; col < boardSize; ++col) {
            cout << col << " ";
        }
        cout << endl;

        for (int row = 0; row < boardSize; ++row) {
            cout << row << " | ";
            for (int col = 0; col < boardSize; ++col) {
                cout << board[row][col] << " ";
            }
            cout << endl;
        }
    }

    void play() override {
        bool gameOver = false;
        int row, col;

        cout << "\n\n///////////////////////////////////////////\n\n";
        cout << "         Minesweeper Game!           " << endl;
        cout << "\n\n///////////////////////////////////////////\n\n";

        while (!gameOver) {
            printBoard();
            cout << "Enter row and column (0-" << boardSize - 1 << "): ";
            cin >> row >> col;

            // Handle invalid input
            if (cin.fail() || row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter valid row and column values." << endl;
                continue;
            }

            gameOver = !revealCell(row, col);

            if (gameOver) {
                cout << "(T-T) (T-T) (T-T) (T-T)" << endl;
                cout << "Game Over! You hit a mine!" << endl;
                cout << "(T-T) (T-T) (T-T) (T-T)" << endl;
            }
        }

        printBoard();
    }
};

int main() {
    srand(time(0));

    int boardSize = 10;
    int numMines = 10;
    char choice;

    cout << "The default board size is 10x10 with 10 mines." << endl;
    cout << "Would you like to change the board dimensions or the number of mines? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cout << "Enter the board size (e.g., 10 for a 10x10 board): ";
        cin >> boardSize;

        cout << "Choose the difficulty of your game. The higher the count of the mines, the more difficult the game becomes!" << endl;

        while (true) {
            cout << "Enter the number of mines: ";
            cin >> numMines;

            if (numMines > boardSize * boardSize) {
                cout << "Error: Number of mines cannot exceed the total number of cells ("
                     << boardSize * boardSize << "). Please enter a valid number of mines." << endl;
            } else {
                break;
            }
        }
    }

    Minesweeper game(boardSize, numMines);
    game.initializeBoard();
    game.play();

    return 0;
}