#include "SymbolTable.h"

// Support function to check Invalid
bool SymbolTable::idValid(string lineName) {
    // Kiem tra ID hop le
    if (lineName[0] >= 'a' && lineName[0] <= 'z') { // Phai bat dau bang ky tu thuong
        for (unsigned int i = 0; i < lineName.size(); i++) {
            if ((lineName[i] >= 'A' && lineName[i] <= 'Z')
                || (lineName[i] >= 'a' && lineName[i] <= 'z')
                || (lineName[i] >= '0' && lineName[i] <= '9')
                || lineName[i] == '_')
                continue;
            else
                return false;
        }
    }
    else
        return false;
    return true;
}
bool SymbolTable::checkNum(string lineValue) {
    regex numConst("\\d+"); // Kiem tra const number (string chi gom cac chu so)
    if (lineValue[0] == '\'' && lineValue[lineValue.size() - 1] == '\'')
        return false;
    if (regex_match(lineValue, numConst))
        return true;
    else
        return false;
    return true;
}
bool SymbolTable::checkStr(string lineValue) {
    regex strConst("^'([a-zA-Z0-9\\s]+)'$"); // Kiem tra const string

    if (lineValue[0] == '\'' && lineValue[lineValue.size() - 1] == '\'') {   // Kiem tra value co la string khong
        if (regex_match(lineValue, strConst))
            return true;
        else
            return false;
    }
    else
        return false;
    return true;
}
void SymbolTable::tokenize(string vec[], string line) {
    regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
    regex_token_iterator<string::iterator> iter(line.begin(), line.end(), reg, -1);
    regex_token_iterator<string::iterator> end;
    int i = 0;
    while (iter != end)
    {
        if (i >= 3)
            throw InvalidInstruction(line);
        vec[i] = *iter++;
        i++;
    }
}
void SymbolTable::checkLine(string line)
{
    string vec[3]{ "" };
    tokenize(vec, line);
    // Kiem tra lenh hop le
    if (line == "") throw InvalidInstruction(line);
    if (vec[0] != "INSERT" && vec[0] != "ASSIGN" && vec[0] != "BEGIN" &&
        vec[0] != "END" && vec[0] != "LOOKUP" && vec[0] != "PRINT" && vec[0] != "RPRINT")
        throw InvalidInstruction(line);

    // Kiem tra space o dau dong va cuoi dong
    if (line[0] == ' ' || line[line.size() - 1] == ' ') {
        throw InvalidInstruction(line);
    }

    // Kiem tra cac khoang trang o giua lenh
    // int spaceNum = 0;
    // for(unsigned int i = 0; i < line.size(); i++){
    //     if(line[i] == ' ')
    //         spaceNum++;
    // }
    if (vec[0] == "INSERT" || vec[0] == "ASSIGN") {
        for (int i = 0; i < 3; i++) {
            if (vec[i] == "")
                throw InvalidInstruction(line);
        }
        string test = vec[0];
        test += ' ' + vec[1] + ' ' + vec[2];
        // if(vec[0] == "ASSIGN"){
        //     if (!checkStr(vec[2]) && !checkNum(vec[2]))
        //         throw InvalidInstruction(line);
        // }
        // else if(vec[0] == "INSERT"){
        //     if(!idValid(vec[1]) || !idValid(vec[2]))
        //         throw InvalidInstruction(line);
        // }
        if (line != test)
            throw InvalidInstruction(line);
    }
    else if (vec[0] == "BEGIN" || vec[0] == "END" || vec[0] == "PRINT" || vec[0] == "RPRINT") {
        if (line != vec[0])
            throw InvalidInstruction(line);
    }
    else if (vec[0] == "LOOKUP") {
        string test = vec[0];
        test += ' ' + vec[1];
        if (line != test)
            throw InvalidInstruction(line);
    }
    // Kiem tra ki tu dac biet
    // for(size_t i = 0; i < line.size(); i++){
    //     if(line[i] != ' ' && !isalpha(line[i]) && !isdigit(line[i]))
    //         throw InvalidInstruction(line);
    // }
    // if((vec.size() == 3 && spaceNum != 2) || (vec.size() == 2 && spaceNum != 1) || (vec.size() == 1 && spaceNum != 0) || vec.size() > 3)   
    //     throw InvalidInstruction(line);

    // // Kiem tra ten hop le
    // if(!idValid(vec[1]) || !idValid(vec[2]))
    //     throw InvalidInstruction(line);

}
bool SymbolTable::insert(NodeTab* currentScope, string lineName, string lineType, int scope) {
    // Check Table is NULL?
    //cout << currentScope->head->nodeLevel;
    if (currentScope->isNotExist(lineName)) {
        currentScope->insertInTab(lineName, lineType, scope);
        return true;
    }
    else {
        return false;
    }
    return true;
}

