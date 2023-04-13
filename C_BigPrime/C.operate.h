#ifndef _OPERATE_H

#define _OPERATE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <math.h>


#define TYPE unsigned int
#define M 11
#define MAXNUMBER 2047
int LENGTH ;
typedef struct BigNumber
{
	int M_number;
	int memory;
	TYPE *arr;
} Num;


void generate(Num *p, int length)
{
    p->M_number=(int)((length+M-1)/M);
    p->memory=p->M_number*2+1;
    p->arr=(TYPE *)malloc( sizeof(TYPE) * (p->memory+1));
    memset(p->arr,0,sizeof(TYPE) * (p->memory+1));
    p->arr[0]=length;
    p->arr[1]=733;
    p->arr[2]=1370;
}

void random(Num *p, int length)
{
    int position=1,left=0;
    p->M_number=(int)( (length+M-1)/M );
    p->memory=p->M_number*2+1;
    p->arr=(TYPE *)malloc( sizeof(TYPE) * (p->memory+1));
    memset(p->arr,0,sizeof(TYPE) * (p->memory+1));
    p->arr[0]=length;

	for (int bit=1; bit<=length; bit++)
    {
        
		if (bit==1||bit==length) 
            p->arr[position]+=1<<(left++);
		else 
            p->arr[position]+=(rand()&1)<<(left++);
		if (bit%M==0)
        {
                position++;
                left=0;
        }
	}
}

void add(Num *num1,Num *num2)
{
    int position;
    TYPE c=0;
    for(position=1;position<=num1->M_number||position<=num2->M_number;position++)
    {
        if(num2->arr[position]!=0)
            num1->arr[position]+=num2->arr[position];
        else
             break;
    }
    for (position=1; position<=num1->memory; position++)
    {
        c+=num1->arr[position];
        num1->arr[position]=c&MAXNUMBER;
        c>>=M;
        if (num1->arr[position]!=0) num1->M_number=position;
    }
    num1->arr[0]=M*(num1->M_number-1);
    for(int top=num1->arr[num1->M_number];top>0;top>>=1) 
        num1->arr[0]++;
}

int minus(Num *num1,Num *num2)
{
    int position,borrow=0;
    if(num1->arr[0]<num2->arr[0]) return -1;
    else if(num1->arr[0]==num2->arr[0])
	    for (position=num1->M_number;position>=1;position--)
        {
		    if (num1->arr[position]>num2->arr[position]) break;
		    else if (num1->arr[position]<num2->arr[position]) return -1;
	    }
	for (position=1;position<=num1->M_number;position++)
    {
		if (num1->arr[position]>=num2->arr[position]+borrow) 
        {
			num1->arr[position]-=num2->arr[position]+borrow;
			borrow=0;
		}
		else 
        {
			num1->arr[position]+=MAXNUMBER+1-(num2->arr[position]+borrow);
			borrow=1;
		}
	}
	for (position=num1->M_number;position>=1;position--)
		if (num1->arr[position]==0) num1->M_number--;
		else break; 
    num1->arr[0]=M*(num1->M_number-1);
    for(int top=num1->arr[num1->M_number];top>0;top>>=1) 
        num1->arr[0]++; 
    return 1;
}

void randoma(Num *p, int length)
{
    int position=1,left=0;
    p->M_number=(int)( (length+M-1)/M );
    p->memory=(int)( (LENGTH+M-1)/M )*2+1;
    p->arr=(TYPE *)malloc( sizeof(TYPE) * (p->memory+1));
    memset(p->arr,0,sizeof(TYPE) * (p->memory+1));
	for (int bit=1; bit<=length; bit++)
    {

        p->arr[position]+=(rand()&1)<<(left++);
		if (bit%M==0)
        {
                position++;
                left=0;
        }
	}
    for (position=p->M_number;position>=1; position--)
    {
        if (p->arr[position]==0)
            p->M_number--;//
        else break;
    }
    if(p->M_number!=0)p->arr[0]=M*(p->M_number-1);
    for(int top=p->arr[p->M_number];top>0;top>>=1) 
        p->arr[0]++;
    if ((p->arr[0]==1 && p->arr[1]<=2)||p->arr[0]==0)
    {
        free(p->arr);
        randoma(p,length);
    }
}

int subtract(Num *num1, Num *p, unsigned long q, int n)
{
    int result;
	if (q==0) return(-1);
    Num p_copy;
    p_copy.M_number=p->M_number+n;
    p_copy.memory=p->memory;
    p_copy.arr=(TYPE *)malloc( sizeof(TYPE) * (p->memory+1));
    memset(p_copy.arr,0,sizeof(TYPE) * (p->memory+1));
    int c=0;
    for (int position=1; position<=p->M_number; position++) 
    {
        c+=p->arr[position]*q;
        p_copy.arr[position+n]=c&MAXNUMBER;
        c=c>>M;
        if (p_copy.arr[position+n]!=0) p_copy.M_number=position+n;
    }
	if (c!=0) 
        p_copy.arr[++p_copy.M_number]=c;
    p_copy.arr[0]=M*(p_copy.M_number-1);
    for(int top=p_copy.arr[p_copy.M_number];top>0;top>>=1)
    {
        p_copy.arr[0]++;
    } 
        
    result=minus(num1,&p_copy);
    free(p_copy.arr);
	return result;
}

