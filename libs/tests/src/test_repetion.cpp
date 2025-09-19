#include "gtest/gtest.h"

#include "logic/move.hpp"
#include "logic/position.hpp"

using namespace game::logic;

TEST(TestRepetition, Basic) 
{
    PositionDynamicMemory posHistory("2k5/8/8/8/8/8/8/6QK w - - 0 1");

    for(int i = 0; i < 3; ++i) {
        posHistory.do_move({g1, a1,DEFAULT_MF});
        posHistory.do_move({c8, d8,DEFAULT_MF});
        posHistory.do_move({a1, g1,DEFAULT_MF});
        posHistory.do_move({d8, c8,DEFAULT_MF});
        if(i < 2) {
            EXPECT_FALSE(posHistory.is_draw());
        } else {
            EXPECT_TRUE(posHistory.is_draw());
        }
    }
}

TEST(TestRepetition, History)
{
    PositionDynamicMemory posHistory("2k5/8/8/8/8/8/8/6QK w - - 0 1");

    for(int i = 0; i < 2; ++i) {
        posHistory.do_move({g1, a1,DEFAULT_MF});
        posHistory.do_move({c8, d8,DEFAULT_MF});
        posHistory.do_move({a1, g1,DEFAULT_MF});
        posHistory.do_move({d8, c8,DEFAULT_MF});
        EXPECT_FALSE(posHistory.is_draw());
    }

    PositionFixedMemory pos(posHistory);
    pos.do_move({g1, a1,DEFAULT_MF});
    pos.do_move({c8, d8,DEFAULT_MF});
    pos.do_move({a1, g1,DEFAULT_MF});
    pos.do_move({d8, c8,DEFAULT_MF});

    EXPECT_TRUE(pos.is_draw(posHistory.get_history()));
}