bool SymbolTable::isDeclared(NodeTab* currentScope, string lineVar, int scope) {
    NodeTab* p = currentScope;
    while (p != nullptr) {
        Node* dupNode = p->head;
        while (dupNode != nullptr) {
            if (dupNode->symName == lineVar)
                return true;
            dupNode = dupNode->next;
        }
        p = p->prevTab;
    }
    return false;
}
bool SymbolTable::assignValue(NodeTab* currentScope, string lineName, string lineValue, int scope) {
    //cout << "OK";
    NodeTab* p = currentScope;
    while (p != nullptr) {
        Node* cur = p->head;
        while (cur != nullptr)
        {
            if (cur->dataType == "number" && checkNum(lineValue) && cur->symName == lineName)
            {
                return true;
            }
            else if (!(cur->dataType != "number" || checkNum(lineValue)) && cur->symName == lineName)
            {
                return false;
            }
            else if (!(cur->dataType == "number" || !checkNum(lineValue)) && cur->symName == lineName)
            {
                return false;
            }
            else if (cur->dataType == "string" && checkStr(lineValue) && cur->symName == lineName)
            {
                return true;
            }
            else if (!(cur->dataType == "string" || !checkStr(lineValue)) && cur->symName == lineName)
            {
                return false;
            }
            else if (!(cur->dataType != "string" || checkStr(lineValue)) && cur->symName == lineName)
            {
                return false;
            }
            cur = cur->next;
        }
        p = p->prevTab;
    }

    return false;
}

bool SymbolTable::assignVar(NodeTab* currentScope, string lineName, string lineValue, int scope) {
    Node* varID = nullptr;
    Node* valID = nullptr;
    NodeTab* varTab = new NodeTab();
    NodeTab* valTab = new NodeTab();
    if (lineName == lineValue) {
        delete varTab;
        delete valTab;
        return true;
    }

    NodeTab* p = currentScope;
    while (p != nullptr) {
        varID = p->head;
        valID = p->head;
        while (varID != nullptr)
        {
            if (varID->symName == lineName)
                varTab->insertInTab(varID);
            varID = varID->next;
        }
        while (valID != nullptr)
        {
            if (valID->symName == lineValue)
                valTab->insertInTab(valID);
            valID = valID->next;
        }
        p = p->prevTab;
    }
    //valTab->print(); cout << endl; varTab->print(); cout << endl;
    if (varTab->head != nullptr && valTab->head != nullptr) {
        //cout << "OK";
        //cout << scope;
        valID = valTab->head;
        while (valID != nullptr) {
            varID = varTab->head;
            while (varID != nullptr)
            {
                if (varID->dataType != valID->dataType) {
                    delete varTab;
                    delete valTab;
                    return false;
                }
                varID = varID->next;
            }
            valID = valID->next;
        }
        delete varTab;
        delete valTab;
    }
    
    return true;
}

int SymbolTable::lookUp(NodeTab* currentScope, string lineVar, int scope) {
    NodeTab* p = currentScope;
    while (p != nullptr) {

        Node* findNode = p->head;
        while (findNode != nullptr)
        {
            if (findNode->symName == lineVar)
            {
                return findNode->nodeLevel;
            }
            findNode = findNode->next;
        }
        p = p->prevTab;
    }
    return -1;
}

