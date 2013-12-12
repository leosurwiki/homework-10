#include "atype.h"
#include <stdio.h>
#include <string.h>
const int shifti[4]={1,-1,0,0};
const int shiftj[4]={0,0,1,-1};
/*
    偏移量，也就是偏移到一个点上下左右四个方向的点
*/
int father2g[N*M];
/*
    把一个节点在并查集中的父节点对应为一个图节点
*/
struct link
{
    int s,t;
    struct link * next;
};
/*
    边结构
*/
int value[1024];
/*
    图节点的权值
*/
int shifter,totalmax=-10000;
/*
    for shifter in range(4),辅助偏移量用的，totalmax即在一次SAA过程中得到的最大值
*/
int chosen[1024]={-1},holychosen[1024];
/*
    chosen:当前状态下在图上的最优选择,holychosen经过几次SAA后得到的最优解
*/
int m,n,holysum;
/*
    m,n为矩阵column数和row数
    holysum为SAA后得到的最优解
*/
int a[1024];
/*
    a为矩阵节点
*/
int father[N][M];
/*
    点[n,m]在并查集中的父节点
*/
int num=0;
struct link *g[1024];
/*
    图组，前向星形式
*/
int visited[1024]={0};
/*
    是否访问过图节点i。
*/
int available[1024],topq;
/*
    available
*/
FILE * fout;
int tmpchosen[1024];
/*
    与上一次的状态相比发生了改变吗？
*/
int step=0;

