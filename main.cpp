#include "LZW.h"


int main()
{
    cout << "Enter Input File Location : ";
    string In_File_Location;
    getline(cin, In_File_Location);

    // First Get the Alphabet
    cout << "\n\nDetermining the Alphabet................\n";
    unsigned char c;
    pair<char, int> ch[256];
    for (int i = 0; i < 256; i++)
    {
        ch[i].second = 0;
        c = i;
        ch[i].first = c;
    }
    ifstream In;
    In.open(In_File_Location, std::ios::in || std::ios::binary);
    if (!In)
    {
        cout << "\nCan't Open Input File";
        exit(1);
    }


    string line = "";
    while (getline(In, line))
    {
        for (unsigned char c : line)
        {
            int k = (int)c;
            ch[k].second++;
        }
    }
    In.close();


    // Second Perform the Required Task
    LZW Task( ch );

    Task.setInFileLocation(In_File_Location);

    cout << "\n\nEnter the Location to save the Encoded File: ";
    Task.setEncodedFileLocation();

    cout << "\n\nEncoding ................................... ";
    Task.encode();


    cout << "\n\nEnter the Location to save the Decoded File: ";
    Task.setDecodedFileLocation();

    cout << "\n\nDecoding ................................... ";
    Task.decode();


}
