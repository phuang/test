// g++ -std=c++0x 
#include <memory>
#include <iostream>
#include <vector>

class A{
public:
    A() {
        static int c = 0;
        i = c++;
        std::cout << "New A(" << i << ")" << std::endl;
    }
    ~A() {
        std::cout << "Delete A(" << i << ")" << std::endl;
    }
    
    void Hello() {
        std::cout << "Hello A(" << i << ")" << std::endl;
    }
    int i;
};

typedef std::shared_ptr<A> APtr;
int main()
{
    APtr p1;
    p1.reset(new A());

    APtr p2(new A());

    p2 = p1;

    p1->Hello();
    p2->Hello();
    
    std::cout << "p1 = NULL" << std::endl;
    p1.reset();
    std::cout << "p2 = NULL" << std::endl;
    p2.reset();
    std::cout << "exit" << std::endl;

    std::vector<APtr> ve;

    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));


    ve.pop_back();
    ve.pop_back();
    ve.pop_back();

    ve.push_back(APtr(new A()));
    ve.push_back(APtr(new A()));

    for (int i = 0; i < ve.size(); i++)
        ve[i]->Hello();

}
