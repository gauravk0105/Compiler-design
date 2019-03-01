#include <bits/stdc++.h>
using namespace std;

map<string, set<string> > grammar;
map<string, set<string> >::iterator gr;
set<string> productions;
set<string>::iterator prodIterator;
string startSymbol,checkString, operators = "/*+-^";
stack<char> reductionStack, parsingStack;
bool error = 0;
void displayProduction();
void checkGrammar();
void checkInput();
int findOperatorRelation(char tos, char ips);
void pushIntoReductionStack(char a);

int main(void){
    FILE *input = freopen("lab3.txt","r",stdin);
	FILE *output = freopen("operatorGrammarOutput.txt","w",stdout);
    int inputLength, terLen;
    string str;
    cin >> startSymbol;
    cin >> inputLength;
    while(inputLength--){
        cin >> str;
		int pos=str.find("->");
		string non_terminal = str.substr(0,pos);
		string production = "";
		set<string> temp;
		for(int i=pos+2;i<str.length();i++){
            if(str.at(i)=='|'){
                temp.insert(production);
                production="";
            }
            else{
                production.push_back(str.at(i));
            }
		}
		temp.insert(production);
		grammar.insert(make_pair(non_terminal,temp));
    }
    cin >> checkString;
    cout << "Start symbol : "<<startSymbol<<endl;
    cout << "String to check : "<<checkString<<endl;
    displayProduction();
    checkGrammar();
    return 0;
}

void displayProduction(){
    cout << "\nProductions :\n";
    for (gr=grammar.begin(); gr!=grammar.end(); ++gr){
        cout << gr->first << " -> ";
        set<string> temp = gr->second;
        set<string>::iterator it1 = temp.begin();
        cout << *it1 ;
        for (++it1 ; it1!=temp.end(); ++it1)
            cout <<" | "<< *it1 ;
        cout << endl;
    }
}

void checkGrammar(){
    for(gr = grammar.begin(); gr != grammar.end(); gr++){
        productions = gr->second;
        for(prodIterator = productions.begin(); prodIterator != productions.end(); ++prodIterator){
            string str = *prodIterator;
            for(int i = 0; i < str.length();i++){
                if(i+1 != str.length()){
                    int pos = operators.find(str[i+1]);
                    if(str[i]>='A' && str[i]<='Z'&& pos != operators.npos){       //non-terminal condition
                        i++;
                        continue;
                    }
                    else if(str[i]>='a' && str[i]<='z'&& pos != operators.npos){      //terminal condition
                        i++;
                        continue;
                    }
                    else{                                                               //otherwise condition
                        error = 1;
                        break;
                    }
                }
                else{
                    if(str[i]>='A' && str[i]<='Z'){       //non-terminal condition
                        continue;
                    }
                    else if(str[i]>='a' && str[i]<='z'){      //terminal condition
                        continue;
                    }
                    else{                                  //otherwise condition
                        error = 1;
                        break;
                    }
                }
            }
        }
    }
    if(error == 1){
        cout << "\nInvalid Grammar\n";
    }
    else{
        cout << "\nValid Grammar\n";
        checkInput();
    }
}

int findOperatorRelation(char tos, char ips){
    if(tos>='a'&&tos<='z'){                                                                 //terminal condition
        if(ips>='a'&&ips<='z')  return 0;
        else  return 2;
    }
    else if(tos=='/'){                                                                      //division condition
        if(ips>='a'&&ips<='z') return 1;
        else return 2;
    }
    else if(tos =='*'){                                                                    // * condition
        if(ips>='a'&&ips<='z'||ips=='/') return 1;
        else return 2;
    }
    else if(tos=='+'){                                                                      // + condition
        if(ips>='a'&&ips<='z'||ips=='/'||ips=='*') return 1;
        else return 2;
    }
    else if(tos=='-'){                                                                      // - condition
        if(ips>='a'&&ips<='z'||ips=='/'||ips=='*'||ips=='+') return 1;
        else return 2;
    }
    else if(tos=='^'){                                                                      // ^ condition
        if(ips>='a'&&ips<='z'||ips=='/'||ips=='*'||ips=='+'||ips=='-'||ips=='^') return 1;
        else return 2;
    }
    else if(tos=='$'){                                                                     // $ condition
        if(ips>='a'&&ips<='z'||ips=='/'||ips=='*'||ips=='+'||ips=='-'||ips=='^') return 1;
        else return 0;
    }
}

void searchProduction(string s){
    for(gr = grammar.begin(); gr != grammar.end(); ++gr){
        productions = gr->second;
        prodIterator = productions.find(s);
        if(prodIterator != productions.end()){
            s = gr->first;
            reductionStack.push(s[0]);
            error = 0;
            break;
        }
        else{
            error =1;
        }
    }
}

void pushIntoReductionStack(char a){
    error =0;
    if(a=='/' || a=='*' || a=='+' || a=='-' || a=='^' ){
        char b1,b2;
        b2 = reductionStack.top();
        if(b2>='a'&&b2<='z')
            searchProduction(string(1,b2));
        b2 = reductionStack.top();
        reductionStack.pop();
        b1 = reductionStack.top();
        if(b1>='a'&&b1<='z')
            searchProduction(string(1,b1));
        b1 = reductionStack.top();
        reductionStack.pop();
        string s = string(1,b1)+string(1,a)+string(1,b2);
        searchProduction(s);
    }
    else{
        searchProduction(string(1,a));
        if(error ==1){
            reductionStack.push(a);
        }
    }
}

void checkInput(){
    char a;
    parsingStack.push('$');
    for(int i=0; i < checkString.length();){
        int j = findOperatorRelation(parsingStack.top(),checkString[i]);
        if(j==1){
            parsingStack.push(checkString[i]);
            i++;
        }
        else if(j==2){
            a = parsingStack.top();
            parsingStack.pop();
            pushIntoReductionStack(a);
        }
        else if(j==0){
            break;
        }
    }
    if(parsingStack.top()=='$'&&startSymbol==string(1,reductionStack.top())){
        cout << "Input String Accepted\n";
    }
    else
        cout << "Input String Rejected\n";
}