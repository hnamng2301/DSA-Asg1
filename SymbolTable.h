#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
//Test change

class Node // khai bao Node
{
    string identifier, dataType, scope;
    int lineNum;
    Node *next;

public:
    Node(){
        next = nullptr;
    }
    
    Node(string name, string type){
        identifier = name;
        dataType = type;
        next = nullptr;
    }

    friend class SymbolTable;
};

class Table{
public:
    Table(): head(NULL) {}
    Table(Node *head){}
    Node *head;
};

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename); //default
    void insert(string name, string type);
    void assign();
    void scope();
    void lookUp();
    void print();
    void rPrint();
};
#endif
