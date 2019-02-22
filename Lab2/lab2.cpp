/*

@author : Gaurav Khandelwal 
@topic : LL(1) ParserresultGrammar

*/

#include<iostream>
#include<cstdio>
#include<bits/stdc++.h>

using namespace std;

map<string, set<string> > First, Follow;
map<string, vector<string> >::iterator it;
vector<string> v, visited;
map<string, vector<string> > intialGrammar, resultGrammar;
set<string> s, vis;

void printGrammar(map<string, vector<string> > grammar)
{
	string str;
	for(it = grammar.begin(); it != grammar.end(); ++it){
		str = it->first;
		cout<<str<<" -> ";
		v = it->second;
		for(int i = 0; i < v.size(); ++i){
			if(i != 0) cout<<" | ";
			cout<<v[i];
		}
		cout<<endl;
	}
}

void findLR()
{
	string str;
	for(it = intialGrammar.begin(); it != intialGrammar.end(); ++it){
		str = it->first;
		v = it->second;
		int ind;
		for(int i = 0; i < v.size(); ++i){
			string y="";
            y+=v[i][0];
			if(y.compare(str)==0){
				visited.push_back(str);
				break;
			}
		}
	}
}

string buildNewVariable()
{
	static int count = -1;
	count++;
	string ret = "X";
	for(int i = 0; i < count; ++i) ret += "'";
	return ret;
}

void removeLR()
{
	for(int i = 0; i < visited.size(); ++i){
		string add = buildNewVariable();	
		string str = visited[i];
		v = resultGrammar[str];
		resultGrammar.erase(str);
		int ind;
		for(int j = 0; j < v.size(); ++j){
			string y = "";
            y+=v[j][0];
			if(y.compare(str)!=0){									//For Non Recursive elements in recursive Non Terminal
				resultGrammar[str].push_back(v[j] + add);
			}
			else{													//For Recursive elements in recursive Non Terminal
				resultGrammar[add].push_back(v[j].substr(str.length()) + add);
			}
		}
		resultGrammar[add].push_back("#");
	}
}

void leftFactoring()
{
	for(it = resultGrammar.begin(); it != resultGrammar.end(); ++it){
        vector<string> pre;
		v = it->second;
		sort(v.begin(), v.end());
		string temp = v[0], prefix = "";
		for(int i = 1; i < v.size(); ++i){
			int len = temp.length() + 1;
			int ind;
			while(--len){
				ind = v[i].find(temp.substr(0, len));
				if(ind != string::npos && ind == 0){
					prefix = temp.substr(0, len);
					temp = prefix;
					break;
				}
			}
			if(len == 0){
				if(prefix != "") pre.push_back(prefix);
				prefix = "";
				temp = v[i];
			}
		}
		if(prefix != "") pre.push_back(prefix);
		if(!pre.empty()){
            vector<string> v1;
            for(int i = 0; i < pre.size(); ++i){
                vector<string> v2;
                string add = buildNewVariable();
                v1.push_back(pre[i] + add);
                for(int j = 0; j < v.size(); ++j){
                    size_t ind = v[j].find(pre[i]);
                    if(ind == 0)
                    {
                        if(v[j] == pre[i]) v2.push_back("#");
                        else v2.push_back(v[j].substr(pre[i].length()));
                        v[j] = "@";
                    }
                }
                resultGrammar[add] = v2;
            }
            for(int i = 0; i < v.size(); ++i) if(v[i] != "@") v1.push_back(v[i]);
            resultGrammar[it->first] = v1;
        }
	}
}




int main()
{
	freopen("lab2.txt", "r", stdin);
	string str;
    int ind;
    cout << "Read # as Epilsion\n";
	while(getline(cin, str)) 
	{
		ind = str.find(" -> ");
		string first = str.substr(0, ind);
		string second = str.substr(ind+4);
		intialGrammar[first].push_back(second);
	}
	resultGrammar = intialGrammar;
	cout<<"The actual grammar with Left Recursion is : "<<endl;
	printGrammar(intialGrammar);
	cout<<"--------------------------------------------"<<endl;
	findLR();
	removeLR();
	cout<<"The changed grammar withour left recursion is : "<<endl;
	printGrammar(resultGrammar);
	cout<<"------------------------------------------------"<<endl;

    leftFactoring();
    cout<<"The Deterministic grammar is : "<<endl;
	printGrammar(resultGrammar);
	cout<<"--------------------------------------------"<<endl;
    return 0;
}
