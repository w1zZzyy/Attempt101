#include "manager.hpp"

#include "../board/view.hpp"

namespace ui 
{

void PiecesManager::Render(sf::RenderWindow& window) {
    for(auto& piece : pieces) {
        if(piece) 
            piece->Render(window);
    }
}

PiecesManager& PiecesManager::SetPieceSelectedCallback(OnPieceSelected&& _callback) noexcept {
    onPieceSelected = std::move(_callback);
    return *this;
}

PiecesManager& PiecesManager::SetPieceMovedCallback(OnPieceMoved&& _callback) noexcept {
    onPieceMoved = std::move(_callback);
    return *this;
}

PiecesManager& PiecesManager::SetSelectionRights(int r) noexcept {
    rights = static_cast<SelectionRights>(r);
    return *this;
}

void PiecesManager::AppendPiece(Color c, Piece p, Square sqr) {
    pieces[sqr].reset();
    pieces[sqr].emplace(c, p);
    pieces[sqr]->setPos(BoardView::ToCoordinates(sqr));
    pieces[sqr]->setSize(BoardView::GetShape());
}

void PiecesManager::RemovePiece(Square s) {
    pieces[s].reset();
}

void PiecesManager::MovePiece(Square from, Square targ, std::optional<Piece> newPiece) 
{
    if(newPiece) 
        pieces[from]->replacePiece(*newPiece);

    pieces[targ] = std::move(pieces[from]);
    pieces[targ]->setPos(BoardView::ToCoordinates(targ));
    pieces[from].reset();
}

void PiecesManager::HandleMousePressedEvent(Square sqr) 
{
    if(!selected) 
        TryToSelectPiece(sqr);
    else 
        TryToMove(selected->sqr, sqr);
}

void PiecesManager::HandleMouseMovedEvent(sf::Vector2f mouse_pos) {
    if(selected) 
        pieces[selected->sqr]->setPos(mouse_pos);
}

void PiecesManager::HandleMouseReleasedEvent() 
{
    if(!selected) 
        return;

    const Square from = selected->sqr;
    if(auto targ = BoardView::ToSquare(pieces[from]->getPos())) {
        TryToMove(from, *targ);
    }
}

void PiecesManager::TryToSelectPiece(Square sqr) 
{
    DropSelectedPiece();
    if(!HasEnoughRights(sqr)) 
        return;

    selected.emplace(sqr);
    return onPieceSelected(sqr);
}

void PiecesManager::TryToMove(Square from, Square targ) 
{
    assert(selected);

    const auto& moves = selected->moves;
    std::vector<game::logic::Move> candidates;

    std::copy_if(
        moves.data(), moves.data() + moves.size(), 
        std::back_inserter(candidates), 
        [from, targ](game::logic::Move move) {
            return move.from() == from && move.targ() == targ;
        }
    );

    if(candidates.empty()) 
        onPieceMoved(std::unexpected{event::MoveErrorEvent(event::MoveErrorEvent::NotFound, from, targ)});
    else if(candidates.size() != 1) 
        onPieceMoved(std::unexpected{event::MoveErrorEvent(event::MoveErrorEvent::Ambiguous, from, targ)});
    else 
        onPieceMoved(candidates[0]);
}

bool PiecesManager::HasEnoughRights(Square sqr) const noexcept 
{
    if(!pieces[sqr]) 
        return false;
    if(rights == SelectionRights::Sudo) 
        return true;
    return (int)rights == pieces[sqr]->getColor();
}

void PiecesManager::DropSelectedPiece() 
{
    if(!selected) 
        return;

    const Square sqr = selected->sqr;
    pieces[sqr]->setPos(BoardView::ToCoordinates(sqr));

    selected.reset();
}


void PiecesManager::SetSelectedMoves(std::vector<Move>&& moves) {
    assert(selected);
    selected->moves = std::move(moves);
}

}