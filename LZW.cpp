#include "LZW.h"

LZW::LZW(pair<char,int>ch[])
{
	this->Value_dict_Count = 0;

	this->In_File_Location = "\0";
	this->Encoded_File_Location = "\0";
	this->Decoded_File_Location = "\0";

	// If a Single Char has a frequency add it to the starter dictionary
	for (int i = 0; i < 256; i++)
	{
		if (ch[i].second != 0)
		{
			char s = i;

			this->Value_Dictionary[ string(1,s) ] = Value_dict_Count;
			this->Dictionary[Value_dict_Count] = s;
			this->Value_dict_Count++;
		}
	}

	dict_Count = Value_dict_Count;

}

void LZW::setInFileLocation(string In)
{
	this->In_File_Location = In;
	cout << "\nIn File Location : " << this->In_File_Location;
}

void LZW::setEncodedFileLocation(string In)
{
	if (In == "")
	{
		this->Encoded_File_Location = "./encoded";
	}
	else
	{
		this->Encoded_File_Location = In;
	}
	cout << "\nEncoded File Location : " << this->Encoded_File_Location;
}

void LZW::setDecodedFileLocation(string In)
{
	if (In == "")
	{
		this->Decoded_File_Location = "./decoded";
	}
	else
	{
		this->Decoded_File_Location = In;
	}
	cout << "\nDecoded File Location : " << this->Decoded_File_Location;
}

void LZW::encode()
{
	ifstream In;
	In.open(this->In_File_Location, std::ios::in | std::ios::binary);
	if (!In)
	{
		cout << "\nCan't Open Input File";
		exit(1);
	}
	ofstream Encoded;

	Encoded.open(this->Encoded_File_Location, std::ios::out);
	if (!Encoded)
		exit(1);

	string line;
	string Input = "";

	string byte = "";

	// In Case input File had one line 
	// The code will enter this condition
	// And won't enter the next condition

	int Last_Match = 0;
	char c;
	char Bytes[3];

	while (In.get(c))
	{
		
		// Match Found add to Input
		if ( this->Value_Dictionary.find( Input + string(1, c) ) != Value_Dictionary.end() )
		{
			Input += c;
		}

		// No Match Found
		// Encode Last Match Index
		// Add Current Match + Char to Dictionary
		else
		{
			Last_Match = this->Value_Dictionary[Input];
	
			Bytes[0] = (unsigned char)(Last_Match >> 16) & 0xFF;
			Bytes[1] = (unsigned char)(Last_Match >> 8)  & 0xFF;
			Bytes[2] = (unsigned char)(Last_Match)       & 0xFF;
			Encoded.write(Bytes, 3);

			this->Value_Dictionary[Input + string(1, c)] = Value_dict_Count;
			Value_dict_Count++;
			
			Input = c;
		}
	}

	In.close();
	Encoded.close();
}

void LZW::decode()
{
	ifstream Encoded;
	Encoded.open(this->Encoded_File_Location, std::ios::out);
	if (!Encoded)
		exit(1);

	ofstream Decoded;
	Decoded.open(this->Decoded_File_Location, std::ios::out);
	if (!Decoded)
		exit(1);
    
	char Bytes[3];
	int index;
	int Last_index;
	string original = "";
	string Stream = "";

	Encoded.read(Bytes, 3);

	index = int((unsigned char)Bytes[0] << 16 |
		        (unsigned char)Bytes[1] << 8 |
		        (unsigned char)Bytes[2]
	);
	original = Dictionary[index];
	Decoded << original;

	Last_index = index;
	Stream = original;

	bool in = true;
	while ( in )
	{
		Encoded.read(Bytes, 3);
		if (!Encoded.eof())
		{
			index = int((unsigned char)Bytes[0] << 16 |
				        (unsigned char)Bytes[1] << 8 |
				        (unsigned char)Bytes[2]
			);

			if (Dictionary.find(index) != Dictionary.end())
			{
				original = Dictionary[index];
				Last_index = index;
			}
			else
			{
				original = Dictionary[Last_index];
				original += original[0];
			}

			Decoded << original;

			Dictionary[dict_Count++] = Stream + original[0];
			Stream = original;
		}
		else
		{
			in = false;
		}
	}
	
	Encoded.close();
	Decoded.close();
	
}

