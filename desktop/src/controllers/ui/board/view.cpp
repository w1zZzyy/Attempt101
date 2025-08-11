#include "view.hpp"

namespace controller
{

UIBoardManager::UIBoardManager(event::Bus &bus) : bus(bus)
{
    SubscribeOnPieceAddedEvent();
    SubscribeOnPieceRemovedEvent();
    SubscribeOnPieceMovedEvent();
}

UIBoardManager &UIBoardManager::SetBoardView(game::logic::Color side)
{
    assert(side.isValid());

    using namespace game::logic;
    board_view = side.is(WHITE) 
        ? ui::CreateBoardView<WHITE>()
        : ui::CreateBoardView<BLACK>();
    board.SetBoardView(board_view);

    return *this;
}

UIBoardManager& UIBoardManager::SetLeftBottomSquarePos(const sf::Vector2f& pos)
{
    assert(board_view);
    board_view->SetLeftBottomSquarePos(pos);
    return *this;
}

UIBoardManager& UIBoardManager::SetCellShape(const sf::Vector2f& shape) 
{
    assert(board_view);
    board_view->SetCellShape(shape);
    return *this;
}   

UIBoardManager& UIBoardManager::SetCellColor(game::logic::Color side, sf::Color color)
{
    assert(board_view);
    board_view->SetCellColor(side, color);
    return *this;
}

UIBoardManager& UIBoardManager::SetHighlightedCellColor(sf::Color color)
{
    assert(board_view);
    board_view->SetHighlightedCellColor(color);
    return *this;
}

void UIBoardManager::Render(sf::RenderWindow &window)
{
    board.Render(window);
    for(auto& piece : pieces) {
        if(piece) 
            piece->Render(window);
    }
}

void UIBoardManager::SubscribeOnPieceAddedEvent()
{
    bus.subscribe<event::PieceAddedEvent>(
        [this](const event::PieceAddedEvent& event) {
            assert(board_view);
            pieces[event.square]
                .emplace(event.color, event.piece)
                .setSize(board_view->GetCellShape())
                .setPos(board_view->GetPosition(event.square));
        }
    );
}

void UIBoardManager::SubscribeOnPieceRemovedEvent()
{
    bus.subscribe<event::PieceRemovedEvent>(
        [this](const event::PieceRemovedEvent& event) {
            pieces[event.captured_on].reset();
        }
    );
}

void UIBoardManager::SubscribeOnPieceMovedEvent()
{
    bus.subscribe<event::PieceMovedEvent>(
        [this](const event::PieceMovedEvent& event) {
            assert(board_view);
            if(event.promotion)
                pieces[event.from]->replacePiece(*event.promotion);
            pieces[event.targ] = std::move(pieces[event.from]);
            pieces[event.targ]->setPos(board_view->GetPosition(event.targ));
            pieces[event.from].reset();
        }
    );
}

}