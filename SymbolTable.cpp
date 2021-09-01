#include "SymbolTable.h"

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
bool SymbolTable::isDeclared(string lineVar){
    Node *p = this->head;
    while(p != nullptr){
        if(p->tabName == lineVar)
            return true;
        p = p->next;  
    }
    return false;
}

void SymbolTable::run(string filename) {
    ifstream file(filename);
    string line = "";
    SymbolTable *symTab = new SymbolTable();
    if(file.is_open()){
        //getline(file, tmp);
        while(getline(file, line)){
            regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
            sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
            sregex_token_iterator end;
            vector<string> vec(iter, end);
            checkLine(line);
            if(vec[0] == "INSERT"){
                if(isSpecial(vec[1]) || isSpecial(vec[2]))
                    throw InvalidInstruction(line);
                if(!(insert(vec[1], vec[2]))){
                    throw Redeclared(line);
                }
                else
                    cout << "success" << endl;
            }
            else if(vec[0] == "ASSIGN"){
                if(isSpecial(vec[1]))
                    throw InvalidInstruction(line);
                if(!isDeclared(vec[1])){   // if vec1 is undeclared
                    throw Undeclared(line);
                }
                else if((isSpecial(vec[2]) && !checkStr(vec[2])))
                    throw InvalidInstruction(line);
                else{
                    if(checkStr(vec[2]) || checkNum(vec[2])){   // if vec2 is a string or a value
                        if (assignValue(vec[1], vec[2]))
                            cout << "success" << endl;
                        if (!assignValue(vec[1], vec[2]))
                            throw TypeMismatch(line);
                    }
                    else{
                        if (isDeclared(vec[1]) && isDeclared(vec[2]))    // If vec1 and vec2 are variables
                        {                                  
                            if (assignVar(vec[1], vec[2])) // If both are same type
                                cout << "success" << endl;
                            else
                                throw TypeMismatch(line);
                        }
                        else 
                            throw Undeclared(line);
                    }
                }

            }
            else if(vec[0] == "BEGIN" || vec[0] == "END"){
                SymbolTable *newTab = new SymbolTable();
                if(vec[0] == "BEGIN"){

                }
                else if(vec[0] == "END"){

                }
                else{

                }
            }
            else if(vec[0] == "LOOKUP"){

            }
            else if(vec[0] == "PRINT"){

            }
            else if(vec[0] == "RPRINT"){

            }
            else
                throw InvalidInstruction(line);
        }
    }
    delete symTab;//
    return;
}

bool SymbolTable::insert(string lineName, string lineType){
    Node *temp = new Node(lineName, lineType);
    // Check Table is NULL?
    if(this->head == nullptr){
        this->head = temp;
        return true;
    }
    // Check table if there are duplicate nodes
    Node *p = this->head;
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

bool SymbolTable::assignValue(string lineName, string lineValue){
    Node *cur = this->head;
    
    if(isDeclared(lineName) && (checkNum(lineValue) || checkStr(lineValue)))
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
bool SymbolTable::assignVar(string lineName, string lineValue){
    Node *curID;
    Node *p = this->head;
    
    if(isDeclared(lineValue) && isDeclared(lineName)){
        while (p != nullptr)
        {
            curID = this->head;
            while (curID != nullptr)
            {
                if (curID->tabName == lineValue && curID->dataType == p->dataType && p->tabName == lineValue)
                {
                    return true;
                }
                curID = curID->next;
            }
            p = p->next;
        }
    }
    
    return false;
}
void SymbolTable::print(){
    Node *p = this->head;
    while(p != nullptr){
        cout << p->tabName << "->";
        p = p->next;
    }
}

void SymbolTable::checkLine(string line)
{
    regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
    regex valid("[a-zA-Z0-9]");
    sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
    sregex_token_iterator end;
    vector<string> vec(iter, end);

    // Kiem tra lenh hop le
    if (vec[0] != "INSERT" && vec[0] != "ASSIGN" && vec[0] != "BEGIN" && 
        vec[0] != "END" && vec[0] != "LOOKUP" && vec[0] != "PRINT" && vec[0] != "RPRINT")
        throw InvalidInstruction(line);

    // Kiem tra space o dau dong va cuoi dong
    if (line[0] == ' ' || line[line.size() - 1] == ' ')
        throw InvalidInstruction(line); 
    
    // Kiem tra cac khoang trang o giua lenh
    // int spaceNum = 0;
    // for(unsigned int i = 0; i < line.size(); i++){
    //     if(line[i] == ' ')
    //         spaceNum++;
    // }
    if(vec[0] == "INSERT" || vec[0] == "ASSIGN"){
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