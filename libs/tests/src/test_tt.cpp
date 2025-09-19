#include "gtest/gtest.h"
#include <cstdint>
#include "engine/tt.hpp"

using namespace game::engine;

TEST(TestTransposition, Basic) {
    Transposition tt;
    tt.resize(4);

    uint64_t key = 12312541515235342;
    tt.store(key, 123, {}, 5, EntryType::Exact);
    auto r = tt.probe(key, 4, -30000, 30000);
    EXPECT_TRUE(r.score.has_value());
    EXPECT_EQ(r.score.value(), 123);

    r = tt.probe(key, 6, -30000, 30000);
    EXPECT_TRUE(!r.score);
}

TEST(TestTransposition, Rewrite) {
    Transposition tt;
    tt.resize(4);

    uint64_t key = 12312541515235342;
    tt.store(key, 123, {}, 5, EntryType::Exact);
    auto r = tt.probe(key, 4, -30000, 30000);
    EXPECT_TRUE(r.score.has_value());
    EXPECT_EQ(r.score.value(), 123);

    tt.store(key, -2785, {}, 3, EntryType::LowerBound);
    r = tt.probe(key, 4, -30000, 30000);
    EXPECT_TRUE(r.score.has_value());
    EXPECT_EQ(r.score.value(), 123);

    tt.store(key, -21, {}, 3, EntryType::Exact);
    r = tt.probe(key, 2, -30000, 30000);
    EXPECT_TRUE(r.score.has_value());
    EXPECT_EQ(r.score.value(), -21); 
}

TEST(TestTransposition, Last) {
    Transposition tt; tt.resize(1);
    uint64_t k = 0xDEADBEEF12345678ULL;
    tt.store(k, 100, {}, 5, EntryType::Exact);
    EXPECT_EQ(tt.probe(k, 4, -9999, 9999).score.value(), 100);
    EXPECT_EQ(tt.probe(k, 5, -9999, 9999).score.value(), 100);
    EXPECT_FALSE(tt.probe(k, 6, -9999, 9999).score.has_value());

    tt.store(k, -50, {}, 3, EntryType::LowerBound);
    EXPECT_EQ(tt.probe(k, 4, -9999, 9999).score.value(), 100); 
    tt.store(k, 30, {}, 3, EntryType::Exact);
    EXPECT_EQ(tt.probe(k, 2, -9999, 9999).score.value(), 30);

}