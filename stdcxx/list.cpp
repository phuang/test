#include <list>
#include <string>
#include <iostream>
#include <algorithm>

void print (std::string &str)
{
    std::cout << str << std::endl;
}

int main(int argc, char **argv)
{
    std::list<std::string> strings;

    strings.push_back("Hello");
    strings.push_back("World");

    std::for_each(strings.begin(), strings.end(), print);
    return 0;
}
