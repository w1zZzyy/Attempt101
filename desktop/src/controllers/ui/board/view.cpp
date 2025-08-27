#include "view.hpp"

#include "event/models/piece_event.hpp"
#include "event/models/mouse_event.hpp"
#include "event/models/position_event.hpp"

namespace controller
{

UIGameController::UIGameController(event::Bus &bus) : bus(bus)
{
    SubscribeOnPieceAddedEvent();
    SubscribeOnPieceRemovedEvent();
    SubscribeOnPieceMovedEvent();
    SubscribeOnMousePressedEvent();
    SubscribeOnMouseMovedEvent();
    SubscribeOnMouseReleasedEvent();
    SetPieceManagerCallbacks();
}

UIGameController &UIGameController::SetBoardView(game::logic::Color side)
{
    assert(side.isValid());

    ui::BoardView::SetPlayerView(side);
    PiecesManager.SetSelectionRights(side);

    return *this;
}

UIGameController& UIGameController::SetOrigin(const sf::Vector2f& pos)
{
    ui::BoardView::SetOrigin(pos);
    return *this;
}

UIGameController& UIGameController::SetCellShape(const sf::Vector2f& shape) 
{
    ui::BoardView::SetCellShape(shape);
    return *this;
}   

UIGameController& UIGameController::SetCellColor(game::logic::Color side, sf::Color color)
{
    ui::BoardView::SetCellColor(side, color);
    return *this;
}

UIGameController& UIGameController::SetHighlightedCellColor(sf::Color color)
{
    ui::BoardView::SetHighlightedColor(color);
    return *this;
}

UIGameController& UIGameController::SetOnPieceSelected(OnPieceSelected&& callback) noexcept {
    onPieceSelected = std::move(callback);
    return *this;
}

void UIGameController::Render(sf::RenderWindow &window)
{
    BoardManager.Render(window);
    PiecesManager.Render(window);
}

void UIGameController::SubscribeOnPieceAddedEvent()
{
    bus.subscribe<event::PieceAddedEvent>(
        [this](const event::PieceAddedEvent& event) {
            PiecesManager.AppendPiece(event.color, event.piece, event.square);
        }
    );
}

void UIGameController::SubscribeOnPieceRemovedEvent()
{
    bus.subscribe<event::PieceRemovedEvent>(
        [this](const event::PieceRemovedEvent& event) {
            PiecesManager.RemovePiece(event.captured_on);
        }
    );
}

void UIGameController::SubscribeOnPieceMovedEvent()
{
    bus.subscribe<event::PieceMovedEvent>(
        [this](const event::PieceMovedEvent& event) {
            PiecesManager.MovePiece(event.from, event.targ, event.promotion);
        }
    );
}

void UIGameController::SubscribeOnMousePressedEvent()
{
    bus.subscribe<event::MousePressedEvent>(
        [this](const event::MousePressedEvent& event) {
            auto sqr = ui::BoardView::ToSquare(event.pos);
            if(sqr) {
                PiecesManager.HandleMousePressedEvent(*sqr);
            }
        }
    );
}

void UIGameController::SubscribeOnMouseMovedEvent() {
    bus.subscribe<event::MouseMovedEvent>(
        [this](const event::MouseMovedEvent& event){
            PiecesManager.HandleMouseMovedEvent(event.pos);
        }
    );
}

void UIGameController::SubscribeOnMouseReleasedEvent() {
    bus.subscribe<event::MouseReleasedEvent>(
        [this](const event::MouseReleasedEvent& event){
            PiecesManager.HandleMouseReleasedEvent();
        }
    );
}

void UIGameController::SetPieceManagerCallbacks() 
{
    using namespace game::logic;

    PiecesManager 
        .SetPieceSelectedCallback(
            [this](Square sqr){
                BoardManager.CleanHighlighted();
                for(Move move : onPieceSelected(sqr)) {
                    BoardManager.AddHighlighted(move.targ());
                }
            })
        .SetPieceMovedCallback(
            [this](std::expected<Move, event::MoveErrorEvent> move)
            {
                BoardManager.CleanHighlighted();
                PiecesManager.DropSelectedPiece();
                if(!move) 
                {
                    const auto error = move.error();
                    switch (error.type) 
                    {
                    case event::MoveErrorEvent::NotFound: 
                        PiecesManager.TryToSelectPiece(error.targ);
                        break;
                    case event::MoveErrorEvent::Ambiguous:
                        // promotion
                        bus.publish<event::PositionMoveAppearedEvent>(
                            {{error.from, error.targ, Q_PROMOTION_MF}}
                        );
                        break;
                    }
                }
                else {
                    const Move move_v = *move;
                    bus.publish<event::PositionMoveAppearedEvent>(
                        {move_v}
                    );
                }
            });
}

}