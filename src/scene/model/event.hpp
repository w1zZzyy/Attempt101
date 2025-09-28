#pragma once

#include <concepts>

namespace Scene::Model 
{

class IEvent {};

template<typename T>
concept EventType = std::derived_from<T, IEvent>;

}