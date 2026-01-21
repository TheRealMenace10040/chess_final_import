#include <iostream>
#include <string>
using namespace std;
// CIS-17 Final Project DHN
enum Color { WHITE, BLACK };

class ChessPiece;

class ChessBoard {
private:
    ChessPiece* board[8][8];

public:
    ChessBoard();

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
    ChessPiece(ChessBoard* board, Color color);

    int getRow();

    int getColumn();

    void setRow(int row);

    void setColumn(int column);

    Color getColor();

    string getPosition();

    void setPosition(string position);

    virtual bool canMove(int toRow, int toColumn) = 0;

    virtual string toString() = 0;
};

class Rook : public ChessPiece {
public:
    Rook(ChessBoard* board, Color color);

    bool canMove(int toRow, int toColumn);

    string toString();
};

class Knight : public ChessPiece {
public:
    Knight(ChessBoard* board, Color color);

    bool canMove(int toRow, int toColumn);

    string toString();
};

class Bishop : public ChessPiece {
public:
    Bishop(ChessBoard* board, Color color);

    bool canMove(int toRow, int toColumn);

    string toString();
};

class Queen : public ChessPiece {
public:
    Queen(ChessBoard* board, Color color);

    bool canMove(int toRow, int toColumn);

    string toString();
};

class King : public ChessPiece {
public:
    King(ChessBoard* board, Color color);

    bool canMove(int toRow, int toColumn);

    string toString();
};

class Pawn : public ChessPiece {
public:
    Pawn(ChessBoard* board, Color color);

    bool canMove(int toRow, int toColumn);

    string toString();
};

ChessBoard::ChessBoard() {
    // Initialize the board with nullptr (no pieces)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
}

void ChessBoard::initialize() {
    // Place white pieces
    placePiece(new Rook(this, WHITE), "a1");
    placePiece(new Knight(this, WHITE), "b1");
    placePiece(new Bishop(this, WHITE), "c1");
    placePiece(new Queen(this, WHITE), "d1");
    placePiece(new King(this, WHITE), "e1");
    placePiece(new Bishop(this, WHITE), "f1");
    placePiece(new Knight(this, WHITE), "g1");
    placePiece(new Rook(this, WHITE), "h1");
    for (char column = 'a'; column <= 'h'; column++) {
        placePiece(new Pawn(this, WHITE), string(1, column) + "2");
    }

    // Place black pieces
    placePiece(new Rook(this, BLACK), "a8");
    placePiece(new Knight(this, BLACK), "b8");
    placePiece(new Bishop(this, BLACK), "c8");
    placePiece(new Queen(this, BLACK), "d8");
    placePiece(new King(this, BLACK), "e8");
    placePiece(new Bishop(this, BLACK), "f8");
    placePiece(new Knight(this, BLACK), "g8");
    placePiece(new Rook(this, BLACK), "h8");
    for (char column = 'a'; column <= 'h'; column++) {
        placePiece(new Pawn(this, BLACK), string(1, column) + "7");
    }
}

ChessPiece* ChessBoard::getPiece(string position) {
    int row = position[1] - '1';
    int column = position[0] - 'a';
    return board[row][column];
}

void ChessBoard::placePiece(ChessPiece* piece, string position) {
    int row = position[1] - '1';
    int column = position[0] - 'a';
    board[row][column] = piece;
    piece->setRow(row);
    piece->setColumn(column);
    piece->setPosition(position);
}

bool ChessBoard::move(string fromPosition, string toPosition) {
    ChessPiece* piece = getPiece(fromPosition);
    if (piece == nullptr) {
        cout << "There is no piece at position " << fromPosition << "." << endl;
        return false;
    }

    int toRow = toPosition[1] - '1';
    int toColumn = toPosition[0] - 'a';

    if (piece->canMove(toRow, toColumn)) {
        ChessPiece* destinationPiece = getPiece(toPosition);
        if (destinationPiece != nullptr && destinationPiece->getColor() == piece->getColor()) {
            cout << "Cannot capture your own piece at position " << toPosition << "." << endl;
            return false;
        }

        // Move the piece
        board[piece->getRow()][piece->getColumn()] = nullptr;
        piece->setRow(toRow);
        piece->setColumn(toColumn);
        piece->setPosition(toPosition);
        board[toRow][toColumn] = piece;

        cout << "Moved " << piece->toString() << " from " << fromPosition << " to " << toPosition << "." << endl;
        return true;
    }

    cout << "Invalid move for " << piece->toString() << " from " << fromPosition << " to " << toPosition << "." << endl;
    return false;
}

