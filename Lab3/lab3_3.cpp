/* 
    Gaurav Khandelwal
    2016KUCP1002
*/

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

set<char> rt;
map<char, vector<char> > lead,trail;
map<char,map<char,char>> store;
char Start;

bool checkTerminal(char c)
{  
 if (!isupper(c) && c != '\0' && c != '|') return true;
 else return false;
}

bool checkEpsilion(char c)
{
   if(c == '#')
     return true;
   return false;
}

bool checkForOOP(char *string)
{
  for(int i=2;string[i]!='\0';i++)
  {
    if(string[i]=='|')
       continue;
    if(checkEpsilion(string[i]))
       return false;
    else
    {
      if(string[i+1]!='\0' &&  string[i+1]!='|' && ( checkTerminal(string[i])==false && checkTerminal(string[i+1])==false ) )
      {
        return false;   
      }
    }
   }
   return true;
}

int findProduction(char NT, char **strings, int currentProduction, int nProductions){
  for (int i = 0; i < nProductions; ++i){ 
  if (i == currentProduction)continue;
  if (strings[i][0] == NT)return i;
 } 
}

void findLeading(char *string, char** strings, int currentProduction, int nProductions,char intial)
{
 int i = 2; 
 for(; i < 50; ++i){ 
   if(checkTerminal(string[i])){ 
   cout << string[i] << " ";
   lead[intial].push_back(string[i]);
   while (string[i] != '|' && string[i] != '\0') i++;   
  }
  else if (string[i] == '|' || string[i] == '\0'){ 
   int productionNo = findProduction(string[i - 1], strings, currentProduction, nProductions); 
   findLeading(strings[productionNo], strings, productionNo, nProductions,intial);
  }
  if (string[i] == '\0')break;
 }
}

void findTrailing(char *string, char** strings, int currentProduction, int nProductions,char intial)
{   
 int i = 49;
 while (string[i] != '\0')i--;
 i = i - 1;
 for (; i >= 1;--i){
  if (checkTerminal(string[i])){ 
   cout << string[i] << " ";
   trail[intial].push_back(string[i]);
   while (string[i] != '|' && string[i] != '=') i--;
  }
  else if (string[i] == '|' || string[i] == '='){
   int productionNo = findProduction(string[i + 1], strings, currentProduction, nProductions);
   findTrailing(strings[productionNo], strings, productionNo, nProductions,intial);
  }
  if (string[i] == '=')break;
 }
}

void parseTable(char** strings,int nproductions)
{
    rt.insert('$');
    vector<char> leadStart = lead[Start];
    vector<char> trailStart = trail[Start];
    set<char>::iterator it;
    
    for(int i=0;i<leadStart.size();i++)
    {
        store['$'][leadStart[i]]='<';
    }
    for(int i=0;i<trailStart.size();i++)
    {
        store[trailStart[i]]['$']='>';
    }

    for(int i=0;i<nproductions;i++)
    {
        char* string = strings[i];
        for(int j=2;j<string[j]!='\0';j++)
        {
            if(string[j]=='|' || string[j+1]=='|') continue;
            if( string[j+1]!='\0' && string[j+2]!='\0' && string[j+1]!='|' && string[j+2]!='|' && (checkTerminal(string[j]) && !checkTerminal(string[j+1]) && checkTerminal(string[j+2]))  )
                store[string[j]][string[j+2]]='=';
            else{
                if( string[j+1]!='\0' && string[j+1]!='|' && ( checkTerminal(string[j]) && checkTerminal(string[j+1]))  )
                    store[string[j]][string[j+2]]='=';
                else{
                    if( string[j+1]!='\0' && string[j+1]!='|' && ( checkTerminal(string[j]) && !checkTerminal(string[j+1]))  )
                    {
                       vector<char> var = lead[string[j+1]];
                       for(int k=0;k<var.size();k++)
                       {
                           cout << string[j] << " " << var[k] << endl;
                          store[string[j]][var[k]]='<';
                       }     
                    }
                    else{
                        if ( string[j+1]!='\0' && string[j+1]!='|' && ( !checkTerminal(string[j]) && checkTerminal(string[j+1])) )
                        {
                            vector<char> var = trail[string[j]];
                            for(int k=0;k<var.size();k++)
                            {
                              
                              store[var[k]][string[j+1]]='>';
                            }
                        }
                    }                 
                }
            }
        }
    }
    
    cout << "    ";
    for(it=rt.begin();it!=rt.end();it++)
    {
        cout << *it << "    ";
    }
    cout << "$" << endl;
    for(it=rt.begin();it!=rt.end();it++)
    {
        cout << *it << "    ";
        char ch1 = *it;
        set<char>::iterator it2;
        for(it2=rt.begin();it2!=rt.end();it2++)
        {
            char ch2 = *it2;
            if( store[ch1][ch2]=='<' || store[ch1][ch2]=='>' || store[ch1][ch2]=='=' )
                cout  << "[ " << ch2 << " , "<< store[ch1][ch2] << " ]" << " , ";
            else
                {}
        }
        cout << endl;
    }
    //cout << "$" << endl;
}

int main()
{
 cout << "Enter the number of Production: ";
 int n_product = 0;
 cin >> n_product;
 
 cout << "Enter the productions: \n";
 char **strings = new char*[n_product];
 
 for (int i = 0; i < n_product; ++i){
  strings[i] = new char[50]; 
  cin >> strings[i];
 } 

 for (int i = 0; i < n_product; ++i){   
  if(checkForOOP(strings[i])==false)
  {
    cout << "Not A Operator Precedance Parser" << endl;
    return 0;
  }
 } 

 Start = strings[0][0];   

 for (int i = 0; i < n_product; ++i){   
  for (int j=2;strings[i][j]!='\0';j++)
  {
      if(strings[i][j]=='|')
        continue;
      else if(checkEpsilion(strings[i][j])) continue;
      else if( checkTerminal(strings[i][j])) rt.insert(strings[i][j]); 
  }
 }

 cout << "Leading Set" << endl; 
 for (int i = 0; i < n_product; ++i){
  cout << "{ ";
  findLeading(strings[i], strings, i, n_product,strings[i][0]);
  cout << "}\n";
 } 

 cout << "Trailing Set" << endl;   
 for (int i = 0; i < n_product; ++i){
  cout << "{ ";
  findTrailing(strings[i], strings, i, n_product,strings[i][0]);
  cout << "}\n";
 }   

 parseTable(strings,n_product);

 return 0;
}