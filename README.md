# Attempt101

[English](#about-the-project) | [Русский](#о-проекте)

## About the project
- Task: playing against a chess engine
- when I started writing this project (~1.5 years ago), I knew practically nothing about programming. many times I rewrote this project from scratch (hence the name), the logic became so confusing that it was impossible to continue and it was easier to start over. that's how I learned to write code. 


## Architecture 

### Component communication
- a system consisting of loosely coupled components (ai, game, UI) communicating via EventBus.
- EventBus is a mechanism operating in a multithreaded environment that ensures asynchronous operation of components and does not block the main stream
- the principle of operation: listeners subscribe to an Event and when the event is published, listeners process it through callbacks

### Managing the state of an object

#### Events 
- to create a new event, you must inherit from IEvent(src/scene/model/event.hpp)
- the EventType concept strictly specifies that the object is inherited from the IEvent
```cpp
class IEvent {};
template<typename T>
concept EventType = std::derived_from<T, IEvent>;
```

#### States 
- the CRTP pattern (src/scene/model/state.hpp)
``cpp' is used to manage object states.
template<typename T, typename Object>
class IState;
``
- stores a pointer to the object and to the EventBus
```cpp
protected:
    Object* object;
    Shared::Bus* bus;
```

##### IState (basic) 
- works only on the template Object (stores a pointer to the object itself)
- has a handleEvent method that accepts a template EventType, and calls HandleEventImpl from the 'child' class, checking through the SupportsImpl<Event> method that the event can be processed in this state.
```cpp
template<EventType Event>
auto HandleEvent(const Event& event) {
    if constexpr (cast()->template SupportsImpl<Event>()) {
        assert(object);
        return cast()->HandleEventImpl(event);
    }
    else
        return NextState<T>();
}
```

##### State (child)
- example from src/scene/game/state/piece/idle.hpp
```cpp
class Idle : public Model::IState<Idle, UI::Renderer::Pieces>;
``
- must have a SupportsImpl method that determines whether a given state can handle an event.
```cpp
template<Model::EventType T>
constexpr bool SupportsImpl() {return std::is_same_v<T, Event::GameStarted>;}
``
- has methods that must be called HandleEventImpl and return the nextState object<NewState1, NewState2.....> to process a specific event
```cpp
Model::NextState<InProgressIdle> HandleEventImpl(const Event::GameStarted& event);
``
is an example where we have one state that can transition to one of several new states (src/scene/game/state/piece/piece-grabbed.hpp)
``cpp
Model::NextState<InProgressIdle, PieceSelected> HandleEventImpl(const Event::MouseReleased&);
```
- an example when we will definitely not move to a new state and will remain in the same (src/scene/game/state/piece/piece-grabbed.hpp)
``cpp
Model::NoNextState<Object> HandleEventImpl(const Event::MouseMoved&);
```

###### Transitions between states
- nextState (src/scene/model/state.hpp) is responsible for this
``cpp
template<typename... TState >
class nextState;
``
- there is no new state by default
- the Load method loads a new state
- the Dump method(&dest) loads the saved state to dest if it is not null.
- the NoNextState<Object> stub just makes it clear that the current state will definitely not change to the new one.

#### The Fortune Machine 
- base class (src/scene/model/machine.hpp)
``cpp
template<typename Object, StateType<Object>... T>
class Machine;
``
- The machine contains a link to the object, options for its possible states, and an EventBus (so that when the state of the object changes, which may affect other components of the system, they can be informed about it)
```cpp
protected:
    Object& object;
    Shared::Bus& bus;
    std::variant<T...> state;
``
- event processing is performed as follows
```cpp
template<EventType Event>
void HandleEvent(const Event& event) {
    std::visit(
        [&event, this](auto&& st) 
        {
            auto next = st.HandleEvent(event);
            if(next.template Dump<Object>(state))
                std::visit([this](auto& st) {st.Init(object, bus);}, state);
        }, 
        state
    );
}
```

##### Description of the machine operation
1. calling the handleEvent method for the current object 
2. after processing the event, the Machine will call the Dump method from the received nextState, passing the current state there.
3. if, when processing an event in the HandleEventImpl method, a new state was loaded into nextState via the Load<Object>(NewState1{Args...}) method, then when calling Dump in state, this new state is assumed and Dump returns true, if the new state was not loaded into nextState, it returns false and the state does not change

- this architecture makes it very easy to create new State Machines (src/scene/game/state/machine.hpp)
``cpp
class PieceMachine : public Model::Machine<UI::Renderer::Pieces, 
    Piece::Idle,
    Piece::InProgressIdle, 
    Piece::PieceGrabbed, 
    Piece::PieceSelected>
{
public:
    PieceMachine(UI::Renderer::Pieces& object, Shared::Bus& bus) noexcept
        : Machine(object, bus) {}
};
```


## Launch

### Requirements 
- GCC or Clang compiler (preferably GCC)
- support for BMI and BMI2 compiler instructions (usually on all +-modern processors)

### Windows 
```bash
git clone https://github.com/w1zZzyy/Attempt101.git
cd Attempt101
make run-release
```

### Linux
- it is necessary that the following dependencies are installed: 
    freetype
    x11
    xrandr
    xcursor
    xi
    udev
    opengl
    flac
    ogg
    vorbis
    vorbisenc
    vorbisfile
    pthread
- after that, you can already launch
```bash
git clone https://github.com/w1zZzyy/Attempt101.git
cd Attempt101
make run-release
```


## Used libraries
- [Google Test](https://github.com/google/googletest)
- [SFML](https://github.com/SFML/SFML)


--- 


## О проекте
- задача: игра против шахматного движка
- когда я начинал писать этот проект (~1.5 года назад) - я практически ничего не умел в программировании. множество раз я переписывал этот проект с полного нуля (отсюда и название), логика становилась настолько запутанной, что невозможно было продолжать и легче было начать заново. таким образом я и учился писать код. 


## Архитектура 

### Связь компонентов
- система, состоящая из слабосвязанных компонентов (ai, game, ui), общающихся через EventBus.
- EventBus - механизм, работающий в многопочной среде, обеспечивающий асинхронную работу компонентов, не блокирует основной поток
- принцип работы: слушатели подписываются на какой-то Ивент и когда ивент публикуется, слушатели обрабатывают его через колбеки

### Управление состоянием обьекта

#### События 
- для создания нового события нужно обязательно унаследоваться от IEvent(src/scene/model/event.hpp)
- концепт EventType строго задает, что обьект наследован от IEvent
```cpp
class IEvent {};
template<typename T>
concept EventType = std::derived_from<T, IEvent>;
```

#### Состояния 
- для управления состояниями обьекта используется паттерн CRTP (src/scene/model/state.hpp)
```cpp
template<typename T, typename Object>
class IState;
```
- хранит указатель на обьект и на EventBus
```cpp
protected:
    Object* object;
    Shared::Bus* bus;
```

##### IState (базовый) 
- работает только над шаблонным обьектом Object (хранит указатель на сам обьект)
- имеет метод HandleEvent, принимающий шаблонный EventType, и вызывает HandleEventImpl у 'дочернего' класса, проверив через метод SupportsImpl<Event>, что событие может быть обработано в данном состоянии
```cpp
template<EventType Event>
auto HandleEvent(const Event& event) {
    if constexpr (cast()->template SupportsImpl<Event>()) {
        assert(object);
        return cast()->HandleEventImpl(event);
    }
    else
        return NextState<T>();
}
```

##### State (дочерний)
- пример из src/scene/game/state/piece/idle.hpp
```cpp
class Idle : public Model::IState<Idle, UI::Renderer::Pieces>;
```
- обязан иметь метод SupportsImpl, который определяет, может ли данное состояние обрабатывать событие.
```cpp
template<Model::EventType T>
constexpr bool SupportsImpl() {return std::is_same_v<T, Event::GameStarted>;}
```
- имеет методы, которые обязаны называться HandleEventImpl и возвращать обьект NextState<NewState1, NewState2.....> для обработки определенного события
```cpp
Model::NextState<InProgressIdle> HandleEventImpl(const Event::GameStarted& event);
```
- пример, когда у нас одно состояние может переходить в одно из нескольких новых состояний (src/scene/game/state/piece/piece-grabbed.hpp)
```cpp
Model::NextState<InProgressIdle, PieceSelected> HandleEventImpl(const Event::MouseReleased&);
```
- пример, когда мы точно не перейдем в новое состояние и останемся в прежнем (src/scene/game/state/piece/piece-grabbed.hpp)
```cpp
Model::NoNextState<Object> HandleEventImpl(const Event::MouseMoved&);
```

###### Переходы между состояними
- за это отвечает NextState (src/scene/model/state.hpp)
```cpp
template<typename... TState>
class NextState;
```
- по умолчанию нового состояния нету
- метод Load загружает новое состояние
- метод Dump(&dest) загружает в dest сохраненное состояние, если оно не нулевое
- заглушка NoNextState<Object> просто дает понять, что текущее состояние точно не перейдет в новое

#### Машина Состояний 
- базовый класс (src/scene/model/machine.hpp)
```cpp
template<typename Object, StateType<Object>... T>
class Machine;
```
- Машина содержит ссылку на обьект, варианты его возможных состояний, и EventBus (чтобы при изменении состоянии обьекта, которое может влиять на другие компоненты системы, можно было им об этом сообщить)
```cpp
protected:
    Object& object;
    Shared::Bus& bus;
    std::variant<T...> state;
```
- обработка событий осуществляется так
```cpp
template<EventType Event>
void HandleEvent(const Event& event) {
    std::visit(
        [&event, this](auto&& st) 
        {
            auto next = st.HandleEvent(event);
            if(next.template Dump<Object>(state))
                std::visit([this](auto& st) {st.Init(object, bus);}, state);
        }, 
        state
    );
}
```

##### Описание работы машины
1. вызываем у текущего обьекта метод HandleEvent 
2. после обработки события, Машина вызовет у полученного NextState метод Dump, передав туда текущее состояние state
3. если при обработке события в методе HandleEventImpl в NextState было загружено новое состояние через метод Load<Object>(NewState1{Args...}), то при вызове Dump в state положиться это новое состояние и Dump вернет true, если новое состояние не было загружено в NextState, то вернется false и state не поменяется

- такая архитектура позволяет очень легко создавать новые Машины состояния (src/scene/game/state/machine.hpp)
```cpp
class PieceMachine : public Model::Machine<UI::Renderer::Pieces, 
    Piece::Idle,
    Piece::InProgressIdle, 
    Piece::PieceGrabbed, 
    Piece::PieceSelected>
{
public:
    PieceMachine(UI::Renderer::Pieces& object, Shared::Bus& bus) noexcept
        : Machine(object, bus) {}
};
```


## Запуск

### Требования 
- компилятор GCC или Clang (желательно GCC)
- поддержка инструкций компилятора BMI и BMI2 (как правило на всех +- современных процессорах)

### Windows 
```bash
git clone https://github.com/w1zZzyy/Attempt101.git
cd Attempt101
make run-release
```

### Linux
- необходимо, чтобы были установлены следующие зависимости: 
    freetype
    x11
    xrandr
    xcursor
    xi
    udev
    opengl
    flac
    ogg
    vorbis
    vorbisenc
    vorbisfile
    pthread
- после чего можно уже запускаться
```bash
git clone https://github.com/w1zZzyy/Attempt101.git
cd Attempt101
make run-release
```


## Использованные библиотеки
- [Google Test](https://github.com/google/googletest)
- [SFML](https://github.com/SFML/SFML)