string ChessBoard::toString() {
    string boardString;
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            ChessPiece* piece = board[row][column];
            if (piece != nullptr) {
                boardString += piece->toString();
            } else {
                boardString += ".";
            }
            boardString += " ";
        }
        boardString += "\n";
    }
    return boardString;
}

ChessPiece::ChessPiece(ChessBoard* board, Color color) {
    this->board = board;
    this->color = color;
}

int ChessPiece::getRow() {
    return row;
}

int ChessPiece::getColumn() {
    return column;
}

void ChessPiece::setRow(int row) {
    this->row = row;
}

void ChessPiece::setColumn(int column) {
    this->column = column;
}

Color ChessPiece::getColor() {
    return color;
}

string ChessPiece::getPosition() {
    string position;
    position += 'a' + column;
    position += '1' + row;
    return position;
}

void ChessPiece::setPosition(string position) {
    column = position[0] - 'a';
    row = position[1] - '1';
}

Rook::Rook(ChessBoard* board, Color color) : ChessPiece(board, color) {}

bool Rook::canMove(int toRow, int toColumn) {
    int fromRow = getRow();
    int fromColumn = getColumn();

    if (fromRow == toRow && fromColumn == toColumn) {
        return false;  
    }

    if (fromRow == toRow) {
        // Moving horizontally
        int columnStep = (toColumn > fromColumn) ? 1 : -1;
        for (int column = fromColumn + columnStep; column != toColumn; column += columnStep) {
            if (board->getPiece(string(1, 'a' + column) + to_string(fromRow)) != nullptr) {
                return false;  // There is a piece in the way
            }
        }
        return true;
    }

    if (fromColumn == toColumn) {
        // Moving vertically
        int rowStep = (toRow > fromRow) ? 1 : -1;
        for (int row = fromRow + rowStep; row != toRow; row += rowStep) {
            if (board->getPiece(string(1, 'a' + fromColumn) + to_string(row)) != nullptr) {
                return false;  // There is a piece in the way
            }
        }
        return true;
    }

    return false;  // Rook cant move diagonally
}

string Rook::toString() {
    return (color == WHITE) ? "R" : "r";
}

Knight::Knight(ChessBoard* board, Color color) : ChessPiece(board, color) {}

bool Knight::canMove(int toRow, int toColumn) {
    int fromRow = getRow();
    int fromColumn = getColumn();

    int rowDiff = abs(toRow - fromRow);
    int columnDiff = abs(toColumn - fromColumn);

    return (rowDiff == 2 && columnDiff == 1) || (rowDiff == 1 && columnDiff == 2);
}

string Knight::toString() {
    return (color == WHITE) ? "N" : "n";
}

Bishop::Bishop(ChessBoard* board, Color color) : ChessPiece(board, color) {}

bool Bishop::canMove(int toRow, int toColumn) {
    int fromRow = getRow();
    int fromColumn = getColumn();

    if (fromRow == toRow && fromColumn == toColumn) {
        return false; 
    }

    int rowDiff = abs(toRow - fromRow);
    int columnDiff = abs(toColumn - fromColumn);

    if (rowDiff != columnDiff) {
        return false;  // Bishop can only move diagonally
    }

    int rowStep = (toRow > fromRow) ? 1 : -1;
    int columnStep = (toColumn > fromColumn) ? 1 : -1;

    for (int row = fromRow + rowStep, column = fromColumn + columnStep; row != toRow; row += rowStep, column += columnStep) {
        if (board->getPiece(string(1, 'a' + column) + to_string(row)) != nullptr) {
            return false;  // There is an obstacle in the way
        }
    }

    return true;
}

string Bishop::toString() {
    return (color == WHITE) ? "B" : "b";
}

Queen::Queen(ChessBoard* board, Color color) : ChessPiece(board, color) {}

