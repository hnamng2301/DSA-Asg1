#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Node // khai bao Node
{
    string tabName, dataType;  // ten va kieu cua node trong table
    Node *next;

public:
    Node(){
        next = nullptr;
    }
    
    Node(string name, string type){
        tabName = name;
        dataType = type;
        next = nullptr;
    }

    friend class SymbolTable;
};

// class Table{
// public:
//     Table(): head(NULL) {}
//     Table(Node *head): head(head) {}
//     Node *head;
// };

class SymbolTable
{
    Node *head;
public:
    SymbolTable() {
        head = nullptr;   // create Table (LinkedList)
    }
    void run(string filename);    // default function
    bool idValid(string lineID);  // support function
    bool checkNum(string lineValue);   // support function check value is a const number
    bool checkStr(string lineValue);   // check value is a const string
    void checkLine(string line);  // pre-check task 3.5
    bool isDeclared(string lineVar);   // check variable are declared
    bool checkValue(string lineValue);  //check valid value
    bool isSpecial(string name);  // check special characters
    bool insert(string lineID, string lineType);   // task 3.5.1 function 
    bool assignValue(string lineID, string lineValue);  // task 3.5.2 function check value is same type with variable
    bool assignVar(string lineID, string lineValue);  // task 3.5.2 function check 2 variables are same type
    void scope();   // task 3.5.3 function
    void lookUp();  // task 3.5.4 function
    void print();   // task 3.5.5 function
    void rePrint(); // task 3.5.6 function
};
#endif