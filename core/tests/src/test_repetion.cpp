#include "gtest/gtest.h"

#include "logic/move.hpp"
#include "logic/position.hpp"

using namespace Core::Logic;

TEST(TestRepetition, Basic) 
{
    Position<DynamicStorage> posHistory("2k5/8/8/8/8/8/8/6QK w - - 0 1");

    for(int i = 0; i < 3; ++i) {
        posHistory.DoMove({g1, a1,DEFAULT_MF});
        posHistory.DoMove({c8, d8,DEFAULT_MF});
        posHistory.DoMove({a1, g1,DEFAULT_MF});
        posHistory.DoMove({d8, c8,DEFAULT_MF});
        if(i < 2) {
            EXPECT_FALSE(posHistory.IsDraw());
        } else {
            EXPECT_TRUE(posHistory.IsDraw());
        }
    }
}

TEST(TestRepetition, History)
{
    Position<DynamicStorage> posHistory("2k5/8/8/8/8/8/8/6QK w - - 0 1");

    for(int i = 0; i < 2; ++i) {
        posHistory.DoMove({g1, a1,DEFAULT_MF});
        posHistory.DoMove({c8, d8,DEFAULT_MF});
        posHistory.DoMove({a1, g1,DEFAULT_MF});
        posHistory.DoMove({d8, c8,DEFAULT_MF});
        EXPECT_FALSE(posHistory.IsDraw());
    }

    Position<StaticStorage> pos(posHistory);
    pos.DoMove({g1, a1,DEFAULT_MF});
    pos.DoMove({c8, d8,DEFAULT_MF});
    pos.DoMove({a1, g1,DEFAULT_MF});
    pos.DoMove({d8, c8,DEFAULT_MF});

    EXPECT_TRUE(pos.IsDraw(posHistory.GetHistory()));
}