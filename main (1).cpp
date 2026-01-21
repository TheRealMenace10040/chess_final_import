#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// CIS-17 Final Project DHN
enum Color { WHITE, BLACK };

class ChessPiece;

class ChessBoard {
private:
    ChessPiece* board[8][8];

public:
    ChessBoard();
    ~ChessBoard(); // Destructor to prevent memory leaks

    void initialize();
    ChessPiece* getPiece(string position);
    void placePiece(ChessPiece* piece, string position);
    bool move(string fromPosition, string toPosition);
    string toString();
};

class ChessPiece {
protected:
    ChessBoard* board;
    Color color;
    int row;
    int column;

public:
    ChessPiece(ChessBoard* board, Color color) : board(board), color(color), row(0), column(0) {}
    virtual ~ChessPiece() {}

    int getRow() { return row; }
    int getColumn() { return column; }
    void setRow(int r) { row = r; }
    void setColumn(int c) { column = c; }
    Color getColor() { return color; }

    void setPosition(string position) {
        column = position[0] - 'a';
        row = position[1] - '1';
    }

    virtual bool canMove(int toRow, int toColumn) = 0;
    virtual string toString() = 0;
};

// --- Piece Implementations ---

class Rook : public ChessPiece {
public:
    Rook(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int toRow, int toColumn) override {
        if (row != toRow && column != toColumn) return false;
        // Logic for checking clear path could be added here
        return true;
    }
    string toString() override { return (color == WHITE) ? "R" : "r"; }
};

class Knight : public ChessPiece {
public:
    Knight(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int toRow, int toColumn) override {
        int rowDiff = abs(toRow - row);
        int colDiff = abs(toColumn - column);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
    string toString() override { return (color == WHITE) ? "N" : "n"; }
};

class Bishop : public ChessPiece {
public:
    Bishop(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int toRow, int toColumn) override {
        return abs(toRow - row) == abs(toColumn - column);
    }
    string toString() override { return (color == WHITE) ? "B" : "b"; }
};

class Queen : public ChessPiece {
public:
    Queen(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int toRow, int toColumn) override {
        return (row == toRow || column == toColumn) || (abs(toRow - row) == abs(toColumn - column));
    }
    string toString() override { return (color == WHITE) ? "Q" : "q"; }
};

class King : public ChessPiece {
public:
    King(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int toRow, int toColumn) override {
        return abs(toRow - row) <= 1 && abs(toColumn - column) <= 1;
    }
    string toString() override { return (color == WHITE) ? "K" : "k"; }
};

class Pawn : public ChessPiece {
public:
    Pawn(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int toRow, int toColumn) override {
        int direction = (color == WHITE) ? 1 : -1;
        if (column == toColumn && toRow == row + direction) return true; // Forward 1
        return false; // Basic pawn logic
    }
    string toString() override { return (color == WHITE) ? "P" : "p"; }
};

// --- Board Functions ---

ChessBoard::ChessBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = nullptr;
}

ChessBoard::~ChessBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            delete board[i][j];
}

void ChessBoard::initialize() {
    placePiece(new Rook(this, WHITE), "a1"); placePiece(new Rook(this, WHITE), "h1");
    placePiece(new Knight(this, WHITE), "b1"); placePiece(new Knight(this, WHITE), "g1");
    placePiece(new Bishop(this, WHITE), "c1"); placePiece(new Bishop(this, WHITE), "f1");
    placePiece(new Queen(this, WHITE), "d1"); placePiece(new King(this, WHITE), "e1");
    for (char c = 'a'; c <= 'h'; c++) placePiece(new Pawn(this, WHITE), string(1, c) + "2");

    placePiece(new Rook(this, BLACK), "a8"); placePiece(new Rook(this, BLACK), "h8");
    placePiece(new Knight(this, BLACK), "b8"); placePiece(new Knight(this, BLACK), "g8");
    placePiece(new Bishop(this, BLACK), "c8"); placePiece(new Bishop(this, BLACK), "f8");
    placePiece(new Queen(this, BLACK), "d8"); placePiece(new King(this, BLACK), "e8");
    for (char c = 'a'; c <= 'h'; c++) placePiece(new Pawn(this, BLACK), string(1, c) + "7");
}

void ChessBoard::placePiece(ChessPiece* piece, string pos) {
    int r = pos[1] - '1';
    int c = pos[0] - 'a';
    board[r][c] = piece;
    piece->setRow(r);
    piece->setColumn(c);
}

ChessPiece* ChessBoard::getPiece(string pos) {
    return board[pos[1] - '1'][pos[0] - 'a'];
}

bool ChessBoard::move(string from, string to) {
    ChessPiece* p = getPiece(from);
    if (!p) return false;

    int tr = to[1] - '1';
    int tc = to[0] - 'a';

    if (p->canMove(tr, tc)) {
        board[p->getRow()][p->getColumn()] = nullptr;
        delete board[tr][tc]; // Capture logic
        board[tr][tc] = p;
        p->setRow(tr);
        p->setColumn(tc);
        return true;
    }
    return false;
}

string ChessBoard::toString() {
    string out = "  a b c d e f g h\n";
    for (int r = 7; r >= 0; r--) {
        out += to_string(r + 1) + " ";
        for (int c = 0; c < 8; c++) {
            out += (board[r][c] ? board[r][c]->toString() : ".") + string(" ");
        }
        out += "\n";
    }
    return out;
}

int main() {
    ChessBoard b;
    b.initialize();
    cout << "Dennis' Chess Engine Initialized\n" << b.toString() << endl;
    return 0;
}
