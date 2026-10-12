#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool accepts(string x)
{
    bool valid = true;

    // Check Format A (ABC 1234)
    bool formatA = (
        x.length() == 8 &&
        isalpha(x[0]) &&
        isalpha(x[1]) &&
        isalpha(x[2]) &&
        x[3] == ' ' &&
        isdigit(x[4]) &&
        isdigit(x[5]) &&
        isdigit(x[6]) &&
        isdigit(x[7])
    );

    // Check Format B (123 ABC)
    bool formatB = (
        x.length() == 7 &&
        isdigit(x[0]) &&
        isdigit(x[1]) &&
        isdigit(x[2]) &&
        x[3] == ' ' &&
        isalpha(x[4]) &&
        isalpha(x[5]) &&
        isalpha(x[6])
    );

    // Must match either format
    if (!formatA && !formatB)
    {
        valid = false;
    }
        
    // Checks I, O, Q
    for (char c : x)
    {
        char upper = toupper(c);

        if (upper == 'I' || upper == 'O' || upper == 'Q')
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
        
    bool firstVowel = vowels.find(first) != string::npos;
    bool lastVowel = vowels.find(last) != string::npos;

    if (firstVowel && lastVowel)
    {
        valid = false;
    }

    return valid;
}

int main()
{
    string plate;
    char choice;

    do
    {
        cout << "\n=== License Plate Validator ===" << endl;
        cout << "Enter plate number: ";
        getline(cin, plate);
        
        if (accepts(plate))
        {
            cout << "Valid license plate." << endl;
        }
        else
        {
            cout << "Invalid license plate." << endl;
        }

        cout << "Do you want to validate another plate? (y/n): ";
        cin >> choice;
        cin.ignore(1000, '\n'); 

    } while (choice == 'y' || choice == 'Y');

    cout << "Thank you for using the License Plate Validator!" << endl;
    
    return 0;
}