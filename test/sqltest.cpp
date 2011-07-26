#include <iostream>
#include <string>
#include <vector>

#include "sqlparser.h"

using namespace std;

int main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tAn sql_select parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout << "Type [q or Q] to quit\n\n";

    bool result;

    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        result = ensighten::Parser::Parse(str);

        if (result)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "\n-------------------------\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
