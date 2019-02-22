/*

@author : Gaurav Khandelwal 
@topic : LL(1) ParserresultGrammar

*/

#include<iostream>
#include<bits/stdc++.h>

using namespace std;

map<string, set<string> > First, Follow;
map<string, vector<string> >::iterator it;
vector<string> v, visited;
set<string> T;
set<string> NT; 
map<string, vector<string> > intial, result;
set<string> s, vis;
map< string ,map<string, vector<string> > > table;

void openFile()
{
    freopen("lab2_2.txt", "r", stdin);
	string str;
    int ind;
    cout << "\nNote : Read # as Epilsion\n\n";
	while(getline(cin, str)) 
	{
		ind = str.find(" -> ");
		string first = str.substr(0, ind);
        T.insert(first);
		string second = str.substr(ind+4);
        for(int i=0;i<second.length();i++)
            if(second[i]>='a' && second[i]<='z')
            {
                 string p="";
                 p+=second[i];
                 NT.insert(p);
            }
		intial[first].push_back(second);
	}
	result = intial;
}


void getLR()
{
	string str;
	for(it = intial.begin(); it != intial.end(); ++it){
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
	static int parse = -1;
	parse++;
	char x = 'L';
	for(int i = 0; i < parse; ++i) x += 1;
    string ret(1, x); 
    return ret;
}

void ridOfLR()
{
	for(int i = 0; i < visited.size(); ++i){
		string add = buildNewVariable();	
		string str = visited[i];
		v = result[str];
		result.erase(str);
		int ind;
		for(int j = 0; j < v.size(); ++j){
			string y = "";
            y+=v[j][0];
			if(y.compare(str)!=0){									//For Non Recursive elements in recursive Non Terminal
				result[str].push_back(v[j] + add);
			}
			else{													//For Recursive elements in recursive Non Terminal
				result[add].push_back(v[j].substr(str.length()) + add);
			}
		}
		result[add].push_back("#");
	}
}

void leftFactoring()
{
	for(it = result.begin(); it != result.end(); ++it){
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
                    if(ind != string::npos)
                    {
                        if(v[j] == pre[i]) v2.push_back("#");
                        else v2.push_back(v[j].substr(pre[i].length()));
                        v[j] = "@";
                    }
                }
                result[add] = v2;
            }
            for(int i = 0; i < v.size(); ++i) if(v[i] != "@") v1.push_back(v[i]);
            result[it->first] = v1;
        }
	}
}

set<string> findFirst(string k)
{
	if(vis.find(k) != vis.end()) return First[k];
	vis.insert(k);
	v = result[k];
	for(int i = 0; i < v.size(); ++i){
		string str = v[i];
		for(int j = 0; j < str.length(); ++j){
			if(str[j] >= 65 && str[j] <= 90)						//Reading a Non Terminal
			{
				s = findFirst(string(1, str[j]));
				First[k].insert(s.begin(), s.end());

				if(s.find("#") == s.end()) break;
				else if(j != str.length() - 1){
                    First[k].erase(First[k].find("#"));
                }
			}
			else													//All remaining are Terminals
			{
				First[k].insert(string(1, str[j]));
				//cout<<"Inserted -> "<<str[j]<<endl;
				break;
			}
		}
	}
	return First[k];
}

set<string> findFollow(string k)
{
    if(vis.find(k) != vis.end()) return Follow[k];
	vis.insert(k);
	for(map<string, vector<string> >::iterator temp = result.begin(); temp != result.end(); ++temp){
		string nt = temp->first;
		v = temp->second;
		for(int i = 0; i < v.size(); ++i)
		{
            int ind = v[i].find(k);
            if(ind == string::npos) continue;
            for(ind = ind+1; ind < v[i].length(); ++ind){
                if(v[i][ind] >= 65 && v[i][ind] <= 90){
                    Follow[k].insert(First[string(1, v[i][ind])].begin(), First[string(1, v[i][ind])].end());
                    Follow[k].erase(Follow[k].find("#"));
                    if(First[string(1, v[i][ind])].find("#") == First[string(1, v[i][ind])].end()) 
						break;
                }
                else{
                    Follow[k].insert(string(1, v[i][ind]));
                    break;
                }
            }
            if(ind == v[i].length()){
                s = findFollow(nt);
                Follow[k].insert(s.begin(), s.end());
            }
		}
	}
	return Follow[k];
}

void showCurrentGrammar(map<string, vector<string> > grammar)
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

