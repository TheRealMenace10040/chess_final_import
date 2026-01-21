#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

// CIS-17 Final Project DHN
enum Color { WHITE, BLACK };

class ChessPiece;

class ChessBoard {
private:
    ChessPiece* board[8][8];

public:
    ChessBoard() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = nullptr;
    }

    ~ChessBoard(); 

    void initialize();
    ChessPiece* getPiece(int r, int c) { 
        if (r < 0 || r > 7 || c < 0 || c > 7) return nullptr;
        return board[r][c]; 
    }
    
    void placePiece(ChessPiece* piece, string position);
    bool move(string from, string to);
    bool isPathClear(int r1, int c1, int r2, int c2);
    string toString();
};

class ChessPiece {
protected:
    ChessBoard* board;
    Color color;
    int row, column;

public:
    ChessPiece(ChessBoard* b, Color c) : board(b), color(c) {}
    virtual ~ChessPiece() {} 

    int getRow() { return row; }
    int getColumn() { return column; }
    void setPos(int r, int c) { row = r; column = c; }
    Color getColor() { return color; }

    virtual bool canMove(int toRow, int toColumn) = 0;
    virtual string toString() = 0;
};

// --- Helper for Path Blocking ---
bool ChessBoard::isPathClear(int r1, int c1, int r2, int c2) {
    int dr = (r2 == r1) ? 0 : (r2 > r1 ? 1 : -1);
    int dc = (c2 == c1) ? 0 : (c2 > c1 ? 1 : -1);
    int r = r1 + dr;
    int c = c1 + dc;
    while (r != r2 || c != c2) {
        if (getPiece(r, c) != nullptr) return false;
        r += dr; c += dc;
    }
    return true;
}

// --- Piece Logic ---
class Rook : public ChessPiece {
public:
    Rook(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int tr, int tc) override {
        if (row != tr && column != tc) return false;
        return board->isPathClear(row, column, tr, tc);
    }
    string toString() override { return (color == WHITE) ? "R" : "r"; }
};

class Knight : public ChessPiece {
public:
    Knight(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int tr, int tc) override {
        return (abs(tr - row) == 2 && abs(tc - column) == 1) || (abs(tr - row) == 1 && abs(tc - column) == 2);
    }
    string toString() override { return (color == WHITE) ? "N" : "n"; }
};

class Bishop : public ChessPiece {
public:
    Bishop(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int tr, int tc) override {
        if (abs(tr - row) != abs(tc - column)) return false;
        return board->isPathClear(row, column, tr, tc);
    }
    string toString() override { return (color == WHITE) ? "B" : "b"; }
};

class Queen : public ChessPiece {
public:
    Queen(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int tr, int tc) override {
        if (!((row == tr || column == tc) || (abs(tr - row) == abs(tc - column)))) return false;
        return board->isPathClear(row, column, tr, tc);
    }
    string toString() override { return (color == WHITE) ? "Q" : "q"; }
};

class King : public ChessPiece {
public:
    King(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int tr, int tc) override {
        return abs(tr - row) <= 1 && abs(tc - column) <= 1;
    }
    string toString() override { return (color == WHITE) ? "K" : "k"; }
};

class Pawn : public ChessPiece {
public:
    Pawn(ChessBoard* b, Color c) : ChessPiece(b, c) {}
    bool canMove(int tr, int tc) override {
        int dir = (color == WHITE) ? 1 : -1;
        int startRow = (color == WHITE) ? 1 : 6;
        // Move forward 1
        if (tc == column && tr == row + dir && board->getPiece(tr, tc) == nullptr) return true;
        // Move forward 2 from start
        if (tc == column && row == startRow && tr == row + 2 * dir && board->getPiece(tr, tc) == nullptr && board->getPiece(row+dir, tc) == nullptr) return true;
        // Diagonal capture
        if (abs(tc - column) == 1 && tr == row + dir && board->getPiece(tr, tc) != nullptr && board->getPiece(tr, tc)->getColor() != color) return true;
        return false;
    }
    string toString() override { return (color == WHITE) ? "P" : "p"; }
};

// --- Board Implementation ---
ChessBoard::~ChessBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            delete board[i][j];
}

void ChessBoard::placePiece(ChessPiece* p, string pos) {
    int r = pos[1] - '1', c = pos[0] - 'a';
    board[r][c] = p; p->setPos(r, c);
}

void ChessBoard::initialize() {
    string cols = "abcdefgh";
    placePiece(new Rook(this, WHITE), "a1"); placePiece(new Rook(this, WHITE), "h1");
    placePiece(new Knight(this, WHITE), "b1"); placePiece(new Knight(this, WHITE), "g1");
    placePiece(new Bishop(this, WHITE), "c1"); placePiece(new Bishop(this, WHITE), "f1");
    placePiece(new Queen(this, WHITE), "d1"); placePiece(new King(this, WHITE), "e1");
    for (int i = 0; i < 8; i++) placePiece(new Pawn(this, WHITE), string(1, cols[i]) + "2");
    
    placePiece(new Rook(this, BLACK), "a8"); placePiece(new Rook(this, BLACK), "h8");
    placePiece(new Knight(this, BLACK), "b8"); placePiece(new Knight(this, BLACK), "g8");
    placePiece(new Bishop(this, BLACK), "c8"); placePiece(new Bishop(this, BLACK), "f8");
    placePiece(new Queen(this, BLACK), "d8"); placePiece(new King(this, BLACK), "e8");
    for (int i = 0; i < 8; i++) placePiece(new Pawn(this, BLACK), string(1, cols[i]) + "7");
}

bool ChessBoard::move(string from, string to) {
    if (from.length() < 2 || to.length() < 2) return false;
    int r1 = from[1] - '1', c1 = from[0] - 'a';
    int r2 = to[1] - '1', c2 = to[0] - 'a';
    
    ChessPiece* p = getPiece(r1, c1);
    if (p && p->canMove(r2, c2)) {
        ChessPiece* target = getPiece(r2, c2);
        if (target && target->getColor() == p->getColor()) return false; // Prevent friendly fire
        
        delete board[r2][c2]; // Capture
        board[r2][c2] = p;
        board[r1][c1] = nullptr;
        p->setPos(r2, c2);
        return true;
    }
    return false;
}

string ChessBoard::toString() {
    string out = "  a b c d e f g h\n";
    for (int r = 7; r >= 0; r--) {
        out += to_string(r + 1) + " ";
        for (int c = 0; c < 8; c++) out += (board[r][c] ? board[r][c]->toString() : ".") + string(" ");
        out += "\n";
    }
    return out;
}

int main() {
    ChessBoard b;
    b.initialize();
    string f, t;
    cout << "Welcome to Dennis Nguyen's Chess Engine!\n(Type 'exit' to quit)\n";
    
    while (true) {
        cout << b.toString() << "\nEnter move (e.g., e2 e4): ";
        cin >> f; if (f == "exit") break;
        cin >> t;
        if (!b.move(f, t)) cout << "!!! Invalid Move !!!\n";
    }
    return 0;
}