void Mod(Num *num1,Num *p)
{
    int n;
	unsigned long q;
	n=num1->M_number-p->M_number;
	num1->arr[num1->M_number+1]=0;
	for (;n>=0;n--) 
    {
        do{
            q=(num1->arr[p->M_number+n+1]*(MAXNUMBER+1)+num1->arr[p->M_number+n]) / (p->arr[p->M_number]+1);
            if (q==0) q=(num1->arr[p->M_number+n+1]*(MAXNUMBER+1)+num1->arr[p->M_number+n]) / (p->arr[p->M_number]);
        } while(subtract(num1,p,q,n)!=-1);
	}
}

void multiply(Num *num1,Num *num2,Num *p)
{
    Num num1_copy;
    num1_copy.M_number=num1->M_number;
    num1_copy.memory=p->memory;
    num1_copy.arr=(TYPE *)malloc( sizeof(TYPE) * (num1_copy.memory+1));
    memcpy(num1_copy.arr,num1->arr,sizeof(TYPE) * (num1_copy.memory+1));
    memset(num1->arr,0,sizeof(TYPE) * (num1->memory+1));
    for (int position2=1; position2<=num2->M_number; position2++)
    {
        for (int position1=1;position1<=num1_copy.M_number;position1++)
            num1->arr[position1+position2-1]+=num1_copy.arr[position1]*num2->arr[position2];
    }
    int c=0;
    num1->M_number=1;
    for (int position=1; position<=num1->memory; position++)
    {
        c+=num1->arr[position];
        num1->arr[position]=c&MAXNUMBER;
        c=c>>M;
        if (num1->arr[position]!=0) num1->M_number=position;
    }
    num1->arr[0]=M*(num1->M_number-1);
    for(int top=num1->arr[num1->M_number];top>0;top>>=1) 
        num1->arr[0]++;
    free(num1_copy.arr);
    Mod(num1,p);
}

void power(Num *num1,Num *num2,Num *p)
{
    int mask;
    int index,position=num2->M_number;
    Num num1_initial;
    num1_initial.M_number=num1->M_number;
    num1_initial.memory=num1->memory;
    num1_initial.arr=(TYPE *)malloc( sizeof(TYPE) * (num1_initial.memory+1));
    memcpy(num1_initial.arr,num1->arr,sizeof(TYPE) * (num1_initial.memory+1));

    Num num1_copy;
    num1_copy.arr=(TYPE *)malloc( sizeof(TYPE) * (num1->memory+1));
    if(num2->arr[position]==1) 
    {
        position--;
        mask=1<<(M-1);
    }
    else if(num2->arr[0]%M==0) 
        mask=1<<(M-2);
    else mask=1<<((num2->arr[0]%M)-2);
    for(index=num2->arr[position];index!=0;)
    {
        num1_copy.M_number=num1->M_number;
        num1_copy.memory=num1->memory;
        memcpy(num1_copy.arr,num1->arr,sizeof(TYPE) * (num1_copy.memory+1));
        multiply(num1,&num1_copy,p);

        if((index&mask)) 
            multiply(num1,&num1_initial,p);
        mask>>=1;
        if(mask==0) index=0;
        if(mask==0&&position>1)
        {
            index=num2->arr[--position];
            mask=1<<(M-1);
        }
  
    }

    free(num1_initial.arr);
    free(num1_copy.arr);
}

_Bool coresidual(Num *num1,Num *num2)
{
    if (num1->M_number==1 && num1->arr[1]==1) return true;
    if (num1->arr[0]!=num2->arr[0]) return false;
    if (num1->arr[1]!=num2->arr[1]-1) return false;
    for (int position=2; position<=num1->M_number;position++)
        if (num1->arr[position]!=num2->arr[position]) return false;
    return true;
}

void print(Num *num1)
{
	int j,k=0;
    int b=0,flag=0;
    int position=num1->M_number;
	unsigned char bits[M+1];
	for (int i=1; i<=num1->M_number; i++,position--)
    {
        b=num1->arr[position];
		for (j=1; j<=M; j++)
        {
            bits[j]=(unsigned char)(b&1);
            b>>=1;
		}
		for (j--; j>=1; j--)
        {
            if(!(flag==0&&bits[j]=='\0'))
            {
                flag=1;
                printf("%d",bits[j]);
                k++;
            }
		}
	}
}


#endif