void createLLParserTable()
{
    for(map<string, vector<string> >::iterator it = result.begin(); it != result.end(); ++it){
        string nt = it->first;
		v = it->second;
        for(int i=0;i<v.size();i++){
            if(v[i][0]>='a' && v[i][0]<='z'){
                string res = nt + " -> " + v[i];
                map<string,vector<string> > m;
                m = table[nt];
                string p="";
                p+=v[i][0];
                vector<string> f = m[p];
                f.push_back(res);
                m[p] = f;
                table[nt] = m;
            }
            else{
                string p = "";
                p+=v[i][0];
                if(p.compare("#")==0){    
                    string res = nt + " -> " + v[i];
                    set<string> g = Follow[nt];
                    map<string,vector<string> > m = table[nt];
                    for(set<string>::iterator j = g.begin(); j != g.end(); ++j){
			            vector<string> f = m[*j];
                        f.push_back(res);
                        m[*j] = f;
		            }
                    table[nt] = m;
                }
                else{
                    string str = v[i];
                    string res = nt + " -> " + v[i];
                    set<string> s;
                    map<string,vector<string> > m = table[nt];
		            for(int j = 0; j < str.length(); ++j){
			            if(str[j] >= 65 && str[j] <= 90)						//Reading a Non Terminal
			            {
				            //cout<<"Recursing For "<<str[j]<<endl;
				            s = findFirst(string(1, str[j]));
				            s.insert(s.begin(), s.end());
				            if(s.find("$") == s.end()) break;
				            else if(j != str.length() - 1)
                            {
                                s.erase(s.find("$"));
                            }
		    	        }
			            else													//All remaining are Terminals
			            {
				            s.insert(string(1, str[j]));
				            //cout<<"Inserted -> "<<str[j]<<endl;
				            break;
			            }
	                }
                    for(set<string>::iterator j = s.begin(); j != s.end(); ++j)
		            {
			            vector<string> f = m[*j];
                        f.push_back(res);
                        m[*j] = f;
		            }
                    table[nt] = m;
                }
            }
        }
    }
    cout << "\n";
    cout << "___________________LL(1) Parsing TABLE__________________\n\n";
    cout << "\n\n";
    for(set<string>::iterator j = T.begin(); j != T.end(); ++j){
		cout << *j << "  ::   ";
        map<string,vector<string> > m = table[*j];
        map<string,vector<string> >::iterator it =m.begin();
        for(it=m.begin();it!=m.end();it++){
        cout << " ( " << it->first << " , ";
        vector<string> f = it->second;
        for(int i=0;i<f.size();i++)
            cout << f[i] << " )      ";
        }
        cout << "\n\n";
	}
}

void buildRRGrammar()
{
    cout<<"Given Grammar :: "<<endl;
    string str;
	for(it =    intial.begin(); it != intial.end(); ++it){
		str = it->first;
		cout<<str<<" -> ";
		v = it->second;
		for(int i = 0; i < v.size(); ++i){
			if(i != 0) cout<<" | ";
			cout<<v[i];
		}
		cout<<endl;
	}
    cout<<"_____________________________________________________________________"<<endl<<endl;
	cout<<"_____________________________________________________________________"<<endl<<endl;
    getLR();
	ridOfLR();
    cout<<"Grammar After Removing Left Recursion :: "<<endl<<endl;
	showCurrentGrammar(result);
    cout<<"_____________________________________________________________________"<<endl<<endl;
	cout<<"_____________________________________________________________________"<<endl<<endl;
}

void doDeterministicGrammar()
{
    leftFactoring();
    cout<<"Building the Deterministic Grammar :: "<<endl;
	showCurrentGrammar(result);
    cout<<"_____________________________________________________________________"<<endl<<endl;
	cout<<"_____________________________________________________________________"<<endl<<endl;
}

void calculateFirst()
{
    for(it = result.begin(); it != result.end(); ++it){
		findFirst(it->first);
	}
	cout<<"Calcuated First of Each Variable :: "<<endl;
	for(map<string, set<string> >::iterator i = First.begin(); i != First.end(); ++i){
		cout<<i->first<<" : { ";
		s = i->second;
		for(set<string>::iterator j = s.begin(); j != s.end(); ++j){
			if(j != s.begin()) cout<<", ";
			cout<<*j;
		}
		cout<<" }"<<endl;
	}
    cout<<"_____________________________________________________________________"<<endl<<endl;
	cout<<"_____________________________________________________________________"<<endl<<endl;
}

void calculateFollow()
{
    vis.clear();
	Follow["S"].insert("$");
    findFollow("S");
	for(it = result.begin(); it != result.end(); ++it){
		findFollow(it->first);
	}
	cout<<"Calculated Follow of Each Variable :: "<<endl;
	for(map<string, set<string> >::iterator i = Follow.begin(); i != Follow.end(); ++i){
		cout<<i->first<<" : { ";
		s = i->second;
		for(set<string>::iterator j = s.begin(); j != s.end(); ++j)
		{
			if(j != s.begin()) cout<<", ";
			cout<<*j;
		}
		cout<<" }"<<endl;
	}
    cout<<"_____________________________________________________________________"<<endl<<endl;
	cout<<"_____________________________________________________________________"<<endl<<endl;
}

int main()
{
    openFile();
    buildRRGrammar();
    doDeterministicGrammar();
    calculateFirst();
    calculateFollow();
    for(it = result.begin(); it != result.end(); ++it){
		string str = it->first;
        T.insert(str);
		v = it->second;
		for(int i = 0; i < v.size(); ++i){
			string p = "";
            p+=v[i];
            for(int i=0;i<p.length();i++)
            if(p[i]>='a' && p[i]<='z'){
                string x ="";
                x+=p[i];
                NT.insert(x);
            }
		}
	}
    createLLParserTable();
    return 0;
}
