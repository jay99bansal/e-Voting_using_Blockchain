//
//  e-Voting_Election_Day_Process.hpp
//  e-Voting_using_blockchain-Project
//
//  Created by Jay Bansal on 25/06/18.
//  Copyright © 2018 Jay Bansal. All rights reserved.
//

#ifndef e_Voting_Election_Day_Process_hpp
#define e_Voting_Election_Day_Process_hpp

#include <bits/stdc++.h>
#include <sqlite3.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); i++)
#define noTxnBlock 4

sqlite3 *db;
char *zErrMsg = 0;
int rc;

struct transaction
{
    long long int tID;
    string regID;
    long int d,n;
    long int signature[4];
}tx[4000];

struct block
{
    struct transaction* txn[noTxnBlock];
    long long int bID;
    int nonce;
    int difficulty;
    string prevHash,Hash,nextHash;
}

int noTx = 1, noBk = 1;
int noOfCandidates,choice;
string cHash,cName;
int errorFlag;
long int d,n,e;
int votedFlag;
long int encryptedString[4];


void createTxn(string regId)
{
    tx[noTx-1].tID=noTx;
    tx[noTx-1].regID=regId;
    tx[noTx-1].d=d;
    tx[noTx-1].n=n;
    FOR(i,0,4) tx[noTx-1].signature[i]=encryptedString[i];
}


void errorHandler(int x)
{
    errorFlag = x;
    switch(x)
    {
        case 2:
            cout<<endl<<"This registration ID has already voted!"<<endl;
            break;
        case 3:
            cout<<endl<<"Invalid registration ID!"<<endl;
            break;
        case 4:
            cout<<endl<<"Invalid Reg. Number/Private Key combination!"<<endl;
            break;
    }
}

void voteSign()
{
    cout<<endl<<"Enter your private key :- ";
    cin>>e;
    FOR(j,0,4)
    {
        long long int msg = cHash[j]-'0'+1;
        long long int c = 1;
        FOR(k,0,e)
        {
            c = c * msg;
            c = c % n;
        }
        encryptedString[j]=c;
    }
}

void checkTempSign(string regId)
{
    string message2;
    long int decryptedString[4];
    FOR(j,0,4)
    {
        long long int m = 1;
        long long int c = encryptedString[j];
        FOR(j,0,d)
        {
            m = m * c;
            m = m % n;
        }
        decryptedString[j]=(m+'0'-1);
        message2+=decryptedString[j];
    }
    if(message2==cHash)
        cout<<endl<<"Voted \""<<cName<<"\" through Reg. number "<<regId<<" successfully!"<<endl;
    else
        errorHandler(4);
}


static int callbck(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i<argc; i++)
    {
        printf("%d. %s\n", (++noOfCandidates), argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

static int callbck1(void *data, int argc, char **argv, char **azColName)
{
    if(!(--choice))
    {
        cHash = argv[0];
        cName = argv[1];
    }
    return 0;
}

static int callbck2(void *data, int argc, char **argv, char **azColName)
{
    d = atoi(argv[0]);
    n = atoi(argv[1]);
    votedFlag = atoi(argv[2]);
    printf("\n");
    return 0;
}

void retrieveCandidates(string zone)
{
    cout<<"These are the candidates for your zone :- "<<endl;
    string query = "SELECT name FROM candidate WHERE zone='"+zone+"';";
    char *sql = new char[query.length() + 1];
    strcpy(sql, query.c_str());
    rc = sqlite3_exec(db, sql, callbck, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        errorFlag = 1;
    } else {
        cout<<endl<<"Select your choice (1-"<<noOfCandidates<<") :- ";
        cin>>choice;
    }
    delete [] sql;
}

void retrieveHashOfSelectedCandidate(string zone)
{
    string query1 = "SELECT Hash,Name FROM candidate WHERE zone='"+zone+"';";
    char *sql1 = new char[query1.length() + 1];
    strcpy(sql1, query1.c_str());
    rc = sqlite3_exec(db, sql1, callbck1, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        errorFlag = 1;
    } else
    {
        cout<<endl<<"You are going to vote \""<<cName<<"\""<<endl;
    }
    delete [] sql1;
}

void checkRegistrationID(string regId)
{
    string query = "SELECT d,n,votedFlag FROM data WHERE regID='"+regId+"';";
    char *sql = new char[query.length() + 1];
    strcpy(sql, query.c_str());
    rc = sqlite3_exec(db, sql, callbck2, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        errorFlag = 1;
    }
    else
    {
        if(votedFlag==1)            errorHandler(2);
        else if(votedFlag==-1)      errorHandler(3);
    }
    delete [] sql;
}

void openElectionPortal()
{
    errorFlag=0;
    votedFlag=-1;
    rc = sqlite3_open("/Users/jaybansal/Documents/X Code/Blockchain_e-Voting_Project/Blockchain_e-Voting_Project/voterData.db", &db);
    if( rc )
    {
        fprintf(stderr, "Can’t open database: %s\n", sqlite3_errmsg(db));
        errorFlag = 1;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    string zone,regId;
    if(!errorFlag)
    {
        cout<<endl<<"Enter registration id :- ";
        cin>>regId;
        noOfCandidates = 0;
        zone = regId.substr(0,6);
        checkRegistrationID(regId);
    }
    if(!errorFlag)
    {
        retrieveCandidates(zone);
        retrieveHashOfSelectedCandidate(zone);
        voteSign();
        checkTempSign(regId);
    }
    if(!errorFlag)
    {
        createTxn(regId);
    }
    sqlite3_close(db);
}

#endif /* e_Voting_Election_Day_Process_hpp */
