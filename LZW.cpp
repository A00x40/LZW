#include "LZW.h"

LZW::LZW(pair<char,int>ch[])
{
	dict_Count = 0;

	this->In_File_Location = "\0";
	this->Encoded_File_Location = "\0";
	this->Decoded_File_Location = "\0";

	// If a Char has a frequency add it to the starter dictionary
	for (int i = 0; i < 256; i++)
	{
		if (ch[i].second != 0)
		{
			unsigned char c = i;
			this->Dictionary[dict_Count] = string(1,c);
			dict_Count++;
		}
	}

}

void LZW::setInFileLocation(string In)
{
	this->In_File_Location = In;
	cout << "\nIn File Location : " << this->In_File_Location;
}

void LZW::setEncodedFileLocation(string In)
{
	this->Encoded_File_Location = In;
	cout << "\nIn File Location : " << this->Encoded_File_Location;
}

void LZW::setDecodedFileLocation(string In)
{
	this->Decoded_File_Location = In;
	cout << "\nIn File Location : " << this->Decoded_File_Location;
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
	string EncodedIn = "";

	// In Case input File had one line 
	// The code will enter this condition
	// And won't enter the next condition

	int Last_Match = 0;
	int i;
	while (getline(In, line))
	{
		// Process characters in each line
		for (unsigned char c : line)
		{
			bool found = false;
			
			for (i = 0; i < dict_Count; i++)
			{
				// Match Found add to Input
				if (this->Dictionary[i] == Input + string(1, c) )
				{
					Input += c;
					found = true;
					break;
				}
				
			}

			// No Match Found
			// Encode Last Match Index
			// Add Current Match + Char to Dictionary
			if (found == false)
			{
				for (i = 0; i < dict_Count; i++)
				{
			       if (this->Dictionary[i] == Input)
			       {
				      Last_Match = i;
			       }
				}
				byte += to_string(Last_Match);
				bitConcat(byte, EncodedIn);

				this->Dictionary[dict_Count++] = Input + string(1, c);
				Input = c;
				
			}
		}
		// Encode new Line or Last String if Last \n in File
		if (!In.eof())
		{
			//New Line 
			bool fnl = false;
			for (int i = 0; i < dict_Count; i++)
			{
				if (this->Dictionary[i] == Input + string(1, '\n'))
				{
					Input += '\n';
					fnl = true;
					break;
				}
			}
			if (!fnl)
			{
				for (i = 0; i < dict_Count; i++)
				{
					if (this->Dictionary[i] == Input)
					{
						Last_Match = i;
					}
				}
				byte += to_string(Last_Match);
				bitConcat(byte, EncodedIn);

				this->Dictionary[dict_Count++] = Input + string(1, '\n');
				Input = '\n';
			}
		}
		
	}
	int count = 8 - byte.size();
	if (byte.size() < 8)
	{
		//append number of 'count' '0' to the last few codes to create the last byte of text
		byte.append(count, '0');
	}
	EncodedIn += (unsigned char)binary_to_decimal(byte);
	Encoded.write(EncodedIn.c_str , EncodedIn.size);

	In.close();
	Encoded.close();
	cout << "\nFinished Encoding\n";
}

void LZW::bitConcat(string& byte, string& in)
{
	while (byte.size() >= 8)
	{
		//cut into 8-bit binary codes that can convert into saving char needed for binary file
		string ch = byte.substr(0, 8);
		in += (unsigned char)binary_to_decimal(ch);
		byte = byte.substr(8);
	}
}
int LZW::binary_to_decimal(string& in)
{
	int result = 0;
	for (int i = 0; i < in.size(); i++)
		result = result * 2 + in[i] - '0';
	return result;
}

void LZW::decode()
{
	ifstream Encoded;
	Encoded.open(this->Encoded_File_Location, std::ios::in | std::ios::binary);
	if (!Encoded)
	{
		cout << "\nCan't Open Input File";
		exit(1);
	}
	ofstream Decoded;

	Decoded.open(this->Decoded_File_Location, std::ios::out);
	if (!Decoded)
		exit(1);

	string line;
	;
	while (getline(Encoded,line))
	{
		for (unsigned char index : line)
		{
			int i = int(index) - 48;
			Decoded << Dictionary[i];
		}
	}
	Encoded.close();
	Decoded.close();
}
