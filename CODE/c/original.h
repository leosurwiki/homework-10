#ifndef ORIGINAL_H_INCLUDED
#define ORIGINAL_H_INCLUDED
#define SOLVE_CHOSEN -10001
/*
    ���ļ�������һϵ�����������Ӷ���ĺ���
*/
int maxsumline(int *p,int size);
/*
    ��һ������p�а�������������У�������ѡ���λ����ΪSOLVE_CHOSEN
*/
int maxsumcycle(int *p,int size);
/*
    ��һ��ѭ������p�а�������������У�������ѡ���λ����ΪSOLVE_CHOSEN
    ѭ�����д����Ÿ�������β������
*/
int maxsumblock(int a[],int n,int m,int cycle,int expand);
/*
    ��һ�����������Ӿ��󣬲�����ѡ���λ����ΪSOLVE_CHOSEN
    n��m�ֱ������ĳ���cycleΪ1ʱ����þ����������������,expandΪ1ʱ�����Ÿþ����������������
*/
#endif
