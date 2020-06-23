#include "LZW.h"

LZW::LZW(pair<char,int>ch[])
{
	this-> dict_Count = 0;

	this-> Byte8_Count = 0;
	this-> Byte16_Count = 0;


	this->In_File_Location = "\0";
	this->Encoded_File_Location = "\0";
	this->Decoded_File_Location = "\0";

	// If a Char has a frequency add it to the starter dictionary
	for (int i = 0; i < 256; i++)
	{
		if (ch[i].second != 0)
		{
			unsigned char s = i;
			this->Dictionary[dict_Count] = s;
			this->Value_Dictionary[ string(1,s) ] = dict_Count;

			this-> dict_Count++;
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
			// Match Found add to Input
			string nextS = Input + string(1, c);
			if ( this->Value_Dictionary.find( nextS ) != Value_Dictionary.end() )
			{
				Input += c;
			}

			// No Match Found
			// Encode Last Match Index
			// Add Current Match + Char to Dictionary
			else
			{
				Last_Match = this->Value_Dictionary[Input];

				if (Last_Match < 256) // 2^8
				{
					char bytes[1] = { (unsigned char)(Last_Match) & 0xFF };
					Encoded.write(bytes,1);
					this->Byte8_Count++;
				}
				else if (Last_Match < 65536) // 2^16
				{
					char bytes[2] = { (unsigned char)(Last_Match >> 16) & 0xFF,
								      (unsigned char)(Last_Match >> 8) & 0xFF
					};
					Encoded.write(bytes, 2);
					this->Byte16_Count++;
				}
				else // 2^24
				{
					char bytes[3] = { (unsigned char)(Last_Match >> 16) & 0xFF,
								  (unsigned char)(Last_Match >> 8) & 0xFF,
								  (unsigned char)(Last_Match) & 0xFF
					};
					Encoded.write(bytes, 3);
				}
				

				dict_Count++;
				this->Dictionary[dict_Count] = Input + string(1, c);
				this->Value_Dictionary[Input + string(1, c)] = dict_Count;
				
				Input = c;
			}
		}
		// Encode new Line or Last String if Last \n in File
		if (!In.eof())
		{
			//New Line 
			if (this->Value_Dictionary.find(Input + string(1, '\n')) != Value_Dictionary.end())
			{
				Input += '\n';
			}
			else
			{
				if (Last_Match < 256) // 2^8
				{
					char bytes[1] = { (unsigned char)(Last_Match) & 0xFF };
					Encoded.write(bytes, 1);
				}
				else if (Last_Match < 65536) // 2^16
				{
					char bytes[2] = { (unsigned char)(Last_Match >> 16) & 0xFF,
									  (unsigned char)(Last_Match >> 8) & 0xFF
					};
					Encoded.write(bytes, 2);
				}
				else // 2^24
				{
					char bytes[3] = { (unsigned char)(Last_Match >> 16) & 0xFF,
								  (unsigned char)(Last_Match >> 8) & 0xFF,
								  (unsigned char)(Last_Match) & 0xFF
					};
					Encoded.write(bytes, 3);
				}

				dict_Count++;
				this->Dictionary[dict_Count] = Input + string(1, '\n');
				this->Value_Dictionary[Input + string(1, '\n')] = dict_Count;

				Input = '\n';
			}
		}
		
	}

	In.close();
	Encoded.close();
	cout << "\nFinished Encoding\n";
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
	
	while (!Encoded.eof())
	{
		for (int i = 0; i < this->Byte8_Count; i++)
		{
			char bytes[1];
			Encoded.read(bytes, 1);

			int Val = int( (unsigned char)bytes[0] );

			Decoded << Dictionary[Val];
		}

		for (int i = 0; i < this->Byte16_Count; i++)
		{
			char bytes[2];
			Encoded.read(bytes, 2);

			int Val = int((unsigned char)bytes[0] << 8 |
				          (unsigned char)bytes[1]
	        );
			Decoded << Dictionary[Val];
		}

		char bytes[3];
		while (Encoded.read(bytes, 3))
		{
			int Val = int((unsigned char)bytes[0] << 16 |
				(unsigned char)bytes[1] << 8 |
				(unsigned char)bytes[2]
			);
			Decoded << Dictionary[Val];
		}		
	}
	Encoded.close();
	Decoded.close();
}
