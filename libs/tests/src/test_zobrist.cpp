#include "gtest/gtest.h"
#include "logic/position.hpp"
#include "logic/movelist.hpp"
#include <tuple>

using namespace game::logic;

using ZobristTestingData = std::tuple<std::string, std::optional<Move>>;

class ZobristHashingTest : public ::testing::TestWithParam<ZobristTestingData>{};

TEST_P(ZobristHashingTest, WithParams) 
{
    auto [fen, move] = GetParam();
    PositionFixedMemory pos(fen);

    Bitboard hash = pos.get_hash();

    if(move) 
        pos.do_move(*move);

    PositionFixedMemory pos2(pos.fen());
    EXPECT_EQ(pos.get_hash(), pos2.get_hash());

    if(move) 
        pos.undo_move();

    EXPECT_EQ((Bitboard)pos.get_hash(), hash);
}

INSTANTIATE_TEST_SUITE_P(
    ZobristHashingParams, 
    ZobristHashingTest, 
    ::testing::Values(
        std::make_tuple("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", std::nullopt), 
        std::make_tuple("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Move{e2, e3, DEFAULT_MF}), 
        std::make_tuple("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Move{e2, e4, DOUBLE_MF}), 
        std::make_tuple("6k1/8/8/8/4Pp2/8/8/6K1 b - e3 0 1", Move{f4, e3, EN_PASSANT_MF}), 
        std::make_tuple("6k1/8/8/8/4Pp2/8/8/6K1 b - e3 0 1", Move{g8, f8, DEFAULT_MF}), 
        std::make_tuple("6k1/8/8/8/8/8/8/R3K2R w KQ - 0 1", Move{e1, g1, S_CASTLE_MF}), 
        std::make_tuple("6k1/8/8/8/8/8/8/R3K2R w KQ - 0 1", Move{e1, c1, L_CASTLE_MF}),
        std::make_tuple("r3k2r/8/8/8/8/8/8/6K1 b kq - 0 1", Move{e8, g8, S_CASTLE_MF}), 
        std::make_tuple("r3k2r/8/8/8/8/8/8/6K1 b kq - 0 1", Move{e8, c8, L_CASTLE_MF}),
        std::make_tuple("r1r3k1/1P6/8/8/8/8/8/6K1 w - - 0 1", Move{b7, b8, Q_PROMOTION_MF}), 
        std::make_tuple("r1r3k1/1P6/8/8/8/8/8/6K1 w - - 0 1", Move{b7, c8, Q_PROMOTION_MF}), 
        std::make_tuple("3bb3/3b4/rr1rr2r/4k3/8/8/B4BB1/6K1 w - - 0 1", Move{a2, e6, DEFAULT_MF})
    )
);

void check_hash_stability(PositionFixedMemory &pos, int depth) 
{
    if (depth == 0) 
        return;

    MoveList moves;
    moves.generate<MoveGenType::NotForced>(pos);

    for (size_t i = 0; i < moves.get_size(); ++i) {
        Move move = moves[i];
        pos.do_move(move);
        pos.update();

        {
            PositionDynamicMemory copy(pos.fen());
            ASSERT_EQ(pos.get_hash(), copy.get_hash()) << pos.fen();
        }

        check_hash_stability(pos, depth - 1);

        pos.undo_move();
    }
}

TEST(ZobristHashingTest, OnDepth) {
    PositionFixedMemory pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    pos.update();
    check_hash_stability(pos, 4);
}