#ifndef ORIGINAL_H_INCLUDED
#define ORIGINAL_H_INCLUDED
#define SOLVE_CHOSEN -10001
/*
    该文件包含了一系列求对象最大子对象的函数
*/
int maxsumline(int *p,int size);
/*
    求一个序列p中包含的最大子序列，并将被选择的位置置为SOLVE_CHOSEN
*/
int maxsumcycle(int *p,int size);
/*
    求一个循环序列p中包含的最大子序列，并将被选择的位置置为SOLVE_CHOSEN
    循环序列代表着该序列首尾相连。
*/
int maxsumblock(int a[],int n,int m,int cycle,int expand);
/*
    求一个矩阵的最大子矩阵，并将被选择的位置置为SOLVE_CHOSEN
    n，m分别是它的长宽，cycle为1时代表该矩阵的左右是相连的,expand为1时代表着该矩阵的上下是相连的
*/
#endif
