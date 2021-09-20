#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable; // gom cac bang NodeTab
class NodeTab;
class Node // khai bao Node
{
    string symName, dataType;  // ten va kieu cua node trong table
    Node* next;
    int nodeLevel;

public:
    Node() {
        next = nullptr;
    }

    Node(string name, string type, int scope) {
        symName = name;
        dataType = type;
        nodeLevel = scope;
        next = nullptr;
    }

    Node(Node* node) {
        symName = node->symName;
        dataType = node->dataType;
        nodeLevel = node->nodeLevel;
        next = nullptr;
    }
    //~Node() {    }
    friend class SymbolTable;
    friend class NodeTab;
};

class NodeTab {  // la bang gom cac Node theo tam vuc
    Node* head;
    Node* tail;
    NodeTab* nextTab;
    NodeTab* prevTab;
    //static int totalTab;
public:
    NodeTab() {
        head = nullptr;
        tail = nullptr;
        nextTab = nullptr;
        prevTab = nullptr;
        //totalTab++;
    }
    void insertInTab(string name, string type, int scope) {
        Node* newNode = new Node(name, type, scope);
        if (head == nullptr) {
            head = newNode;
            tail = head;
            return;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        return;
    }
    void insertInTab(Node* newNode) {
        this->insertInTab(newNode->symName, newNode->dataType, newNode->nodeLevel);
    }
    bool isNotExist(string lineName) {
        Node* p = head;
        while (p != nullptr) {
            if (p->symName == lineName)
                return false;
            p = p->next;
        }
        return true;
    }
    void print() {
        Node* p = head;
        while (p != nullptr) {
            cout << "(" << p->symName << "," << p->dataType << "," << p->nodeLevel << ")";
            if (p->next != nullptr)
                cout << "->";
            p = p->next;
        }
    }
    Node* reverse(Node* head){
        if (head == NULL || head->next == NULL)
            return head;
    /* reverse the rest List and put
          the first element at the end */
        Node* rest = reverse(head->next);
        head->next->next = head;
    /* tricky step -- see the diagram */
        head->next = NULL;
    /* fix the head pointer */
        return rest;
    }
    void deleteNode() {
        Node* current = head;
        Node* nextPtr = nullptr;
        while (current != nullptr) {
            nextPtr = current->next;
            delete current;
            current = nullptr;
            current = nextPtr;
        }
        current = nullptr;
        head = nullptr;
        tail = nullptr;
        //this = nullptr;
    }
    ~NodeTab() {
        this->deleteNode();
    }
    friend class SymbolTable;
    friend class Node;
};
//int NodeTab::totalTab = -1;

class SymbolTable
{
public:
    NodeTab* headTabs;
    NodeTab* tailTabs;
    //int totalTab;

    SymbolTable() {
        headTabs = new NodeTab();
        tailTabs = headTabs;
    }
    void addNewTab() {
        NodeTab* newTab = new NodeTab();
        if (headTabs == nullptr) {
            headTabs = newTab;
            tailTabs = headTabs;
            return;
        }
        else {
            tailTabs->nextTab = newTab;
            newTab->prevTab = tailTabs;
            tailTabs = newTab;
        }
        return;
    }
    void run(string filename);    // default function
    void implementation(NodeTab* current, ifstream& file, string filename, int& beginCount, int& endCount, int& scope);
    void tokenize(string vec[], string line);
    bool idValid(string lineID);  // support function
    bool checkNum(string lineValue);   // support function check value is a const number
    bool checkStr(string lineValue);   // check value is a const string
    void checkLine(string line);  // pre-check task 3.5
    bool isDeclared(NodeTab* currentScope, string lineVar, int scope);   // check variable are declared
    bool checkValue(string lineValue);  //check valid value
    bool insert(NodeTab* currentScope, string lineID, string lineType, int scope);   // task 3.5.1 function 
    bool assignValue(NodeTab* currentScope, string lineID, string lineValue, int scope);  // task 3.5.2 function check value is same type with variable
    bool assignVar(NodeTab* currentScope, string lineID, string lineValue, int scope);  // task 3.5.2 function check 2 variables are same type
    int lookUp(NodeTab* currentScope, string lineVar, int scope);  // task 3.5.4 function
    Node* List(int scope);
    void print(int scope);   // task 3.5.5 function
    void rePrint(int scope); // task 3.5.6 function
    void deleteSymbolTab() {
        NodeTab* current = this->headTabs;
        NodeTab* nextTab = nullptr;
        while (current != nullptr) {
            nextTab = current->nextTab;
            current->deleteNode();
            //if (current->head == NULL) cout << "Here NULL";
            delete current;
            //delete current;
            //if (current != NULL) cout << "Here NULL";
            current = nullptr;
            current = nextTab;
            //current->prevTab = nullptr;
        }
        headTabs = nullptr;
        tailTabs = nullptr;
    }
    ~SymbolTable() {
        this->deleteSymbolTab();
    }
    
};
#endif