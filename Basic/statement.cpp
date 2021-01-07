/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

Statement *parseState(string line) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string first_Tok, Tok;
    Statement *stmt = nullptr;
    Expression *exp = nullptr, *e1, *e2;
    first_Tok = scanner.nextToken();
    //LET
    if (first_Tok == "LET") {
        try {
            stmt = new Let(scanner);
            return stmt;
        }
        catch (ErrorException err) {
            if (stmt != nullptr) delete stmt;
            throw err;
        } catch (...) {}
    } else if (first_Tok == "INPUT") {
        //INPUT
        try {
            exp = parseExp(scanner);
            if (exp->getType() != IDENTIFIER) error("SYNTAX ERROR");
            return new Input(exp);
        } catch (ErrorException err) {
            if (exp != nullptr) delete exp;
            throw err;
        } catch (...) {
            if (exp != nullptr) delete exp;
        }
    } else if (first_Tok == "PRINT") {
        //PRINT
        try {
            exp = parseExp(scanner);
            if (exp->getType()==COMPOUND)
                if (((CompoundExp*)exp)->getOp()=="=") error("SYNTAX ERROR");
            stmt = new Print(exp);
            return stmt;
        } catch (ErrorException err) {
            if (exp != nullptr) delete exp;
            if (stmt != nullptr) delete stmt;
            throw err;
        }
    } else if (first_Tok == "IF") {
        //IF_THEN
        if (line.find("=") == string::npos) {
            e1 = readE(scanner);
            string cmp = scanner.nextToken();
            e2 = readE(scanner);
            string tmp = scanner.nextToken();
            tmp = scanner.nextToken();
            int lineNum;
            try {
                lineNum = stringToInteger(tmp);
            } catch (...) {
                delete e1;
                delete e2;
                error("SYNTAX ERROR");
            }
            if ((cmp != "=" && cmp != "<" && cmp != ">") || scanner.hasMoreTokens()) {
                delete e1;
                delete e2;
                error("SYNTAX ERROR");
            }
            return new If(e1, cmp, e2, lineNum);
        }else {
            string Tok,s1,s2;
            while (scanner.hasMoreTokens()){
                Tok=scanner.nextToken();
                if (Tok=="=") break;
                s1+=Tok + ' ';
            }
            while (scanner.hasMoreTokens()){
                Tok=scanner.nextToken();
                if (Tok=="THEN") break;
                s2+=Tok + ' ';
            }//Read till '=' or 'THEN' appears
            if (scanner.hasMoreTokens())
                Tok=scanner.nextToken();
            else error("SYNTAX ERROR");
            scanner.setInput(s1);
            e1=parseExp(scanner);
            scanner.setInput(s2);
            e2=parseExp(scanner);
            int lineNum=stringToInteger(Tok);
            return new If(e1,"=",e2,lineNum);
        }
    } else if (first_Tok == "GOTO") {
        string s = scanner.nextToken();
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        int lineNum;
        try {
            lineNum = stringToInteger(s);
        } catch (...) {
            error("SYNTAX ERROR");
        }
        return new Goto(lineNum);
    } else if (first_Tok == "REM") {
        //REM
        return new Rem;
    } else if (first_Tok == "END") {
        //END
        return new End;
    } else error("SYNTAX ERROR");
}

Let::Let(TokenScanner &scanner) {
    exp = parseExp(scanner);
    if (exp->getType()!=COMPOUND || ((CompoundExp*)exp)->getOp()!="=") error("SYNTAX ERROR");
}

Let::~Let() {
    if(exp != nullptr)delete exp;
}

void Let::execute(EvalState &state) {
    exp->eval(state);
}

If::If(Expression *exp_1, string cmp_, Expression *exp_2, int line_num) {
    e1 = exp_1;
    e2 = exp_2;
    cmp = cmp_;
    gotoNum = line_num;
}

If::~If() {
    delete e1;
    delete e2;
}

void If::execute(EvalState &state) {
    int diff = e1->eval(state) - e2->eval(state);
    bool hitoshi = (cmp == "=" && diff == 0);
    bool ijo = (cmp == ">" && diff > 0);
    bool miman = (cmp == "<" && diff < 0);
    if(hitoshi || ijo || miman)throw LineState(1, gotoNum);
    else throw LineState(2, 0);
}

Input::Input(Expression *e) {
    exp = e;
}

Input::~Input() {
    delete exp;
}

void Input::execute(EvalState &state) {
    int vari;
    while (true) {
        try {
            vari = stringToInteger(getLine(" ? "));
        } catch (...) {
            cout<<"INVALID NUMBER"<<endl;
            continue;
        }
        break;
    }
    state.setValue(((IdentifierExp *) exp)->getName(), vari);
}

Print::Print(Expression *e) {
    exp = e;
}

Print::~Print() {
    delete exp;
}

void Print::execute(EvalState &state) {
    cout << exp->eval(state) << endl;
}



Goto::Goto(int Num) {
    lineNumber = Num;
}

Goto::~Goto() {}

void Goto::execute(EvalState &state) {
    throw LineState(1, lineNumber);
}

Rem::Rem() {}

Rem::~Rem() {}

void Rem::execute(EvalState &state) {}

End::End() {}

End::~End() {}

void End::execute(EvalState &state) {
    throw LineState(0);
}
