#include <map>
#include <iostream>



int main(int argc, char **argv)
{
    std::map<std::string, int> week_name;
    week_name["Mon"] = 1;
    week_name["Tue"] = 2;
    week_name["Wen"] = 3;
    week_name["Thi"] = 4;
    week_name["Fri"] = 5;
    week_name["Sat"] = 6;
    week_name["Sun"] = 7;

    std::cout << "Sun is " << week_name["Sun"] << std::endl;

    std::map<std::string, int>::iterator it;

    for (it = week_name.begin(); it != week_name.end(); it++) {
        std::cout << "Name=" << (*it).first <<
            " i=" << (*it).second << std::endl;
    }
}
