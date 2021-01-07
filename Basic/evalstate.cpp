/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"
using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
   /* Empty */
}

EvalState::~EvalState() {
   /* Empty */
}

void EvalState::setValue(string var, int value) {
   varList.put(var, value);
}

int EvalState::getValue(string var) {
   return varList.get(var);
}

bool EvalState::isDefined(string var) {
   return varList.containsKey(var);
}

void EvalState::clear(){
    varList.clear();
}

bool check_reserve(string s){
    /*switch (sentence) {
        case "":
        case "LET":
        case "RUN":
        case "INPUT":
        case "PRINT":
        case "LIST":
        case "HELP":
        case "CLEAR":
        case "REM":
        case "GOTO":
        case "END":
        case "QUIT":return 1;
        default:    return 0;
    }*/
    if(s==""||s=="LET"||s=="RUN"||s=="INPUT"||s=="PRINT"||s=="LIST"||s=="HELP"||s=="CLEAR"||s=="REM"||s=="GOTO"||s=="END"||s=="QUIT") return 1;
    return 0;
}