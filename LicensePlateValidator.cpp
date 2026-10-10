#include <iostream>
using namespace std;

int main()
{
    string x;

    cout << "License Plate Validator" << endl;
    cout << "Enter plate number: ";
    getline(cin, x);

    bool valid = true;

    // Checks I, O, Q
    for (char c : x)
    {
        if (c == 'I' || c == 'O' || c == 'Q' || c == 'i' || c == 'o' || c == 'q')
        
        {
            valid = false;
        }
    }

    // number cannot start with 000
    if (x.length() >= 3 &&
        x[0] == '0' &&
        x[1] == '0' &&
        x[2] == '0')
    {
        valid = false;
    }

    // Check first and last alphanumeric characters
    char first = '\0';
    char last = '\0';

    for (char c : x)
    {
        if (isalnum(c))
        {
            if (first == '\0')
            {
                first = c;
            }

            last = c;
        }
    }

    string vowels = "AEIOUaeiou";

    if (vowels.find(first) != string::npos &&
        vowels.find(last) != string::npos)
    {
        valid = false;
    }

    if (valid)
    {
        cout << "Valid license plate." << endl;
    }
    else
    {
        cout << "Invalid license plate." << endl;
    }

    return 0;
}
