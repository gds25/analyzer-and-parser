#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <bits/stdc++.h> 
#include "lex.h"


/*
 * Spring 2021
 */

// Lexical Analyzer test - convert file into sequence of tokens to send to the parser

#include <cctype>

using std::map;


using namespace std;

int main(int argc, char *argv[]) {
    
    int lineNumber = 0;
    int fileCount = 0;
    int tokCount = 0;
    LexItem tok;
    ifstream file;
    
    std::string fileName;
    
    bool vflag = false, iconstflag = false, rconstflag = false, sconstflag = false, idflag = false;
   
   // const char *args[] = { "-v", "-iconsts", "-rconsts", "-sconsts", "-ids" };
    
    if(argc < 2){
        cerr << "No Specified Input File Name." << endl;
        exit(1);
    }
    else {
        for  (int i = 1; i < argc; i++) {
            string arg( argv[i] );
            if( arg == "-iconsts" )
                iconstflag = true;
            else if( arg == "-rconsts" )
                rconstflag = true;
            else if( arg == "-sconsts" )
                sconstflag = true;
            else if( arg == "-ids" )
                idflag = true;
            else if( arg == "-v" )
                vflag = true;
            else if( arg[0] == '-' ) {
                cerr << "UNRECOGNIZED FLAG " << arg << endl;
                exit(1);
            }
            else if( arg[0] != '-' && fileCount ==  0) {
                fileCount++;
                fileName = arg;
                file.open(fileName);
            }
             else if( arg[0] != '-' && fileCount !=  0) {
                 cerr << "ONLY ONE FILE NAME ALLOWED" << arg << endl;
                 exit(1);
             }
        }
        if (fileCount == 0){
            cerr << "No Specified Input File Name." << endl;
            exit(1);
        }
    }
   
    if (!file) {
        cerr << "CANNOT OPEN THE FILE " << fileName << endl;
        exit(1);
    }
        
    std::vector<std::string> strings;
    std::vector<int> ints;
    std::vector<float> reals;
    std::vector<std::string> ids;
        
    while((tok = getNextToken(file, lineNumber)) != DONE){
      //  cout << "in here" << endl;
        /*if (tok != ERR && tok != DONE)*/
        
        if (tok == ERR && !vflag) {
            cerr << "Error in line " << tok.GetLinenum() + 1 << " (" << tok.GetLexeme() << ")" << endl;
            exit(0);
        }
        
        if (vflag) {
            cout << tok;
            if (tok == ERR) {
                exit(0);
            }
        }
        //handleflagsmode
        //keeprequiredinformation...
        if (sconstflag) {
            if (tok.GetToken() == SCONST) 
                if (!std::count(strings.begin(), strings.end(), tok.GetLexeme()))
                strings.push_back(tok.GetLexeme());
        }
        if (iconstflag) {
            if (tok.GetToken() == ICONST) {
                int convert = stoi(tok.GetLexeme());
                if (!std::count(ints.begin(), ints.end(), convert))
                    ints.push_back(convert);
            }
        }
        if (rconstflag) {
            if (tok.GetToken() == RCONST) {
                float convert = stof(tok.GetLexeme());
                if (!std::count(reals.begin(), reals.end(), convert))
                    reals.push_back(convert);
            }
        }
        if (idflag) {
            if (tok.GetToken() == IDENT)
                if (!std::count(ids.begin(), ids.end(), tok.GetLexeme()))
                ids.push_back(tok.GetLexeme());
        }
        tokCount++;
    }
 
    
    cout << "Lines: " << lineNumber << endl;
    if (tokCount > 0)
        cout << "Tokens: " << tokCount << endl;
    
    if (sconstflag) {
        sort(strings.begin(), strings.end());
       // strings.erase(unique(strings.begin(), strings.end()));
        cout << "STRINGS:" << endl;
        for (std::string s : strings) {
            cout << s << endl;
        }
    }
    if (iconstflag) {
        sort(ints.begin(), ints.end());

        cout << "INTEGERS:" << endl;
        for (int i : ints) {
            cout << i << endl;
        }
    }
    if (rconstflag) {
        sort(reals.begin(), reals.end());
  
        cout << "REALS:" << endl;
        for (float r : reals) {
            cout << r << endl;
        }
    }
    if (idflag) {
        sort(ids.begin(), ids.end());
        
        cout << "IDENTIFIERS:" << endl;
        cout << ids.front();
        for (int i = 1; i < ids.size(); i++) {
            cout << ", " << ids[i];
        }
        cout << endl;
    }
    
    file.close();
    return 0;
}
