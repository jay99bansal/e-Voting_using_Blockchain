//
//  e-Voting_Registration_Process.hpp
//  e-Voting_using_blockchain-Project
//
//  Created by Jay Bansal on 18/06/18.
//  Copyright © 2018 Jay Bansal. All rights reserved.
//

#ifndef e_Voting_Registration_Process_hpp
#define e_Voting_Registration_Process_hpp


#include <bits/stdc++.h>
#include <sqlite3.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); i++)

struct voters
{
    string regID;
    long int d,n;
    string aadhaarNo,voterID;
}voter[4000];

int regNoVoters;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int callback2(void *data, int argc, char **argv, char **azColName){
    int i;
    
    for(i = 0; i<argc; i++)
    {
        regNoVoters = atoi(argv[i]);
    }
    return 0;
}

void continueRegistrationProcess()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("/Users/jaybansal/Documents/X Code/Blockchain_e-Voting_Project/Blockchain_e-Voting_Project/voterData.db", &db);
    if( rc ){
        fprintf(stderr, "Can’t open database: %s\n", sqlite3_errmsg(db));
        return;
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    
    
    ifstream input_file("/Users/jaybansal/Documents/X Code/Blockchain_e-Voting_Project/Blockchain_e-Voting_Project/RSA_Keys", ios::binary);
    input_file.read((char*)&keyCombo, sizeof(keyCombo));
    input_file.close();
    
    
    string query = "SELECT COUNT(regID) FROM DATA;";
    char *sql2 = new char[query.length() + 1];
    strcpy(sql2, query.c_str());
    rc = sqlite3_exec(db, sql2, callback2, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        cout<<"Successfully retrieved Count = "<<regNoVoters<<endl;
    }
    
    int t=1;
    while(t)
    {
        cout<<endl<<"Enter your AADHAAR Number :- ";
        cin>>voter[regNoVoters].aadhaarNo;
        cout<<"Enter your voter ID :- ";
        cin>>voter[regNoVoters].voterID;
        string zipcode = "305801";
        voter[regNoVoters].regID = zipcode+to_string(1000+regNoVoters);
        voter[regNoVoters].d = keyCombo[regNoVoters].d;
        voter[regNoVoters].n = keyCombo[regNoVoters].n;
        string temp = "INSERT INTO DATA VALUES('";
        temp+=voter[regNoVoters].regID+"','"+voter[regNoVoters].aadhaarNo+"','"+voter[regNoVoters].voterID+"',";
        temp+=to_string(keyCombo[regNoVoters].d)+","+to_string(keyCombo[regNoVoters].n)+",0);";
        char *sql = new char[temp.length() + 1];
        strcpy(sql, temp.c_str());
        
        
        
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            cout<<"Please make a note of these details! This is for one time use on election day!"<<endl;
            cout<<"Your Registration ID : "<<voter[regNoVoters].regID<<endl;
            cout<<"Your Private Key : "<<keyCombo[regNoVoters++].e<<endl;
        }
        cout<<endl<<"Enter 1 to continue, 0 to exit : ";
        cin>>t;
    }
    
    sqlite3_close(db);
}

#endif /* e_Voting_Registration_Process_hpp */
