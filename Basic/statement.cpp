/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str) {
    int start =0;
    int symbol=1;
    int finish = str.length();
    if(str[0]=='+'||str[0]=='-') {
        start=1;
        finish--;
        symbol=(str[0]=='-')?-1:1;
    }
    int result=0;
    for(int i=start;i<=finish;i++) {
        result=result*10+str[i];
    }
    return result* symbol;
};

Statement::Statement() = default;

Statement::~Statement() = default;

//todo
