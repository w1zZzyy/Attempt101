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

PiecesManager& PiecesManager::SetSelectionRights(Color r) noexcept {
    selected.SetRights(r);
    return *this;
}

void PiecesManager::ResetSelected() noexcept 
{
    if(selected.IsSettled()) {
        pieces[Square(selected)]->setPos(BoardView::ToCoordinates(selected));
        selected.Reset();
    }
}

void PiecesManager::AppendPiece(Color c, Piece p, Square sqr) 
{
    pieces[sqr].reset();
    pieces[sqr].emplace(c, p) 
        .setPos(BoardView::ToCoordinates(sqr))
        .setSize(BoardView::GetShape())
        .setOriginCenter(); 
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
    if(!selected.IsSettled()) 
        TryToSelectPiece(sqr);
    else 
        TryToMove(selected, sqr);
}

void PiecesManager::HandleMouseMovedEvent(sf::Vector2f mouse_pos) 
{
    if(selected.IsGrabbed())
        pieces[Square(selected)]->setPos(mouse_pos);
}

void PiecesManager::HandleMouseReleasedEvent(sf::Vector2f mouse_pos) 
{
    if(!selected.IsGrabbed()) 
        return;

    if(auto sqr = BoardView::ToSquare(mouse_pos)) {
        if(!TryToMove(selected, *sqr)) {
            pieces[Square(selected)]->setPos(BoardView::ToCoordinates(selected));
            selected.Drop();
        }
    }
    else ResetSelected();
}

bool PiecesManager::TryToSelectPiece(Square sqr) 
{
    ResetSelected();
    
    if(
        !pieces[sqr] || 
        !selected.HasRights(pieces[sqr]->getColor())
    ) return false;

    selected.SetSelected(sqr);
    onPieceSelected(selected);

    return true;
}


bool PiecesManager::TryToMove(Square from, Square targ) 
{
    const Selected::Moves& moves = selected.GetMoves();
    std::vector<game::logic::Move> candidates;

    std::copy_if(
        moves.data(), moves.data() + moves.size(), 
        std::back_inserter(candidates), 
        [from, targ](game::logic::Move move) {
            return move.from() == from && move.targ() == targ;
        }
    );

    if(candidates.empty()) {
        if(!TryToSelectPiece(targ))
            onPieceMoved(std::unexpected{event::MoveErrorEvent(event::MoveErrorEvent::NotFound, from, targ)});
        return false;
    }
    else if(candidates.size() != 1) {
        onPieceMoved(std::unexpected{event::MoveErrorEvent(event::MoveErrorEvent::Ambiguous, from, targ)}); 
        return false;
    }
    
    onPieceMoved(candidates[0]);
    return true;
}


}