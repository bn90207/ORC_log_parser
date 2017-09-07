#include<iostream>
#include<stdio.h> //sscanf
#include<fstream>
#include<sstream>
#include<vector>
#include<windows.h>
#include<bitset>
#include<iomanip>
using namespace std;

int main(){
	ifstream in_file;
	in_file.open("string.txt");
	ofstream out_file;
	out_file.open("binary.txt");
	
	string row;
	while(getline(in_file, row))
	{
		string str;
		for(size_t i=0;i<row.length();i++)
			str = str + bitset<8>(row.c_str()[i]).to_string();
		out_file << "length:" << row.length() <<" binary_str:" << str << endl;
	} 
	
	
}
