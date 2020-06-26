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
        ch[10].second++;
    }
    In.close();
    
    
    // Second Perform the Required Task
    LZW Task( ch );
    
    Task.setInFileLocation(In_File_Location);

    cout << "\n\nPress 0 For Encoding / 1 For Decoding : ";

    string Operation;
    getline(cin, Operation);

    // Encoding
    if (Operation == "0")
    {
        cout << "\n\nEnter the Location to save the Encoded File: ";

        string Encoded_File_Location;
        getline(cin, Encoded_File_Location);

        Task.setEncodedFileLocation(Encoded_File_Location);

        cout << "\n\nEncoding ................................... ";
        Task.encode();
    }

    // Decoding
    else if (Operation == "1")
    {
        cout << "\n\nEnter the Location to save the Decoded File: ";

        string Decoded_File_Location;
        getline(cin, Decoded_File_Location);

        Task.setEncodedFileLocation("");
        Task.setDecodedFileLocation(Decoded_File_Location);

        cout << "\n\nDecoding ................................... ";
        Task.decode();
    }
    cout << "\n\nFinished Task\n\n";
    exit(0);
}
