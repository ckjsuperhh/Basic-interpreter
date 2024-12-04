/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    //puts("A");
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

const std::string keywords[] = {"REM", "LET", "PRINT", "INPUT", "END", "GOTO", "IF", "RUN", "LIST", "CLEAR", "QUIT", "HELP"};
bool ValidVariableName(const std::string &str) {
    for (const auto &s : keywords)
        if (str == s)
            return false;
    return true;
}
/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    bool ok = 0;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string next = scanner.nextToken();
    if (next[0] <= '9' && next[0] >= '0') {
        program.addSourceLine(stoi(next), line);
        ok = 1;
    }
    if (!ok) {
        scanner.saveToken(next);
        next = scanner.nextToken();
        if (next == "LIST") {
            int linenum = program.getFirstLineNumber();
            if (linenum == -1) {
                ok=true;
            }
            if(!ok) {
                if (program.Is_empty_line(linenum)) {
                    std::cout << program.getSourceLine(linenum) << '\n';
                }
                while ((linenum = program.getNextLineNumber(linenum)) != -1) {
                    if (program.Is_empty_line(linenum)) {
                        std::cout << program.getSourceLine(linenum) << '\n';
                    }
                }
            }
        } else if (next == "RUN") {
            int linenumb = program.getFirstLineNumber();
            if (linenumb == -1) {
                goto end;
            }
back:
            if (program.Is_empty_line(linenumb)) {
                if (scanner.deleteNumber(program.getSourceLine(linenumb)) == "END") {
                    goto end;
                }
                if (scanner.deleteNumber(program.getSourceLine(linenumb)).substr(0, 4) == "GOTO") {
                    if ((scanner.deleteNumber(program.getSourceLine(linenumb)).substr(5) != "")) {
                        if (program.getSourceLine(stoi(scanner.deleteNumber(program.getSourceLine(linenumb)).substr(5))) != "") {
                            linenumb = stoi(scanner.deleteNumber(program.getSourceLine(linenumb)).substr(5));
                            goto back;
                        }
                        throw ErrorException("LINE NUMBER ERROR");
                    }
                }
                if (scanner.deleteNumber(program.getSourceLine(linenumb)).substr(0, 2) == "IF") {
                    TokenScanner scanner1(scanner.deleteNumber(program.getSourceLine(linenumb)));
                    scanner1.ignoreWhitespace();
                    scanner1.scanNumbers();
                    scanner1.nextToken();
                    std::unique_ptr<Expression> e1(readE(scanner1));
                    std::string op = scanner1.nextToken();
                    std::unique_ptr<Expression> e2(readE(scanner1));
                    scanner1.nextToken();
                    int num_i = stoi(scanner1.nextToken());
                    try {
                        const int val1 = e1->eval(state);
                        const int val2 = e2->eval(state);
                        if ((op == "=" && val1 == val2) || (op == ">" && val1 > val2) || (op == "<" && val1 < val2)) {
                            if (program.getSourceLine(num_i) != "") {
                                linenumb = num_i;
                                goto back;
                            }
                            throw ErrorException("LINE NUMBER ERROR");
                        }
                    } catch (...) {
                        throw;
                    }
                }
                processLine(scanner.deleteNumber(program.getSourceLine(linenumb)), program, state);
            }
            while ((linenumb = program.getNextLineNumber(linenumb)) != -1) {
                if (program.Is_empty_line(linenumb)) {
                    if (scanner.deleteNumber(program.getSourceLine(linenumb)) == "END") {
                        goto end;
                    }
                    if (scanner.deleteNumber(program.getSourceLine(linenumb)).substr(0, 4) == "GOTO") {
                        //std::cerr<<"scanner.deleteNumber(program.getSourceLine(linenumb)).substr(4)"<<scanner.deleteNumber(program.getSourceLine(linenumb)).substr(4)<<std::endl;
                        if (scanner.deleteNumber(program.getSourceLine(linenumb)).substr(5) != "") {
                            if (program.getSourceLine(stoi(scanner.deleteNumber(program.getSourceLine(linenumb)).substr(5))) != "") {
                                linenumb = stoi(scanner.deleteNumber(program.getSourceLine(linenumb)).substr(5));
                                goto back;
                            }
                            throw ErrorException("LINE NUMBER ERROR");

                        }
                    }
                    if (scanner.deleteNumber(program.getSourceLine(linenumb)).substr(0, 2) == "IF") {
                        TokenScanner scanner1(scanner.deleteNumber(program.getSourceLine(linenumb)));
                        scanner1.ignoreWhitespace();
                        scanner1.scanNumbers();
                        scanner1.nextToken();
                        std::unique_ptr<Expression> e1(readE(scanner1));
                        std::string op = scanner1.nextToken();
                        std::unique_ptr<Expression> e2(readE(scanner1));
                        scanner1.nextToken();
                        int num_i = stoi(scanner1.nextToken());
                        try {
                            const int val1 = e1->eval(state);
                            const int val2 = e2->eval(state);
                            if ((op == "=" && val1 == val2) || (op == ">" && val1 > val2) || (op == "<" && val1 < val2)) {
                                if (program.getSourceLine(num_i) != "") {
                                    linenumb = num_i;
                                    goto back;
                                }
                                throw ErrorException("LINE NUMBER ERROR");
                            }
                        } catch (...) {
                            throw;
                        }
                    }
                    processLine(scanner.deleteNumber(program.getSourceLine(linenumb)), program, state);
                }
            }
end:
        } else if (next == "QUIT") {
            exit(0);
        } else if (next == "CLEAR") {
            state.Clear();
            program.clear();
        } else if (next == "LET") {
            const std::string var_name = scanner.nextToken();
            if (!ValidVariableName(var_name))
                throw ErrorException("SYNTAX ERROR");
            if (scanner.nextToken() != "=")
                throw ErrorException("SYNTAX ERROR");
            std::unique_ptr<Expression> exp(parseExp(scanner));
            try {
                state.setValue(var_name, exp->eval(state));
            } catch (...) {
                throw;
            }
        } else if (next == "PRINT") {
            std::unique_ptr<Expression> exp(parseExp(scanner));
            try {
                const int val = exp->eval(state);
                std::cout << val << std::endl;
            } catch (...) {
                throw;
            }
        } else if (next == "END") {
            scanner.saveToken("END");
            parseExp(scanner);
        } else if (next == "INPUT") {
            const std::string var_name = scanner.nextToken();
INPUT_LOOP:
            std::cout << " ? ";
            std::string input;
            do {
                getline(std::cin, input);
            }while (input.empty());
            try {
                state.setValue(var_name, stringToInteger(input));
            } catch (...) {
                std::cout << "INVALID NUMBER" << std::endl;
                goto INPUT_LOOP;
            }
        }
    }
}


