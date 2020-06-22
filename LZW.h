#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;


class LZW
{
	string In_File_Location;
	string Encoded_File_Location;
	string Decoded_File_Location;
	map<int,string> Dictionary;
	int dict_Count;
public:
	LZW(pair<char,int>ch[] );
	void setInFileLocation(string);
	void setEncodedFileLocation(string = "./encoded");
	void setDecodedFileLocation(string = "./decoded");
        
	void encode();
	void decode();
};

