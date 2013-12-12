#include "original.h"
#include <stdio.h>
int prex1=-1,preyy=-1,prex2=-1,prey2=-1;
int x1,yy,x2,y2;
FILE * fout;
int X,Y,SUM=0,ok=0,corl;/*corl表示是否有参数h*/
void print_XY()
{
    int i,j;
    int tmp[1024];
    if (!ok) return ;
    if (prex1==x1&&preyy==yy&&prex2==x2&&prey2==y2)
    {
        return;
    }
    else
    {
        prex1=x1;preyy=yy;prex2=x2;prey2=y2;
    }
    for (i=0;i<X;i++)
    {
        for (j=0;j<Y;j++)
        {
            tmp[i*Y+j] = 0;
        }
    }
    for (i=x1;;i=(i+1)%X)
    {
        for (j=yy;;j=(j+1)%Y)
        {
            tmp[i*Y+j]=1;
            if (j==y2) {break;}
        }
        if (i==x2) {break;}
    }
    fprintf(fout,"%d %d %d\n",SUM,X,Y);
    for (i=0;i<X;i++)
    {
        for (j=0;j<Y;j++)
        {
            fprintf(fout,"%d ",tmp[i*Y+j]);
        }
        fprintf(fout,"\n");
    }
    SUM++;
}
/*
    打印当前状态
*/
int maxsumline(int *p,int size)
{
    int i;
    int sum,ans,tmp=0;
    s = 1;
    sum=0;
    ans=-1000000;
    for(i=0;i<size;i++)
    {
        if(sum<0){sum=0;tmp=i;}
        sum+=p[i];
        if(ans<sum){ans=sum;yy=tmp;y2=i;print_XY();}
    }
    corl = 0;
    return ans;
}
/*
    求一个序列的最大连续子序列
*/
int maxsumcycle(int *p,int size)
{
    int i;
    int sum,ans,tmp,tmp1;
    sum=0;
    corl=0;
    ans=maxsumline(p,size);
    for (i=0;i<size;i++)
    {
        sum+=p[i];
        p[i] = -p[i];
    }
    if ((tmp=(sum+maxsumline(p,size)))>ans)
    {
        tmp1=yy;
        yy=(y2+1)%size;
        y2=(tmp1-1+size)%size;
        print_XY();
        for (i=0;i<size;i++)
        {
            sum+=p[i];
            p[i] = -p[i];
        }
        return tmp;
    }
    else
    {
        for (i=0;i<size;i++)
        {
            sum+=p[i];
            p[i] = -p[i];
        }
        maxsumline(p,size);
        return ans;
    }
}
/*
    求一个环的最大子序列
*/
int maxsumblock(int a[],int n,int m,int cycle,int expand)
{
    int i,j,k,tmp,totalmax=0;
    int sum[32][32];
    int t[32];
    X=n;Y=m;
    fout = fopen("internal_result.dat","w");
    yy=0;
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            if (i!=0)
            {
                sum[i][j]=sum[i-1][j]+a[i*m+j];
            }
            else
            {
                sum[i][j]=a[i*m+j];
            }
        }
    }
    for (i=0;i<n;i++)
    {
        for (j=0;j<=i;j++)
        {
            for (k=0;k<m;k++)
            {
                t[k] = (j==0)?0:-sum[j-1][k];
                t[k] +=sum[i][k];
            }
            tmp = cycle?maxsumcycle(t,m):maxsumline(t,m);
            if (maxsumline(t,m)>tmp) {ok =1;tmp=maxsumline(t,m);ok =0;}
            else {ok =1;tmp=maxsumcycle(t,m);ok =0;}
            if (tmp>totalmax)
            {
                totalmax= tmp;
                x1=j;x2=i;
                print_XY();
            }
            if (expand)
            {
                for (k=0;k<m;k++)
                {
                    t[k]=sum[n-1][k]-sum[i][k]+sum[j][k];
                }
                tmp = cycle?maxsumcycle(t,m):maxsumline(t,m);
                if (tmp>totalmax)
                {
                    totalmax= tmp;
                    x1=i+1;x2=j;
                    ok=2;
                    print_XY();
                    ok=0;
                }
            }
        }
    }
    for (k=0;k<m;k++)
    {
        tmp=0;
        for (i=x1;;i=(i+1)%n)
        {
            tmp+=a[i*m+k];
            if (i==x2) {break;}
        }
        t[k]=tmp;
    }
    tmp = maxsumline(t,m);

    if (cycle)
    {
        if (maxsumcycle(t,m)>tmp)
        {
            ok =1;
            tmp=maxsumcycle(t,m);
            ok =0;
        }
        else
        {
            ok = 1;
            maxsumline(t,m);
            ok =0;
        }
    }
    for (i=x1;;i=(i+1)%n)
    {
        for (j=yy;;j=(j+1)%m)
        {
            a[i*m+j]=-10001;
            if (j==y2) {break;}
        }
        if (i==x2) {break;}
    }
    fclose(fout);
    return totalmax;
}
/*
    主程序
*/
int main()
{
    int a[6]={1,-2,3,1,-2,3};
    printf("%d\n",maxsumblock(a,2,3,1,0));
    return 0;
}
