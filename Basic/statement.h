/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
* excellent model for the Statement class hierarchy.`
 */

#ifndef _statement_h
#define _statement_h

#include <exception>
#include <set>
#include "evalstate.h"
#include "exp.h"
#include "parser.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/error.h"
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class LineState{
public:
    int type;
    int lineNum;
    LineState(int t,int num=0):
        type(t),lineNum(num){}
};
Statement *interpret(string line);

class Rem : public Statement {
public:
    Rem();

    ~Rem();

    virtual void execute(EvalState &state);

private:
};

class Let : public Statement {
public:
    Let(TokenScanner &scanner);

    ~Let();

    virtual void execute(EvalState &state);

private:
    Expression *exp;
};

class If: public Statement {
public:
    If(Expression* exp_1,string cmp_,Expression* exp_2,int line_num);

    ~If();

    virtual void execute(EvalState &state);

private:
    Expression* e1,*e2;
    string cmp;
    int gotoNum;
};

class Input : public Statement {
public:
    Input(Expression* e);

    ~Input();

    virtual void execute(EvalState &state);

private:
    Expression* exp;
};

class Print : public Statement {
public:
    Print(Expression* e);

    ~Print();

    virtual void execute(EvalState &state);

private:
    Expression* exp;
};


class End : public Statement {
public:
    End();

    ~End();

    virtual void execute(EvalState &state);
private:
};

class Goto : public Statement {
public:
    Goto(int Num);

    ~Goto();

    virtual void execute(EvalState &state);

private:
    int lineNumber;
};

#endif
