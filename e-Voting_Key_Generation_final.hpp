//
//  e-Voting_Key_Generation_final.hpp
//  e-Voting_using_blockchain-Project
//
//  Created by Jay Bansal on 16/06/18.
//  Copyright © 2018 Jay Bansal. All rights reserved.
//

#ifndef e_Voting_Key_Generation_final_hpp
#define e_Voting_Key_Generation_final_hpp

#define FOR(i, a, b) for (int i = (a); i < (b); i++)
#include <bits/stdc++.h>
using namespace std;
int primes[100],indexPrime=-1;
///////////////////////——code——///////////////////////

struct keyCombination
{
    long int n,e,d;
}keyCombo[4000];

int gcd(int a, int h)
{
    int temp;
    while (1)
    {
        temp = a%h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}

void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n )
{
    srand ( (int)time(NULL) );
    for (int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

void SieveOfEratosthenes(int n)
{
    bool prime[n+1];
    memset(prime, true, sizeof(prime));
    for (int p=2; p*p<=n; p++)
    {
        if (prime[p] == true)
        {
            for (int i=p*2; i<=n; i += p)
                prime[i] = false;
        }
    }
    for (int p=3; p<=n; p++)
        if (prime[p])
            primes[++indexPrime] = p;
    indexPrime = -1;
}

int prime(long int pr)
{
    if(pr==0||pr==1)
        return 0;
    int i;
    int j = sqrt(pr);
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

long int cd(long int x, long int t)
{
    long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

void ce(long int t, int index, int p, int q)
{
    int k=0,j=0;
    long int e[100],d[100];
    for(long int i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;
        long int flag = prime(i);
        j=sqrt(i);
        if (flag == 1 && i != p && i != q)
        {
            e[k] = i;
            flag = cd(e[k],t);
            if (flag > 0)
            {
                d[k] = flag;
                k++;
            }
            if (k == 99)
                break;
        }
    }
    
    int val = rand()%(j-1);
    keyCombo[index].d = e[val];
    keyCombo[index].e = d[val];
}

void generateKeys()
{
    int ind1=0,ind2=1;
    FOR(i,0,4000)
    {
        int p = primes[ind1];
        int q = primes[ind2++];
        keyCombo[i].n = p*q;
        long int phi = (p-1)*(q-1);
        ce(phi,i,p,q);
        if(keyCombo[i].e==0 || keyCombo[i].d==0 || keyCombo[i].e==keyCombo[i].d)
            i--;
        if(ind2>99)
        {
            ind1++;
            ind2 = ind1+1;
        }
    }
}

void generateKeyPairFile()
{
    SieveOfEratosthenes(550);
    randomize (primes, 100);
    generateKeys();
    ofstream output_file("/Users/jaybansal/Documents/X Code/e-Voting_using_blockchain-Project/e-Voting_using_blockchain-Project/RSA_Keys", ios::binary);
    output_file.write((char*)&keyCombo, sizeof(keyCombo));
    output_file.close();
}

#endif /* e_Voting_Key_Generation_final_hpp */
