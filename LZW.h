#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

using namespace std;


class LZW
{
	string In_File_Location;
	string Encoded_File_Location;
	string Decoded_File_Location;

	

	unsigned int Value_dict_Count;
	unordered_map<string, unsigned int> Value_Dictionary;

	unsigned int dict_Count;
	unordered_map<unsigned int, string> Dictionary;
public:
	LZW(pair<char,int>ch[] );
	void setInFileLocation(string);
	void setEncodedFileLocation(string);
	void setDecodedFileLocation(string);
	void encode();
	void decode();
};

