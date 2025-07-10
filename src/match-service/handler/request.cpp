#include "request.hpp"

#include "field.hpp"

#include <string_view>
#include <unordered_map>
#include <functional>
#include <format>

using namespace match::handler;
using json = boost::json::object;

namespace 
{


template<typename T>
std::optional<T> FindItem(const json& msg, std::string_view name)
{
    constexpr bool isNum = std::is_same_v<T, uint64_t>;
    constexpr bool isStr = std::is_same_v<T, std::string_view>;

    static_assert(isNum || isStr);

    auto iter = msg.find(name);
    if(iter == msg.end()) {
        return std::nullopt;
    }

    auto& v = iter->value();

    if constexpr (isNum) {
        if(v.is_uint64()) {
            return v.as_uint64();
        }
    } 

    if constexpr (isStr) {
        if(v.is_string()) 
        {
            auto& str = v.as_string();
            return std::string_view(
                str.data(),
                str.size()
            );
        }
    }

    return std::nullopt;
}


}


ResponsePtr match::handler::MoveRequest::response(
    const boost::json::object& req
) {
    auto move_notation = FindItem<std::string_view>(
        req, FieldName(RequestField::DoMove)
    );
    if(!move_notation) {
        return std::make_shared<ErrorResponse>(
            std::format(
            "no field {} was found",
            FieldName(RequestField::DoMove)
        ));
    }

    auto move = player->DoMove(*move_notation);
    if(!move) {
        return std::make_shared<ErrorResponse>(
            move.error()
        );
    }

    return std::make_shared<UpdateResponse>(
        player->getGame(), move.value()
    );
}



ResponsePtr match::handler::ErrorRequest::response(const boost::json::object &req)
{
    return std::make_shared<ErrorResponse>(ec);
}



std::shared_ptr<IPlayerRequest> match::handler::PlayerRequestFactory::Create(
    std::string_view type, model::PlayerPtr _p
) {
    static const std::unordered_map<
        std::string_view, 
        std::function<PlayerRequestPtr(model::PlayerPtr _p)>> 
    Creator =  
    {
        {
            FieldName(RequestField::DoMove), 
            [](model::PlayerPtr _p) {return std::make_shared<MoveRequest>(_p);}
        }
    };

    auto iter = Creator.find(type);

    if(iter == Creator.end()) {
        return nullptr;
    }

    return iter->second(_p);
}



match::handler::RequestManager::RequestManager(
    model::PlayerPtr _p, 
    const boost::json::object &req)
{
    auto type = FindItem<std::string_view>(
        req, FieldName(RequestField::Type)
    );
    if(!type) {
        handler = std::make_shared<ErrorRequest>(
            std::format(
            "no field {} was found",
            FieldName(RequestField::Type)
        ));
        return;
    }

    handler = PlayerRequestFactory::Create(type.value(), _p);
}
