#include "view.hpp"

namespace event
{


void Bus::publish_all()
{
    while(!event_queue.empty()) {
        auto& event = event_queue.front();
        std::type_index key = typeid(*event);
        if(auto it = subscribers.find(key); it != subscribers.end()) {
            for(const auto& handler : it->second) {
                handler(*event);
            }
        }
        event_queue.pop();
    } 
}


}