#pragma once

#include <iostream>
#include <fstream>
#include <cstring>

void compare_file(const char * name1,const char * name2){
	using namespace std;
	ifstream if1(name1);
	ifstream if2(name2);
	if(!if1.is_open() || !if2.is_open())
		return;
	const int MAX_LEN = 1024;
	char buf1[MAX_LEN],buf2[MAX_LEN];
	int line = 1,ret = 1;
	while(!if1.eof() && !if2.eof()){
		if1.getline(buf1,MAX_LEN - 1);
		if2.getline(buf2,MAX_LEN - 1);
		if(strcmp(buf1,buf2)){
			cout<<"Line "<<line<<endl
				<<name1<<": \n"<<buf1<<endl
				<<name2<<": \n"<<buf2<<endl;
			ret = 0;
		}
		++line;
	}
	if(ret)
		cout<<"Match!\n";
}
