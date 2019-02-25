#ifndef NQUEEN_H
#define NQUEEN_H

#include <QObject>
#include <vector>
#include <QDebug>
#include <iostream>
#include <fstream>

class Board
{
public:
    Board(int size=0);
    Board(const Board& other);
    bool isSafe(int row, int col);
    bool isSafeD(int row, int col, int a, int b);
    void placeQueen(int row, int col);
    void removeQueen(int row, int col);
    int size() const;
    void clear();
    void print(std::ostream& os);
    std::vector<int> get_board();
    friend std::ostream& operator<<(std::ostream& os, Board& b);

private:
    //std::vector<std::vector<int>> board;
    std::vector<int> board;
    int board_size;
};

class NQueen : public QObject
{
    Q_OBJECT
public:
    NQueen();
    void solveAllHelper(Board board, int col);
    void solveAll(Board& board);
    bool solveTaskHelper(Board& board, int col, int count);
    bool solveTask(Board& board);
    void placeQueen(Board& board, int row, int col);
    void print_solutions(std::ostream& os);
    int s_row() const;
    int s_col() const;

signals:
    void widget_placeQueen(int row, int col);
    void widget_removeQueen(int row, int col);

private:
    int start_row;
    int start_col;
    std::vector<Board> solutions;
};

#endif // NQUEEN_H
