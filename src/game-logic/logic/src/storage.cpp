#include "storage.hpp"

using namespace game::logic;

State &game::logic::DynamicStateStorage::create() {
    if (history.empty()) {
        history.emplace_back();
        return history.front();
    }

    if (history.size() > curr + 1) {
        history[curr + 1].copy_core(history[curr]);
    } else {
        history.emplace_back(history[curr]);
    }

    return history[++curr];
}

State &game::logic::DynamicStateStorage::rollback() {
    if (curr == 0) {
        throw std::runtime_error("cant rollback");
    }
    return history[--curr];
}

State &game::logic::DynamicStateStorage::top() {
    return history.at(curr);
}

const State &game::logic::DynamicStateStorage::top() const {
    return history.at(curr);
}