bool Queen::canMove(int toRow, int toColumn) {
    int fromRow = getRow();
    int fromColumn = getColumn();

    if (fromRow == toRow && fromColumn == toColumn) {
        return false;  // The selected place to go is the same as the current position
    }

    if (fromRow == toRow || fromColumn == toColumn) {
        // Queen is moving horizontally or vertically
        int rowDiff = abs(toRow - fromRow);
        int columnDiff = abs(toColumn - fromColumn);

        if (fromRow == toRow) {
            // Moving horizontally
            int columnStep = (toColumn > fromColumn) ? 1 : -1;
            for (int column = fromColumn + columnStep; column != toColumn; column += columnStep) {
                if (board->getPiece(string(1, 'a' + column) + to_string(fromRow)) != nullptr) {
                    return false;  // There is a piece in the way
                }
            }
            return true;
        }

        if (fromColumn == toColumn) {
            // Moving vertically
            int rowStep = (toRow > fromRow) ? 1 : -1;
            for (int row = fromRow + rowStep; row != toRow; row += rowStep) {
                if (board->getPiece(string(1, 'a' + fromColumn) + to_string(row)) != nullptr) {
                    return false;  // There is a piece in the way
                }
            }
            return true;
        }
    }

    if (abs(toRow - fromRow) == abs(toColumn - fromColumn)) {
        // Queen is moving diagonally
        int rowDiff = abs(toRow - fromRow);
        int columnDiff = abs(toColumn - fromColumn);

        int rowStep = (toRow > fromRow) ? 1 : -1;
        int columnStep = (toColumn > fromColumn) ? 1 : -1;

        for (int row = fromRow + rowStep, column = fromColumn + columnStep; row != toRow; row += rowStep, column += columnStep) {
            if (board->getPiece(string(1, 'a' + column) + to_string(row)) != nullptr) {
                return false;  // There is an a piece in the way
            }
        }

        return true;
    }

    return false;  // Queen cannot move in any other pattern
}

string Queen::toString() {
    return (color == WHITE) ? "Q" : "q";
}

King::King(ChessBoard* board, Color color) : ChessPiece(board, color) {}

bool King::canMove(int toRow, int toColumn) {
    int fromRow = getRow();
    int fromColumn = getColumn();

    int rowDiff = abs(toRow - fromRow);
    int columnDiff = abs(toColumn - fromColumn);

    return (rowDiff <= 1 && columnDiff <= 1);
}

string King::toString() {
    return (color == WHITE) ? "K" : "k";
}

Pawn::Pawn(ChessBoard* board, Color color) : ChessPiece(board, color) {}

bool Pawn::canMove(int toRow, int toColumn) {
    int fromRow = getRow();
    int fromColumn = getColumn();

    int rowDiff = toRow - fromRow;
    int columnDiff = abs(toColumn - fromColumn);

    if (getColor() == WHITE) {
        if (rowDiff == 1 && columnDiff == 0 && board->getPiece(string(1, 'a' + toColumn) + to_string(toRow)) == nullptr) {
            return true;  // Moving one step forward
        }
        if (fromRow == 1 && rowDiff == 2 && columnDiff == 0 && board->getPiece(string(1, 'a' + toColumn) + to_string(toRow)) == nullptr &&
            board->getPiece(string(1, 'a' + toColumn) + to_string(toRow - 1)) == nullptr) {
            return true;  // Moving two steps forward from the starting position
        }
        if (rowDiff == 1 && columnDiff == 1 && board->getPiece(string(1, 'a' + toColumn) + to_string(toRow)) != nullptr) {
            return true;  // Capturing an opponent's piece diagonally
        }
    } else {
        if (rowDiff == -1 && columnDiff == 0 && board->getPiece(string(1, 'a' + toColumn) + to_string(toRow)) == nullptr) {
            return true;  // Moving one step forward
        }
        if (fromRow == 6 && rowDiff == -2 && columnDiff == 0 && board->getPiece(string(1, 'a' + toColumn) + to_string(toRow)) == nullptr &&
            board->getPiece(string(1, 'a' + toColumn) + to_string(toRow + 1)) == nullptr) {
            return true;  // Moving two steps forward from the starting position
        }
        if (rowDiff == -1 && columnDiff == 1 && board->getPiece(string(1, 'a' + toColumn) + to_string(toRow)) != nullptr) {
            return true;  // Capturing an opponent's piece diagonally
        }
    }

    return false;
}

string Pawn::toString() {
    return (color == WHITE) ? "P" : "p";
}

int main() {
    ChessBoard board;
    board.initialize();

    cout << "Welcome to Dennis' Mess!" << endl;
    cout << "To make a move, enter the position of the piece you want to move and the position you want to move it to." << endl;
    cout << "For example, enter 'e2' to select the pawn at e2." << endl;

    Color currentPlayer = WHITE;
    bool gameEnded = false;

    while (!gameEnded) {
        cout << endl;
        cout << "Current board:" << endl;
        cout << board.toString() << endl;

        string fromPosition, toPosition;

        // Ask the player for the piece to move and where they want to move
        cout << "Player " << (currentPlayer == WHITE ? "WHITE" : "BLACK") << ", it's your turn." << endl;
        cout << "Which piece would you like to move? ";
        cin >> fromPosition;
        cout << "Where would you like to move it? ";
        cin >> toPosition;

        if (board.move(fromPosition, toPosition)) {
            currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
        }

        gameEnded = true;
    }

    cout << endl;
    cout << "Game over!" << endl;
    cout << "Final board:" << endl;
    cout << board.toString() << endl;

    return 0;
}