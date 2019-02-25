#include "nqueen.h"

Board::Board(int size)
    : board_size(size)
{
    board.assign(board_size*board_size, 0);
//    for(auto& el : board){
//        el.assign(board_size, 0);
//    }
}

Board::Board(const Board& other)
    : board_size(other.board_size)
{
    board = other.board;
}

bool Board::isSafe(int row, int col){
//    for (int i=0; i < board_size; i++){
//        if(board[i][col] || board[row][i]) return false;
//        int temp = col - row + i;//from up left to bottom right
//        if(temp >= 0 && temp < board_size){
//            if(board[i][temp]) return false;
//        }
//        temp = col + row - i;//from bottom left to up right
//        if(temp >= 0 && temp < board_size){
//            if(board[temp][i]) return false;
//        }
//    }
    for (int i=0; i < board_size; i++){
        if(board[i*board_size + col] || board[row*board_size + i]) return false;
        int temp = col - row + i;//from up left to bottom right
        if(temp >= 0 && temp < board_size){
            if(board[i*board_size + temp]) return false;
        }
        temp = col + row - i;//from bottom left to up right
        if(temp >= 0 && temp < board_size){
            if(board[temp * board_size + i]) return false;
        }
    }
    return true;
}

void Board::placeQueen(int row, int col){
//    board[row][col] = 1;
    board[row*board_size + col] = 1;
}

void Board::removeQueen(int row, int col){
//    board[row][col] = 0;
    board[row*board_size + col] = 0;
}

int Board::size() const{
    return board_size;
}

void Board::clear(){
    board.clear();
    board.assign(board_size*board_size, 0);
}

void Board::print(std::ostream& os){
    os << "   ";
    for(int y = 0; y < board_size; y++){
        os << QString(" %1 ").arg((char)(y+65)).toStdString();
    }
    os << '\n';
    for(int y = 0; y < board_size; y++){
        os <<" "<<y+1 << " ";
        for(int x = 0; x < board_size; x++){
            if(board[y*board_size + x])
                os << " Q ";
            else
                os << " - ";
        }
        os << '\n';
    }
    os << '\n';
}

std::vector<int> Board::get_board(){
    return board;
}

std::ostream& operator<<(std::ostream& os, Board& b){
    b.print(os);
    return os;
}

NQueen::NQueen() :
    QObject()
    {}

bool NQueen::solveTaskHelper(Board& board, int col, int count){
    if(!count){
        solutions.push_back(board);
        return true;
    }
    for(int i = 0; i < board.size(); i++){
        if(board.isSafe(i, col)){
            board.placeQueen(i, col);
            emit widget_placeQueen(i, col);
            int t_col = col;
            if(++t_col >= board.size()) t_col = 0;
            if(solveTaskHelper(board, t_col, count-1)) return true;
            board.removeQueen(i, col);
            emit widget_removeQueen(i, col);
        }
    }
    return false;
}

bool NQueen::solveTask(Board& board){
    solutions.clear();
    return solveTaskHelper(board, start_col+1, board.size()-1);
}

void NQueen::solveAllHelper(Board board, int col){
    if(col >= board.size()){
        solutions.push_back(board);
        return;
    }
    for(int i = 0; i < board.size(); i++){
        if(board.isSafe(i, col)){
            board.placeQueen(i, col);
            emit widget_placeQueen(i, col);
            solveAllHelper(board, col+1);
            board.removeQueen(i, col);
            emit widget_removeQueen(i, col);
        }
    }
}

void NQueen::solveAll(Board& board){
    solutions.clear();
    board.clear();
    solveAllHelper(board, 0);
}

void NQueen::print_solutions(std::ostream& os){
    for(int i = 0; i < solutions.size(); i++){
        os << "Solution #" << i << "\n";
        os << solutions[i];
    }
}

void NQueen::placeQueen(Board& board, int row, int col){
    board.clear();
    board.placeQueen(row, col);
    emit widget_placeQueen(row, col);
    start_row = row;
    start_col = col;
}

int NQueen::s_row() const{
    return start_row;
}

int NQueen::s_col() const{
    return start_col;
}



