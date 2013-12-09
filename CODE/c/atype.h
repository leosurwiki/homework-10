#ifndef ATYPE_H_INCLUDED
#define ATYPE_H_INCLUDED
#define SOLVE_CHOSEN -10001
#include <stdio.h>
#define N 32
#define M 32
#include <math.h>
#include <stdlib.h>
#include <time.h>
int deal(int o,int p,int c[],int vertical,int horizontal);
/*
    使用的是退火算法求出一个相对较优解，o,p代表着矩阵c的长宽，vertical为1表示矩阵垂直相连，horizontal为1表示矩阵水平相连。返回得到块的值。
    并将选中的点置为SOLVE_CHOSEN
    执行过以次deal后才可以使用之后的单步功能
*/
int get_situation(int idx);
/*
    返回第i次的退火解
*/
#endif
