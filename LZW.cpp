#include "LZW.h"

LZW::LZW(pair<char,int>ch[])
{
        dict_Count = 0;

        this->In_File_Location = "\0";
        this->Encoded_File_Location = "\0";
        this->Decoded_File_Location = "\0";

        bool NewLine = true;
        // If a Char has a frequency add it to the starter dictionary
        for (int i = 0; i < 256; i++)
        {
                if (ch[i].second != 0)
                {
                        char c = i;
                        this->Dictionary[dict_Count] = i;
                        dict_Count++;
                }

                if (i == 10 && ch[i].second == 0)
                {
                        NewLine = false;
                }
        }
        if (!NewLine) // Add new Line char to theend of the starting alphabet
        {
                this->Dictionary[dict_Count] = 10;
                dict_Count++;
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
                                Encoded << Last_Match;
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
                                Encoded << Last_Match;
                                this->Dictionary[dict_Count++] = Input + string(1, '\n');
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
