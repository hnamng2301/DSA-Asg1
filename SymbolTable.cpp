#include "SymbolTable.h"

// Support function to check Invalid
bool SymbolTable::idValid(string lineName){
    // Kiem tra ID hop le
    if(lineName[0] >= 'a' && lineName[0] <= 'z'){ // Phai bat dau bang ky tu thuong
        for(unsigned int i = 0; i < lineName.size(); i++){
            if((lineName[i] >= 'A' && lineName[i] <= 'Z')
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
bool SymbolTable::checkNum(string lineValue){
    regex numConst("\\d+"); // Kiem tra const number (string chi gom cac chu so)
    if(lineValue[0] == '\'' && lineValue[lineValue.size() -1] == '\'')
        return false;
    if(regex_match(lineValue,numConst))
        return true;
    else
        return false;
    return true;
}
bool SymbolTable::isSpecial(string lineValue){
    for(size_t i = 0; i < lineValue.size(); i++){
        if(lineValue[i] != ' ' && !isalpha(lineValue[i]) && !isdigit(lineValue[i]))
            return true;
    }
    return false;
}
bool SymbolTable::checkStr(string lineValue){
    regex strConst("^'([a-zA-Z0-9\\s]+)'$"); // Kiem tra const string
    
    if(lineValue[0] == '\'' && lineValue[lineValue.size() -1] == '\''){   // Kiem tra value co la string khong
        if (regex_match(lineValue, strConst))
            return true;
        else
            return false;
    }
    else
        return false;
    return true;
}
bool SymbolTable::isDeclared(SymbolTable *symTab, string lineVar, Node **tabs, int scope){
    for(int i = 0; i <= scope; i++){
        Node *p = tabs[i];
        while (p != nullptr)
        {
            if (p->tabName == lineVar)
                return true;
            p = p->next;
        }
    }
    return false;
}
void SymbolTable::checkLine(string line)
{
    // regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
    // regex valid("[a-zA-Z0-9]");
    // sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
    // sregex_token_iterator end;
    //

    regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
    regex_token_iterator<string::iterator> iter(line.begin(), line.end(), reg, -1);
    regex_token_iterator<string::iterator> end;
    string vec[3]{""};
    int i = 0;
    while (iter != end)
    {
        vec[i] = *iter++;
        i++;
    }
    // Kiem tra lenh hop le
    if(line == "") throw InvalidInstruction(line);
    if (vec[0] != "INSERT" && vec[0] != "ASSIGN" && vec[0] != "BEGIN" && 
        vec[0] != "END" && vec[0] != "LOOKUP" && vec[0] != "PRINT" && vec[0] != "RPRINT")
        throw InvalidInstruction(line);

    // Kiem tra space o dau dong va cuoi dong
    if (line[0] == ' ' || line[line.size() - 1] == ' '){
        throw InvalidInstruction(line); 
    }
    
    // Kiem tra cac khoang trang o giua lenh
    // int spaceNum = 0;
    // for(unsigned int i = 0; i < line.size(); i++){
    //     if(line[i] == ' ')
    //         spaceNum++;
    // }
    if(vec[0] == "INSERT" || vec[0] == "ASSIGN"){
        for(int i = 0; i < 3; i++){
            if(vec[i] == "")
                throw InvalidInstruction(line);
        } 
        string test = vec[0];
        test  += ' ' + vec[1] + ' ' + vec[2];
        // if(vec[0] == "ASSIGN"){
        //     if (!checkStr(vec[2]) && !checkNum(vec[2]))
        //         throw InvalidInstruction(line);
        // }
        // else if(vec[0] == "INSERT"){
        //     if(!idValid(vec[1]) || !idValid(vec[2]))
        //         throw InvalidInstruction(line);
        // }
        if(line != test)
            throw InvalidInstruction(line);
    }
    else if(vec[0] == "BEGIN" || vec[0] == "END" || vec[0] == "PRINT" || vec[0] == "RPRINT"){
        if(line != vec[0])
            throw InvalidInstruction(line);
    }
    else if(vec[0] == "LOOKUP"){
        string test = vec[0];
        test  += ' ' + vec[1];
        if(line != test)
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
bool SymbolTable::assignValue(SymbolTable *symTab, string lineName, string lineValue, Node **tabs, int scope){
    Node *cur = symTab->head;
    
    if(isDeclared(symTab, lineName, tabs, scope) && (checkNum(lineValue) || checkStr(lineValue)))
    {
        while(cur != nullptr){
            if (cur->dataType == "number" && checkNum(lineValue) && cur->tabName == lineName){
                return true;
            }
            
            else if (cur->dataType == "string" && checkStr(lineValue) && cur->tabName == lineName){
                return true;
            }
            cur = cur->next;
        }
    }
    return false;
}
bool SymbolTable::assignVar(SymbolTable *symTab, string lineName, string lineValue, Node **tabs, int scope){
    Node *varID;
    Node *valID;
    
    if(isDeclared(symTab, lineValue, tabs, scope) && isDeclared(symTab, lineName, tabs, scope)){
        //cout << "OK";
        if(lineName == lineValue)
            return true;
        for(int i = 0; i <= scope; i++)
        {
            varID = tabs[i];
            valID = tabs[i];
            while (varID != nullptr){
                while (valID != nullptr)
                {
                    if (varID->tabName == lineName && varID->dataType == valID->dataType && valID->tabName == lineValue)
                    {
                        return true;
                    }
                    valID = valID->next;
                }
                varID = varID->next;
            }
        }
    }
    
    return false;
}
bool SymbolTable::insert(SymbolTable *symTab, string lineName, string lineType, int &scope){
    Node *temp = new Node(lineName, lineType, scope);
    // Check Table is NULL?
    if(symTab->head == nullptr){
        symTab->head = temp;
        return true;
    }
    // Check table if there are duplicate nodes
    Node *p = symTab->head;
    while(p->next != nullptr){
        if(lineName == p->tabName){
            //cout << "valName: " << p->tabName;
            return false;
        }
        else{
            p = p->next;
        }
    }
    if(lineName == p->tabName)
        return false;
    p->next = temp; // Add new node at tail
    return true;
}
ifstream& SymbolTable::goNextLine(ifstream& file, unsigned int iLine){
    file.seekg(ios::beg);
    for(unsigned int i = 0; i < iLine - 1; i++){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return file;
}
void SymbolTable::enterScope(SymbolTable *currentScope, Node **tabs, int &scope){
    scope++;
    tabs[scope] = nullptr;
    currentScope->head = tabs[scope];

}
void SymbolTable::exitScope(SymbolTable *currentScope, Node **tabs, int &scope){
    scope--;
    currentScope->head = tabs[scope];
}
int SymbolTable::howManyScope(string filename){
    string line;
    int countTab = 1;
    ifstream file(filename);
    while(getline(file, line)){
        regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
        regex_token_iterator<string::iterator> iter(line.begin(), line.end(), reg, -1);
        regex_token_iterator<string::iterator> end;
        string vec[3]{""};
        int i = 0;
        while (iter != end)
        {
            vec[i] = *iter++;
            i++;
        }
        //checkLine(line);
        if(vec[0] == "BEGIN")
            countTab++;
        else continue;
    }
    return countTab;
}
int SymbolTable::lookUp(SymbolTable *symTab, Node **tabs, string lineVar, int &scope){
    for(int i = scope; i >= 0; i--){
        //symTab->head = tabs[i];
        Node *findNode = tabs[i];
        while (findNode != nullptr)
        {
            if (findNode->tabName == lineVar)
            {
                return i;
            }
            findNode = findNode->next;
        }
    }
    return -1;
}
void SymbolTable::print(int scope, Node **tabs){
    Node *List = new Node();

    Node *p = tabs[scope]->next;
    while (p != nullptr)
    {
        List->next = p;
        p = p->next;
    }

    for(int i = scope - 1; i >= 0; i--){
        p = tabs[i];
        while (p != nullptr)
        {
            List->next = p;
            p = p->next;
        }
    }
    Node *printList = List;
    while(printList != nullptr){
        cout << printList->tabName << "//" << printList->scopeLevel << " ";
        printList = printList->next;
    }
    cout << endl;
    // delete List;
    // delete printList;
    // delete p;
}
void SymbolTable::rePrint(Node **tabs, int scope){

}
SymbolTable::~SymbolTable(){

}
void SymbolTable::implementation(SymbolTable *symTab, ifstream &file, string line, unsigned int &iLine, int &beginCount, int &endCount, Node **tabs, int &scope){
    enterScope(symTab, tabs, scope);
    while (symTab->goNextLine(file, iLine))
    {
        if (getline(file, line))
        {
            //cout << line << " ";
            regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
            regex_token_iterator<string::iterator> iter(line.begin(), line.end(), reg, -1);
            regex_token_iterator<string::iterator> end;
            string vec[3]{""};
            int i = 0;
            while (iter != end)
            {
                vec[i] = *iter++;
                i++;
            }
            checkLine(line);
            if (vec[0] == "INSERT")
            {
                //tabs[beginCount] = symTab->head;
                if (vec[2] != "number" && vec[2] != "string" || !idValid(vec[1]))
                    throw InvalidInstruction(line);
                // if (isSpecial(vec[1]) || isSpecial(vec[2]))
                //     throw InvalidInstruction(line);
                if (!(insert(symTab, vec[1], vec[2], scope)))
                {
                    throw Redeclared(line);
                }
                else
                    cout << "success" << endl;
            }
            else if (vec[0] == "ASSIGN")
            {
                cout << idValid(vec[1]) << " " << idValid(vec[2]);
                if (!idValid(vec[1]))
                    throw InvalidInstruction(line);

                if ((idValid(vec[1]) && !isDeclared(symTab, vec[1], tabs, scope))
                || (idValid(vec[2]) && !isDeclared(symTab, vec[2], tabs, scope))) // if name and value are variables, valid and are undeclared
                { // if vec1 is undeclared
                    throw Undeclared(line);
                }
                else if (!checkNum(vec[2]) && !checkStr(vec[2]) && !idValid(vec[2]))  // if value is not a num, a string and a valid ID
                    throw InvalidInstruction(line);
                else
                {
                    if (checkStr(vec[2]) || checkNum(vec[2]))
                    { // if vec2 is a string or a value
                        if (assignValue(symTab, vec[1], vec[2], tabs, scope))
                            cout << "success" << endl;
                        if (!assignValue(symTab, vec[1], vec[2], tabs, scope))
                            throw TypeMismatch(line);
                    }
                    else
                    {
                        if (isDeclared(symTab, vec[1], tabs, scope) && isDeclared(symTab, vec[2], tabs, scope)) // If vec1 and vec2 are variables
                        {
                            if (assignVar(symTab, vec[1], vec[2], tabs, scope)) // If both are same type
                                cout << "success" << endl;
                            else
                                throw TypeMismatch(line);
                        }
                        else
                            throw Undeclared(line);
                    }
                }
            }
            else if (vec[0] == "BEGIN")
            {
                beginCount++;
                iLine++;
                goNextLine(file, iLine);
                //enterScope(symTab, tabs, scope);
                //cout << endl <<  iLine << " ";
                implementation(symTab, file, line, iLine, beginCount, endCount, tabs, scope);
            }
            else if (vec[0] == "END")
            {
                //tabs[beginCount] = symTab->head;
                endCount++; //iLine++;
                if (beginCount == 0)
                    throw UnknownBlock();
                exitScope(symTab, tabs, scope);
                goNextLine(file, iLine);
                getline(file,line);
                //cout << endl;
                //implementation(file, line, iLine, beginCount, endCount);
                //delete symTab;
            }
            else if (vec[0] == "LOOKUP")
            {
                if(lookUp(symTab, tabs, vec[1], scope) != -1){
                    cout << lookUp(symTab, tabs, vec[1], scope) << endl;
                }
                else
                    throw Undeclared(line);
            }
            else if (vec[0] == "PRINT")
            {
                print(scope, tabs);
            }
            else if (vec[0] == "RPRINT")
            {
            }
            else
                throw InvalidInstruction(line);
            //cout << "scope: " << scope << endl;
            tabs[scope] = symTab->head;
            symTab->head = tabs[scope];
            iLine++;
        }
        else {
            if (beginCount < endCount)
                throw UnknownBlock();
            else if (beginCount > endCount)
                throw UnclosedBlock(endCount);
            else 
                return;
        }
    }

    return;
}
void SymbolTable::run(string filename) {
    ifstream file(filename);
    string line = "";
    int beginCount = 0, endCount = 0;
    unsigned int iLine = 1;
    int scope = -1;
    Node **tabs = new Node*[howManyScope(filename)];
    if(file.is_open()){
        implementation(this, file, line, iLine, beginCount, endCount, tabs, scope);
        //cout << "Tabs = " << howManyScope(filename) << endl;
    }
    file.close();
    delete[] tabs;
    return;
}


int SymbolTable::tab = 1; //
