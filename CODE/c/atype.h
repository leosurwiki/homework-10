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
    ʹ�õ����˻��㷨���һ����Խ��Ž⣬o,p�����ž���c�ĳ���verticalΪ1��ʾ����ֱ������horizontalΪ1��ʾ����ˮƽ���������صõ����ֵ��
    ����ѡ�еĵ���ΪSOLVE_CHOSEN
    ִ�й��Դ�deal��ſ���ʹ��֮��ĵ�������
*/
int get_situation(int idx);
/*
    ���ص�i�ε��˻��
*/
#endif
