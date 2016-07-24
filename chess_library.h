#ifndef CHESS_TABLE_H
#define CHESS_TABLE_H

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

/*
 * 定义一个二维数组记录棋盘上每个位置的分值，
 * 根据棋盘分布问题，越中心的点分值越高。
 */
const int position_score[CK_SIZE][CK_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

extern int position[CK_SIZE][CK_SIZE];
extern bool setblack;

#define MAX_SIZE 7
#define MIN_SIZE 4

#define NO_PIECE '0'
#define OUR      '1'
#define ENEMY    '2' 

unordered_map<string, size_t> chess_type = {
    // 五连
    {"11111", 100000},
    // 活四
    {"011110", 10000},
    // 冲四
    {"011112", 500}, {"0101110", 500}, {"0110110", 500},
    {"211110", 500}, {"0111010", 500}, 
    // 活三
    {"01110", 200}, {"010110", 200},
                    {"011010", 200},
    // 眠三
    {"001112", 50}, {"010112", 50}, {"011012", 50}, {"10011", 50}, {"10101", 50}, {"2011102", 50},
    {"211100", 50}, {"211010", 50}, {"210110", 50}, {"11001", 50},
    // 活二
    {"00110", 5}, {"01010", 5}, {"010010", 5},
    // 眠二
    {"000112", 3}, {"001012", 3}, {"010012", 3}, {"10001", 3}, {"2010102", 3}, {"2011002", 3}, 
    {"211000", 3}, {"210100", 3}, {"210010", 3},                               {"2001102", 3},
    // 死四
    {"211112", -5},
    // 死三
    {"21112", -5},
    // 死二
    {"2112", -5}
};

#endif