void print_situation()
{
    int i,j,yes;
    if (step==0)
    {
        fprintf(fout,"%d %d %d\n",step,n,m);
        for (i=0;i<n;i++)
        {
            for (j=0;j<m;j++)
            {
                fprintf(fout,"%d ",chosen[i*m+j]);
                tmpchosen[i*m+j]=chosen[i*m+j];
            }
            fprintf(fout,"\n");
        }
        step++;
    }
    else
    {
        yes=0;
        for (i=0;i<n;i++)
        {
            for (j=0;j<m;j++)
            {
                if (tmpchosen[i*m+j]!=chosen[i*m+j])
                {
                    yes=1;
                    break;
                }
            }
        }
        if (yes)
        {
            fprintf(fout,"%d %d %d\n",step,n,m);
            for (i=0;i<n;i++)
            {
                for (j=0;j<m;j++)
                {
					if (chosen[father2g[fatherfind(i,j)]])fprintf(fout,"1 "); else fprintf(fout,"0 ");
                    tmpchosen[i*m+j]=chosen[i*m+j];
                }
                fprintf(fout,"\n");
            }
            step++;
        }
    }
}
/*
    打印当前状态下的矩阵
*/
int fatherfind(int x,int y)
{
    int result;
    result = x*m+y;
    if (father[x][y]!=x*m+y)
    {
        result = fatherfind(father[x][y]/m,father[x][y]%m);
        father[x][y] = result;
    }
    return result;
}
/*
    并查集寻祖
*/
void setgraph(int vertical,int horizontal)
{
    int i1,j1,i,j,yes;
    struct link * p;
    num=0;
    for (i=0;i<n;i++){for (j=0;j<m;j++){father[i][j]=i*m+j;}}
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            father2g[i*m+j]=-1;
            if (a[i*m+j]>=0)
            {
                if (i>0&&a[(i-1)*m+j]>=0){father[fatherfind(i,j)/m][fatherfind(i,j)%m]=father[fatherfind(i-1,j)/m][fatherfind(i-1,j)%m];}
                if (i==0&&a[(n-1)*m+j]>=0&&vertical){father[fatherfind(i,j)/m][fatherfind(i,j)%m]=father[fatherfind(n-1,j)/m][fatherfind(n-1,j)%m];}
                if (j>0&&a[i*m+j-1]>=0){father[fatherfind(i,j)/m][fatherfind(i,j)%m]=father[fatherfind(i,j-1)/m][fatherfind(i,j-1)%m];}
                if (j==0&&a[i*m+m-1]>=0&&horizontal){father[fatherfind(i,j)/m][fatherfind(i,j)%m]=father[fatherfind(i,m-1)/m][fatherfind(i,m-1)%m];}
            }
        }
    }
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            if (a[i*m+j]>0)
            {
                if (father2g[fatherfind(i,j)]==-1)
                {
                    father2g[fatherfind(i,j)]=num;
                    value[num]=a[i*m+j];
                    g[num]=NULL;
                    num++;
                }
                else{value[father2g[fatherfind(i,j)]]+=a[i*m+j];}
            }
            else
            {
                value[num]=a[i*m+j];
                father2g[i*m+j]=num;
                g[num]=NULL;
                num++;
            }
        }
    }
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            for (shifter=0;shifter<4;shifter++)
            {
                i1=i+shifti[shifter];
                j1=j+shiftj[shifter];
                if (i1<0||i1>=n)
                {
                    if (vertical) {i1=(i1+n)%n;}
                    else {continue;}
                }
                if (j1<0||j1>=m)
                {
                    if (horizontal) {j1=(j1+m)%m;}
                    else {continue;}
                }
                p=g[father2g[fatherfind(i,j)]];
                yes=1;
                while (p!=NULL)
                {
                    if (p->t==father2g[fatherfind(i1,j1)])
                    {
                        yes=0;
                        break;
                    }
                    p=p->next;
                }
                if (yes)
                {
                    p=(struct link*)malloc(sizeof(struct link));
                    p->next=g[father2g[fatherfind(i,j)]];
                    p->t=father2g[fatherfind(i1,j1)];
                    p->s=value[p->t]>=0?1:0;
                    g[father2g[fatherfind(i,j)]]=p;
                }
            }
        }
    }
}
/*
    缩点建图
*/
int estimate(int v)
{
    int result=value[v];
    struct link * p;
    p=g[v];
    while (p!=NULL)
    {
        if (!visited[p->t]){result=value[p->t]>0?value[p->t]+result:result;}
        p=p->next;
    }
    return result;
}
/*
    选择该节点的估价函数
*/
int expand(int v)
{
    struct link * p;
    int t;
    int result;
    result=value[v];
    p=g[v];
    visited[v]=1;available[v]=0;
    t=0;
    while (p!=NULL)
    {
        if (!visited[p->t])
        {
            if (value[p->t]<0){available[p->t]=1;}
            else{visited[p->t]=1;result+=value[p->t];}
        }
        p=p->next;
    }
    return result;
}
/*
    实在行扩张
*/
void pseudoexpand(int v)
{
    struct link * p;
    int i;
    for (i=0;i<1024;i++)
    {
        chosen[i] = visited[i];
    }
    chosen[v]=1;
    p=g[v];
    while (p!=NULL)
    {
        if (value[p->t]>=0){chosen[p->t]=1;}
        p=p->next;
    }
}
/*
    伪扩张
*/
void SAA(int v,float T,float r,float Tmin)
{
    int i,max,maxS=0,ranS,tmp,SMR,sum,ri,j;//State
    struct link * p;
    srand(time(0));
    sum=value[v];
    for(i=0;i<1024;i++){visited[i]=0;available[i]=0;}
    p=g[v];visited[v]=1;
    while (p!=NULL)
    {
        if (!visited[p->t]){available[p->t]=1;}
        p=p->next;
    }
    while(1)
    {
        max=-10000000;
        ri=0;
        for (i=0;i<num;i++)
        {
            if (visited[i]) {continue;}
            if (available[i])
            {
                tmp=estimate(i);
                if (tmp>max)
                {
                    max=tmp;
                    maxS=i;
                }
                ri++;
            }
        }
        if (ri==0) {break;}
        ranS=rand()%ri+1;
        for (i=0;i<num;i++)
        {
            if (available[i]&&!visited[i])
            {
                ranS--;
                if (ranS==0)
                {
                    ranS=i;
                    break;
                }
            }
        }
        if (visited[ranS]||!available[ranS]) {return ;}
        SMR=0;
        tmp=estimate(maxS);
        if (sum+tmp>totalmax)
		{
			totalmax=sum+tmp;pseudoexpand(maxS);
			print_situation();
		}
        if (tmp>0){SMR=1;}
        else
        {
            if (exp(tmp/T)>(rand()%10000)/10000.0){SMR=1;}
        }
        T*=r;
        tmp=SMR==1?estimate(maxS)-estimate(ranS):estimate(ranS);
        if (sum+estimate(ranS)>totalmax)
		{
			totalmax=sum+estimate(ranS);
			pseudoexpand(ranS);
			print_situation();
		}
        if (exp(tmp/T)>(rand()%10000)/10000.0){SMR=2;}
        T*=r;
        switch(SMR)
        {
            case 1:sum+=expand(maxS);break;
            case 2:sum+=expand(ranS);break;
        }
        if (T<Tmin){break;}
        if (sum>totalmax)
		{
			totalmax=sum;printf("%d\n",totalmax);
			for (j=0;j<1024;j++){chosen[j]=visited[j];}
			print_situation();
		}
    }
}
/*
    退火算法主体
*/
struct link * DFS(int v)
{
    struct link * p;
    struct link * q;
    struct link * tmp;
    int result=-100000;
    p = g[v];
    visited[v] = 1;
    q = (struct link *)malloc(sizeof(struct link));
    q->next = NULL;q->s = value[v];q->t = v;
    while (p!=NULL)
    {
        //printf("%d %d %d\n",p->t,visited[p->t],chosen[p->t]);
        if (chosen[p->t])
        {
            visited[v] = 0;
            q->next = NULL;q->s = value[v];q->t = v;
            return q;
        }
        if (visited[p->t]||value[p->t]>0)
        {
            p=p->next;
            continue;
        }
        if (value[p->t]<0&&!chosen[p->t])
        {
            tmp = DFS(p->t);
            if (tmp!=NULL&&result<tmp->s+value[v])
            {
                result = tmp->s+value[v];
                q->next = tmp;
                q->s=tmp->s+value[v];
            }
        }
        p = p->next;
    }
    visited[v] = 0;
    if (q->next!=NULL) return q;
    return NULL;
}
/*
    图遍历
*/
void patch()
{
    int i,j,sum,yes;
    struct link * p,*q;
    while(1)
    {
        yes = 0;
        for (i=0;i<num;i++)
        {
            if (value[i]>0&&!chosen[i])
            {
                for (j=0;j<1024;j++) {visited[j]=0;}
                q = DFS(i);
                p = q;
                if (p==NULL)
                {
                    continue;
                }
                sum = p->s;
                if (sum>=0)
                {
                    yes = 1;
                    while (q!=NULL)
                    {
                        chosen[q->t] = 1;
                        q = q->next;
                    }
                }
            }
        }
        if (yes==0) {break;}
    }
}
/*
    对当前状态进行贪心扩张
*/
int deal(int o,int p,int c[],int vertical,int horizontal)
{
    int i,j,z;
    float x=100;
    fout = fopen("internal_result.dat","w");
    m=p;
    n=o;
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            a[i*m+j]=c[i*m+j];
        }
    }
    setgraph(vertical,horizontal);
    holysum = 0;
    for(z=0;z<20;z++)
    {
        for (i=0;i<num;i++)
    	{
    	    if (value[i]>0)
    	    {
    	        x=1;
    	        for (j=0;j<(num/2+1);j++)
    	        {
    	            x*=0.5;
    	            SAA(i,x,0.999,0.001);
    	        }
    	    }
    	}
    	patch();
		print_situation();
    	totalmax = 0;
    	for (i=0;i<num;i++)
    	{
    	    if(chosen[i])
    	    {
    	        totalmax+=value[i];
    	    }
    	}
    	if (totalmax>holysum)
        {
            holysum = totalmax;
            for (i=0;i<num;i++)
            {
                holychosen[i] = chosen[i];
            }

        }
    }
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            if (holychosen[father2g[fatherfind(i,j)]])
            {
                c[i*m+j]=-10001;
            }
        }
    }
    fclose(fout);
    return holysum;
}
/*
    进行多次退火，得到一个最优解
*/
int main()
{
    int a[15]={1,-2,3,-4,5,-6,7,-8,9,-10,11,-12,13,-14,15},i;
    deal(3,5,a,0,0);
}