Node* SymbolTable::List(int scope) {
    Node* headList = nullptr;
    NodeTab* p = this->headTabs;
    int beginCount = 0;
    while (p != nullptr) {
        if (p->head != nullptr) {
            headList = new Node(p->head);
            //scope--;      // Node khong null dau tien cua tabs la Node head cua List
            //cout << beginCount;
            break;
        }
        p = p->nextTab;
        beginCount++;
    }
    //if (headList == nullptr) cout << "List Null";
    if (headList == nullptr) return nullptr;

    NodeTab* temp = p;
    Node* pList = headList;
    Node* nodeInTab = nullptr;
    while (temp != nullptr) {
        // if (temp->head->nodeLevel < scope){
        //     temp = temp->nextTab;
        //     continue;
        // } 
        if (temp == p)
            nodeInTab = temp->head->next;
        //cout << p->head->nodeLevel;
        else
            nodeInTab = temp->head;
        while (nodeInTab != nullptr) {
            Node* newNode = new Node(nodeInTab);
            pList->next = newNode;
            pList = pList->next;
            nodeInTab = nodeInTab->next;
        }
        temp = temp->nextTab;
    }
    // cout << headList->symName << ", ";
    return headList;
}

void SymbolTable::print(int scope) {
    NodeTab* printTab = new NodeTab();
    Node* reversePrint = nullptr;
    //cout << printTab->head->dataType;

    printTab->head = List(scope);
    reversePrint = printTab->head;
    if (printTab->head == nullptr) {
        //cout << endl;
        delete printTab;
        return;
    }
    reversePrint = printTab->reverse(printTab->head);
    printTab->head = reversePrint;
    Node* ptr1, * ptr2, * dup;
    ptr1 = reversePrint;
    while (ptr1 != nullptr && ptr1->next != nullptr) {
        ptr2 = ptr1;
        while (ptr2->next != nullptr) {
            if (ptr1->symName == ptr2->next->symName) {
                dup = ptr2->next;
                ptr2->next = ptr2->next->next;
                delete dup;
            }
            else
                ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
    printTab->head = printTab->reverse(printTab->head);
    //if (printTab->head == nullptr) cout << "NULL";
    Node* printList = printTab->head;
    while (printList != nullptr) {
        cout << printList->symName << "//" << printList->nodeLevel;
        if (printList->next != nullptr) cout << " ";
        printList = printList->next;
    }
    cout << endl;
    delete printTab;
    reversePrint = NULL;
}

void SymbolTable::rePrint(int scope) {
    Node* rePrTab = nullptr;
    NodeTab* reverseRTab = new NodeTab();

    rePrTab = List(scope);

    if (rePrTab == nullptr) {
        //cout << endl;
        delete reverseRTab;
        return;
    }
    reverseRTab->head = rePrTab;
    reverseRTab->head = reverseRTab->reverse(reverseRTab->head);

    Node* ptr1, * ptr2, * dup;
    ptr1 = reverseRTab->head;
    while (ptr1 != nullptr && ptr1->next != nullptr) {
        ptr2 = ptr1;
        while (ptr2->next != nullptr) {
            if (ptr1->symName == ptr2->next->symName) {
                dup = ptr2->next;
                ptr2->next = ptr2->next->next;
                delete dup;
            }
            else
                ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
    Node* printRList = reverseRTab->head;
    while (printRList != nullptr) {
        cout << printRList->symName << "//" << printRList->nodeLevel;
        if (printRList->next != nullptr) cout << " ";
        printRList = printRList->next;
    }
    cout << endl;
    reverseRTab->deleteNode();
    delete reverseRTab;
    //delete rePrTab;
    rePrTab = nullptr;
    reverseRTab = nullptr;
}

void SymbolTable::implementation(NodeTab* currentTab, ifstream& file, string line, int& beginCount, int& endCount, int& scope)
{
    //this->addNewTab();
    
    //if (currentTab->prevTab == nullptr) cout << "Prev NULL";
    //cout << this->tailTabs->head->nodeLevel << " " << this->headTabs->head->nodeLevel;
    while (getline(file, line))
    {
        //cout << line << " ";
        //cout << "Scope = " << scope;
        //currentTab->print(); cout << endl;
        string vec[3]{ "" };
        tokenize(vec, line);
        checkLine(line);
        if (vec[0] == "INSERT")
        {
            if ((vec[2] != "number" && vec[2] != "string") || !idValid(vec[1]))
                throw InvalidInstruction(line);

            if (!(this->insert(currentTab, vec[1], vec[2], scope)))
            {
                throw Redeclared(line);
            }
            else
                cout << "success" << endl;
            //symTab->headTabs[scope]->print();
        }
        else if (vec[0] == "ASSIGN")
        {
            //currentTab->print();
            if (!idValid(vec[1]))
                throw InvalidInstruction(line);

            if ((idValid(vec[1]) && !this->isDeclared(currentTab, vec[1], scope))
                || (idValid(vec[2]) && !this->isDeclared(currentTab, vec[2], scope))) // if name and value are variables, valid and are undeclared
            {                                                                                                                   // if vec1 is undeclared
                throw Undeclared(line);
            }
            else if (!checkNum(vec[2]) && !checkStr(vec[2]) && !idValid(vec[2])) // if value is not a num, a string and a valid ID
                throw InvalidInstruction(line);
            else
            {
                if (checkStr(vec[2]) || checkNum(vec[2]))
                { // if vec2 is a string or a value
                    if (this->assignValue(currentTab, vec[1], vec[2], scope))
                        cout << "success" << endl;
                    if (!this->assignValue(currentTab, vec[1], vec[2], scope))
                    {
                        //cout << "Here 1";
                        throw TypeMismatch(line);
                    }
                }
                else
                {
                    if (this->isDeclared(currentTab, vec[1], scope)
                        && this->isDeclared(currentTab, vec[2], scope)) // If vec1 and vec2 are variables
                    {
                        if (this->assignVar(currentTab, vec[1], vec[2], scope)) // If both are same type
                            cout << "success" << endl;
                        else
                        {
                            //cout << "Here 2";
                            throw TypeMismatch(line);
                        }
                    }
                    else
                        throw Undeclared(line);
                }
            }
        }
        else if (vec[0] == "BEGIN")
        {
            beginCount++;
            scope++;
            this->addNewTab();
            //NodeTab* p = currentTab;
            currentTab = currentTab->nextTab;
            //currentTab->prevTab = p;
        }
        else if (vec[0] == "END")
        {
            //tabs[beginCount] = symTab->head;
            endCount++; //iLine++;
            beginCount--;
            scope--;
            if (beginCount < 0)
            {
                //cout << "?";
                throw UnknownBlock();
            }
            if (beginCount == 0)
                endCount = 0;
            NodeTab* pPrev = currentTab->prevTab;
            NodeTab* pNext = currentTab->nextTab;
            if (pPrev != nullptr) {
                pPrev->nextTab = pNext;
                delete currentTab;
                currentTab = pPrev;
            }
            else {
                headTabs = pNext;
                currentTab = headTabs;
            }
            if (pNext != nullptr) {
                pNext->prevTab = pPrev;
                delete currentTab;
                currentTab = pPrev;
            }
            else {
                tailTabs = pPrev;
                currentTab = tailTabs;
            }


        }
        else if (vec[0] == "LOOKUP")
        {
            //cout << "???";
            if (lookUp(currentTab, vec[1], scope) != -1)
            {
                cout << lookUp(currentTab, vec[1], scope) << endl;
            }
            else
                throw Undeclared(line);
        }
        else if (vec[0] == "PRINT")
        {
            //cout << "OK";
            this->print(scope);
            //currentTab->print();
        }
        else if (vec[0] == "RPRINT")
        {
            this->rePrint(scope);
        }
        else
            throw InvalidInstruction(line);
        //cout << "scope: " << scope << endl;
        //tabs[scope] = symTab->head;
        //symTab->head = tabs[scope];
    }
    if (beginCount == 0)
    {
        //cout << beginCount << " " << endCount << " ";
        return;
    }
    else if (beginCount > 0)
        throw UnclosedBlock(beginCount);
    if (beginCount < endCount)
    {
        //cout << beginCount << " " << endCount << " ";
        //cout << "Here 2";
        throw UnknownBlock();
    }
    else if (beginCount >= endCount)
    {
        throw UnclosedBlock(beginCount);
    }
    else
    {
        return;
    }


    return;
}

void SymbolTable::run(string filename) {
    ifstream file(filename);
    string line = "";
    int beginCount = 0, endCount = 0;
    int scope = 0;
    NodeTab* currentTab = this->headTabs;
    if (file.is_open())
    {
        this->implementation(currentTab, file, line, beginCount, endCount, scope);
    }
    //cout << this->totalTab;
    //delete currentTab;
    file.close();
    return;

}
