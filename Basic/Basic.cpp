/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: ame_kiri]
 * Section: [TODO: meow!]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    while (true) {
        try {
            processLine(getLine(), program, state);
        } catch (ErrorException & ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    Statement* stmt;
    std::string Tok;
    if (scanner.hasMoreTokens()) Tok=scanner.nextToken();
    else return;
    TokenType tokType=scanner.getTokenType(Tok);
    if (tokType == NUMBER) {
        int lineNum=stringToInteger(Tok);
        try {
            if (scanner.hasMoreTokens())
                program.addSourceLine(lineNum, line);
            else program.removeSourceLine(lineNum);
        } catch (ErrorException Er) {
            cout<<"SYNTAX ERROR"<<endl;
        }
    }
    else if (scanner.hasMoreTokens()) {
        if(Tok=="LET" || Tok=="INPUT" || Tok=="PRINT"){
            stmt = parseState(line);
            stmt->execute(state);
            delete stmt;
        }
        else error("SYNTAX ERROR\n");
    }
    else {
        if(Tok == "RUN")program.read(state);
        else if(Tok == "CLEAR"){program.clear();state.clear();}
        else if(Tok == "HELP")cout << "Zhen%Bu&Chuo!" << endl;
        else if(Tok == "QUIT")exit(0);
        else if(Tok == "LIST"){
            int num = program.getFirstLineNumber();
            while (num != 2e9) {
                cout << program.getSourceLine(num) << '\n';
                num = program.getNextLineNumber(num);
            }
        }
        else cout << "SYNTAX ERROR" << endl;
    }
}
