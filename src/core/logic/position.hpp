#pragma once

#include "bitboard.hpp"
#include "defs.hpp"
#include "zobrist.hpp"
#include "storage.hpp"

#include <cstring>

namespace Core::Logic
{

class PositionBase {
public:

    static void Setup();
    PositionBase() {Setup();};

    constexpr Piece GetPiece(Square sqr) const {return types[sqr];}
    Color GetPieceColor(Square) const;

    template<typename... Pieces>
    constexpr Bitboard GetPieces(Color, Pieces...) const noexcept;

    template<typename... Colors>
    constexpr Bitboard GetOccupied(Colors...) const noexcept;

    constexpr Color GetSide() const noexcept {return side;}

protected:

    void AddPiece(Color, Piece, Square) noexcept;
    void AddPiece(Color, Piece, Square, Zobrist&) noexcept;

    [[maybe_unused]] Piece RemovePiece(Color, Square) noexcept;
    void RemovePiece(Color, Square, Zobrist&) noexcept;

    [[maybe_unused]] Piece MovePiece(Square, Square) noexcept;
    void MovePiece(Square, Square, Zobrist&) noexcept;

    [[maybe_unused]] Piece ReplacePiece(Piece, Square) noexcept;
    void ReplacePiece(Piece, Square, Zobrist&) noexcept;

protected:

    Bitboard pieces[COLOR_COUNT][PIECE_COUNT];
    Bitboard occupied[COLOR_COUNT];
    Piece types[SQUARE_COUNT];
    Color side;

};

template <typename... Pieces>
inline constexpr Bitboard PositionBase::GetPieces(Color c, Pieces... p) const noexcept 
{
    constexpr bool isPiece      =   (std::is_same_v<Pieces, Piece> && ...);
    constexpr bool isPieceType  =   (std::is_same_v<Pieces, PieceType> && ...);
    static_assert((isPiece || isPieceType), "All template arguments must be of type Piece or PieceType");
    if(c == ANY_COLOR) 
        return Bitboard(((pieces[WHITE][p] | pieces[BLACK][p]) | ...));
    return Bitboard((pieces[c][p] | ...));
}

template <typename... Colors>
inline constexpr Bitboard PositionBase::GetOccupied(Colors... c) const noexcept 
{
    constexpr bool isColor = (std::is_same_v<Colors, Color> && ...);
    constexpr bool isColorType = (std::is_same_v<Colors, ColorType> && ...);
    static_assert((isColor || isColorType), "All template arguments must be of type Color or ColorType");
    return Bitboard((occupied[c] | ...));
}




class PositionAttacks : public PositionBase {
public:

    constexpr bool IsAttacker(Square sqr) const noexcept {return checkers & sqr.bitboard();}
    bool CanPassant(Square from, Square targ) const noexcept;
 
    constexpr bool IsCheck() const noexcept {return checkers;}
    constexpr bool IsDoubleCheck() const noexcept {return checkers.count() == 2;}

    constexpr Bitboard GetAttackers() const noexcept {return attackers;}
    constexpr Bitboard GetDeffensiveSquares() const noexcept {return defense;}
    constexpr Bitboard GetPinnedPieces() const noexcept {return pinned;}
    Bitboard GetPinMask(Square) const noexcept;
    Bitboard GetAttacksTo(Square sqr, Bitboard occ) const noexcept;

    void UpdateAttacks() noexcept;

private:

    void ComputePins() noexcept;
    void ComputeAttacks() noexcept;

protected:

    Bitboard attackers;
    Bitboard pinned;
    Bitboard checkers;
    Bitboard defense;

};


template<StorageType Policy>
class Position : public PositionAttacks {
public:

    Position() = default;
    Position(std::string_view fen) {SetFen(fen);};
    template<StorageType T>
    Position(const Position<T>&);

    void SetFen(std::string_view fen) noexcept;
    std::string GetFen() const noexcept;
    constexpr Square GetPassant() const {return st.back().passant;}
    constexpr const Policy& GetHistory() const noexcept {return st;}
    constexpr Zobrist GetHash() const {return st.back().hash;}
    constexpr Piece GetCaptured() const {return st.back().captured;}
    constexpr int GetPly() const noexcept {return st.size();}

    void DoMove(Move) noexcept;
    void UndoMove() noexcept;

    template<StorageType T> 
    bool IsDraw(const T& globalHistory) const noexcept;
    bool IsDraw() const noexcept;

    bool CanCastle(CastleType) const noexcept;

private:

    void UpdatePassant(Square) noexcept;
    void UpdateCastle(Color, CastleType) noexcept;
    void TryToUpdateCastle(Color, Square maybe_rook) noexcept;
    bool NotEnoughPieces() const noexcept;

private:

    Policy st;
    template<StorageType> friend class Position;

};

template <StorageType Policy>
template <StorageType T>
inline Position<Policy>::Position(const Position<T> &pos) 
{
    std::memcpy(pieces, pos.pieces, sizeof(Bitboard) * COLOR_COUNT * PIECE_COUNT);
    std::memcpy(occupied, pos.occupied, sizeof(Bitboard) * COLOR_COUNT);
    std::memcpy(types, pos.types, sizeof(Piece) * SQUARE_COUNT);

    side = pos.side;

    st.create() = pos.st.back();
}

template <StorageType Policy>
template <StorageType T>
inline bool Position<Policy>::IsDraw(const T &globalHistory) const noexcept {
  return st.back().rule50 == 50 || NotEnoughPieces() ||
         st.hasRepeated(globalHistory);
}


using PositionFM = Position<StaticStorage>;
using PositionDM = Position<DynamicStorage>;


} // namespace Core::Logic