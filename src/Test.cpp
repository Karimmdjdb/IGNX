#include "Test.hpp"

Test::Test(int a, int b) : a(a), b(b) {

}

Test::Test(std::initializer_list<int> list) {
    auto it = list.begin();
    b = *(it);
    a = *(it+1);
}
void Test::show() {
    std::cout << a << " | " << b << std::endl;
}

void Test::updateA(int a) {
    this->a = a;
}

Test& Test::getRef() {
    return *this;
}

