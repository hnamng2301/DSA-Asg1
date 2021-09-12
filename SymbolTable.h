#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable;
class Node // khai bao Node
{
    string tabName, dataType;  // ten va kieu cua node trong table
    Node *next;
    int scopeLevel;

public:
    Node(){
        next = nullptr;
    }
    
    Node(string name, string type, int scope){
        tabName = name;
        dataType = type;
        scopeLevel = scope;
        next = nullptr;
    }
    
    Node(Node *node){
        tabName = node->tabName;
        dataType = node->dataType;
        scopeLevel = node->scopeLevel;
        next = nullptr;
    }
    friend class SymbolTable;
};

class SymbolTable
{
    Node *head;
    SymbolTable *parrentTab;
    static int tab;
public:
    SymbolTable() {
        head = nullptr;   // create Table (LinkedList)
    }
    void run(string filename);    // default function
    ifstream& goNextLine(ifstream& file, unsigned int iLine);
    void implementation(SymbolTable *symTab, ifstream &file, string filename, unsigned int &iLine, int &beginCount, int &endCount, Node **tabs, int &scope);
    bool idValid(string lineID);  // support function
    bool checkNum(string lineValue);   // support function check value is a const number
    bool checkStr(string lineValue);   // check value is a const string
    void checkLine(string line);  // pre-check task 3.5
    bool isDeclared(SymbolTable *symTab, string lineVar, Node **tabs, int scope);   // check variable are declared
    bool checkValue(string lineValue);  //check valid value
    bool isSpecial(string name);  // check special characters
    bool insert(SymbolTable *symTab, string lineID, string lineType, int &scope);   // task 3.5.1 function 
    bool assignValue(SymbolTable *symTab, string lineID, string lineValue, Node **tabs, int scope);  // task 3.5.2 function check value is same type with variable
    bool assignVar(SymbolTable *symTab, string lineID, string lineValue, Node **tabs, int scope);  // task 3.5.2 function check 2 variables are same type
    int howManyScope(string filename); //
    void enterScope(SymbolTable *currentScope, Node **tabs, int &scope);   // task 3.5.3 function
    void exitScope(SymbolTable *currentScope, Node **tabs, int &scope);    // task 3.5.3 function
    int lookUp(SymbolTable *symTab, Node **tabs, string lineVar, int &scope);  // task 3.5.4 function
    Node *List(Node **tabs, int scope);
    void print(Node **headList, int scope);   // task 3.5.5 function
    void rePrint(Node **tabs, int scope); // task 3.5.6 function
    ~SymbolTable(); 
};
#endif