/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include <cstdio>
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"

using namespace std;

Program::Program() {
    mapProgram.clear();
}

Program::~Program() {
    clear();
}

void Program::clear() {
    auto it = mapProgram.begin();
    while (it != mapProgram.end()) {
        delete it->second.stmt;
        ++it;
    }
    mapProgram.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    if (mapProgram.count(lineNumber)) {
        Statement *temp;
        try{
            temp = parseState(line);
        } catch(ErrorException error){
            if(temp != nullptr)delete temp;
            throw error;
        }
        delete mapProgram[lineNumber].stmt;
        mapProgram[lineNumber].line=line;
        setParsedStatement(lineNumber, temp);
    } else {
        try {
            if (lineNumber <= 0) error("LINE NUMBER ERROR");
            mapProgram[lineNumber].line = line;
            setParsedStatement(lineNumber, parseState(line));
        } catch(ErrorException error){
            cout << error.getMessage() << endl;
            delete mapProgram[lineNumber].stmt;
            mapProgram.erase(lineNumber);
        }
    }
}

void Program::removeSourceLine(int lineNumber) {
    auto it = mapProgram.find(lineNumber);
    if (it != mapProgram.end()) {
        delete it->second.stmt;
        mapProgram.erase(lineNumber);
    }
}

string Program::getSourceLine(int lineNumber) {
    auto it = mapProgram.find(lineNumber);
    return it->second.line;
}

void Program::setParsedStatement(int lineNumber, Statement *sta) {
    mapProgram[lineNumber].stmt = sta;
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = mapProgram.find(lineNumber);
    return it->second.stmt;
}

int Program::getFirstLineNumber() {
    if (mapProgram.empty()) return 2e9;
    return mapProgram.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = mapProgram.find(lineNumber);
    it++;
    if (it == mapProgram.end()) return 2e9;
    return it->first;
}

void Program::read(EvalState &state) {
    int num = getFirstLineNumber();
    while (num != 2e9) {
        try {
            getParsedStatement(num)->execute(state);
        } catch (LineState Class) {
            if (Class.type == 1) {
                if (mapProgram.count(Class.lineNum)) {
                    num = Class.lineNum;
                    continue;
                } else cout << "LINE NUMBER ERROR" << endl;
            }
            if (Class.type == 0) { break; }
        }
        catch (ErrorException err) {
            cout << err.getMessage() << endl;
        }
        catch (...) {}
        num = getNextLineNumber(num);
    }
}
