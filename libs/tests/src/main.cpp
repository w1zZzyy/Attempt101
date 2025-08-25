#include <gtest/gtest.h>
#include "logic/position.hpp"

class GlobalInitializer {
public:
    static void Init() {
        PositionFixedMemory::Init();
    }
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    GlobalInitializer::Init();

    return RUN_ALL_TESTS();
}
