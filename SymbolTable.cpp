#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    ifstream file(filename);
    string tmp = "";

    if(file.is_open()){
        //getline(file, tmp);
        while(getline(file, tmp)){
            regex reg("\\s+");
            sregex_token_iterator iter(tmp.begin(), tmp.end(), reg, -1);
            sregex_token_iterator end;
            vector<string> vec(iter, end);
            if(vec[0] == "INSERT"){
                insert(vec[0],vec[1]);
            }
            else if(vec[0] == "ASSIGN"){

            }
            else if(vec[0] == "LOOKUP"){

            }
            else if(vec[0] == "PRINT"){

            }
            else if(vec[0] == "RPRINT"){

            }
        }
    }
    cout << "success";
}

void SymbolTable::insert(string name, string type){
    Node *p = new Node(name, type);

    
}