#include <iostream>

// #include "Fixture/Fixture1D.h"
#include "fixtures.h"

auto fix = MakeAstera60();
auto fix2 = MakeStrobo24x8();

int main() {
    // auto fix2d = make_fixture<Fixture2D, LedOrder::RGB, 5>(10, 6, 2, 5);

    std::cout << fix->describe() << std::endl;
    std::cout << fix2->describe() << std::endl;
    return 0;
}