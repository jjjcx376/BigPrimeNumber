/* Miller_Rabin 素性检验算法 */
#ifndef _MILLERABIN_H

#define _MILLERABIN_H
//#define TIMES 10

_Bool millerRabin(Num *p,int TIMES)
{
    int low=p->arr[1]-1,right,position,k,i;
    Num *q=(Num *)malloc(sizeof(Num));
    Num a,a_copy;
    _Bool flag=true;
    for (right=0,position=1;low%2==0;)
    {
        right++;
        low>>=1;
        if (right%M==0) low=p->arr[++position];
	}
    q->M_number=(int)((p->arr[0]-right+M-1)/M);
    q->memory=q->M_number*2+1;
    q->arr=(TYPE *)malloc( sizeof(TYPE) * (q->memory+1));
    memset(q->arr,0,sizeof(TYPE) * (q->memory+1));
    q->arr[0]=p->arr[0]-right;
    for (int bit=0,positionq=1,positionp=1,left=0;bit<q->arr[0];)
    {
        if((right+bit)%M==0) low=p->arr[++positionp];
        bit++;
        q->arr[positionq]+=(low&1)<<(left++);
        if (bit%M==0)
        {
            positionq++;
            left=0;
        }
        low>>=1;
	}

    a_copy.arr=(TYPE *)malloc( sizeof(TYPE) * (p->memory+1));
    
    for(i=1;i<=TIMES;i++)
    {
        randoma(&a, 3);
        power(&a,q,p);
        if(coresidual(&a,p)==true) 
        {
            free(a.arr);
            a.arr=NULL;
            continue;
        }
        for(k=1;k<=right;k++)
        {
            a_copy.M_number=a.M_number;
            a_copy.memory=a.memory;
            memset(a_copy.arr,0,sizeof(TYPE) * (p->memory+1));
            memcpy(a_copy.arr,a.arr,sizeof(TYPE) * (a.memory+1));
            multiply(&a,&a_copy,p);
            if (coresidual(&a,p)==true) break;
        }
        free(a.arr);
        if (k==right+1) 
        {
            flag=false;
            break;
        }
    }
    free(q->arr);
    free(q);
    free(a_copy.arr);
    return flag;
}

#endif