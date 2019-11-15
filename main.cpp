#include <iostream>
#include "CScript.h"
#include "Event.h"

class PrintLoop : public CScriptFunc {
private:
    int num;
    int storedNum;

    bool action() final {
        std::cout << this->storedNum << ": " << this->num-- << std::endl;
        if (num == 0) {
            std::cout << this->storedNum << ": done" << std::endl;
            return true;
        }
        return false;
    }

    void reset() final {
        this->num = this->storedNum;
        std::cout << this->storedNum << ": reseted" << std::endl;
    }

public:
    explicit PrintLoop(int num) : CScriptFunc() {
        this->num = num;
        this->storedNum = num;
    }
};

int main() {
    // define our functions, there is no dynamic memory allocation
    // PrintLoop prints num: num--
    CScriptFunc test;
    PrintLoop t1(1);
    PrintLoop t2(2);
    PrintLoop t3(3);
    PrintLoop t4(4);
    PrintLoop t5(5);
    PrintLoop t6(6);

    // This test event always true
    Event tevent;

    // create our CScript
    test
        .then(&t1)->geth(&t2)->geth(&t3)->drop(&tevent)
            ->then(&t4)
                ->then(&t5)->geth(&t6);

    // execute CScript
    while (!test.exec()) {
        //test is NOT BLOCKS the loop!!!
        std::cout << "<loop>" << std::endl;
    }

    return 0;
}