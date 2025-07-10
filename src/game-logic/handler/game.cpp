#include "game.hpp"

using namespace game;
using namespace logic;
using namespace handler;

game::handler::GameState::GameState() : position(), moves(position) {
    position.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    moves.generate();
}



std::expected<Move, std::string> game::handler::GameState::DoMove(
    std::string_view notation
) {
    auto move = FindMove(notation);
    if (move) { DoMove(move.value()); } 
    return move;
}

void game::handler::GameState::getMoveUpdates(
    boost::json::object& upd, Move move
) const {
    boost::json::object sqr;

    auto& bb = position.getBoard();
    auto from = move.from();
    auto targ = move.to();
    
    sqr[SquareToString(from)] = PieceToChar(EMPTY_PIECE);
    sqr[SquareToString(targ)] = PieceToChar(
        bb.piece_on(targ), 
        bb.piece_color(targ)
    );

    switch (move.flag())
    {
    case CastleShortFlag:
        sqr[SquareToString(from + EAST)] = PieceToChar(EMPTY_PIECE);
        sqr[SquareToString(targ + EAST)] = PieceToChar(EMPTY_PIECE);
        break;
    case CastleLongFlag:
        sqr[SquareToString(from + WEST)] = PieceToChar(EMPTY_PIECE);
        sqr[SquareToString(targ + 2 * WEST)] = PieceToChar(EMPTY_PIECE);
        break;
    case EnPassantFlag:
    {
        auto dir = (targ > from) ? NORTH : SOUTH;
        sqr[SquareToString(targ - dir)] = PieceToChar(EMPTY_PIECE);
        break;
    }
    default:
        break;
    }

    upd["sqr"] = sqr;
}



Move game::handler::GameState::PermutateMove(std::string_view notation) const
{
    if(notation.size() < 4 || notation.size() > 5) {
        return NullMove;
    }

    std::string_view note_from(notation.begin(), 2),
                     note_targ(notation.begin() + 2, 2);

    Square from = toSquare(note_from), 
           targ = toSquare(note_targ);

    MoveType flag = 
        (notation.back() > 'A' && notation.back() < 'Z') ?
            (notation.back() == 'Q') ? QueenPromFlag :
            (notation.back() == 'R') ? RookPromFlag :
            (notation.back() == 'N') ? KnightPromFlag :
            (notation.back() == 'B') ? BishopPromFlag :
            ErrorFlag : 
        AnyFlag;

    return moves.permutate(Move(from, targ, flag));
}



std::expected<Move, std::string> game::handler::GameState::FindMove(
    std::string_view notation
) const {
    if(result != GameResult::InProgress) {
        return std::unexpected("game finished, move cant be done");
    }

    Move move = PermutateMove(notation);

    if(move == NullMove) {
        return std::unexpected("move notation is incorrect");
    }

    return move;
}



void game::handler::GameState::DoMove(Move move)
{
    position.do_move(move);
    moves.generate();

    auto side = position.getSide2Move();

    if(moves.empty()) {
        result.store(position.isKingInCheck() 
                    ? static_cast<GameResult>(-side) 
                    : GameResult::Draw);
        return;
    }

    auto& bb = position.getBoard();

    int wp = popcount(bb.getPiece(WHITE));
    int bp = popcount(bb.getPiece(BLACK));

    if(
        wp == 1 && bp == 1 || 
        wp == 2 && bp == 1 && (bb.getPiece(WHITE, BISHOP) || bb.getPiece(WHITE, KNIGHT)) ||
        wp == 1 && bp == 2 && (bb.getPiece(BLACK, BISHOP) || bb.getPiece(BLACK, KNIGHT)) || 
        position.getRule50() == 50
    ) { result.store(GameResult::Draw); }
}
