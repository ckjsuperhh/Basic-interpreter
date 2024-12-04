/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    accordance.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    if (accordance.find(lineNumber) != accordance.end()) {
        accordance[lineNumber] = line;
    } else {
        accordance.emplace(lineNumber, line);
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    if (accordance.find(lineNumber) != accordance.end()) {
        accordance.erase(lineNumber);
    }
}
bool Program::Is_empty_line(int lineNumber) {
    bool ok = false;
    for (int i = 0; i < accordance[lineNumber].size(); i++) {
        if ((accordance[lineNumber][i] >= '0' && accordance[lineNumber][i] <= '9') || accordance[lineNumber][i] == ' ') {
            continue;
        }
        ok = true;
        break;
    }
    return ok;
}
std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    if (accordance.find(lineNumber) != accordance.end()) {
        return accordance[lineNumber];
    }
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
}


Statement *Program::getParsedStatement(int lineNumber) {
    // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    if (accordance.empty()) {
        return -1;
    }
    return accordance.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    if (accordance.upper_bound(lineNumber) != accordance.end()) {
        return accordance.upper_bound(lineNumber)->first;
    }
    return -1;
}

//more func to add
//todo


