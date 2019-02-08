#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Feb  7 16:33:24 2019

@author: ubuntu

"""

file_ptr=open("lab1.txt","r")

systemfunctions={'printf':0,'scanf':0}

keywords_in_c = {'auto':0,  'const' :0,    'double' :0, 'float' :0, 'int' :0, 'short' :0,  'struct'  :0, 'unsigned':0,
    'break' :0, 'continue':0,  'else'   :0, 'for' :0,   'long' :0,     'signed'  :0,'switch' :0,  'void':0,
    'case':0,   'default':0,   'enum' :0,   'goto' :0,  'register':0,  'sizeof' :0, 'typedef' :0, 'volatile':0,
    'char'  :0, 'do' :0, 'extern' :0, 'if' :0,    'return' :0,   'static':0,  'union' :0,   'while':0,
    'asm'	:0,'dynamic_cast':0, 'namespace':0,	'reinterpret_cast':0, 'bool':0,	'explicit':0,	'new':0,'static_cast':0,
    'catch':0,	'false':0,	'operator':0,	'template':0,
    'class'	:0,'friend':0,	'private':0,	'this':0,
    'const_cast':0,	'inline':0,	'public':0,'throw':0,
    'delete':0,	'mutable':0,	'protected':0,	'true':0,
    'try':0,	'typeid':0,	'typename':0,	'using':0,
    'using'	:0,'virtual'	:0,'wchar_t':0,'include':0,'define':0}

double_single_operators_in_c_in_c={'<<':0,'>>':0,'&&':0, '||':0,'++':0,'--':0,'==':0,'<=':0,'>=':0,'->':0}

single_operators_in_c ={'+' : 0,'-' : 0,'*' : 0,'/' : 0,'%' :0,'<'  :0,'>':0,'&':0,'|':0,'!' :0,'^':0,'=':0}

punctuation_in_c= {'"' : 0,'#' : 0,'$' : 0,'(' : 0,')' : 0,',' : 0,'.' : 0,':' : 0,';' : 0,'[' : 0,']' : 0,'`' : 0,'{' : 0,'}' : 0,'~' : 0 }

leftover_tokens=[]
flag=0

def unique_list(l):
    ulist = []
    [ulist.append(x) for x in l if x not in ulist]
    return ulist

for line in file_ptr:
    if line.find("//")!=-1:
        line=line.replace(line[line.find("//"):],"")
        
    if line.find("/*")!=-1:
        
        flag=1
        
        if line.find("*/")!=-1:
            line=line.replace(line[line.find("/*"):line.find("*/")+2],"")
            flag=0
            
        else :
            line=line.replace(line[line.find("/*"):],"")
    
    if line.find("*/")!=-1:
        line=line.replace(line[:line.find("*/")+2],"")
        flag=0
        
    if flag==1:
        line=line.replace(line[:],"")

    for t in systemfunctions.keys():
        if line.find(t)!=-1:
            systemfunctions[t]+=1
            line=line.replace(line[line.find(t):line.find(t)+len(t)]," ")
            leftover_tokens.append( line[ line.find('\"'):line.find('\"',line.find('\"')+1)+1 ] )
            line=line.replace( line[ line.find('\"'):line.find('\"',line.find('\"')+1)+1 ] , " ")

    for t in double_single_operators_in_c_in_c.keys():
        pos=0
        while line.find(t,pos)!=-1:
            double_single_operators_in_c_in_c[t]+=1
            pos=line.find(t,pos)+len(t)
        if line.find(t)!=-1:
            line=line.replace(line[line.find(t):line.find(t)+len(t)]," ")
        
    for t in single_operators_in_c.keys():
        pos=0
        while line.find(t,pos)!=-1:
            single_operators_in_c[t]+=1
            pos=line.find(t,pos)+len(t)
        if line.find(t)!=-1:
            line=line.replace(line[line.find(t):line.find(t)+len(t)]," ")

    for t in punctuation_in_c.keys():
        pos=0
        while line.find(t,pos)!=-1:
            punctuation_in_c[t]+=1
            pos=line.find(t,pos)+len(t)
        if line.find(t)!=-1:
            line=line.replace(line[line.find(t):line.find(t)+len(t)]," ")

    for t in keywords_in_c.keys():
        pos=0
        line=line.rstrip()
        line=" "+line+" "
        while line.find(" "+t+" ",pos)!=-1:
            keywords_in_c[t]+=1
            pos=line.find(" "+t+" ",pos)+len(t)
        if line.find(" "+t+" ")!=-1:
            line=line.replace(line[line.find(" "+t+" "):line.find(" "+t+" ")+len(t)+1]," ")
    
    line1=line.split()
    for words in line1:
        if words.split()!="":
            leftover_tokens.append(words)
    



keywords_table=[]
for line in keywords_in_c:
    if keywords_in_c[line]!=0:
         #print(line + "  "+str(keywords_in_c[line]))
         keywords_table.append("< keyword"+" , "+line+" , "+str(keywords_in_c[line])+" >") 

print("\n****************** TABLES **********************\n")									
print("\n ______________	Keywords Table______________ \n")
for item in keywords_table:
	print(item)									

									
# punctuation_table
punctuation_table=[]
for line in punctuation_in_c:
    if punctuation_in_c[line]!=0:
        #print(line + "  "+str(punctuation_in_c[line]))
        punctuation_table.append("< punctuation"+" , "+line+" , "+str(punctuation_in_c[line])+" >")

print("\n ______________	Punctuation Table______________ \n")
for item in punctuation_table:
	print(item)									


								
# operator_table	
operator_table=[]
for line in double_single_operators_in_c_in_c:
    if double_single_operators_in_c_in_c[line]!=0:
        #print(line + "  "+str(double_single_operators_in_c_in_c[line]))
        operator_table.append("< operator"+" , "+line+" , "+str(double_single_operators_in_c_in_c[line])+" >")
									
								
for line in single_operators_in_c:
    if single_operators_in_c[line]!=0:
        #print(line + "  "+str(single_operators_in_c[line]))
        operator_table.append("< operator"+" , "+line+" , "+str(single_operators_in_c[line])+" >") 														 							

print("\n ______________	Operators Table______________ \n")
for item in operator_table:
	print(item)								

								
#print("Symbol Table")
leftover_tokens2={}
leftover_tokens1=list(set(leftover_tokens))
for item in leftover_tokens1:
    if item.isdigit()==False:
        leftover_tokens2[item]=""
        #print(item)

#print(leftover_tokens2)
for item in leftover_tokens2.keys():
    file_ptr.seek(0,0)
    for line in file_ptr:
        if line.find("//")!=-1:
            line=line.replace(line[line.find("//"):],"")
        if line.find("/*")!=-1:
            flag=1
            if line.find("*/")!=-1:
                line=line.replace(line[line.find("/*"):line.find("*/")+2],"")
                flag=0
            else :
                line=line.replace(line[line.find("/*"):],"")
        if line.find("*/")!=-1:
            line=line.replace(line[:line.find("*/")+2],"")
            flag=0
        
        if flag==1:
            line=line.replace(line[:],"")

        if line.find(item)!=-1:
            if line[line.find(item)+len(item)]=="(":
                leftover_tokens2[item]+=' function'
                #print(item + " is a function name")
            elif line[line.find(item)+len(item)]=="{":
                leftover_tokens2[item]+=' class'
                #print(item + " is a class name")
            else:
                leftover_tokens2[item]+=' user_def'
                #print(item + " is a user defined name")


for item in leftover_tokens2.keys():
    leftover_tokens2[item]=' '.join(unique_list(leftover_tokens2[item].split()))

				
symbol_table=[]
for item in leftover_tokens2.keys():
    if leftover_tokens2[item].find("function")!=-1:
        #print("   " + item + "         is a function name")
        symbol_table.append("< symbol"+" , "+item+" , "+"Function Name"+" >")								

    elif leftover_tokens2[item].find("class")!=-1:
        #print("   " + item + "          is a class name")
        symbol_table.append("< symbol"+" , "+item+" , "+"Class Name"+" >")  								
    else:
        #print("   " + item + "          is a user defined name")
        symbol_table.append("< symbol"+" , "+item+" , "+"User Defined Name"+" >")							

print("\n ______________Symbol Table______________ \n")
for item in symbol_table:
	print(item)		

print("\n ______________System Function Table______________ \n")
for key in systemfunctions:
	print("< system function "+" , "+key+" , "+str(systemfunctions[key])+" >")		