#include<iostream>
#include<bits/stdc++.h>
#include<fstream>

using namespace std;

map<char,bool> isDelimiter;
map<char,bool> isOperator;
map<char,bool> isIdentifier;
map<string,bool> isKeyword;
 

void init() 
{   
    isOperator['+']=isOperator['-']=isOperator['*']=isOperator['/']=isOperator['%']=isOperator['<']=isOperator['>']=isOperator['=']=true;

    isDelimiter[' ']=isDelimiter['+']=isDelimiter['-']=isDelimiter['*']=isDelimiter['/']=isDelimiter[',']=isDelimiter[';']=isDelimiter['>']=true;
    isDelimiter['<']=isDelimiter['=']=isDelimiter['(']=isDelimiter[')']=isDelimiter['[']=isDelimiter[']']=isDelimiter['{']=isDelimiter['}']=true;

    isIdentifier['0']=isIdentifier['1']=isIdentifier['2']=isIdentifier['3']=isIdentifier['4']=isIdentifier['5']=isIdentifier['6']=true;
    isIdentifier['7']=isIdentifier['8']=isIdentifier['9']=true;

    isKeyword["if"]=isKeyword["else"]=isKeyword["while"]=isKeyword["do"]=true;
    isKeyword["break"]=isKeyword["continue"]=isKeyword["int"]=isKeyword["double"]=isKeyword["float"]=true;
    isKeyword["return"]=isKeyword["char"]=isKeyword["case"]=isKeyword["sizeof"]=isKeyword["long"]=isKeyword["typedef"]=isKeyword["short"]=true;
    isKeyword["if"]=isKeyword["switch"]=isKeyword["void"]=isKeyword["static"]=isKeyword["goto"]=isKeyword["struct"]=isKeyword["unsigned"]=true;
    isKeyword["if"]=true;
}



int main()
{
    init();

            

}